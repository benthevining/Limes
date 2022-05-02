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
#include "weekday.h"
#include "month.h"
#include "year.h"

LIMES_BEGIN_NAMESPACE

namespace time
{

class LIMES_EXPORT Date final
{
public:

	constexpr Date() = default;

	explicit constexpr Date (const Year& y, const Month& m, int d) noexcept;

	explicit constexpr Date (int y, int m, int d) noexcept;

	LIMES_CONSTEXPR_MOVABLE (Date);
	LIMES_CONSTEXPR_COPYABLE (Date);

	[[nodiscard]] constexpr Year  getYear() const noexcept;
	[[nodiscard]] constexpr Month getMonth() const noexcept;
	[[nodiscard]] constexpr int	  getDay() const noexcept;

	template <bool StartWeekOnSunday = true>
	[[nodiscard]] constexpr Weekday<StartWeekOnSunday> getWeekday() const noexcept;

	[[nodiscard]] bool isInPast() const noexcept;
	[[nodiscard]] bool isInFuture() const noexcept;

	[[nodiscard]] constexpr bool isBefore (const Date& other) const noexcept;
	[[nodiscard]] constexpr bool isAfter (const Date& other) const noexcept;

	[[nodiscard]] constexpr bool operator> (const Date& other) const noexcept;
	[[nodiscard]] constexpr bool operator< (const Date& other) const noexcept;

	[[nodiscard]] constexpr bool operator== (const Date& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Date& other) const noexcept;

	[[nodiscard]] static Date getCurrent();

	[[nodiscard]] static consteval Date getCompilationDate() noexcept;

private:

	Year  year;
	Month month;
	int	  dayOfMonth { 0 };
};

/*-------------------------------------------------------------------------------------------------------------------------------*/

constexpr Date::Date (const Year& y, const Month& m, int d) noexcept
	: year (y), month (m), dayOfMonth (d)
{
}

constexpr Date::Date (int y, int m, int d) noexcept
	: year (y), month (m), dayOfMonth (d)
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

constexpr int Date::getDay() const noexcept
{
	return dayOfMonth;
}

template <bool StartWeekOnSunday>
constexpr Weekday<StartWeekOnSunday> Date::getWeekday() const noexcept
{
	return Weekday<StartWeekOnSunday> { year, month, dayOfMonth };
}

bool Date::isInPast() const noexcept
{
	return isBefore (getCurrent());
}

bool Date::isInFuture() const noexcept
{
	return isAfter (getCurrent());
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
