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
#include "../misc/preprocessor.h"
#include "../system/compiler_defs.h"

LIMES_BEGIN_NAMESPACE

namespace time
{

LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION consteval int build_year() noexcept
{
	const char* date = __DATE__;

	date += 7;

	auto result = 0;

	for (auto i = 0, mult = 1000; i < 4; ++i, mult /= 10)
		result += (date[i] - '0') * mult;

	return result;
}

LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION consteval int build_month() noexcept
{
	constexpr const char months[12][4] {
		"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
	};

	constexpr const char date[] = __DATE__;

	for (auto i = 0; i < 12; ++i)
	{
		const char* const m = months[i];

		if (m[0] == date[0] && m[1] == date[1] && m[2] == date[2])
			return i + 1;
	}

	return 0;
}

LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION consteval int build_day() noexcept
{
	const char* date = __DATE__;

	date += 4;

	auto result = 0;

	if (*date != ' ')
		result += ((date[0] - '0') * 10);

	return result + (date[1] - '0');
}

LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION consteval int build_hour() noexcept
{
	constexpr const char time[] = __TIME__;

	return (time[0] - '0') * 10 + (time[1] - '0');
}

LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION consteval int build_minute() noexcept
{
	constexpr const char time[] = __TIME__;

	return (time[3] - '0') * 10 + (time[4] - '0');
}

LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION consteval int build_second() noexcept
{
	constexpr const char time[] = __TIME__;

	return (time[6] - '0') * 10 + (time[7] - '0');
}


/// TODO_BEFORE() inserts a compilation "time bomb" that will trigger a "TODO" build error a soon as
/// the given date is reached.
///
/// This is useful to force attention on a specific piece of code that should not been forgotten
/// among a growing list of many other "TODO" comments...
///
/// Example:
///     TODO_BEFORE(01, 2019, "refactor to use std::optional<> once we compile in C++17 mode");
#define TODO_BEFORE(month, year, msg)                                                                                                                                      \
	LIMES_BLOCK_WITH_FORCED_SEMICOLON (static_assert (year >= ::limes::time::build_year() && year <= ::limes::time::build_year() + 3 && month > 0 && month <= 12,          \
													  "Invalid date constraint in TODO_BEFORE macro!");                                                                    \
									   static_assert (::limes::time::build_year() < year || (::limes::time::build_year() == year && ::limes::time::build_month() < month), \
													  "expired TODO: " msg);)

}  // namespace time

LIMES_END_NAMESPACE
