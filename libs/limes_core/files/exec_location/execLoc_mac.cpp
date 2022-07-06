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

#include "../../limes_namespace.h"
#include <limes_platform.h>
#include <dlfcn.h>			  // for dladdr, Dl_info
#include <mach-o/dyld.h>	  // for _NSGetExecutablePath
#include <cstdint>			  // for uint32_t
#include <limits.h>			  // for PATH_MAX
#include <cstdlib>			  // for free, realpath, malloc
#include <cstring>			  // for strlen
#include <string>			  // for string
#include "./exec_location.h"  // IWYU pragma: associated
#include <array>
#include "../misc.h"
#include "../../misc/Functions.h"

LIMES_BEGIN_NAMESPACE

namespace files
{

std::string getExecutablePath()
{
	std::array<char, maxPathLength()> buffer1 {};
	std::array<char, maxPathLength()> buffer2 {};

	auto* path = buffer1.data();

	const auto endCB = func::defer_call ([&path, d = buffer1.data()]
										 {
			if (path != d)
				std::free (path); });

	auto size = static_cast<std::uint32_t> (sizeof (buffer1));

	if (_NSGetExecutablePath (path, &size) == -1)
	{
		path = static_cast<char*> (std::malloc (size));

		if (! _NSGetExecutablePath (path, &size))
			return {};
	}

	if (const auto* resolved = realpath (path, buffer2.data()))
		return { resolved };

	return {};
}

std::string getModulePath()
{
#if LIMES_MSVC
#	define limes_get_return_address() _ReturnAddress()	 // NOLINT
#elif defined(__GNUC__)
#	define limes_get_return_address() __builtin_extract_return_addr (__builtin_return_address (0))	 // NOLINT
#endif

#ifdef limes_get_return_address

	Dl_info info;

	if (dladdr (limes_get_return_address(), &info))
	{
		std::array<char, maxPathLength()> buffer {};

		if (const auto* resolved = realpath (info.dli_fname, buffer.data()))
			return { resolved };
	}

	return {};

#	undef limes_get_return_address

#else
	return {};
#endif
}

}  // namespace files

LIMES_END_NAMESPACE
