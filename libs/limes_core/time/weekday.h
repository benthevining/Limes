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
#include <limes_platform.h>
#include "year.h"
#include "month.h"
#include "../misc/preprocessor.h"
#include <string_view>
#include "build_date.h"
#include <chrono>
#include <ctime>
#include <type_traits>

LIMES_BEGIN_NAMESPACE

namespace time
{

template <bool StartWeekOnSunday = true>
class LIMES_EXPORT Weekday final
{
public:

	constexpr Weekday() = default;

	explicit constexpr Weekday (int daysSinceStartOfWeek) noexcept;

	explicit constexpr Weekday (int year, int month, int day) noexcept;

	explicit constexpr Weekday (const Year& year, const Month& month, int day) noexcept;

	using OtherWeekdayType = std::conditional_t<StartWeekOnSunday,
												Weekday<false>,
												Weekday<true>>;

	explicit constexpr Weekday (const OtherWeekdayType& other) noexcept;

	LIMES_CONSTEXPR_MOVABLE (Weekday);
	LIMES_CONSTEXPR_COPYABLE (Weekday);

	constexpr Weekday& operator++() noexcept;
	constexpr Weekday& operator--() noexcept;

	constexpr Weekday& operator+= (int numDaysToAdd) noexcept;
	constexpr Weekday& operator-= (int numDaysToSubtract) noexcept;

	[[nodiscard]] constexpr Weekday operator+ (int numDaysToAdd) const noexcept;
	[[nodiscard]] constexpr Weekday operator- (int numDaysToSubtract) const noexcept;

	[[nodiscard]] constexpr bool operator> (const Weekday& other) const noexcept;
	[[nodiscard]] constexpr bool operator< (const Weekday& other) const noexcept;

	[[nodiscard]] constexpr bool operator> (const OtherWeekdayType& other) const noexcept;
	[[nodiscard]] constexpr bool operator< (const OtherWeekdayType& other) const noexcept;

	[[nodiscard]] constexpr bool operator== (const Weekday& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Weekday& other) const noexcept;

	[[nodiscard]] constexpr bool operator== (const OtherWeekdayType& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const OtherWeekdayType& other) const noexcept;

	[[nodiscard]] constexpr int daysSinceStartOfWeek() const noexcept;

	[[nodiscard]] constexpr std::string_view getString (bool useShort = false) const noexcept;

	[[nodiscard]] constexpr OtherWeekdayType toOtherWeekdayType() const noexcept;

	[[nodiscard]] static Weekday getCurrent();

	[[nodiscard]] static consteval Weekday getCompilationWeekday() noexcept;

private:

