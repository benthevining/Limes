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
#include <string_view>
#include <ctime>

LIMES_BEGIN_NAMESPACE

namespace time
{

class LIMES_EXPORT Month final
{
public:

	enum class Name : int
	{
		January = 1,
		February,
		March,
		April,
		May,
		June,
		July,
		August,
		September,
		October,
		November,
		December
	};

	constexpr Month() = default;

	explicit constexpr Month (int number) noexcept;

	explicit constexpr Month (Name monthName) noexcept;

	explicit constexpr Month (const std::tm& timeObj) noexcept;

	explicit Month (std::time_t time);

	LIMES_CONSTEXPR_MOVABLE (Month);
	LIMES_CONSTEXPR_COPYABLE (Month);

	constexpr Month& operator= (Name newMonth) noexcept;
	constexpr Month& operator= (int newMonth) noexcept;

	[[nodiscard]] constexpr int getMonthNumber() const noexcept;

	[[nodiscard]] constexpr Name getMonthName() const noexcept;

	[[nodiscard]] constexpr int getNumDays (bool leapYear = false) const noexcept;

	[[nodiscard]] constexpr int getNumWeeks (bool leapYear = false) const noexcept;

	[[nodiscard]] constexpr std::string_view getString (bool useShort = false) const noexcept;

	constexpr Month& operator++() noexcept;
	constexpr Month& operator--() noexcept;

	constexpr Month& operator++ (int) noexcept;	 // NOLINT
	constexpr Month& operator-- (int) noexcept;	 // NOLINT

	constexpr Month& operator+= (int numMonthsToAdd) noexcept;
	constexpr Month& operator-= (int numMonthsToSubtract) noexcept;

	[[nodiscard]] constexpr Month operator+ (int numMonthsToAdd) const noexcept;
	[[nodiscard]] constexpr Month operator- (int numMonthsToSubtract) const noexcept;

	[[nodiscard]] constexpr bool operator> (const Month& other) const noexcept;
	[[nodiscard]] constexpr bool operator< (const Month& other) const noexcept;

	[[nodiscard]] constexpr bool operator> (Name other) const noexcept;
	[[nodiscard]] constexpr bool operator< (Name other) const noexcept;

	[[nodiscard]] constexpr bool operator== (const Month& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Month& other) const noexcept;

	[[nodiscard]] constexpr bool operator== (Name other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (Name other) const noexcept;

	[[nodiscard]] static Month getCurrent();

	[[nodiscard]] static consteval Month getCompilationMonth() noexcept;

private:

	int monthNum { 1 };
};

}  // namespace time

LIMES_END_NAMESPACE

#include "impl/month_impl.h"
