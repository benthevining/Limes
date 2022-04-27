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
#include "build_date.h"
#include <chrono>
#include <ctime>
#include "../math/mathHelpers.h"

LIMES_BEGIN_NAMESPACE

namespace time
{

class LIMES_EXPORT Year final
{
public:

	constexpr Year() = default;

	explicit constexpr Year (int yearNumber) noexcept;

	LIMES_CONSTEXPR_MOVABLE (Year);
	LIMES_CONSTEXPR_COPYABLE (Year);

	[[nodiscard]] constexpr bool isLeap() const noexcept;

	[[nodiscard]] constexpr int getNumDays() const noexcept;

	[[nodiscard]] constexpr bool isBC() const noexcept;

	[[nodiscard]] constexpr int getYear() const noexcept;

	constexpr operator int() const noexcept { return getYear(); }

	constexpr Year& operator++() noexcept;
	constexpr Year& operator--() noexcept;

	constexpr Year& operator+= (int numYearsToAdd) noexcept;
	constexpr Year& operator-= (int numYearsToSubtract) noexcept;

	[[nodiscard]] constexpr Year operator+ (int numYearsToAdd) const noexcept;
	[[nodiscard]] constexpr Year operator- (int numYearsToSubtract) const noexcept;

	[[nodiscard]] constexpr bool operator> (const Year& other) const noexcept;
	[[nodiscard]] constexpr bool operator< (const Year& other) const noexcept;

	[[nodiscard]] constexpr bool operator== (const Year& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Year& other) const noexcept;

	[[nodiscard]] static Year getCurrent();

	[[nodiscard]] static consteval Year getCompilationYear() noexcept;

private:

	int year { 0 };
};

/*-------------------------------------------------------------------------------------------------------------------------------*/

constexpr Year::Year (int yearNumber) noexcept
	: year (yearNumber)
{
}

Year Year::getCurrent()
{
	const auto now = std::chrono::system_clock::now();

	const auto timeT = std::chrono::system_clock::to_time_t (now);

	if (auto* localtime = std::localtime (&timeT))
		return Year { localtime->tm_year + 1900 };

	return {};
}

consteval Year Year::getCompilationYear() noexcept
{
	return Year { build_year() };
}

constexpr bool Year::isLeap() const noexcept
{
	if (math::isDivisibleBy (year, 400))
		return true;

	if (math::isDivisibleBy (year, 4) && ! math::isDivisibleBy (year, 100))
		return true;

	return false;
}

constexpr int Year::getNumDays() const noexcept
{
	if (isLeap())
		return 366;

	return 365;
}

constexpr bool Year::isBC() const noexcept
{
	return year < 0;
}

constexpr int Year::getYear() const noexcept
{
	return year;
}

constexpr Year& Year::operator++() noexcept
{
	++year;
	return *this;
}

constexpr Year& Year::operator--() noexcept
{
	--year;
	return *this;
}

constexpr Year Year::operator+ (int numYearsToAdd) const noexcept
{
	return Year { year + numYearsToAdd };
}

constexpr Year Year::operator- (int numYearsToSubtract) const noexcept
{
	return Year { year - numYearsToSubtract };
}

constexpr Year& Year::operator+= (int numYearsToAdd) noexcept
{
	year += numYearsToAdd;
	return *this;
}

constexpr Year& Year::operator-= (int numYearsToSubtract) noexcept
{
	year -= numYearsToSubtract;
	return *this;
}

constexpr bool Year::operator> (const Year& other) const noexcept
{
	return year > other.year;
}

constexpr bool Year::operator< (const Year& other) const noexcept
{
	return year < other.year;
}

constexpr bool Year::operator== (const Year& other) const noexcept
{
	return year == other.year;
}

constexpr bool Year::operator!= (const Year& other) const noexcept
{
	return year != other.year;
}

}  // namespace time

LIMES_END_NAMESPACE
