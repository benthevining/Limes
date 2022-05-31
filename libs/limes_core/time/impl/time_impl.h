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
#include "../build_date.h"

/** @dir libs/limes_core/time/impl
	This directory contains implementation details for the various %time classes.
	@ingroup time
 */

/** @file
	This file contains implementation details for the Time class.
	@ingroup time
	@see time.h
 */

LIMES_BEGIN_NAMESPACE

namespace time
{

/// @cond

constexpr Time::Time (const Hour& h, int min, int sec) noexcept
	: hour (h), minute (min), second (sec)
{
	minute %= 60;
	second %= 60;
}

constexpr Time::Time (int hourIn24HourFormat, int min, int sec) noexcept
	: hour (hourIn24HourFormat), minute (min), second (sec)
{
	minute %= 60;
	second %= 60;
}

constexpr Time::Time (const std::tm& timeObj) noexcept
	: hour (timeObj.tm_hour), minute (timeObj.tm_min), second (timeObj.tm_sec)
{
}

template <Clock ClockType>
Time::Time (const Point<ClockType>& timePoint)
	: Time (toTimeObj (timePoint))
{
}

constexpr bool Time::isAM() const noexcept
{
	return hour.isAM();
}

constexpr bool Time::isPM() const noexcept
{
	return hour.isPM();
}

constexpr bool Time::isOnTheHour() const noexcept
{
	return minute == 0 && second == 0;
}

constexpr Hour Time::getHour() const noexcept
{
	return hour;
}

constexpr int Time::getMinute() const noexcept
{
	return minute;
}

constexpr int Time::getSecond() const noexcept
{
	return second;
}

constexpr bool Time::isBefore (const Time& other) const noexcept
{
	if (hour > other.hour)
		return false;

	if (minute > other.minute)
		return false;

	return second < other.second;
}

constexpr bool Time::isAfter (const Time& other) const noexcept
{
	if (hour < other.hour)
		return false;

	if (minute < other.minute)
		return false;

	return second > other.second;
}

constexpr bool Time::operator> (const Time& other) const noexcept
{
	return isAfter (other);
}

constexpr bool Time::operator< (const Time& other) const noexcept
{
	return isBefore (other);
}

constexpr bool Time::operator== (const Time& other) const noexcept
{
	return hour == other.hour && minute == other.minute && second == other.second;
}

constexpr bool Time::operator!= (const Time& other) const noexcept
{
	return ! (*this == other);
}

consteval Time Time::getCompilationTime() noexcept
{
	return Time { build_hour(), build_minute(), build_second() };
}

/// @endcond

}  // namespace time

LIMES_END_NAMESPACE
