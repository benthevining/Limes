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

#include <dlfcn.h>			  // for dladdr, Dl_info
#include <limes_namespace.h>  // for LIMES_BEGIN_NAMESPACE, LIMES_END_NAMESPACE
#include <mach-o/dyld.h>	  // for _NSGetExecutablePath
#include <cstdint>			  // for uint32_t
#include <limits.h>			  // for PATH_MAX
#include <cstdlib>			  // for free, realpath, malloc
#include <cstring>			  // for strlen
#include <string>			  // for string
#include "exec_location.h"	  // for getExecutablePath, getModulePath

LIMES_BEGIN_NAMESPACE

namespace files
{

std::string getExecutablePath()
{
	char  buffer1[PATH_MAX];
	char  buffer2[PATH_MAX];
	auto* path = buffer1;

	do
	{
		auto size = static_cast<uint32_t> (sizeof (buffer1));

		if (_NSGetExecutablePath (path, &size) == -1)
		{
			path = static_cast<char*> (std::malloc (size));

			if (! _NSGetExecutablePath (path, &size))
				break;
		}

		if (const auto* resolved = realpath (path, buffer2))
		{
			std::string result { resolved, static_cast<std::string::size_type> (std::strlen (resolved)) };

			if (path != buffer1)
				std::free (path);

			return result;
		}
	} while (false);

	if (path != buffer1)
		std::free (path);

	return {};
}

std::string getModulePath()
{
#if LIMES_MSVC
#	define WAI_RETURN_ADDRESS() _ReturnAddress()
#elif defined(__GNUC__)
#	define WAI_RETURN_ADDRESS() __builtin_extract_return_addr (__builtin_return_address (0))
#endif

#ifdef WAI_RETURN_ADDRESS

	Dl_info info;

	if (dladdr (WAI_RETURN_ADDRESS(), &info))
	{
		char buffer[PATH_MAX];

		if (auto* resolved = realpath (info.dli_fname, buffer))
			return { resolved, static_cast<std::string::size_type> (std::strlen (resolved)) };
	}

	return {};

#	undef WAI_RETURN_ADDRESS

#else
	return {};
#endif
}

}  // namespace files

LIMES_END_NAMESPACE
