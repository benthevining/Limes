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

#include "limes_assert.h"
#include <limes_namespace.h>
#include <limes_platform.h>
#include <iostream>

#if LIMES_IOS || LIMES_LINUX
#	include <sys/types.h>
#	include <signal.h>
#endif

LIMES_BEGIN_NAMESPACE

namespace assert
{

#if LIMES_MSVC
#	ifndef __INTEL_COMPILER
#		pragma intrinsic(__debugbreak)
#	endif
#endif

void break_in_debugger() noexcept
{
#if LIMES_IOS || LIMES_LINUX
	::kill (0, SIGTRAP);

#elif LIMES_ANDROID
	__builtin_trap();

#elif LIMES_MSVC
	__debugbreak();

#elif LIMES_INTEL && (LIMES_GCC || LIMES_CLANG || LIMES_OSX)
	asm("int $3");

#elif LIMES_ARM && LIMES_OSX
	__builtin_debugtrap();

#else
	__asm int 3
#endif
}

void log_assertion (const char* fileName, int lineNum, const char* condition) noexcept
{
	std::cerr << "Assertion failure in file " << fileName << ", line " << lineNum;

	if (condition == nullptr)
		std::cerr << " (LIMES_ASSERT_FALSE)";
	else
		std::cerr << ": condition '" << condition << "' failed";

	std::cerr << std::endl;
}

}  // namespace assert

LIMES_END_NAMESPACE
