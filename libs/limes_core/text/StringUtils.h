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

#include <limes_export.h>	  // for LIMES_EXPORT
#include <limes_namespace.h>  // for LIMES_BEGIN_NAMESPACE, LIMES_END_NAMESPACE
#include <limes_platform.h>	  // for LIMES_WINDOWS
#include <string>			  // for string
#include <string_view>		  // for string_view, operator""sv, string_view_...
#include <vector>			  // for vector
#include "../system/compiler_defs.h"

/** @defgroup text Text
	Utilities for working with text and strings.
	@ingroup limes_core

	@todo function to convert a string or number to ordinal ("one" -> "first", "five" -> "fifth")
	@todo write unit tests
 */

/** @dir libs/limes_core/text
	This directory contains utilities for working with strings.
	@ingroup text
 */

/** @file
	This file contains free functions for working with strings.
	@ingroup text
 */

LIMES_BEGIN_NAMESPACE

/** This namespace contains some text manipulation utilities for working with both C++ and C-style strings.
	@ingroup text
 */
namespace text
{

/** @ingroup text
	@{
 */

/** Returns true if the passed string is \c \\n or \c \\r\\n . */
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION bool isNewline (const std::string_view& string) noexcept;

/** Returns true if the two strings contain exactly the same characters. */
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION bool areSame (const char* string1, const char* string2);

/** Returns true if the two strings are the same, regardless of the case of their characters. */
///@{
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION bool areSameIgnoringCase (const char* string1, const char* string2);
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION bool areSameIgnoringCase (const std::string_view& lhs, const std::string_view& rhs);
///@}

/** Returns the length of the string. */
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION int length (const char* string);

/** Removes any whitespace characters from the beginning and end of the string. */
LIMES_EXPORT void trim (std::string& string);

/** Removes a specified number of characters from the beginning of the string. */
LIMES_EXPORT void dropFirstChars (std::string& string, std::size_t numChars);

/** Removes a specified number of characters from the end of the string. */
LIMES_EXPORT void dropLastChars (std::string& string, std::size_t numChars);

/** Adds \c " characters to the beginning and end of the string, if they aren't already there. */
LIMES_EXPORT [[nodiscard]] std::string quoted (const std::string_view& string);

/** Removes \c " characters from the beginning and end of the string, if they were there. */
LIMES_EXPORT [[nodiscard]] std::string unquoted (const std::string_view& string);

/** Returns a copy of the string, with all its characters converted to upper case. */
LIMES_EXPORT [[nodiscard]] std::string toUpper (const std::string_view& string);

/** Returns a copy of the string, with all its characters converted to lower case. */
LIMES_EXPORT [[nodiscard]] std::string toLower (const std::string_view& string);

/** Returns a copy of the passed string, ending at the first occurrence of \c stringToFind .
	@param inputString The input string to slice
	@param stringToFind The delimiter to split the output string at.
 */
LIMES_EXPORT [[nodiscard]] std::string upToFirstOccurrenceOf (const std::string_view& inputString,
															  const std::string_view& stringToFind);

/** Returns a copy of the passed string, beginning at the first occurrence of \c stringToFind .
	@param inputString The input string to slice
	@param stringToFind The delimiter to split the output string at.
 */
LIMES_EXPORT [[nodiscard]] std::string fromFirstOccurrenceOf (const std::string_view& inputString,
															  const std::string_view& stringToFind);

/** Takes an input string and splits it into a series of smaller strings.
	@param stringToSplit The input string to slice
	@param delimiter The string to use as the delimiter between smaller tokens.
	@param includeDelimiterInResults If true, each output token will end with \c delimiter .
 */
LIMES_EXPORT [[nodiscard]] std::vector<std::string> split (const std::string_view& stringToSplit,
														   const std::string_view& delimiter,
														   bool					   includeDelimiterInResults = false);

/** Splits the input string using whitespace characters as the delimiter, not including the whitespace in the output tokens.
	Each output token is also trimmed, meaning that if multiple spaces are between words, the output tokens will still contain only characters.
 */
LIMES_EXPORT [[nodiscard]] std::vector<std::string> splitAtWhitespace (const std::string_view& stringToSplit);

/** Splits the input string into lines.
	The input string can use \c \\n or \c \\r\\n for its newline characters, or a mixture of both.
 */
LIMES_EXPORT [[nodiscard]] std::vector<std::string> splitAtNewlines (const std::string_view& stringToSplit);

/** Joins the input strings into one string, placing \c delimiter between each input string.
	@param strings Strings to be joined
	@param delimiter String that will be placed between each input string in the output string
 */
LIMES_EXPORT [[nodiscard]] std::string join (const std::vector<std::string>& strings, const std::string_view& delimiter);

/** Joins the strings together, using a space character as the token delimiter. */
LIMES_EXPORT [[nodiscard]] std::string joinWithWhitespace (const std::vector<std::string>& strings);

/** Joins the strings together, using the platform's preferred newline character as the token delimiter. */
LIMES_EXPORT [[nodiscard]] std::string joinWithNewlines (const std::vector<std::string>& strings);

/** Returns the platform's preferred newline separator: \c \\r\\n on Windows, \c \\n everywhere else.
	@see new_line
 */
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION constexpr std::string_view getNewline() noexcept;

/** A static variable that can be referenced for convenience, instead of needing to call \c getNewline() .
	@see getNewline
 */
LIMES_EXPORT static const std::string_view new_line { getNewline() };

/** Returns the Limes library ASCII logo as a string.
	@see printLimesASCII
 */
LIMES_EXPORT [[nodiscard]] std::string getLimesASCII() noexcept;

/** Prints the Limes library ASCII logo to std::cout.
	@see getLimesASCII
 */
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

/** @} */

}  // namespace text

LIMES_END_NAMESPACE
