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
#include "../../math/mathHelpers.h"
#include "../build_date.h"

LIMES_BEGIN_NAMESPACE

namespace time
{

constexpr Year::Year (int yearNumber) noexcept
	: year (yearNumber)
{
}

constexpr Year::Year (const std::tm& timeObj) noexcept
	: year (timeObj.tm_year + 1900)
{
}

constexpr Year& Year::operator= (int newYear) noexcept
{
	year = newYear;
	return *this;
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

constexpr bool Year::operator<(const Year& other) const noexcept
{
	return year < other.year;
}

constexpr bool Year::operator> (int other) const noexcept
{
	return year > other;
}

constexpr bool Year::operator<(int other) const noexcept
{
	return year < other;
}

constexpr bool Year::operator== (const Year& other) const noexcept
{
	return year == other.year;
}

constexpr bool Year::operator!= (const Year& other) const noexcept
{
	return year != other.year;
}

constexpr bool Year::operator== (int other) const noexcept
{
	return year == other;
}

constexpr bool Year::operator!= (int other) const noexcept
{
	return year != other;
}

}  // namespace time

LIMES_END_NAMESPACE
