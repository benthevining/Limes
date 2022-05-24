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
#include "../build_date.h"

LIMES_BEGIN_NAMESPACE

namespace time
{

constexpr Date::Date (const Year& y, const Month& m, int d) noexcept
	: year (y), month (m), dayOfMonth (d)
{
	LIMES_ASSERT (dayOfMonth >= 1 && dayOfMonth <= month.getNumDays (year.isLeap()));
}

constexpr Date::Date (int y, int m, int d) noexcept
	: year (y), month (m), dayOfMonth (d)
{
	LIMES_ASSERT (dayOfMonth >= 1 && dayOfMonth <= month.getNumDays (year.isLeap()));
}

constexpr Date::Date (const std::tm& timeObj) noexcept
	: year (timeObj), month (timeObj), dayOfMonth (timeObj.tm_mday)
{
}

template <Clock ClockType>
Date::Date (const Point<ClockType>& timePoint)
	: Date (toTimeObj (timePoint))
{
}

constexpr Year Date::getYear() const noexcept
{
	return year;
}

constexpr Month Date::getMonth() const noexcept
{
	return month;
}

constexpr int Date::getDayOfMonth() const noexcept
{
	return dayOfMonth;
}

template <bool StartWeekOnSunday>
constexpr Weekday<StartWeekOnSunday> Date::getWeekday() const noexcept
{
	return Weekday<StartWeekOnSunday> { year, month, dayOfMonth };
}

constexpr int Date::getDayOfYear() const noexcept
{
	const auto isLeap = year.isLeap();

	auto res = 0;

	for (auto i = 1; i < month.getMonthNumber(); ++i)
		res += Month { i }.getNumDays (isLeap);

	res += dayOfMonth;

	return res;
}

constexpr int Date::getWeekNumber() const noexcept
{
	const auto dayOfYear = getDayOfYear();
	const auto weekday	 = getWeekday<false>();

	if (dayOfYear < 8)
	{
		const auto daysUntilFirstMonday = [&weekday]
		{
			const auto firstWeekday = weekday.getWeekdayName();

			using Day = Weekday<false>::Name;

			if (firstWeekday == Day::Friday)
				return 3;

			if (firstWeekday == Day::Saturday)
				return 2;

			if (firstWeekday == Day::Sunday)
				return 1;

			return -1;
		}();

		if (daysUntilFirstMonday > 0 && dayOfYear <= daysUntilFirstMonday)
			return -1;
	}

	const auto dayOfWeek = weekday.daysSinceStartOfWeek();

	auto jan1wday = (dayOfWeek - dayOfYear) % 7;

	if (jan1wday < 0)
		jan1wday += 7;

	constexpr const int firstdays[7] = { 0, -1, -2, -3, -4, 2, 1 };

	const auto week1first = firstdays[jan1wday];

	if (dayOfYear < week1first)
		return 1;

	return ((dayOfYear - week1first) / 7) + 1;
}

constexpr bool Date::isBefore (const Date& other) const noexcept
{
	if (year > other.year)
		return false;

	if (month > other.month)
		return false;

	return dayOfMonth < other.dayOfMonth;
}

constexpr bool Date::isAfter (const Date& other) const noexcept
{
	if (year < other.year)
		return false;

	if (month < other.month)
		return false;

	return dayOfMonth > other.dayOfMonth;
}

constexpr bool Date::operator> (const Date& other) const noexcept
{
	return isAfter (other);
}

constexpr bool Date::operator< (const Date& other) const noexcept
{
	return isBefore (other);
}

constexpr bool Date::operator== (const Date& other) const noexcept
{
	return year == other.year && month == other.month && dayOfMonth == other.dayOfMonth;
}

constexpr bool Date::operator!= (const Date& other) const noexcept
{
	return ! (*this == other);
}

consteval Date Date::getCompilationDate() noexcept
{
	return Date { build_year(), build_month(), build_day() };
}

}  // namespace time

LIMES_END_NAMESPACE
