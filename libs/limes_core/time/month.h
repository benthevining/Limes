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
#include "../misc/preprocessor.h"
#include "build_date.h"
#include <string_view>
#include <chrono>
#include <ctime>

LIMES_BEGIN_NAMESPACE

namespace time
{

class LIMES_EXPORT Month final
{
public:

	constexpr Month() = default;

	explicit constexpr Month (int number) noexcept;

	LIMES_CONSTEXPR_MOVABLE (Month);
	LIMES_CONSTEXPR_COPYABLE (Month);

	[[nodiscard]] constexpr int getMonthNumber() const noexcept;

	[[nodiscard]] constexpr int getNumDays (bool leapYear = false) noexcept;

	[[nodiscard]] constexpr std::string_view getString (bool useShort = false) const noexcept;

	constexpr Month& operator++() noexcept;
	constexpr Month& operator--() noexcept;

	constexpr Month& operator+= (int numMonthsToAdd) noexcept;
	constexpr Month& operator-= (int numMonthsToSubtract) noexcept;

	[[nodiscard]] constexpr Month operator+ (int numMonthsToAdd) const noexcept;
	[[nodiscard]] constexpr Month operator- (int numMonthsToSubtract) const noexcept;

	[[nodiscard]] constexpr bool operator> (const Month& other) const noexcept;
	[[nodiscard]] constexpr bool operator< (const Month& other) const noexcept;

	[[nodiscard]] constexpr bool operator== (const Month& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Month& other) const noexcept;

	[[nodiscard]] static Month getCurrent();

	[[nodiscard]] static consteval Month getCompilationMonth() noexcept;

private:

	int monthNum { 0 };
};

/*-------------------------------------------------------------------------------------------------------------------------------*/

constexpr Month::Month (int number) noexcept
	: monthNum (number)
{
	LIMES_ASSERT (monthNum >= 0 && monthNum < 12);
}

constexpr int Month::getMonthNumber() const noexcept
{
	return monthNum + 1;
}

constexpr int Month::getNumDays (bool leapYear) noexcept
{
	if (monthNum == 1)
	{
		if (leapYear)
			return 29;

		return 28;
	}

	if (monthNum == 3 || monthNum == 5 || monthNum == 8 || monthNum == 10)
		return 30;

	return 31;
}

constexpr Month& Month::operator++() noexcept
{
	monthNum = (monthNum + 1) % 12;

	return *this;
}

constexpr Month& Month::operator--() noexcept
{
	monthNum = (monthNum - 1) % 12;

	return *this;
}

constexpr Month& Month::operator+= (int numMonthsToAdd) noexcept
{
	monthNum = (monthNum + numMonthsToAdd) % 12;
	return *this;
}

constexpr Month& Month::operator-= (int numMonthsToSubtract) noexcept
{
	monthNum = (monthNum - numMonthsToSubtract) % 12;
	return *this;
}

constexpr Month Month::operator+ (int numMonthsToAdd) const noexcept
{
	return Month { (monthNum + numMonthsToAdd) % 12 };
}

constexpr Month Month::operator- (int numMonthsToSubtract) const noexcept
{
	return Month { (monthNum - numMonthsToSubtract) % 12 };
}

constexpr bool Month::operator> (const Month& other) const noexcept
{
	return monthNum > other.monthNum;
}

constexpr bool Month::operator< (const Month& other) const noexcept
{
	return monthNum < other.monthNum;
}

constexpr bool Month::operator== (const Month& other) const noexcept
{
	return monthNum == other.monthNum;
}

constexpr bool Month::operator!= (const Month& other) const noexcept
{
	return monthNum != other.monthNum;
}

Month Month::getCurrent()
{
	const auto now = std::chrono::system_clock::now();

	const auto timeT = std::chrono::system_clock::to_time_t (now);

	if (auto* localtime = std::localtime (&timeT))
		return Month { localtime->tm_mon };

	return {};
}

consteval Month Month::getCompilationMonth() noexcept
{
	return Month { build_month() - 1 };
}

constexpr std::string_view Month::getString (bool useShort) const noexcept
{
	constexpr const std::string_view months[12] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

	const auto result = months[monthNum];

	if (useShort)
		return result.substr (0, 3);

	return result;
}

}  // namespace time

LIMES_END_NAMESPACE