	int dayOfWeek { 0 };
};

/*-------------------------------------------------------------------------------------------------------------------------------*/

template <bool StartWeekOnSunday>
constexpr Weekday<StartWeekOnSunday>::Weekday (int daysSinceStartOfWeek) noexcept
	: dayOfWeek (daysSinceStartOfWeek)
{
	LIMES_ASSERT (dayOfWeek >= 0 && dayOfWeek < 7);
}

template <bool StartWeekOnSunday>
constexpr Weekday<StartWeekOnSunday>::Weekday (int year, int month, int day) noexcept
{
	LIMES_ASSERT (month >= 0 && month < 12);
	LIMES_ASSERT (day >= 0 && day < Month { month }.getNumDays (Year { year }.isLeap()));

	constexpr const int month_table[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };

	if (month < 3)
		--year;

	dayOfWeek = (year + year / 4 - year / 100 + year / 400 + month_table[month - 1] + day) % 7;

	if constexpr (! StartWeekOnSunday)
		dayOfWeek = (dayOfWeek - 1) % 7;

	LIMES_ASSERT (dayOfWeek >= 0 && dayOfWeek < 7);
}

template <bool StartWeekOnSunday>
constexpr Weekday<StartWeekOnSunday>::Weekday (const Year& year, const Month& month, int day) noexcept
	: Weekday (year.getYear(), month.getMonthNumber(), day)
{
}

template <>
constexpr Weekday<true>::Weekday (const Weekday<false>& other) noexcept
	: Weekday ((other.daysSinceStartOfWeek() - 1) % 7)
{
}

template <>
constexpr Weekday<false>::Weekday (const Weekday<true>& other) noexcept
	: Weekday ((other.daysSinceStartOfWeek() + 1) % 7)
{
}

template <bool StartWeekOnSunday>
constexpr int Weekday<StartWeekOnSunday>::daysSinceStartOfWeek() const noexcept
{
	return dayOfWeek;
}

template <bool StartWeekOnSunday>
constexpr Weekday<StartWeekOnSunday>& Weekday<StartWeekOnSunday>::operator++() noexcept
{
	dayOfWeek = (dayOfWeek + 1) % 7;
	return *this;
}

template <bool StartWeekOnSunday>
constexpr Weekday<StartWeekOnSunday>& Weekday<StartWeekOnSunday>::operator--() noexcept
{
	dayOfWeek = (dayOfWeek - 1) % 7;
	return *this;
}

template <bool StartWeekOnSunday>
constexpr Weekday<StartWeekOnSunday>& Weekday<StartWeekOnSunday>::operator+= (int numDaysToAdd) noexcept
{
	dayOfWeek = (dayOfWeek + numDaysToAdd) % 7;
	return *this;
}

template <bool StartWeekOnSunday>
constexpr Weekday<StartWeekOnSunday>& Weekday<StartWeekOnSunday>::operator-= (int numDaysToSubtract) noexcept
{
	dayOfWeek = (dayOfWeek - numDaysToSubtract) % 7;
	return *this;
}

template <bool StartWeekOnSunday>
constexpr Weekday<StartWeekOnSunday> Weekday<StartWeekOnSunday>::operator+ (int numDaysToAdd) const noexcept
{
	return Weekday { (dayOfWeek + numDaysToAdd) % 7 };
}

template <bool StartWeekOnSunday>
constexpr Weekday<StartWeekOnSunday> Weekday<StartWeekOnSunday>::operator- (int numDaysToSubtract) const noexcept
{
	return Weekday { (dayOfWeek - numDaysToSubtract) % 7 };
}

template <bool StartWeekOnSunday>
constexpr bool Weekday<StartWeekOnSunday>::operator> (const Weekday& other) const noexcept
{
	return dayOfWeek > other.dayOfWeek;
}

template <bool StartWeekOnSunday>
constexpr bool Weekday<StartWeekOnSunday>::operator< (const Weekday& other) const noexcept
{
	return dayOfWeek < other.dayOfWeek;
}

template <>
constexpr bool Weekday<true>::operator< (const Weekday<false>& other) const noexcept
{
	return (dayOfWeek + 1) % 7 < other.daysSinceStartOfWeek();
}

template <>
constexpr bool Weekday<false>::operator< (const Weekday<true>& other) const noexcept
{
	return (dayOfWeek - 1) % 7 < other.daysSinceStartOfWeek();
}

template <>
constexpr bool Weekday<true>::operator> (const Weekday<false>& other) const noexcept
{
	return (dayOfWeek + 1) % 7 > other.daysSinceStartOfWeek();
}

template <>
constexpr bool Weekday<false>::operator> (const Weekday<true>& other) const noexcept
{
	return (dayOfWeek - 1) % 7 > other.daysSinceStartOfWeek();
}

template <bool StartWeekOnSunday>
constexpr bool Weekday<StartWeekOnSunday>::operator== (const Weekday& other) const noexcept
{
	return dayOfWeek == other.dayOfWeek;
}

template <bool StartWeekOnSunday>
constexpr bool Weekday<StartWeekOnSunday>::operator!= (const Weekday& other) const noexcept
{
	return dayOfWeek != other.dayOfWeek;
}

template <>
constexpr bool Weekday<true>::operator== (const Weekday<false>& other) const noexcept
{
	return (dayOfWeek + 1) % 7 == other.daysSinceStartOfWeek();
}

template <>
constexpr bool Weekday<false>::operator== (const Weekday<true>& other) const noexcept
{
	return (dayOfWeek - 1) % 7 == other.daysSinceStartOfWeek();
}

template <bool StartWeekOnSunday>
constexpr bool Weekday<StartWeekOnSunday>::operator!= (const OtherWeekdayType& other) const noexcept
{
	return ! (*this == other);
}

template <bool StartWeekOnSunday>
constexpr std::string_view Weekday<StartWeekOnSunday>::getString (bool useShort) const noexcept
{
	constexpr const std::string_view full_days[7] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

	constexpr const std::string_view short_days[7] = { "Sun", "Mon", "Tues", "Wed", "Thurs", "Fri", "Sat" };

	const auto idx = [w = dayOfWeek]
	{
		if constexpr (! StartWeekOnSunday)
			return (w + 1) % 7;

		return w;
	}();

	if (useShort)
		return short_days[idx - 1];

	return full_days[idx - 1];
}

template <bool StartWeekOnSunday>
constexpr typename Weekday<StartWeekOnSunday>::OtherWeekdayType Weekday<StartWeekOnSunday>::toOtherWeekdayType() const noexcept
{
	if constexpr (StartWeekOnSunday)
		return OtherWeekdayType { (dayOfWeek - 1) % 7 };

	return OtherWeekdayType { (dayOfWeek + 1) % 7 };
}

template <bool StartWeekOnSunday>
Weekday<StartWeekOnSunday> Weekday<StartWeekOnSunday>::getCurrent()
{
	const auto now = std::chrono::system_clock::now();

	const auto timeT = std::chrono::system_clock::to_time_t (now);

	if (auto* localtime = std::localtime (&timeT))
	{
		if constexpr (StartWeekOnSunday)
			return Weekday { localtime->tm_wday };
		else
			return Weekday { (localtime->tm_wday - 1) % 7 };
	}

	return {};
}

template <bool StartWeekOnSunday>
consteval Weekday<StartWeekOnSunday> Weekday<StartWeekOnSunday>::getCompilationWeekday() noexcept
{
	return Weekday { build_year(), build_month(), build_day() };
}

}  // namespace time

LIMES_END_NAMESPACE
