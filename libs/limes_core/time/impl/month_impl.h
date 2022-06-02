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

#include <limes_namespace.h>
#include <limes_platform.h>
#include "../../math/mathFunctions.h"
#include "../build_date.h"

/** @file
	This file contains implementation details for the Month class.
	@ingroup time
	@see month.h
 */

LIMES_BEGIN_NAMESPACE

namespace time
{

/// @cond

constexpr Month::Month (int number) noexcept
	: monthNum (number)
{
	LIMES_ASSERT (monthNum >= 1 && monthNum <= 12);
}

constexpr Month::Month (Name monthName) noexcept
	: monthNum (static_cast<int> (monthName))
{
}

constexpr Month& Month::operator= (Name newMonth) noexcept
{
	monthNum = static_cast<int> (newMonth);
	return *this;
}

constexpr Month& Month::operator= (int newMonth) noexcept
{
	monthNum = newMonth;
	return *this;
}

constexpr int Month::getMonthNumber() const noexcept
{
	return monthNum;
}

constexpr Month::Name Month::getMonthName() const noexcept
{
	return static_cast<Name> (monthNum);
}

constexpr Month::Month (const std::tm& timeObj) noexcept
	: monthNum (timeObj.tm_mon)
{
}

constexpr int Month::getNumDays (bool leapYear) const noexcept
{
	if (monthNum == 2)
	{
		if (leapYear)
			return 29;

		return 28;
	}

	if (monthNum == 4 || monthNum == 6 || monthNum == 9 || monthNum == 11)
		return 30;

	return 31;
}

constexpr int Month::getNumWeeks (bool leapYear) const noexcept
{
	return math::round (static_cast<double> (getNumDays (leapYear)) / 7.);
}

constexpr Month& Month::operator++() noexcept
{
	if (monthNum == 12)
		monthNum = 1;
	else
		++monthNum;

	return *this;
}

constexpr Month& Month::operator--() noexcept
{
	if (monthNum == 1)
		monthNum = 12;
	else
		--monthNum;

	return *this;
}

constexpr Month& Month::operator++ (int) noexcept  // NOLINT
{
	return ++(*this);
}

constexpr Month& Month::operator-- (int) noexcept  // NOLINT
{
	return --(*this);
}

constexpr Month& Month::operator+= (int numMonthsToAdd) noexcept
{
	for (auto i = 0; i < numMonthsToAdd; ++i)
		++(*this);

	return *this;
}

constexpr Month& Month::operator-= (int numMonthsToSubtract) noexcept
{
	for (auto i = 0; i < numMonthsToSubtract; ++i)
		--(*this);

	return *this;
}

constexpr Month Month::operator+ (int numMonthsToAdd) const noexcept
{
	Month res { monthNum };

	res += numMonthsToAdd;

	return res;
}

constexpr Month Month::operator- (int numMonthsToSubtract) const noexcept
{
	Month res { monthNum };

	res -= numMonthsToSubtract;

	return res;
}

constexpr bool Month::operator> (const Month& other) const noexcept
{
	return monthNum > other.monthNum;
}

constexpr bool Month::operator< (const Month& other) const noexcept
{
	return monthNum < other.monthNum;
}

constexpr bool Month::operator> (Name other) const noexcept
{
	return monthNum > static_cast<int> (other);
}

constexpr bool Month::operator< (Name other) const noexcept
{
	return monthNum < static_cast<int> (other);
}

constexpr bool Month::operator== (const Month& other) const noexcept
{
	return monthNum == other.monthNum;
}

constexpr bool Month::operator!= (const Month& other) const noexcept
{
	return monthNum != other.monthNum;
}

constexpr bool Month::operator== (Name other) const noexcept
{
	return monthNum == static_cast<int> (other);
}

constexpr bool Month::operator!= (Name other) const noexcept
{
	return monthNum != static_cast<int> (other);
}

consteval Month Month::getCompilationMonth() noexcept
{
	return Month { build_month() };
}

constexpr std::string_view Month::getString (bool useShort) const noexcept
{
	constexpr const std::string_view months[12] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

	const auto result = months[monthNum - 1];

	if (useShort)
		return result.substr (0, 3);

	return result;
}

/// @endcond

}  // namespace time

LIMES_END_NAMESPACE
