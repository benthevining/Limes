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
#include "../DateTime.h"  // IWYU pragma: associated
#include <string>
#include <sstream>

LIMES_BEGIN_NAMESPACE

namespace time
{

DateTime::DateTime (const Date& d, const Time& t) noexcept
	: date (d), time (t)
{
}

DateTime::DateTime (const std::tm& timeObj) noexcept
	: date (timeObj), time (timeObj)
{
}

Date DateTime::getDate() const
{
	return date;
}

Time DateTime::getTime() const
{
	return time;
}

std::tm DateTime::getStdTime() const noexcept
{
	std::tm res;

	res.tm_sec	= time.getSecond();
	res.tm_min	= time.getMinute();
	res.tm_hour = time.getHour().hoursSinceMidnight();
	res.tm_mday = date.getDayOfMonth();
	res.tm_mon	= date.getMonth().getMonthNumber();
	res.tm_year = date.getYear().getYear() - 1900;
	res.tm_yday = date.getDayOfYear();

	res.tm_wday = date.getWeekday<true>().daysSinceStartOfWeek();

	// set to negative for "no information available"
	res.tm_isdst = -1;

	return res;
}

bool DateTime::isBefore (const DateTime& other) const noexcept
{
	if (date.isAfter (other.date))
		return false;

	return time.isBefore (other.time);
}

bool DateTime::isAfter (const DateTime& other) const noexcept
{
	if (date.isBefore (other.date))
		return false;

	return time.isAfter (other.time);
}

bool DateTime::operator> (const DateTime& other) const noexcept
{
	return isAfter (other);
}

bool DateTime::operator<(const DateTime& other) const noexcept
{
	return isBefore (other);
}

bool DateTime::operator== (const DateTime& other) const noexcept
{
	return date == other.date && time == other.time;
}

bool DateTime::operator!= (const DateTime& other) const noexcept
{
	return ! (*this == other);
}

DateTime DateTime::getCompilation() noexcept
{
	return DateTime { Date::getCompilationDate(), Time::getCompilationTime() };
}

DateTime::DateTime (std::time_t t)
	: DateTime (*std::localtime (&t))
{
}

std::time_t DateTime::getTimeT() const
{
	auto tm = getStdTime();

	return std::mktime (&tm);
}

bool DateTime::isInPast() const noexcept
{
	return isBefore (getCurrent());
}

bool DateTime::isInFuture() const noexcept
{
	return isAfter (getCurrent());
}

std::string DateTime::toString (bool as24HourTime, bool shortMonthName) const
{
	std::stringstream stream;

	stream << time.toString (as24HourTime) << ' ' << date.toString (shortMonthName);

	return stream.str();
}

}  // namespace time

LIMES_END_NAMESPACE
