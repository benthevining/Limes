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
#include <ctime>

LIMES_BEGIN_NAMESPACE

namespace time
{

class LIMES_EXPORT Year final
{
public:

	constexpr Year() = default;

	explicit constexpr Year (int yearNumber) noexcept;

	explicit constexpr Year (const std::tm& timeObj) noexcept;

	explicit Year (std::time_t time);

	LIMES_CONSTEXPR_MOVABLE (Year);
	LIMES_CONSTEXPR_COPYABLE (Year);

	constexpr Year& operator= (int newYear) noexcept;

	[[nodiscard]] constexpr bool isLeap() const noexcept;

	[[nodiscard]] constexpr int getNumDays() const noexcept;

	[[nodiscard]] int getNumWeeks() const noexcept;

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
	[[nodiscard]] constexpr bool operator<(const Year& other) const noexcept;

	[[nodiscard]] constexpr bool operator== (const Year& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Year& other) const noexcept;

	[[nodiscard]] bool isInPast() const noexcept;
	[[nodiscard]] bool isInFuture() const noexcept;

	[[nodiscard]] static Year getCurrent();

	[[nodiscard]] LIMES_PURE_FUNCTION static consteval Year getCompilationYear() noexcept;

private:

	int year { 0 };
};

}  // namespace time

LIMES_END_NAMESPACE

#include "impl/year_impl.h"
