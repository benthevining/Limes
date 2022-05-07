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

#include <windows.h>
#include <cstdlib>
#include <limes_platform.h>
#include <string>
#include <limes_namespace.h>
#include "exec_location.h"

#if LIMES_MSVC
#	include <intrin.h>
#endif

LIMES_BEGIN_NAMESPACE

namespace files
{

static inline std::string getModulePathInternal (HMODULE module)
{
	wchar_t buffer1[MAX_PATH];
	wchar_t buffer2[MAX_PATH];

	auto* path = buffer1;

	do
	{
		auto size = GetModuleFileNameW (module, buffer1, sizeof (buffer1) / sizeof (buffer1[0]));

		if (size == 0)
			return {};

		if (size == static_cast<DWORD> (sizeof (buffer1) / sizeof (buffer1[0])))
		{
			auto size_ = size;

			do
			{
				if (auto* path_ = static_cast<wchar_t*> (std::realloc (path, sizeof (wchar_t) * size_ * 2)))
				{
					size_ *= 2;
					path = path_;
					size = GetModuleFileNameW (module, path, size_);
				}
				else
				{
					break;
				}
			} while (size == size_);

			if (size == size_)
				break;
		}

		if (! _wfullpath (buffer2, path, MAX_PATH))
			break;

		auto length_ = static_cast<int> (wcslen (buffer2));

		char output[MAX_PATH];

		auto length__ = WideCharToMultiByte (CP_UTF8, 0, buffer2, length_, output, MAX_PATH, nullptr, nullptr);

		if (length__ == 0)
			length__ = WideCharToMultiByte (CP_UTF8, 0, buffer2, length_, nullptr, 0, nullptr, nullptr);

		if (length__ == 0)
			break;

		std::string result { output, static_cast<std::string::size_type> (MAX_PATH) };

		if (path != buffer1)
			std::free (path);

		return result;
	} while (false);

	if (path != buffer1)
		std::free (path);

	return {};
}

std::string getExecutablePath()
{
	return getModulePathInternal (nullptr);
}

std::string getModulePath()
{
#if LIMES_MSVC
#	define WAI_RETURN_ADDRESS() _ReturnAddress()
#elif defined(__GNUC__)
#	define WAI_RETURN_ADDRESS() __builtin_extract_return_addr (__builtin_return_address (0))
#else
	return {};
#endif

	HMODULE module;

#if LIMES_MSVC
#	pragma warning(push)
#	pragma warning(disable : 4054)
#endif
	if (GetModuleHandleEx (GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, static_cast<LPCTSTR> (WAI_RETURN_ADDRESS()), &module))
#if LIMES_MSVC
#	pragma warning(pop)
#endif
	{
		return getModulePathInternal (module);
	}

	return {};

#undef WAI_RETURN_ADDRESS
}

}  // namespace files

LIMES_END_NAMESPACE
