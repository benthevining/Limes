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
#include "../../limes_namespace.h"
#include "./exec_location.h"  // IWYU pragma: associated
#include <array>
#include "../../system/compiler_warnings.h"
#include "../misc.h"
#include "../../misc/Functions.h"

#if LIMES_MSVC
#	include <intrin.h>
#endif

LIMES_BEGIN_NAMESPACE

namespace files
{

[[nodiscard]] static inline std::string getModulePathInternal (HMODULE module)
{
	std::array<wchar_t, maxPathLength()> buffer1 {};
	std::array<wchar_t, maxPathLength()> buffer2 {};

	auto* path = buffer1.data();

	const auto endCB = func::defer_call ([&path, d = buffer1.data()]
										 {
			if (path != d)
				std::free (path); });

	auto size = GetModuleFileNameW (module, buffer1.data(), sizeof (buffer1) / sizeof (buffer1[0]));

	if (size == 0)
		return {};

	if (size == static_cast<DWORD> (sizeof (buffer1) / sizeof (buffer1[0])))
	{
		auto size_ = size;

		do
		{
			if (const auto* path_ = static_cast<wchar_t*> (std::realloc (path, sizeof (wchar_t) * size_ * 2)))
			{
				size_ *= 2;
				path = path_;
				size = GetModuleFileNameW (module, path, size_);
			}
			else
			{
				break;
			}
		}
		while (size == size_);

		if (size == size_)
			return {};
	}

	if (! _wfullpath (buffer2.data(), path, maxPathLength()))
		return {};

	const auto length = static_cast<int> (wcslen (buffer2.data()));

	std::array<char, maxPathLength()> output {};

	auto length_ = WideCharToMultiByte (CP_UTF8, 0, buffer2.data(), length, output.data(), maxPathLength(), nullptr, nullptr);

	if (length_ == 0)
		length_ = WideCharToMultiByte (CP_UTF8, 0, buffer2.data(), length, nullptr, 0, nullptr, nullptr);

	if (length_ == 0)
		return {};

	return { output.data() };
}

std::string getExecutablePath()
{
	return getModulePathInternal (nullptr);
}

std::string getModulePath()
{
#if LIMES_MSVC
#	define limes_get_return_address() _ReturnAddress()	 // NOLINT
#elif defined(__GNUC__)
#	define limes_get_return_address() __builtin_extract_return_addr (__builtin_return_address (0))	 // NOLINT
#endif

#ifdef limes_get_return_address

	HMODULE module;

	LIMES_DISABLE_ALL_COMPILER_WARNINGS
	if (GetModuleHandleEx (GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, static_cast<LPCTSTR> (limes_get_return_address()), &module))
		return getModulePathInternal (module);
	LIMES_REENABLE_ALL_COMPILER_WARNINGS

	return {};

#	undef limes_get_return_address

#else
	return {};
#endif
}

}  // namespace files

LIMES_END_NAMESPACE
