/*
 * ======================================================================================
 *  __    ____  __  __  ____  ___
 * (  )  (_  _)(  \/  )( ___)/ __)
 *  )(__  _)(_  )    (  )__) \__ \
 * (____)(____)(_/\/\_)(____)(___/
 *
 *  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

#include "DynamicLibrary.h"
#include <limes_namespace.h>
#include <limes_platform.h>
#include <string>
#include <exception>

#if LIMES_WINDOWS
#	include <windows.h>
#	include <locale>  // for wstring_convert
#	include <codecvt>
#else
#	include <dlfcn.h>

#	if LIMES_LINUX
#		include <link.h>
#		include <linux/limits.h>
#	elif LIMES_APPLE
#		include <cstdint>
#		include <cstring>
#		include <mach-o/dyld.h>
#		include <mach-o/nlist.h>
#		include "../system/compiler_warnings.h"
#	endif
#endif

LIMES_BEGIN_NAMESPACE

namespace files
{

DynamicLibrary::DynamicLibrary (const std::string_view& nameOrPath) noexcept
{
	open (nameOrPath);
}

DynamicLibrary::~DynamicLibrary()
{
	close();
}

bool DynamicLibrary::operator== (const DynamicLibrary& other) const noexcept
{
	return handle == other.handle;
}

bool DynamicLibrary::operator!= (const DynamicLibrary& other) const noexcept
{
	return handle != other.handle;
}

bool DynamicLibrary::isOpen() const noexcept
{
	return handle != nullptr;
}

void* DynamicLibrary::getHandle() const noexcept
{
	return handle;
}

bool DynamicLibrary::open (const std::string_view& nameOrPath) noexcept
{
	try
	{
		close();

		if (nameOrPath.empty())
			return false;

#if LIMES_WINDOWS
		handle = ::LoadLibraryA (std::string (nameOrPath).c_str());
#else
		handle = ::dlopen (std::string (nameOrPath).c_str(), RTLD_LOCAL | RTLD_NOW);
#endif

		return handle != nullptr;
	}
	catch (std::exception&)
	{
		return false;
	}
}

void DynamicLibrary::close()
{
	if (handle == nullptr)
		return;

#if LIMES_WINDOWS
	::FreeLibrary (static_cast<HMODULE> (handle));
#else
	::dlclose (handle);
#endif
}

void* DynamicLibrary::findFunction (const std::string_view& functionName) noexcept
{
	try
	{
		if (handle == nullptr || functionName.empty())
			return nullptr;

#if LIMES_WINDOWS
		return ::GetProcAddress (static_cast<HMODULE> (handle), std::string (functionName).c_str());
#else
		return ::dlsym (handle, std::string (functionName).c_str());
#endif
	}
	catch (std::exception&)
	{
		return nullptr;
	}
}

#if LIMES_APPLE

[[nodiscard]] const char* pathname_for_handle (void* handle);

#endif

File DynamicLibrary::getFile() const
{
	if (handle == nullptr)
		return {};

#if LIMES_WINDOWS

	char buffer[MAX_PATH];

	::GetModuleFileNameA (static_cast<HMODULE> (handle), buffer, sizeof (buffer) / sizeof (buffer[0]));

	const std::string path { buffer };

	if (path.empty())
		return {};

	return File { path };

#elif LIMES_APPLE

	const std::string path { pathname_for_handle (handle) };

	if (path.empty())
		return {};

	return File { path };

#else

	char buffer[PATH_MAX];

	::dlinfo (handle, RTLD_DI_ORIGIN, buffer);

	return File { std::string { buffer } };

#endif
}


#if LIMES_APPLE

#	ifdef __LP64__
using mach_header_t		= mach_header_64;
using segment_command_t = segment_command_64;
using nlist_t			= nlist_64;
#	  else
using mach_header_t		= mach_header;
using segment_command_t = segment_command;
using nlist_t			= struct nlist;
#	  endif

LIMES_DISABLE_ALL_COMPILER_WARNINGS

[[nodiscard]] static inline const char* first_external_symbol_for_image (const mach_header_t* header)
{
	Dl_info info;

	if (::dladdr (header, &info) == 0)
		return nullptr;

	segment_command_t*	   seg_linkedit = nullptr;
	segment_command_t*	   seg_text		= nullptr;
	struct symtab_command* symtab		= nullptr;

	struct load_command* cmd = (struct load_command*) ((intptr_t) header + sizeof (mach_header_t));

	for (uint32_t i = 0; i < header->ncmds; i++, cmd = (struct load_command*) ((intptr_t) cmd + cmd->cmdsize))
	{
		switch (cmd->cmd)
		{
			case LC_SEGMENT :
				[[fallthrough]];
			case LC_SEGMENT_64 :
				if (! std::strcmp (((segment_command_t*) cmd)->segname, SEG_TEXT))			 // NOLINT
					seg_text = (segment_command_t*) cmd;									 // NOLINT
				else if (! std::strcmp (((segment_command_t*) cmd)->segname, SEG_LINKEDIT))	 // NOLINT
					seg_linkedit = (segment_command_t*) cmd;								 // NOLINT
				break;

			case LC_SYMTAB :
				symtab = (struct symtab_command*) cmd;
				break;
		}
	}

	if ((seg_text == nullptr) || (seg_linkedit == nullptr) || (symtab == nullptr))
		return nullptr;

	intptr_t file_slide = ((intptr_t) seg_linkedit->vmaddr - (intptr_t) seg_text->vmaddr) - seg_linkedit->fileoff;
	intptr_t strings	= (intptr_t) header + (symtab->stroff + file_slide);
	nlist_t* sym		= (nlist_t*) ((intptr_t) header + (symtab->symoff + file_slide));  // NOLINT

	for (uint32_t i = 0; i < symtab->nsyms; i++, sym++)
	{
		if ((sym->n_type & N_EXT) != N_EXT || ! sym->n_value)
			continue;

		return (const char*) strings + sym->n_un.n_strx;
	}

	return nullptr;
}

[[nodiscard]] const char* pathname_for_handle (void* handle)
{
	for (int32_t i = ::_dyld_image_count(); i >= 0; --i)
	{
		if (const auto* first_symbol = first_external_symbol_for_image (reinterpret_cast<const mach_header_t*> (::_dyld_get_image_header (i))))
		{
			if (std::strlen (first_symbol) <= 1)
				continue;

			// in order to trigger findExportedSymbol instead of findExportedSymbolInImageOrDependentImages.
			// See `dlsym` implementation at http://opensource.apple.com/source/dyld/dyld-239.3/src/dyldAPIs.cpp
			handle = reinterpret_cast<void*> ((intptr_t) handle | 1);

			first_symbol++;	 // in order to remove the leading underscore

			auto* address = ::dlsym (handle, first_symbol);

			Dl_info info;

			if (::dladdr (address, &info) != 0)
				return info.dli_fname;
		}
	}

	return nullptr;
}

LIMES_REENABLE_ALL_COMPILER_WARNINGS

#endif

}  // namespace files

LIMES_END_NAMESPACE
