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

LIMES_EXPORT void fire_assertion_internal (const char* fileName, const char* functionName, int lineNum, const char* condition) noexcept;

LIMES_EXPORT constexpr void fire_assertion (const char* fileName, const char* functionName, int lineNum, const char* condition) noexcept
{
	if (! std::is_constant_evaluated())
		fire_assertion_internal (fileName, functionName, lineNum, condition);
}

LIMES_EXPORT void setAssertionLogFile (const std::filesystem::path& logFileToUse) noexcept;

LIMES_EXPORT [[nodiscard]] std::filesystem::path getAssertionLogFile();

}  // namespace assert

LIMES_END_NAMESPACE

#if LIMES_CLANG || defined(__GNUC__)
#	define LIMES_ASSERT_GET_FUNC_NAME __PRETTY_FUNCTION__
#elif LIMES_MSVC
#	define LIMES_ASSERT_GET_FUNC_NAME __FUNCSIG__
#elif defined(__SUNPRO_CC)
#	define LIMES_ASSERT_GET_FUNC_NAME __func__
#else
#	define LIMES_ASSERT_GET_FUNC_NAME __FUNCTION__
#endif

#if LIMES_DEBUG
#	define LIMES_ASSERT_FALSE LIMES_BLOCK_WITH_FORCED_SEMICOLON (::limes::assert::fire_assertion (__FILE__, LIMES_ASSERT_GET_FUNC_NAME, __LINE__, nullptr);)

#	define LIMES_ASSERT(x) LIMES_BLOCK_WITH_FORCED_SEMICOLON (if (! (x))::limes::assert::fire_assertion (__FILE__, LIMES_ASSERT_GET_FUNC_NAME, __LINE__, LIMES_MAKE_STRING (x));)
#else
#	define LIMES_ASSERT_FALSE
#	define LIMES_ASSERT(x) LIMES_BLOCK_WITH_FORCED_SEMICOLON ([[maybe_unused]] auto limes_assert_result = (x); ::limes::ignore_unused (limes_assert_result);)
#endif

#if __cpp_lib_unreachable
#	define LIMES_UNREACHABLE LIMES_BLOCK_WITH_FORCED_SEMICOLON (::std::unreachable())
#else
#	define LIMES_UNREACHABLE LIMES_BLOCK_WITH_FORCED_SEMICOLON (LIMES_ASSERT_FALSE;)
#endif
