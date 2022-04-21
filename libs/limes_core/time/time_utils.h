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
#include <limes_export.h>
#include <string_view>

LIMES_BEGIN_NAMESPACE

namespace time
{

LIMES_EXPORT [[nodiscard]] constexpr int getDayOfWeek (int month, int day, int year) noexcept
{
	constexpr const int month_table[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };

	if (month < 3)
		--year;

	return (year + year / 4 - year / 100 + year / 400 + month_table[month - 1] + day) % 7;
}

LIMES_EXPORT [[nodiscard]] constexpr std::string_view getDayOfWeekString (int dayOfWeek, bool startOnSunday = true, bool shortName = false) noexcept
{
	constexpr const std::string_view full_days[7] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

	constexpr const std::string_view short_days[7] = { "Sun", "Mon", "Tues", "Wed", "Thurs", "Fri", "Sat" };

	if (! startOnSunday)
		dayOfWeek = (dayOfWeek + 1) % 7;

	if (shortName)
		return short_days[dayOfWeek - 1];

	return full_days[dayOfWeek - 1];
}

LIMES_EXPORT [[nodiscard]] constexpr std::string_view getMonthString (int month, bool shortName = false) noexcept
{
	constexpr const std::string_view months[12] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

	const auto result = months[month - 1];

	if (shortName)
		return result.substr (0, 3);

	return result;
}

}  // namespace time

LIMES_END_NAMESPACE
