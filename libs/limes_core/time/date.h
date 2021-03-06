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
#include "../limes_namespace.h"
#include "../misc/preprocessor.h"
#include "./weekday.h"
#include "./month.h"
#include "./year.h"
#include <ctime>
#include <string>
#include "./time_utils.h"
#include "../system/compiler_defs.h"

/** @file
	This file defines the Date class.
	@ingroup time
 */

LIMES_BEGIN_NAMESPACE

namespace time
{

/** This class represents a %date on the Gregorian calendar.
	@ingroup time
	@see DateTime, Year, Month
	@test This class's API is tested with static assertions at compile-time.
 */
class LIMES_EXPORT Date final
{
public:
	/** @name Constructors */
	///@{
	/** Creates a Date object representing January 1, year 0. */
	Date() = default;

	/** Creates a Date object.
		@param y The year to use.
		@param m The month to use.
		@param d The day of the month to use. An assertion will be thrown if this is greater than the number of days in the given month.
	 */
	explicit Date (const Year& y, const Month& m, int d) noexcept;

	/** Creates a Date object.
		@param y The year to use.
		@param m The month to use. 1 is January and 12 is December.
		@param d The day of the month to use. An assertion will be thrown if this is greater than the number of days in the given month.
	 */
	explicit Date (int y, int m, int d) noexcept;

	/** Creates a Date object from the given \c std::tm object. */
	explicit Date (const std::tm& timeObj) noexcept;

	/** Creates a Date object from the given \c std::time_t object. */
	explicit Date (std::time_t time);

	/** Creates a Date object from a \c std::chrono::time_point of any Clock type. */
	template <Clock ClockType>
	explicit Date (const Point<ClockType>& timePoint)
		: Date (toTimeObj (timePoint))
	{
	}
	///@}

	LIMES_CONSTEXPR_MOVABLE (Date)
	LIMES_CONSTEXPR_COPYABLE (Date)

	/** Returns the year of this %date. */
	[[nodiscard]] Year getYear() const noexcept;

	/** Returns the month of this %date. */
	[[nodiscard]] Month getMonth() const noexcept;

	/** Returns the day of the month of this %date. */
	[[nodiscard]] int getDayOfMonth() const noexcept;

	/** Returns the day of the year this %date represents. */
	[[nodiscard]] int getDayOfYear() const noexcept;

	/** Returns a Weekday object representing this %date. */
	template <bool StartWeekOnSunday = false>
	[[nodiscard]] Weekday<StartWeekOnSunday> getWeekday() const noexcept;

	/** Returns the week number of the year that this %date represents.
		If this %date is part of the final week of the previous year, -1 will be returned.
	 */
	[[nodiscard]] int getWeekNumber() const noexcept;

	/** Returns true if this %date is in the past. */
	[[nodiscard]] bool isInPast() const noexcept;

	/** Returns true if this %date is in the future. */
	[[nodiscard]] bool isInFuture() const noexcept;

	/** @name Greater/less than comparisons */
	///@{
	[[nodiscard]] bool isBefore (const Date& other) const noexcept;
	[[nodiscard]] bool isAfter (const Date& other) const noexcept;
	[[nodiscard]] bool operator> (const Date& other) const noexcept;
	[[nodiscard]] bool operator<(const Date& other) const noexcept;
	///@}

	/** @name Equality comparisons */
	///@{
	[[nodiscard]] bool operator== (const Date& other) const noexcept;
	[[nodiscard]] bool operator!= (const Date& other) const noexcept;
	///@}

	/** Returns a string representing this %date.
		The format of the string is \c "Day Month, Year". The day of the month will have a leading 0 if it is less than 10.
	 */
	[[nodiscard]] std::string toString (bool shortMonthName = true) const;

	/** Returns a Date object representing the current %date. */
	[[nodiscard]] static Date getCurrent();

	/** Returns a Date object representing the %date this code was compiled. */
	[[nodiscard]] LIMES_PURE_FUNCTION static Date getCompilationDate() noexcept;

private:
	Year  year;
	Month month;
	int	  dayOfMonth { 1 };
};

}  // namespace time

LIMES_END_NAMESPACE
