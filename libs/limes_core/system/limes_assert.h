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
#include "../misc/preprocessor.h"
#include <type_traits>
#include <utility>
#include "../misc/IgnoreUnused.h"
#include <limes_platform.h>
#include <filesystem>
#include "./compiler_defs.h"

/** @defgroup system System
	Utilities that may be OS- or compiler-specific.
	@ingroup limes_core
 */

/** @dir libs/limes_core/system
	This directory contains utilities that may be OS- or compiler-specific.
	@ingroup system
 */

/** @file
	This file defines functions and macros related to runtime assertions.
	@ingroup system

	@todo optional message with assertion
 */

LIMES_BEGIN_NAMESPACE

/** This namespace contains utilities for runtime assertions.
	@ingroup system
 */
namespace assert
{

/// @cond internals

/** The actual implementation of the assertion function.
	@see fire_assertion, LIMES_ASSERT
	@ingroup system
 */
LIMES_EXPORT void fire_assertion_internal (const char* fileName, const char* functionName, int lineNum, const char* condition) noexcept;

/** This function is called by \c LIMES_ASSERT if its condition fails.
	This function can be called in constexpr contexts, but does nothing if actually evaluated at compile-time.
	@ingroup system
 */
LIMES_EXPORT constexpr void fire_assertion (const char* fileName, const char* functionName, int lineNum, const char* condition) noexcept
{
	if (! std::is_constant_evaluated())
		fire_assertion_internal (fileName, functionName, lineNum, condition);
}

/// @endcond

/** Sets a filepath to be used to log assertion failures for the current process.
	When a log file is being used, assertion failures will still stop the debugger and be printed to standard error.

	@envvar @b LIMES_ASSERTION_LOG_FILE This environment variable may be set to a filepath that will be used to record all assertion failures.
	Note that this variable's value is only used as a fallback; calling the function \c setAssertionLogFile() will override this variable's value.

	@see getAssertionLogFile
	@ingroup system
 */
LIMES_EXPORT void setAssertionLogFile (const std::filesystem::path& logFileToUse) noexcept;

/** Returns the path to the current assertion log file, if any.
	@see setAssertionLogFile
	@ingroup system
 */
LIMES_EXPORT [[nodiscard]] std::filesystem::path getAssertionLogFile();

}  // namespace assert

LIMES_END_NAMESPACE


#ifdef DOXYGEN

/** @def LIMES_ASSERT_FALSE
	This macro, if reached within the control flow, always causes an assertion failure, and stops in the debugger if one is attached.
	This macro can be used in constexpr functions, but assertion failures will not be caught or logged if the function is evaluated at compile time.
	In release builds, this macro expands to nothing.
	@see LIMES_ASSERT, LIMES_UNREACHABLE
	@ingroup preprocessor
 */
#	define LIMES_ASSERT_FALSE

/** @def LIMES_ASSERT
	Evaluates the condition, and if it is false, logs an assertion failure message and stops in the debugger if one is attached.
	This macro can be used in constexpr functions, but assertion failures will not be caught or logged if the function is evaluated at compile time.
	In release builds, this macro expands to nothing.
	@see LIMES_ASSERT_FALSE
	@ingroup preprocessor
 */
#	define LIMES_ASSERT(condition)

/** @def LIMES_UNREACHABLE
	Invokes undefined behavior.
	The use case of this macro is to indicate to the compiler and to static analysis tools that a control path is intended to be unreachable.
	If a code path should not be reached, but may be in some cases, use \c LIMES_ASSERT_FALSE instead.
	@see LIMES_ASSERT_FALSE
	@ingroup preprocessor
 */
#	define LIMES_UNREACHABLE

#endif /* DOXYGEN */

/// @cond

#if LIMES_DEBUG
#	define LIMES_ASSERT_FALSE		LIMES_BLOCK_WITH_FORCED_SEMICOLON (::limes::assert::fire_assertion (__FILE__, LIMES_CURRENT_FUNCTION_NAME, __LINE__, nullptr);)
#	define LIMES_ASSERT(condition) LIMES_BLOCK_WITH_FORCED_SEMICOLON (if (! (condition))::limes::assert::fire_assertion (__FILE__, LIMES_CURRENT_FUNCTION_NAME, __LINE__, LIMES_MAKE_STRING (condition));)
#else
#	define LIMES_ASSERT_FALSE
#	define LIMES_ASSERT(condition) LIMES_BLOCK_WITH_FORCED_SEMICOLON ([[maybe_unused]] auto limes_assert_result = (condition); ::limes::misc::ignore_unused (limes_assert_result);)
#endif

#ifdef __cpp_lib_unreachable
#	if __cpp_lib_unreachable >= 202202L
#		define LIMES_UNREACHABLE LIMES_BLOCK_WITH_FORCED_SEMICOLON (::std::unreachable())
#	endif
#endif

#ifndef LIMES_UNREACHABLE
#	ifdef __has_builtin
#		if __has_builtin(__builtin_unreachable)
#			define LIMES_UNREACHABLE LIMES_BLOCK_WITH_FORCED_SEMICOLON (__builtin_unreachable();)
#		endif
#	endif
#endif

#ifndef LIMES_UNREACHABLE
#	define LIMES_UNREACHABLE LIMES_BLOCK_WITH_FORCED_SEMICOLON (LIMES_ASSERT_FALSE;)
#endif

/// @endcond
