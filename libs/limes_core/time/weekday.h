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
#include "./year.h"
#include "./month.h"
#include "../misc/preprocessor.h"
#include "../system/compiler_defs.h"
#include <string_view>
#include <type_traits>
#include <ctime>

/** @file
	This file defines the Weekday class.
	@ingroup time
 */

LIMES_BEGIN_NAMESPACE

namespace time
{

/** This class represents a day of the week in the Gregorian calendar.
	@tparam StartWeekOnSunday If true, "day 0" of a week is Sunday; otherwise, "day 0" is Monday. The ISO standard starts weeks on Monday, so this is false by default, but you can use either type of %weekday.

	If the \c StartWeekOnSunday template parameter is \c true , a week looks like this:

	Day # | Name
	----- | ----------
	0     | Sunday
	1     | Monday
	2     | Tuesday
	3     | Wednesday
	4     | Thursday
	5     | Friday
	6     | Saturday

	However, if this parameter is \c false , then a week looks like this:

	Day # | Name
	----- | -----------
	0     | Monday
	1     | Tuesday
	2     | Wednesday
	3     | Thursday
	4     | Friday
	5     | Saturday
	6     | Sunday

	@ingroup time
	@test This class's API is tested with static assertions at compile-time.
 */
template <bool StartWeekOnSunday = false>
class LIMES_EXPORT Weekday final
{
public:

	/** Represents names of each %weekday. */
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

	/** Convenience typdef for the other template specialization of Weekday. */
	using OtherWeekdayType = Weekday<! StartWeekOnSunday>;

	/** @name Constructors */
	///@{

	/** Creates a Weekday object representing the first day of the week (Sunday or Monday, depending on the template parameter StartWeekOnSunday). */
	constexpr Weekday() = default;

	/** Creates a Weekday object representing the day that is the specified number of days after the first day of the week. */
	explicit constexpr Weekday (int daysSinceStartOfWeek) noexcept;

	/** Creates a Weekday object representing a specific date. */
	explicit constexpr Weekday (int year, int month, int day) noexcept;

	/** Creates a Weekday object representing a specific date. */
	explicit constexpr Weekday (const Year& year, const Month& month, int day) noexcept;

	/** Creates a Weekday object representing the day with the given name. */
	explicit constexpr Weekday (Name day) noexcept;

	/** Creates a Weekday object from the given \c std::tm object. */
	explicit constexpr Weekday (const std::tm& timeObj) noexcept;

	/** Creates a Weekday object from the given \c std::time_t object. */
	explicit Weekday (std::time_t time);

	/** Creates a Weekday object from another Weekday object with the opposite StartWeekOnSunday value. */
	explicit constexpr Weekday (const OtherWeekdayType& other) noexcept;
	///@}

	LIMES_CONSTEXPR_MOVABLE (Weekday)
	LIMES_CONSTEXPR_COPYABLE (Weekday)

	/** Returns the name of the day this object represents. */
	[[nodiscard]] constexpr Name getWeekdayName() const noexcept;

	/** @name Incrementing
		Increments the day of the week. Incrementing the final day of the week will wrap around to the first day of the week.
	 */
	///@{
	constexpr Weekday&				operator++() noexcept;
	constexpr Weekday&				operator+= (int numDaysToAdd) noexcept;
	[[nodiscard]] constexpr Weekday operator+ (int numDaysToAdd) const noexcept;
	///@}

	/** @name Decrementing
		Decrements the day of the week. Decrementing the first day of the week will wrap around to the final day of the week.
	 */
	///@{
	constexpr Weekday&				operator--() noexcept;
	constexpr Weekday&				operator-= (int numDaysToSubtract) noexcept;
	[[nodiscard]] constexpr Weekday operator- (int numDaysToSubtract) const noexcept;
	///@}

	/** @name Greater/less than comparisons */
	///@{
	[[nodiscard]] constexpr bool operator> (const Weekday& other) const noexcept;
	[[nodiscard]] constexpr bool operator<(const Weekday& other) const noexcept;
	[[nodiscard]] constexpr bool operator> (const OtherWeekdayType& other) const noexcept;
	[[nodiscard]] constexpr bool operator<(const OtherWeekdayType& other) const noexcept;
	///@}

	/** @name Equality comparisons */
	///@{
	[[nodiscard]] constexpr bool operator== (const Weekday& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Weekday& other) const noexcept;
	[[nodiscard]] constexpr bool operator== (const OtherWeekdayType& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const OtherWeekdayType& other) const noexcept;
	///@}

	/** Returns the number of days since the first day of the week that this Weekday represents. */
	[[nodiscard]] constexpr int daysSinceStartOfWeek() const noexcept;

	/** Returns true if this Weekday represents Saturday or Sunday. */
	[[nodiscard]] constexpr bool isWeekend() const noexcept;

	/** Returns true if this Weekday is not Saturday or Sunday. */
	[[nodiscard]] constexpr bool isWeekday() const noexcept;

	/** Returns a string containing the name of this %weekday.
		@param useShort If false, the full %weekday name will be returned. If true, an abbreviated name will be returned.
	 */
	[[nodiscard]] constexpr std::string_view getString (bool useShort = false) const noexcept;

	/** Converts this Weekday into a Weekday object of the opposite type. */
	[[nodiscard]] constexpr OtherWeekdayType toOtherWeekdayType() const noexcept;

	/** Returns a Weekday object representing the current day of the week. */
	[[nodiscard]] static Weekday getCurrent();

	/** Returns a Weekday object representing the day this code was compiled. */
	[[nodiscard]] static consteval Weekday getCompilationWeekday() noexcept;

	/** Returns true if this template specialization is starting the week on Sunday.
		This is the same as checking whether the template parameter StartWeekOnSunday is true.
	 */
	[[nodiscard]] LIMES_PURE_FUNCTION static consteval bool startsWeekOnSunday() noexcept;

private:

	int dayOfWeek { 0 };
};

}  // namespace time

LIMES_END_NAMESPACE

#include "./impl/weekday_impl.h"  // IWYU pragma: export
