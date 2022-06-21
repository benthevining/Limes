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

/** @file
	This file defines the Year class.
	@ingroup time
 */

LIMES_BEGIN_NAMESPACE

namespace time
{

/** This class represents a calendar %year.
	@ingroup time
	@see Date, DateTime
	@test This class's API is tested with static assertions at compile-time.
 */
class LIMES_EXPORT Year final
{
public:

	/** @name Constructors */
	///@{
	/** Creates a Year object representing the %year 0. */
	constexpr Year() = default;

	/** Creates a Year object representing the given %year. */
	explicit constexpr Year (int yearNumber) noexcept;

	/** Creates a Year object from the given \c std::tm object. */
	explicit constexpr Year (const std::tm& timeObj) noexcept;

	/** Creates a Year object from the given \c std::time_t object. */
	explicit Year (std::time_t time);
	///@}

	LIMES_CONSTEXPR_MOVABLE (Year)
	LIMES_CONSTEXPR_COPYABLE (Year)

	/** Assigns this year object to a new year number. */
	constexpr Year& operator= (int newYear) noexcept;

	/** Returns true if this year is a leap year. */
	[[nodiscard]] constexpr bool isLeap() const noexcept;

	/** Returns the number of days in this year.
		This returns 366 if \c isLeap() returns true, and 365 otherwise.
	 */
	[[nodiscard]] constexpr int getNumDays() const noexcept;

	/** Returns the number of ISO weeks in this year.
		Most years contain 52 weeks, but some years will contain 53.
	 */
	[[nodiscard]] int getNumWeeks() const noexcept;

	/** Returns true if this year is before the %year 0. */
	[[nodiscard]] constexpr bool isBC() const noexcept;

	/** Returns the number of this %year as an integer. */
	[[nodiscard]] constexpr int getYear() const noexcept;

	/** Returns the number of this %year as an integer. */
	constexpr operator int() const noexcept { return getYear(); }

	/** @name Incrementing */
	///@{
	constexpr Year&				 operator++() noexcept;
	constexpr Year&				 operator+= (int numYearsToAdd) noexcept;
	[[nodiscard]] constexpr Year operator+ (int numYearsToAdd) const noexcept;
	///@}

	/** @name Decrementing */
	///@{
	constexpr Year&				 operator--() noexcept;
	constexpr Year&				 operator-= (int numYearsToSubtract) noexcept;
	[[nodiscard]] constexpr Year operator- (int numYearsToSubtract) const noexcept;
	///@}

	/** @name Greater/less than comparisons */
	///@{
	[[nodiscard]] constexpr bool operator> (const Year& other) const noexcept;
	[[nodiscard]] constexpr bool operator<(const Year& other) const noexcept;
	[[nodiscard]] constexpr bool operator> (int other) const noexcept;
	[[nodiscard]] constexpr bool operator<(int other) const noexcept;
	///@}

	/** @name Equality comparisons */
	///@{
	[[nodiscard]] constexpr bool operator== (const Year& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Year& other) const noexcept;
	[[nodiscard]] constexpr bool operator== (int other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (int other) const noexcept;
	///@}

	/**	Returns true if this %year is less than the current %year.
		@see getCurrent()
	 */
	[[nodiscard]] bool isInPast() const noexcept;

	/**	Returns true if this %year is greater than the current %year.
		@see getCurrent()
	 */
	[[nodiscard]] bool isInFuture() const noexcept;

	/** Returns a Year object representing the current %year. */
	[[nodiscard]] static Year getCurrent();

	/** Returns a Year object representing the %year this code was compiled.
		@see build_year()
	 */
	[[nodiscard]] LIMES_PURE_FUNCTION static consteval Year getCompilationYear() noexcept;

private:

	int year { 0 };
};

}  // namespace time

LIMES_END_NAMESPACE

#include "impl/year_impl.h"	 // IWYU pragma: export
