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

#include <limes_export.h>
#include <limes_namespace.h>
#include "debugger.h"
#include "preprocessor.h"
#include <type_traits>
#include <utility>
#include <limes_platform.h>
#include <limes_build_type.h>

#if LIMES_IOS || LIMES_LINUX
#	include <sys/types.h>
#	include <signal.h>
#endif

LIMES_BEGIN_NAMESPACE

LIMES_NO_EXPORT constexpr void limes_fire_assertion_failure()
{
	if (! std::is_constant_evaluated())
	{
		if (! isRunningUnderDebugger())
			return;

#if LIMES_IOS || LIMES_LINUX
		::kill (0, SIGTRAP);

#elif LIMES_MSVC
#	ifndef __INTEL_COMPILER
#		pragma intrinsic(__debugbreak)
#	endif
		__debugbreak();

#elif LIMES_INTEL && (LIMES_GCC || LIMES_CLANG || LIMES_OSX)
		asm("int $3");

#elif LIMES_ARM && LIMES_OSX
		__builtin_debugtrap();

#elif LIMES_ANDROID
		__builtin_trap();
#else
		__asm int 3
#endif
	}
}

#if LIMES_DEBUG
#	define LIMES_ASSERT_FALSE LIMES_BLOCK_WITH_FORCED_SEMICOLON (::limes::limes_fire_assertion_failure();)

#	define LIMES_ASSERT(x) LIMES_BLOCK_WITH_FORCED_SEMICOLON (if (! (x)) LIMES_ASSERT_FALSE;)
#else
#	define LIMES_ASSERT_FALSE
#	define LIMES_ASSERT
#endif


#if __cpp_lib_unreachable
#	define LIMES_UNREACHABLE LIMES_BLOCK_WITH_FORCED_SEMICOLON (::std::unreachable())
#else
#	define LIMES_UNREACHABLE LIMES_BLOCK_WITH_FORCED_SEMICOLON (LIMES_ASSERT_FALSE;)
#endif

LIMES_END_NAMESPACE
