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
#include "../time.h"	  // for Time
#include "../DateTime.h"  // for DateTime

LIMES_BEGIN_NAMESPACE

namespace time
{

[[nodiscard]] static inline std::tm* getCurrentLocalTime()
{
	const auto now = std::chrono::system_clock::now();

	const auto timeT = std::chrono::system_clock::to_time_t (now);

	return std::localtime (&timeT);
}

/*-------------------------------------------------------------------------------------------------------------*/

#pragma mark Year

Year Year::getCurrent()
{
	if (const auto* localtime = getCurrentLocalTime())
		return Year { *localtime };

	return {};
}

bool Year::isInPast() const noexcept
{
	return (*this) < getCurrent();
}

bool Year::isInFuture() const noexcept
{
	return (*this) > getCurrent();
}

Year::Year (std::time_t time)
	: Year (*std::localtime (&time))
{
}

// NB the only reason this function isn't constexpr is to move it out of the Year class's header files,
// to prevent a cycle with the Weekday class's headers.
int Year::getNumWeeks() const noexcept
{
	using WD = Weekday<true>;

	const auto firstDay = WD { year, 1, 1 }.getWeekdayName();

	if (firstDay == WD::Name::Thursday)
		return 53;

	if (isLeap())
		if (firstDay == WD::Name::Wednesday)
			return 53;

	return 52;
}

/*-------------------------------------------------------------------------------------------------------------*/

#pragma mark Month

Month::Month (std::time_t time)
	: Month (*std::localtime (&time))
{
}

Month Month::getCurrent()
{
	if (const auto* localtime = getCurrentLocalTime())
		return Month { *localtime };

	return {};
}

/*-------------------------------------------------------------------------------------------------------------*/

#pragma mark Weekday

template <bool StartWeekOnSunday>
Weekday<StartWeekOnSunday> Weekday<StartWeekOnSunday>::getCurrent()
{
	if (const auto* localtime = getCurrentLocalTime())
		return Weekday { *localtime };

	return {};
}

template <bool StartWeekOnSunday>
Weekday<StartWeekOnSunday>::Weekday (std::time_t time)
	: Weekday (*std::localtime (&time))
{
}

template class Weekday<true>;
template class Weekday<false>;

/*-------------------------------------------------------------------------------------------------------------*/

#pragma mark Date

Date::Date (std::time_t time)
	: Date (*std::localtime (&time))
{
}

Date Date::getCurrent()
{
	if (const auto* localtime = getCurrentLocalTime())
		return Date { *localtime };

	return {};
}

/*-------------------------------------------------------------------------------------------------------------*/

#pragma mark Hour

Hour::Hour (std::time_t time)
	: Hour (*std::localtime (&time))
{
}

Hour Hour::getCurrent()
{
	if (const auto* localtime = getCurrentLocalTime())
		return Hour { *localtime };

	return {};
}

/*-------------------------------------------------------------------------------------------------------------*/

#pragma mark Time

Time::Time (std::time_t time)
	: Time (*std::localtime (&time))
{
}

Time Time::getCurrent()
{
	if (const auto* localtime = getCurrentLocalTime())
		return Time { *localtime };

	return {};
}

/*-------------------------------------------------------------------------------------------------------------*/

#pragma mark DateTime

DateTime DateTime::getCurrent()
{
	if (const auto* localtime = getCurrentLocalTime())
		return DateTime { *localtime };

	return {};
}

}  // namespace time

LIMES_END_NAMESPACE
