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
#include "year.h"
#include "month.h"
#include "../misc/preprocessor.h"
#include <string_view>
#include <type_traits>
#include <ctime>

LIMES_BEGIN_NAMESPACE

namespace time
{

template <bool StartWeekOnSunday = false>
class LIMES_EXPORT Weekday final
{
public:

	enum class Name : int
	{
		Sunday = 0,
		Monday,
		Tuesday,
		Wednesday,
		Thursday,
		Friday,
		Saturday
	};

	constexpr Weekday() = default;

	explicit constexpr Weekday (int daysSinceStartOfWeek) noexcept;

	explicit constexpr Weekday (int year, int month, int day) noexcept;

	explicit constexpr Weekday (const Year& year, const Month& month, int day) noexcept;

	explicit constexpr Weekday (Name day) noexcept;

	explicit constexpr Weekday (const std::tm& timeObj) noexcept;

	explicit Weekday (std::time_t time);

	using OtherWeekdayType = Weekday<! StartWeekOnSunday>;

	explicit constexpr Weekday (const OtherWeekdayType& other) noexcept;

	LIMES_CONSTEXPR_MOVABLE (Weekday);
	LIMES_CONSTEXPR_COPYABLE (Weekday);

	[[nodiscard]] constexpr Name getWeekdayName() const noexcept;

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

	[[nodiscard]] constexpr bool isWeekend() const noexcept;
	[[nodiscard]] constexpr bool isWeekday() const noexcept;

	[[nodiscard]] constexpr std::string_view getString (bool useShort = false) const noexcept;

	[[nodiscard]] constexpr OtherWeekdayType toOtherWeekdayType() const noexcept;

	[[nodiscard]] static Weekday getCurrent();

	[[nodiscard]] static consteval Weekday getCompilationWeekday() noexcept;

	[[nodiscard]] static consteval bool startsWeekOnSunday() noexcept;

private:

	int dayOfWeek { 0 };
};

}  // namespace time

LIMES_END_NAMESPACE

#include "impl/weekday_impl.h"
