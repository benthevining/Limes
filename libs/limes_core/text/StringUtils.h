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
#include <limes_platform.h>
#include <string_view>
#include <string>
#include <vector>

LIMES_BEGIN_NAMESPACE

namespace strings
{

LIMES_EXPORT [[nodiscard]] bool isNewline (const std::string_view& string);

LIMES_EXPORT [[nodiscard]] bool areSame (const char* string1, const char* string2);

LIMES_EXPORT [[nodiscard]] bool areSameIgnoringCase (const char* string1, const char* string2);

LIMES_EXPORT [[nodiscard]] bool areSameIgnoringCase (const std::string_view& lhs, const std::string_view& rhs);

LIMES_EXPORT [[nodiscard]] int length (const char* string);

LIMES_EXPORT void trim (std::string& string);

LIMES_EXPORT [[nodiscard]] std::string toUpper (const std::string_view& string);
LIMES_EXPORT [[nodiscard]] std::string toLower (const std::string_view& string);

LIMES_EXPORT [[nodiscard]] std::vector<std::string> split (const std::string_view& stringToSplit,
														   const std::string_view& delimiter,
														   bool					   includeDelimiterInResults = false);

LIMES_EXPORT [[nodiscard]] std::vector<std::string> splitAtWhitespace (const std::string_view& stringToSplit);

LIMES_EXPORT [[nodiscard]] std::vector<std::string> splitAtNewlines (const std::string_view& stringToSplit);

LIMES_EXPORT [[nodiscard]] std::string join (const std::vector<std::string>& strings, const std::string_view& delimiter);

LIMES_EXPORT [[nodiscard]] std::string joinWithWhitespace (const std::vector<std::string>& strings);

LIMES_EXPORT [[nodiscard]] std::string joinWithNewlines (const std::vector<std::string>& strings);

LIMES_EXPORT [[nodiscard]] constexpr std::string_view getNewline() noexcept;

LIMES_EXPORT static const std::string_view new_line { getNewline() };

LIMES_EXPORT [[nodiscard]] std::string getLimesASCII() noexcept;

LIMES_EXPORT void printLimesASCII();


constexpr std::string_view getNewline() noexcept
{
	using namespace std::literals::string_view_literals;

#if LIMES_WINDOWS
	return "\r\n"sv;
#else
	return "\n"sv;
#endif
}

}  // namespace strings

LIMES_END_NAMESPACE
