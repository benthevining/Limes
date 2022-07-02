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
#include <chrono>		  // for system_clock
#include <ctime>		  // for tm, localtime, time_t
#include <sstream>		  // for char_traits, operator<<, basic_ostream
#include "../date.h"	  // for Date
#include "../month.h"	  // for Month
#include "../weekday.h"	  // for Weekday
#include "../year.h"	  // for Year
#include "../hour.h"	  // for Hour
#include "../time.h"	  // IWYU pragma: associated
#include "../DateTime.h"  // for DateTime

LIMES_BEGIN_NAMESPACE

namespace time
{

Time::Time (std::time_t time)
	: Time (*std::localtime (&time))  // NOLINT
{
}

bool Time::isInPast() const noexcept
{
	return isBefore (getCurrent());
}

bool Time::isInFuture() const noexcept
{
	return isAfter (getCurrent());
}

std::string Time::toString (bool as24HourTime) const
{
	std::stringstream stream;

	const auto hourNum = as24HourTime ? hour.hoursSinceMidnight() : hour.getIn12HourFormat();

	if (hourNum < 10)
		stream << 0;

	stream << hourNum << ':';

	if (minute < 10)
		stream << 0;

	stream << minute << ':';

	if (second < 10)
		stream << 0;

	stream << second;

	if (! as24HourTime)
	{
		stream << ' ';

		if (isAM())
			stream << "AM";
		else
			stream << "PM";
	}

	return stream.str();
}

Time::Time (const Hour& h, int min, int sec) noexcept
	: hour (h), minute (min % 60), second (sec % 60)
{
}

Time::Time (int hourIn24HourFormat, int min, int sec) noexcept
	: Time (Hour { hourIn24HourFormat }, min, sec)
{
}

Time::Time (const std::tm& timeObj) noexcept
	: Time (Hour { timeObj.tm_hour }, timeObj.tm_min, timeObj.tm_sec)
{
}

bool Time::isAM() const noexcept
{
	return hour.isAM();
}

bool Time::isPM() const noexcept
{
	return hour.isPM();
}

bool Time::isOnTheHour() const noexcept
{
	return minute == 0 && second == 0;
}

Hour Time::getHour() const noexcept
{
	return hour;
}

int Time::getMinute() const noexcept
{
	return minute;
}

int Time::getSecond() const noexcept
{
	return second;
}

bool Time::isBefore (const Time& other) const noexcept
{
	if (hour > other.hour)
		return false;

	if (minute > other.minute)
		return false;

	return second < other.second;
}

bool Time::isAfter (const Time& other) const noexcept
{
	if (hour < other.hour)
		return false;

	if (minute < other.minute)
		return false;

	return second > other.second;
}

bool Time::operator> (const Time& other) const noexcept
{
	return isAfter (other);
}

bool Time::operator<(const Time& other) const noexcept
{
	return isBefore (other);
}

bool Time::operator== (const Time& other) const noexcept
{
	return hour == other.hour && minute == other.minute && second == other.second;
}

bool Time::operator!= (const Time& other) const noexcept
{
	return ! (*this == other);
}

Time Time::getCompilationTime() noexcept
{
	return Time { build_hour(), build_minute(), build_second() };
}

}  // namespace time

LIMES_END_NAMESPACE
