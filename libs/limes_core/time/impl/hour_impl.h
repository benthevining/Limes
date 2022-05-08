#pragma once

#include <limes_namespace.h>
#include <limes_platform.h>
#include "../build_date.h"

LIMES_BEGIN_NAMESPACE

namespace time
{

constexpr Hour::Hour (int hoursSinceMidnight) noexcept
: hour(hoursSinceMidnight)
{
	LIMES_ASSERT (hour >= 0 && hour < 24);
}

constexpr Hour::Hour (int hourNumber, bool isAM) noexcept
: hour(hourNumber)
{
	if (! isAM)
		hour += 12;

	LIMES_ASSERT (hour >= 0 && hour < 24);
}

constexpr Hour::Hour (const std::tm& timeObj) noexcept
: hour(timeObj.tm_hour)
{

}

constexpr bool Hour::isAM() const noexcept
{
	return hour < 12;
}

constexpr bool Hour::isPM() const noexcept
{
	return hour >= 12;
}

constexpr bool Hour::isNoon() const noexcept
{
	return hour == 12;
}

constexpr bool Hour::isMidnight() const noexcept
{
	return hour == 0;
}

constexpr int Hour::hoursSinceMidnight() const noexcept
{
	return hour;
}

constexpr int Hour::getIn12HourFormat() const noexcept
{
	if (hour == 0)
		return 12;

	if (hour <= 12)
		return hour;

	return hour - 12;
}

constexpr Hour& Hour::operator++() noexcept
{
	hour = (hour + 1) % 24;
	return *this;
}

constexpr Hour& Hour::operator--() noexcept
{
	hour = (hour - 1) % 24;
	return *this;
}

constexpr Hour& Hour::operator+=(int hoursToAdd) noexcept
{
	hour = (hour + hoursToAdd) % 24;
	return *this;
}

constexpr Hour& Hour::operator-=(int hoursToSubtract) noexcept
{
	hour = (hour - hoursToSubtract) % 24;
	return *this;
}

constexpr Hour Hour::operator+ (int hoursToAdd) const noexcept
{
	return Hour { (hour + hoursToAdd) % 24 };
}

constexpr Hour Hour::operator- (int hoursToSubtract) const noexcept
{
	return Hour { (hour - hoursToSubtract) % 24 };
}

constexpr bool Hour::operator>(const Hour& other) const noexcept
{
	return hour > other.hour;
}

constexpr bool Hour::operator<(const Hour& other) const noexcept
{
	return hour < other.hour;
}

constexpr bool Hour::operator==(const Hour& other) const noexcept
{
	return hour == other.hour;
}

constexpr bool Hour::operator!=(const Hour& other) const noexcept
{
	return hour != other.hour;
}

consteval Hour Hour::getCompilationHour() noexcept
{
	return Hour { build_hour() };
}

constexpr Hour Hour::noon() noexcept
{
	return Hour { 12 };
}

constexpr Hour Hour::midnight() noexcept
{
	return Hour { 0 };
}

}

LIMES_END_NAMESPACE
