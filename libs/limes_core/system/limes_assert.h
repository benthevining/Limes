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

LIMES_BEGIN_NAMESPACE

namespace assert
{

LIMES_EXPORT [[nodiscard]] bool isRunningUnderDebugger() noexcept;

LIMES_EXPORT [[noreturn]] void break_in_debugger() noexcept;

LIMES_EXPORT void log_assertion (const char* fileName, int lineNum, const char* condition) noexcept;

LIMES_EXPORT constexpr void fire_assertion_failure() noexcept
{
	if (! std::is_constant_evaluated())
		if (isRunningUnderDebugger())
			break_in_debugger();
}

}  // namespace assert

LIMES_END_NAMESPACE


#if LIMES_DEBUG
#	define LIMES_ASSERT_FALSE LIMES_BLOCK_WITH_FORCED_SEMICOLON (::limes::assert::log_assertion (__FILE__, __LINE__, nullptr); ::limes::assert::fire_assertion_failure();)

#	define LIMES_ASSERT(x) LIMES_BLOCK_WITH_FORCED_SEMICOLON (if (! (x)) { ::limes::assert::log_assertion(__FILE__, __LINE__, LIMES_MAKE_STRING(x)); ::limes::assert::fire_assertion_failure(); })
#else
#	define LIMES_ASSERT_FALSE
#	define LIMES_ASSERT(x) LIMES_BLOCK_WITH_FORCED_SEMICOLON ([[maybe_unused]] auto limes_assert_result = (x); ::limes::ignore_unused (limes_assert_result);)
#endif


#if __cpp_lib_unreachable
#	define LIMES_UNREACHABLE LIMES_BLOCK_WITH_FORCED_SEMICOLON (::std::unreachable())
#else
#	define LIMES_UNREACHABLE LIMES_BLOCK_WITH_FORCED_SEMICOLON (::limes::assert::fire_assertion_failure();)
#endif
