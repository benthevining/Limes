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
#include "hour.h"
#include "../system/compiler_defs.h"
#include <ctime>
#include <string>
#include "time_utils.h"

/** @file
	This file defines the Time class.
	@ingroup time
 */

LIMES_BEGIN_NAMESPACE

namespace time
{

/** This class represents a %time of day, with a specific Hour, minute, and second.
	@see Hour, DateTime
	@ingroup time
 */
class LIMES_EXPORT Time final
{
public:

	/** @name Constructors */
	///@{

	/** Creates a Time object representing midnight on the hour. */
	constexpr Time() = default;

	/** Creates a Time object. */
	explicit constexpr Time (const Hour& h, int min, int sec) noexcept;

	/** Creates a Time object. */
	explicit constexpr Time (int hourIn24HourFormat, int min, int sec) noexcept;

	/** Creates a Time object from the given \c std::tm object. */
	explicit constexpr Time (const std::tm& timeObj) noexcept;

	/** Creates a Time object from the given \c std::time_t object. */
	explicit Time (std::time_t time);

	/** Creates a Time object from a \c std::chrono::time_point of any Clock type. */
	template <Clock ClockType>
	explicit Time (const Point<ClockType>& timePoint);
	///@}

	LIMES_CONSTEXPR_MOVABLE (Time);
	LIMES_CONSTEXPR_COPYABLE (Time);

	/** Returns true if this %time is between midnight and noon. */
	[[nodiscard]] constexpr bool isAM() const noexcept;

	/** Returns true if this %time is between noon and midnight. */
	[[nodiscard]] constexpr bool isPM() const noexcept;

	/** Returns true if this %time is exactly on the hour. */
	[[nodiscard]] constexpr bool isOnTheHour() const noexcept;

	/** Returns the Hour of this %time. */
	[[nodiscard]] constexpr Hour getHour() const noexcept;

	/** Returns the minute of the %hour of this %time. */
	[[nodiscard]] constexpr int getMinute() const noexcept;

	/** Returns the second of the minute of this %time. */
	[[nodiscard]] constexpr int getSecond() const noexcept;

	/** @name Greater/less than comparisons */
	///@{
	[[nodiscard]] constexpr bool isBefore (const Time& other) const noexcept;
	[[nodiscard]] constexpr bool isAfter (const Time& other) const noexcept;
	[[nodiscard]] constexpr bool operator> (const Time& other) const noexcept;
	[[nodiscard]] constexpr bool operator< (const Time& other) const noexcept;
	///@}

	/** @name Equality comparisons */
	///@{
	[[nodiscard]] constexpr bool operator== (const Time& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Time& other) const noexcept;
	///@}

	/** Returns true if this %time is before the current %time (agnostic of the date). */
	[[nodiscard]] bool isInPast() const noexcept;

	/** Returns true if this %time is after the current %time (agnostic of the date). */
	[[nodiscard]] bool isInFuture() const noexcept;

	/** Returns this Time represented as a string.
		The format of the string is \c "HH:MM:SS", suffixed with "AM" or "PM" if as24HourTime is false.
	 */
	[[nodiscard]] std::string toString (bool as24HourTime = true) const;

	/** Returns a Time object representing the current %time. */
	[[nodiscard]] static Time getCurrent();

	/** Returns a Time object representing the %time this code was compiled. */
	[[nodiscard]] LIMES_PURE_FUNCTION static consteval Time getCompilationTime() noexcept;

private:

	Hour hour;
	int	 minute { 0 }, second { 0 };
};

}  // namespace time

LIMES_END_NAMESPACE

#include "impl/time_impl.h"	 // IWYU pragma: export
