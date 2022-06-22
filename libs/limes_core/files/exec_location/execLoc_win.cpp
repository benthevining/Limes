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
#include <array>

#if LIMES_MSVC
#	include <intrin.h>
#endif

LIMES_BEGIN_NAMESPACE

namespace files
{

static inline std::string getModulePathInternal (HMODULE module)
{
	std::array<wchar_t, MAX_PATH> buffer1 {};
	std::array<wchar_t, MAX_PATH> buffer2 {};

	auto* path = buffer1.data();

	do
	{
		auto size = GetModuleFileNameW (module, buffer1.data(), sizeof (buffer1) / sizeof (buffer1[0]));

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

		if (! _wfullpath (buffer2.data(), path, MAX_PATH))
			break;

		auto length_ = static_cast<int> (wcslen (buffer2.data()));

		std::array<char, MAX_PATH> output {};

		auto length__ = WideCharToMultiByte (CP_UTF8, 0, buffer2.data(), length_, output, MAX_PATH, nullptr, nullptr);

		if (length__ == 0)
			length__ = WideCharToMultiByte (CP_UTF8, 0, buffer2.data(), length_, nullptr, 0, nullptr, nullptr);

		if (length__ == 0)
			break;

		std::string result { output, static_cast<std::string::size_type> (MAX_PATH) };

		if (path != buffer1.data())
			std::free (path);

		return result;
	} while (false);

	if (path != buffer1.data())
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
#	define WAI_RETURN_ADDRESS() _ReturnAddress()  // NOLINT
#elif defined(__GNUC__)
#	define WAI_RETURN_ADDRESS() __builtin_extract_return_addr (__builtin_return_address (0))  // NOLINT
#endif

#ifdef WAI_RETURN_ADDRESS

	HMODULE module;

#	if LIMES_MSVC
#		pragma warning(push)
#		pragma warning(disable : 4054)
#	endif
	if (GetModuleHandleEx (GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, static_cast<LPCTSTR> (WAI_RETURN_ADDRESS()), &module))
#	if LIMES_MSVC
#		pragma warning(pop)
#	endif
	{
		return getModulePathInternal (module);
	}

	return {};

#	undef WAI_RETURN_ADDRESS

#else
	return {};
#endif
}

}  // namespace files

LIMES_END_NAMESPACE
