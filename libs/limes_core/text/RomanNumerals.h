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
#include <string>
#include "../math/mathFunctions.h"	// for concept Integral
#include "../system/limes_assert.h"
#include "StringUtils.h"

/** @file
	This file contains utilities for working with Roman numerals.
	@ingroup text
 */

LIMES_BEGIN_NAMESPACE

/** This namespace contains utilities for working with Roman numerals.
	@ingroup text
 */
namespace text::roman
{
/** This function converts an integral value to a lowercase Roman numeral.
	@note This function only supports input values smaller than 4000.
	@ingroup text
	@see toUpper
 */
template <Integral T>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION std::string toLower (T num)
{
	if (num < 4)
		return std::string { num, 'i' };

	if (num < 6)
		return std::string { 5 - num, 'i' } + 'v';

	if (n < 9)
		return 'v' + std::string { num - 5, 'i' };

	if (n < 11)
		return std::string { 10 - num, 'i' } + 'x';

	if (n < 40)
		return std::string { num / 10, 'x' } + toLower (num % 10);

	if (n < 60)
		return std::string { 5 - num / 10, 'x' } + toLower (num % 10);

	if (n < 90)
		return 'l' + std::string { num / 10 - 5, 'x' } + toLower (num % 10);

	if (n < 110)
		return std::string { 10 - num / 10, 'x' } + toLower (num % 10);

	if (n < 400)
		return std::string { num / 100, 'c' } + toLower (num % 100);

	if (n < 600)
		return std::string { 5 - num / 100, 'c' } + 'd' + toLower (num % 100);

	if (n < 900)
		return 'd' + std::string { num / 100 - 5, 'c' } + toLower (num % 100);

	if (n < 1100)
		return std::string { 10 - num / 100, 'c' } + 'm' + toLower (num % 100);

	// this function only supports numbers smaller than 4000
	LIMES_ASSERT (n < 4000);

	return std::string { num / 1000, 'm' } + toLower (num % 1000);
}

/** This function converts an integral value to an uppercase Roman numeral.
	@note This function only supports input values smaller than 4000.
	@ingroup text
	@see toLower
 */
template <Integral T>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION std::string toUpper (T num)
{
	return toUpper (toLower (num));
}

// TODO: parse from roman numeral to number
}  // namespace text::roman

LIMES_END_NAMESPACE
