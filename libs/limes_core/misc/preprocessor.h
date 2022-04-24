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

#pragma once

#include <limes_platform.h>
#include <utility>

#if LIMES_MSVC
#	define LIMES_BLOCK_WITH_FORCED_SEMICOLON(statement)         \
		__pragma (warning (push))                                \
			__pragma (warning (disable : 4127)) do { statement } \
		while (false)                                            \
		__pragma (warning (pop))
#else
#	define LIMES_BLOCK_WITH_FORCED_SEMICOLON(statement) \
		do {                                             \
			statement                                    \
		} while (false)
#endif


#define LIMES_MAKE_STRING_HELPER(str) #str

#define LIMES_MAKE_STRING(str) LIMES_MAKE_STRING_HELPER (str)


#define LIMES_CONCATENATE_HELPER(a, b) a##b

#define LIMES_CONCATENATE(item1, item2) LIMES_CONCATENATE_HELPER (item1, item2)


#if __cpp_lib_unreachable
#	define LIMES_UNREACHABLE ::std::unreachable();
#else
#	define LIMES_UNREACHABLE LIMES_ASSERT_FALSE;
#endif
