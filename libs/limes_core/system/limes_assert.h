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
#include <limes_build_type.h>
#include "../misc/IgnoreUnused.h"
#include <limes_platform.h>
#include <filesystem>

LIMES_BEGIN_NAMESPACE

namespace assert
{

/** The internal implementation of firing an assertion failure.  This function is intended for internal use by the \c LIMES_ASSERT macro.
	@see LIMES_ASSERT
 */
LIMES_EXPORT void fire_assertion_internal (const char* fileName, const char* functionName, int lineNum, const char* condition) noexcept;

/** Fires an assertion failure. This function is intended for internal use by the \c LIMES_ASSERT macro.
	@see LIMES_ASSERT
 */
LIMES_EXPORT constexpr void fire_assertion (const char* fileName, const char* functionName, int lineNum, const char* condition) noexcept
{
	if (! std::is_constant_evaluated())
		fire_assertion_internal (fileName, functionName, lineNum, condition);
}

/** Sets a filepath to be used to log assertion failures for the current process.
	When a log file is being used, assertion failures will still stop the debugger and be printed to standard error.
	The assertion log file can also be controlled by the environment variable \c LIMES_ASSERTION_LOG_FILE .
	@see getAssertionLogFile
 */
LIMES_EXPORT void setAssertionLogFile (const std::filesystem::path& logFileToUse) noexcept;

/** Returns the path to the current assertion log file, if any.
	@see setAssertionLogFile
 */
LIMES_EXPORT [[nodiscard]] std::filesystem::path getAssertionLogFile();

}  // namespace assert

LIMES_END_NAMESPACE

#ifdef DOXYGEN

/** This macro, if reached within the control flow, always causes an assertion failure, and stops in the debugger if one is attached.
	This macro can be used in constexpr functions, but assertion failures will not be caught or logged if the function is evaluated at compile time.
	In release builds, this macro expands to nothing.
	@see LIMES_ASSERT, LIMES_UNREACHABLE
 */
#	define LIMES_ASSERT_FALSE

/** Evaluates the condition, and if it is false, logs an assertion failure message and stops in the debugger if one is attached.
	This macro can be used in constexpr functions, but assertion failures will not be caught or logged if the function is evaluated at compile time.
	In release builds, this macro expands to nothing.
	@see LIMES_ASSERT_FALSE
 */
#	define LIMES_ASSERT(condition)

/** Invokes undefined behavior.
	The use case of this macro is to indicate to the compiler and to static analysis tools that a control path is intended to be unreachable.
	If a code path should not be reached, but may be in some cases, use \c LIMES_ASSERT_FALSE instead.
	@see LIMES_ASSERT_FALSE
 */
#	define LIMES_UNREACHABLE

#else

#	if LIMES_CLANG || defined(__GNUC__)
#		define LIMES_ASSERT_GET_FUNC_NAME __PRETTY_FUNCTION__
#	elif LIMES_MSVC
#		define LIMES_ASSERT_GET_FUNC_NAME __FUNCSIG__
#	elif defined(__SUNPRO_CC)
#		define LIMES_ASSERT_GET_FUNC_NAME __func__
#	else
#		define LIMES_ASSERT_GET_FUNC_NAME __FUNCTION__
#	endif

#	if LIMES_DEBUG
#		define LIMES_ASSERT_FALSE LIMES_BLOCK_WITH_FORCED_SEMICOLON (::limes::assert::fire_assertion (__FILE__, LIMES_ASSERT_GET_FUNC_NAME, __LINE__, nullptr);)

#		define LIMES_ASSERT(condition) LIMES_BLOCK_WITH_FORCED_SEMICOLON (if (! (condition))::limes::assert::fire_assertion (__FILE__, LIMES_ASSERT_GET_FUNC_NAME, __LINE__, LIMES_MAKE_STRING (condition));)
#	else
#		define LIMES_ASSERT_FALSE
#		define LIMES_ASSERT(x) LIMES_BLOCK_WITH_FORCED_SEMICOLON ([[maybe_unused]] auto limes_assert_result = (x); ::limes::ignore_unused (limes_assert_result);)
#	endif

#	if __cpp_lib_unreachable
#		define LIMES_UNREACHABLE LIMES_BLOCK_WITH_FORCED_SEMICOLON (::std::unreachable())
#	else
#		define LIMES_UNREACHABLE LIMES_BLOCK_WITH_FORCED_SEMICOLON (LIMES_ASSERT_FALSE;)
#	endif

#endif /* ifdef DOXYGEN */