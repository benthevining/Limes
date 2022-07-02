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

#include "../../limes_namespace.h"
#include <limes_platform.h>
#include "../build_date.h"
#include <string>
#include <sstream>
#include "../date.h"  // IWYU pragma: associated

LIMES_BEGIN_NAMESPACE

namespace time
{

Date::Date (const Year& y, const Month& m, int d) noexcept
	: year (y), month (m), dayOfMonth (d)
{
	LIMES_ASSERT (dayOfMonth > 0 && dayOfMonth <= month.getNumDays (year.isLeap()));
}

Date::Date (int y, int m, int d) noexcept
	: Date (Year { y }, Month { m }, d)
{
}

Date::Date (const std::tm& timeObj) noexcept
	: Date (Year { timeObj }, Month { timeObj }, timeObj.tm_mday)
{
}

template <bool StartWeekOnSunday>
Weekday<StartWeekOnSunday> Date::getWeekday() const noexcept
{
	return Weekday<StartWeekOnSunday> { year, month, dayOfMonth };
}

template Weekday<true>	Date::getWeekday() const noexcept;
template Weekday<false> Date::getWeekday() const noexcept;

Year Date::getYear() const noexcept
{
	return year;
}

Month Date::getMonth() const noexcept
{
	return month;
}

int Date::getDayOfMonth() const noexcept
{
	return dayOfMonth;
}

int Date::getDayOfYear() const noexcept
{
	const auto isLeap = year.isLeap();

	auto res = 0;

	for (auto i = 1; i < month.getMonthNumber(); ++i)
		res += Month { i }.getNumDays (isLeap);

	return res + dayOfMonth;
}

int Date::getWeekNumber() const noexcept
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

bool Date::isBefore (const Date& other) const noexcept
{
	if (year > other.year)
		return false;

	if (month > other.month)
		return false;

	return dayOfMonth < other.dayOfMonth;
}

bool Date::isAfter (const Date& other) const noexcept
{
	if (year < other.year)
		return false;

	if (month < other.month)
		return false;

	return dayOfMonth > other.dayOfMonth;
}

bool Date::isInPast() const noexcept
{
	return isBefore (getCurrent());
}

bool Date::isInFuture() const noexcept
{
	return isAfter (getCurrent());
}

bool Date::operator> (const Date& other) const noexcept
{
	return isAfter (other);
}

bool Date::operator<(const Date& other) const noexcept
{
	return isBefore (other);
}

bool Date::operator== (const Date& other) const noexcept
{
	return year == other.year && month == other.month && dayOfMonth == other.dayOfMonth;
}

bool Date::operator!= (const Date& other) const noexcept
{
	return ! (*this == other);
}

Date Date::getCompilationDate() noexcept
{
	return Date { build_year(), build_month(), build_day() };
}

std::string Date::toString (bool shortMonthName) const
{
	std::stringstream stream;

	if (dayOfMonth < 10)
		stream << 0;

	stream << dayOfMonth << ' ' << month.getString (shortMonthName) << ' ' << year.getYear();

	return stream.str();
}

}  // namespace time

LIMES_END_NAMESPACE
