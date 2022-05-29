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

#include "../build_date.h"
#include <limes_namespace.h>
#include <limes_platform.h>

/** @file
	This file contains implementation details for the Weekday class.
	@ingroup time
	@see weekday.h
 */

LIMES_BEGIN_NAMESPACE

namespace time
{

/// @cond

template <bool StartWeekOnSunday>
constexpr Weekday<StartWeekOnSunday>::Weekday (int daysSinceStartOfWeek) noexcept
	: dayOfWeek (daysSinceStartOfWeek)
{
	dayOfWeek %= 7;
}

template <bool StartWeekOnSunday>
constexpr Weekday<StartWeekOnSunday>::Weekday (int year, int month, int day) noexcept
{
	LIMES_ASSERT (month >= 1 && month <= 12);
	LIMES_ASSERT (day >= 1 && day <= Month { month }.getNumDays (Year { year }.isLeap()));

	constexpr const int month_table[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };

	if (month < 4)
		--year;

	dayOfWeek = (year + year / 4 - year / 100 + year / 400 + month_table[month - 1] + day) % 7;

	if constexpr (! StartWeekOnSunday)
		dayOfWeek -= 1;

	dayOfWeek %= 7;
}

template <bool StartWeekOnSunday>
constexpr Weekday<StartWeekOnSunday>::Weekday (const Year& year, const Month& month, int day) noexcept
	: Weekday (year.getYear(), month.getMonthNumber(), day)
{
}

template <bool StartWeekOnSunday>
constexpr Weekday<StartWeekOnSunday>::Weekday (Name day) noexcept
	: dayOfWeek (static_cast<int> (day))
{
	if constexpr (! StartWeekOnSunday)
		dayOfWeek = (dayOfWeek - 1) % 7;
}

template <>
constexpr Weekday<true>::Weekday (const Weekday<false>& other) noexcept
	: Weekday ((other.daysSinceStartOfWeek() - 1) % 7)
{
}

template <>
constexpr Weekday<false>::Weekday (const Weekday<true>& other) noexcept
	: Weekday ((other.daysSinceStartOfWeek() + 1) % 7)
{
}

template <bool StartWeekOnSunday>
constexpr Weekday<StartWeekOnSunday>::Weekday (const std::tm& timeObj) noexcept
	: dayOfWeek (timeObj.tm_wday)
{
	if constexpr (! StartWeekOnSunday)
		dayOfWeek = (dayOfWeek - 1) % 7;
}

template <bool StartWeekOnSunday>
constexpr typename Weekday<StartWeekOnSunday>::Name Weekday<StartWeekOnSunday>::getWeekdayName() const noexcept
{
	if constexpr (StartWeekOnSunday)
		return static_cast<Name> (dayOfWeek);
	else
		return static_cast<Name> ((dayOfWeek + 1) % 7);
}

template <bool StartWeekOnSunday>
constexpr int Weekday<StartWeekOnSunday>::daysSinceStartOfWeek() const noexcept
{
	return dayOfWeek;
}

template <bool StartWeekOnSunday>
constexpr Weekday<StartWeekOnSunday>& Weekday<StartWeekOnSunday>::operator++() noexcept
{
	dayOfWeek = (dayOfWeek + 1) % 7;
	return *this;
}

template <bool StartWeekOnSunday>
constexpr Weekday<StartWeekOnSunday>& Weekday<StartWeekOnSunday>::operator--() noexcept
{
	dayOfWeek = (dayOfWeek - 1) % 7;
	return *this;
}

template <bool StartWeekOnSunday>
constexpr Weekday<StartWeekOnSunday>& Weekday<StartWeekOnSunday>::operator+= (int numDaysToAdd) noexcept
{
	dayOfWeek = (dayOfWeek + numDaysToAdd) % 7;
	return *this;
}

template <bool StartWeekOnSunday>
constexpr Weekday<StartWeekOnSunday>& Weekday<StartWeekOnSunday>::operator-= (int numDaysToSubtract) noexcept
{
	dayOfWeek = (dayOfWeek - numDaysToSubtract) % 7;
	return *this;
}

template <bool StartWeekOnSunday>
constexpr Weekday<StartWeekOnSunday> Weekday<StartWeekOnSunday>::operator+ (int numDaysToAdd) const noexcept
{
	return Weekday { (dayOfWeek + numDaysToAdd) % 7 };
}

template <bool StartWeekOnSunday>
constexpr Weekday<StartWeekOnSunday> Weekday<StartWeekOnSunday>::operator- (int numDaysToSubtract) const noexcept
{
	return Weekday { (dayOfWeek - numDaysToSubtract) % 7 };
}

template <bool StartWeekOnSunday>
constexpr bool Weekday<StartWeekOnSunday>::operator> (const Weekday& other) const noexcept
{
	return dayOfWeek > other.dayOfWeek;
}

template <bool StartWeekOnSunday>
constexpr bool Weekday<StartWeekOnSunday>::operator< (const Weekday& other) const noexcept
{
	return dayOfWeek < other.dayOfWeek;
}

template <>
constexpr bool Weekday<true>::operator< (const Weekday<false>& other) const noexcept
{
	return (dayOfWeek + 1) % 7 < other.daysSinceStartOfWeek();
}

template <>
constexpr bool Weekday<false>::operator< (const Weekday<true>& other) const noexcept
{
	return (dayOfWeek - 1) % 7 < other.daysSinceStartOfWeek();
}

template <>
constexpr bool Weekday<true>::operator> (const Weekday<false>& other) const noexcept
{
	return (dayOfWeek + 1) % 7 > other.daysSinceStartOfWeek();
}

template <>
constexpr bool Weekday<false>::operator> (const Weekday<true>& other) const noexcept
{
	return (dayOfWeek - 1) % 7 > other.daysSinceStartOfWeek();
}

template <bool StartWeekOnSunday>
constexpr bool Weekday<StartWeekOnSunday>::operator== (const Weekday& other) const noexcept
{
	return dayOfWeek == other.dayOfWeek;
}

template <bool StartWeekOnSunday>
constexpr bool Weekday<StartWeekOnSunday>::operator!= (const Weekday& other) const noexcept
{
	return dayOfWeek != other.dayOfWeek;
}

template <>
constexpr bool Weekday<true>::operator== (const Weekday<false>& other) const noexcept
{
	return (dayOfWeek + 1) % 7 == other.daysSinceStartOfWeek();
}

template <>
constexpr bool Weekday<false>::operator== (const Weekday<true>& other) const noexcept
{
	return (dayOfWeek - 1) % 7 == other.daysSinceStartOfWeek();
}

template <bool StartWeekOnSunday>
constexpr bool Weekday<StartWeekOnSunday>::operator!= (const OtherWeekdayType& other) const noexcept
{
	return ! (*this == other);
}

template <bool StartWeekOnSunday>
constexpr bool Weekday<StartWeekOnSunday>::isWeekend() const noexcept
{
	const auto day = getWeekdayName();

	return day == Name::Saturday || day == Name::Sunday;
}

template <bool StartWeekOnSunday>
constexpr bool Weekday<StartWeekOnSunday>::isWeekday() const noexcept
{
	return ! isWeekend();
}

template <bool StartWeekOnSunday>
constexpr std::string_view Weekday<StartWeekOnSunday>::getString (bool useShort) const noexcept
{
	constexpr const std::string_view full_days[7] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

	constexpr const std::string_view short_days[7] = { "Sun", "Mon", "Tues", "Wed", "Thurs", "Fri", "Sat" };

	const auto idx = [w = dayOfWeek]
	{
		if constexpr (StartWeekOnSunday)
			return w;

		return w + 1;
	}() % 7;

	if (useShort)
		return short_days[idx];

	return full_days[idx];
}

template <bool StartWeekOnSunday>
constexpr typename Weekday<StartWeekOnSunday>::OtherWeekdayType Weekday<StartWeekOnSunday>::toOtherWeekdayType() const noexcept
{
	if constexpr (StartWeekOnSunday)
		return OtherWeekdayType { (dayOfWeek - 1) % 7 };

	return OtherWeekdayType { (dayOfWeek + 1) % 7 };
}

template <bool StartWeekOnSunday>
consteval Weekday<StartWeekOnSunday> Weekday<StartWeekOnSunday>::getCompilationWeekday() noexcept
{
	return Weekday { build_year(), build_month(), build_day() };
}

template <bool StartWeekOnSunday>
consteval bool Weekday<StartWeekOnSunday>::startsWeekOnSunday() noexcept
{
	return StartWeekOnSunday;
}

/// @endcond

}  // namespace time

LIMES_END_NAMESPACE
