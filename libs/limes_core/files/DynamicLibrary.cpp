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

#if LIMES_WINDOWS
#	include <windows.h>
#else
#	include <dlfcn.h>
#endif

LIMES_BEGIN_NAMESPACE

DynamicLibrary::DynamicLibrary (const std::string_view& nameOrPath)
{
	open (nameOrPath);
}

DynamicLibrary::~DynamicLibrary()
{
	close();
}

bool DynamicLibrary::isOpen() const noexcept
{
	return handle != nullptr;
}

void* DynamicLibrary::getHandle() const noexcept
{
	return handle;
}

bool DynamicLibrary::open (const std::string_view& nameOrPath)
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

void* DynamicLibrary::findFunction (const std::string_view& functionName)
{
	if (handle == nullptr || functionName.empty())
		return nullptr;

#if LIMES_WINDOWS
	return ::GetProcAddress (static_cast<HMODULE> (handle), std::string (functionName).c_str());
#else
	return ::dlsym (handle, std::string (functionName).c_str());
#endif
}

LIMES_END_NAMESPACE
