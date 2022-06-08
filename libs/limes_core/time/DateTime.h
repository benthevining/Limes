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
#include "time.h"
#include "date.h"
#include <ctime>
#include <string>
#include "time_utils.h"
#include "../system/compiler_defs.h"

/** @file
	This file defines the DateTime class.
	@ingroup time
 */

LIMES_BEGIN_NAMESPACE

namespace time
{

/** This class represents both a Date and a Time.
	@ingroup time
	@see Date, Time
 */
class LIMES_EXPORT DateTime final
{
public:

	/** @name Constructors */
	///@{
	/** Creates a DateTime object representing midnight on January 1, year 0. */
	constexpr DateTime() = default;

	/** Creates a DateTime object from a Date and Time. */
	explicit constexpr DateTime (const Date& d, const Time& t) noexcept;

	/** Creates a DateTime object from the given \c std::tm object. */
	explicit constexpr DateTime (const std::tm& timeObj) noexcept;

	/** Creates a DateTime object from the given \c std::time_t object. */
	explicit DateTime (std::time_t t);

	/** Creates a DateTime object from a \c std::chrono::time_point of any Clock type. */
	template <Clock ClockType>
	explicit DateTime (const Point<ClockType>& timePoint);
	///@}

	LIMES_CONSTEXPR_MOVABLE (DateTime)
	LIMES_CONSTEXPR_COPYABLE (DateTime)

	/** Converts this DateTime to a \c std::tm object. */
	[[nodiscard]] constexpr std::tm getStdTime() const noexcept;

	/** Converts this DateTime to a \c std::time_t object. */
	[[nodiscard]] std::time_t getTimeT() const;

	/** Converts this DateTime to a \c std::chrono::time_point object for the given Clock type. */
	template <Clock ClockType = SystemClock>
	[[nodiscard]] Point<ClockType> getTimePoint() const;

	/** @name Greater/less than comparisons */
	///@{
	[[nodiscard]] constexpr bool isBefore (const DateTime& other) const noexcept;
	[[nodiscard]] constexpr bool isAfter (const DateTime& other) const noexcept;
	[[nodiscard]] constexpr bool operator> (const DateTime& other) const noexcept;
	[[nodiscard]] constexpr bool operator< (const DateTime& other) const noexcept;
	///@}

	/** @name Equality comparisons */
	///@{
	[[nodiscard]] constexpr bool operator== (const DateTime& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const DateTime& other) const noexcept;
	///@}

	/** Returns true if this %date and %time is in the past. */
	[[nodiscard]] bool isInPast() const noexcept;

	/** Returns true if this %date and %time is in the future. */
	[[nodiscard]] bool isInFuture() const noexcept;

	/** Returns a string representation of this %date and %time.
		The format of the string is \c "HH:MM:SS [AM|PM] Day Month, Year".
	 */
	[[nodiscard]] std::string toString (bool as24HourTime = true, bool shortMonthName = true) const;

	/** Returns a DateTime object representing the current %date and %time. */
	[[nodiscard]] static DateTime getCurrent();

	/** Returns a DateTime object representing the %date and %time this code was compiled. */
	[[nodiscard]] LIMES_PURE_FUNCTION static consteval DateTime getCompilation() noexcept;

private:

	Time time;
	Date date;
};

/*-----------------------------------------------------------------------------------------------------------------------*/

constexpr DateTime::DateTime (const Date& d, const Time& t) noexcept
	: time (t), date (d)
{
}

constexpr DateTime::DateTime (const std::tm& timeObj) noexcept
	: time (timeObj), date (timeObj)
{
}

template <Clock ClockType>
DateTime::DateTime (const Point<ClockType>& timePoint)
	: DateTime (toTimeObj (timePoint))
{
}

constexpr std::tm DateTime::getStdTime() const noexcept
{
	std::tm res;

	res.tm_sec	= time.getSecond();
	res.tm_min	= time.getMinute();
	res.tm_hour = time.getHour().hoursSinceMidnight();
	res.tm_mday = date.getDayOfMonth();
	res.tm_mon	= date.getMonth().getMonthNumber();
	res.tm_year = date.getYear().getYear() - 1900;
	res.tm_wday = date.getWeekday<true>().daysSinceStartOfWeek();
	res.tm_yday = date.getDayOfYear();

	// set to negative for "no information available"
	res.tm_isdst = -1;

	return res;
}

template <Clock ClockType>
Point<ClockType> DateTime::getTimePoint() const
{
	return fromTimeObj<ClockType> (getTimeT());
}

constexpr bool DateTime::isBefore (const DateTime& other) const noexcept
{
	if (date.isAfter (other.date))
		return false;

	return time.isBefore (other.time);
}

constexpr bool DateTime::isAfter (const DateTime& other) const noexcept
{
	if (date.isBefore (other.date))
		return false;

	return time.isAfter (other.time);
}

constexpr bool DateTime::operator> (const DateTime& other) const noexcept
{
	return isAfter (other);
}

constexpr bool DateTime::operator< (const DateTime& other) const noexcept
{
	return isBefore (other);
}

constexpr bool DateTime::operator== (const DateTime& other) const noexcept
{
	return date == other.date && time == other.time;
}

constexpr bool DateTime::operator!= (const DateTime& other) const noexcept
{
	return ! (*this == other);
}

consteval DateTime DateTime::getCompilation() noexcept
{
	return DateTime { Date::getCompilationDate(), Time::getCompilationTime() };
}

}  // namespace time

LIMES_END_NAMESPACE
