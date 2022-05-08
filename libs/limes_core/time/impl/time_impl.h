#pragma once

#include <limes_namespace.h>
#include "../build_date.h"

LIMES_BEGIN_NAMESPACE

namespace time
{

constexpr Time::Time (const Hour& h, int min, int sec) noexcept
: hour(h), minute(min), second(sec)
{
	minute %= 60;
	second %= 60;
}

constexpr Time::Time (int hourIn24HourFormat, int min, int sec) noexcept
: hour(hourIn24HourFormat), minute(min), second(sec)
{
	minute %= 60;
	second %= 60;
}

constexpr Time::Time (const std::tm& timeObj) noexcept
: hour(timeObj.tm_hour), minute(timeObj.tm_min), second(timeObj.tm_sec)
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

constexpr bool Time::operator>(const Time& other) const noexcept
{
	return isAfter (other);
}

constexpr bool Time::operator<(const Time& other) const noexcept
{
	return isBefore (other);
}

constexpr bool Time::operator==(const Time& other) const noexcept
{
	return hour == other.hour && minute == other.minute && second == other.second;
}

constexpr bool Time::operator!=(const Time& other) const noexcept
{
	return ! (*this == other);
}

consteval Time Time::getCompilationTime() noexcept
{
	return Time{ build_hour(), build_minute(), build_second() };
}

}

LIMES_END_NAMESPACE
