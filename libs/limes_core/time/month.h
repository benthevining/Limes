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
#include <string_view>
#include <ctime>

/** @file
	This file defines the Month class.
	@ingroup time
 */

LIMES_BEGIN_NAMESPACE

namespace time
{

/** This class represents a %month of the Gregorian calendar.
	@ingroup time
	@see Date, DateTime
 */
class LIMES_EXPORT Month final
{
public:

	/** Represents month names. */
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

	/** @name Constructors */
	///@{

	/** Creates a Month object representing January. */
	constexpr Month() = default;

	/** Creates a Month object representing the given %month of the year, with January being 1 and December being 12. */
	explicit constexpr Month (int number) noexcept;

	/** Creates a Month object representing the given named %month. */
	explicit constexpr Month (Name monthName) noexcept;

	/** Creates a Month object from the \c std::tm object. */
	explicit constexpr Month (const std::tm& timeObj) noexcept;

	/** Creates a Month object from the \c std::time_t object. */
	explicit Month (std::time_t time);

	///@}

	LIMES_CONSTEXPR_MOVABLE (Month);
	LIMES_CONSTEXPR_COPYABLE (Month);

	/** @name Assignment
		Reassigns this Month object to refer to a new %month.
	 */
	///@{
	constexpr Month& operator= (Name newMonth) noexcept;
	constexpr Month& operator= (int newMonth) noexcept;
	///@}

	/** Returns the number of this %month.
		January is 1 and December is 12.
	 */
	[[nodiscard]] constexpr int getMonthNumber() const noexcept;

	/** Returns the Name of this %month. */
	[[nodiscard]] constexpr Name getMonthName() const noexcept;

	/** Returns the number of days in this %month.
		@param leapYear If true, this will be evaluated in the context of a leap year. This only has any effect if this %month represents February.
	 */
	[[nodiscard]] constexpr int getNumDays (bool leapYear = false) const noexcept;

	/** Returns the number of full weeks in this %month. */
	[[nodiscard]] constexpr int getNumWeeks (bool leapYear = false) const noexcept;

	/** Returns a string containing the name of this %month.
		@param useShort If false, the full name of the %month will be returned. If true, a 3-character abbreviation will be returned.
	 */
	[[nodiscard]] constexpr std::string_view getString (bool useShort = false) const noexcept;

	/** @name Incrementing
		Increments the number of this %month. Incrementing December will wrap around to January.
	 */
	///@{
	constexpr Month&			  operator++() noexcept;
	constexpr Month&			  operator++ (int) noexcept;  // NOLINT
	constexpr Month&			  operator+= (int numMonthsToAdd) noexcept;
	[[nodiscard]] constexpr Month operator+ (int numMonthsToAdd) const noexcept;
	///@}

	/** @name Decrementing
		Decrements the number of this %month. Decrementing January will wrap around to December.
	 */
	///@{
	constexpr Month&			  operator--() noexcept;
	constexpr Month&			  operator-- (int) noexcept;  // NOLINT
	constexpr Month&			  operator-= (int numMonthsToSubtract) noexcept;
	[[nodiscard]] constexpr Month operator- (int numMonthsToSubtract) const noexcept;
	///@}

	/** @name Greater/less than comparisons */
	///@{
	[[nodiscard]] constexpr bool operator> (const Month& other) const noexcept;
	[[nodiscard]] constexpr bool operator< (const Month& other) const noexcept;
	[[nodiscard]] constexpr bool operator> (Name other) const noexcept;
	[[nodiscard]] constexpr bool operator< (Name other) const noexcept;
	///@}

	/** @name Equality comparisons */
	///@{
	[[nodiscard]] constexpr bool operator== (const Month& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Month& other) const noexcept;
	[[nodiscard]] constexpr bool operator== (Name other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (Name other) const noexcept;
	///@}

	/** Returns a Month object representing the current %month. */
	[[nodiscard]] static Month getCurrent();

	/** Returns a Month object representing the %month this code was compiled.
		@see build_month()
	 */
	[[nodiscard]] LIMES_PURE_FUNCTION static consteval Month getCompilationMonth() noexcept;

private:

	int monthNum { 1 };
};

}  // namespace time

LIMES_END_NAMESPACE

#include "impl/month_impl.h"
