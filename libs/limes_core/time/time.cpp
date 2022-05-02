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

#include <limes_namespace.h>
#include "year.h"
#include "weekday.h"
#include "month.h"
#include "date.h"
#include <chrono>
#include <ctime>

LIMES_BEGIN_NAMESPACE

namespace time
{

[[nodiscard]] static inline std::tm* getCurrentLocalTime()
{
	const auto now = std::chrono::system_clock::now();

	const auto timeT = std::chrono::system_clock::to_time_t (now);

	return std::localtime (&timeT);
}

Year Year::getCurrent()
{
	if (const auto* localtime = getCurrentLocalTime())
		return Year { localtime->tm_year + 1900 };

	return {};
}

Month Month::getCurrent()
{
	if (const auto* localtime = getCurrentLocalTime())
		return Month { localtime->tm_mon };

	return {};
}

template <bool StartWeekOnSunday>
Weekday<StartWeekOnSunday> Weekday<StartWeekOnSunday>::getCurrent()
{
	if (const auto* localtime = getCurrentLocalTime())
	{
		if constexpr (StartWeekOnSunday)
			return Weekday { localtime->tm_wday };
		else
			return Weekday { (localtime->tm_wday - 1) % 7 };
	}

	return {};
}

template class Weekday<true>;
template class Weekday<false>;


Date Date::getCurrent()
{
	if (const auto* localtime = getCurrentLocalTime())
		return Date { localtime->tm_year + 1900, localtime->tm_mon, localtime->tm_mday };

	return {};
}

}  // namespace time

LIMES_END_NAMESPACE
