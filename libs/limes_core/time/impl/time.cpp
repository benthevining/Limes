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

#include <limes_namespace.h>  // for LIMES_BEGIN_NAMESPACE, LIMES_END_NAMESPACE
#include <chrono>			  // for system_clock
#include <ctime>			  // for tm, localtime, time_t
#include <sstream>			  // for char_traits, operator<<, basic_ostream
#include "../date.h"		  // for Date
#include "../month.h"		  // for Month
#include "../weekday.h"		  // for Weekday
#include "../year.h"		  // for Year
#include "../hour.h"		  // for Hour
#include "../time.h"		  // for Time
#include "../DateTime.h"	  // for DateTime

LIMES_BEGIN_NAMESPACE

namespace time
{

Time::Time (std::time_t time)
	: Time (*std::localtime (&time))
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

	const auto hourNum = as24HourTime ? getHour().hoursSinceMidnight() : getHour().getIn12HourFormat();

	if (hourNum < 10)
		stream << 0;

	stream << hourNum << ':';

	const auto min = getMinute();

	if (min < 10)
		stream << 0;

	stream << min << ':';

	const auto sec = getSecond();

	if (sec < 10)
		stream << 0;

	stream << sec;

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
{
	min %= 60;
	sec %= 60;

	internal_data.set_minute (min);
	internal_data.set_second (sec);
	internal_data.set_hour (h.hoursSinceMidnight());
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
	return getHour().isAM();
}

bool Time::isPM() const noexcept
{
	return getHour().isPM();
}

bool Time::isOnTheHour() const noexcept
{
	return getMinute() == 0 && getSecond() == 0;
}

Hour Time::getHour() const noexcept
{
	return Hour { static_cast<int> (internal_data.hour()) };
}

int Time::getMinute() const noexcept
{
	return static_cast<int> (internal_data.minute());
}

int Time::getSecond() const noexcept
{
	return static_cast<int> (internal_data.second());
}

bool Time::isBefore (const Time& other) const noexcept
{
	if (getHour() > other.getHour())
		return false;

	if (getMinute() > other.getMinute())
		return false;

	return getSecond() < other.getSecond();
}

bool Time::isAfter (const Time& other) const noexcept
{
	if (getHour() < other.getHour())
		return false;

	if (getMinute() < other.getMinute())
		return false;

	return getSecond() > other.getSecond();
}

bool Time::operator> (const Time& other) const noexcept
{
	return isAfter (other);
}

bool Time::operator< (const Time& other) const noexcept
{
	return isBefore (other);
}

bool Time::operator== (const Time& other) const noexcept
{
	return getHour() == other.getHour() && getMinute() == other.getMinute() && getSecond() == other.getSecond();
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
