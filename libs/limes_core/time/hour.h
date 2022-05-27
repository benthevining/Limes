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
	This file defines the Hour class.
	@ingroup time
 */

LIMES_BEGIN_NAMESPACE

namespace time
{

/** This class represents an %hour of the day.
	@ingroup time
	@see Time
 */
class LIMES_EXPORT Hour final
{
public:

	/** @name Constructors */
	///@{

	/** Creates an Hour object representing midnight. */
	constexpr Hour() = default;

	/** Creates an Hour object representing the given number of hours since midnight. */
	explicit constexpr Hour (int hoursSinceMidnight) noexcept;

	/** Creates an Hour object. */
	explicit constexpr Hour (int hourNumber, bool isAM) noexcept;

	/** Creates an Hour object from the given \c std::tm object. */
	explicit constexpr Hour (const std::tm& timeObj) noexcept;

	/** Creates an Hour object from the given \c std::time_t object. */
	explicit Hour (std::time_t time);

	///@}

	LIMES_CONSTEXPR_MOVABLE (Hour);
	LIMES_CONSTEXPR_COPYABLE (Hour);

	/** Returns true if this %hour is between midnight and noon. */
	[[nodiscard]] constexpr bool isAM() const noexcept;

	/** Returns true if this %hour is between noon and midnight. */
	[[nodiscard]] constexpr bool isPM() const noexcept;

	/** Returns true if this hour is exactly noon. */
	[[nodiscard]] constexpr bool isNoon() const noexcept;

	/** Returns true if this hour is exactly midnight. */
	[[nodiscard]] constexpr bool isMidnight() const noexcept;

	/** Returns the number of hours since midnight that this Hour represents.
		This is commonly known as 24-hour or military time.
	 */
	[[nodiscard]] constexpr int hoursSinceMidnight() const noexcept;

	/** Returns this Hour represented as an integer between 1 and 12.
		12 is returned for midnight.
	 */
	[[nodiscard]] constexpr int getIn12HourFormat() const noexcept;

	/** @name Incrementing */
	///@{
	constexpr Hour&				 operator++() noexcept;
	constexpr Hour&				 operator+= (int hoursToAdd) noexcept;
	[[nodiscard]] constexpr Hour operator+ (int hoursToAdd) const noexcept;
	///@}

	/** @name Decrementing */
	///@{
	constexpr Hour&				 operator--() noexcept;
	constexpr Hour&				 operator-= (int hoursToSubtract) noexcept;
	[[nodiscard]] constexpr Hour operator- (int hoursToSubtract) const noexcept;
	///@}

	/** @name Greater/less than comparisons */
	///@{
	[[nodiscard]] constexpr bool operator> (const Hour& other) const noexcept;
	[[nodiscard]] constexpr bool operator<(const Hour& other) const noexcept;
	///@}

	/** @name Equality comparisons */
	///@{
	[[nodiscard]] constexpr bool operator== (const Hour& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Hour& other) const noexcept;
	///@}

	/** Returns an Hour object representing the current %hour. */
	[[nodiscard]] static Hour getCurrent();

	/** Returns an Hour object representing the %hour this code was compiled. */
	[[nodiscard]] LIMES_PURE_FUNCTION static consteval Hour getCompilationHour() noexcept;

	/** Utility function for creating an Hour representing noon. */
	[[nodiscard]] LIMES_PURE_FUNCTION static constexpr Hour noon() noexcept;

	/** Utility function for creating an Hour representing midnight. */
	[[nodiscard]] LIMES_PURE_FUNCTION static constexpr Hour midnight() noexcept;

private:

	int hour { 0 };
};

}  // namespace time

LIMES_END_NAMESPACE

#include "impl/hour_impl.h"
