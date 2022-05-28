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
#include <chrono>
#include <ctime>
#include <type_traits>
#include "../meta/TypeTraits.h"
#include "../system/compiler_defs.h"

/** @defgroup time Time
	Utilities for working with time measurements.
	@ingroup limes_core
 */

/** @file
	This file contains general time utilities.
	@ingroup time
 */

LIMES_BEGIN_NAMESPACE

/** This namespace contains utilities for working with time measurements.
	@ingroup time
 */
namespace time
{

/** @concept Clock
	The same as std::is_clock_v, only implemented here because the standard library concept seems to be missing from some compilers.
 */
// clang-format off
template <typename T>
concept Clock = requires (T c)
{
	{ c.now() } -> meta::convertible_to<std::chrono::time_point<T>>;
};
// clang-format on

/** Convenience typedef for a time point. */
template <Clock ClockType>
using Point = std::chrono::time_point<ClockType>;

/** Convenience typedef for the system clock. */
using SystemClock = std::chrono::system_clock;

/** Converts a time point on one clock to a time point on another clock. */
template <Clock InputClock, Clock OutputClock>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION Point<OutputClock> convertClockTimePoints (const Point<InputClock>& inputTime)
{
	if constexpr (std::is_same_v<InputClock, OutputClock>)
		return inputTime;
	else
	{
		const auto inputNow	 = InputClock::now();
		const auto systemNow = OutputClock::now();

		using OutputDuration = typename OutputClock::duration;

		const auto inputDuration = std::chrono::duration_cast<OutputDuration> (inputTime - inputNow);

		return std::chrono::time_point_cast<OutputDuration> (inputDuration + systemNow);
	}
}

/** Converts a time point on any Clock type to a time point on the SystemClock. */
template <Clock InputClock>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION Point<SystemClock> toSystemTime (const Point<InputClock>& inputTime)
{
	return convertClockTimePoints<InputClock, SystemClock> (inputTime);
}

/** Converts a time point on any Clock type to a \c std::tm object. */
template <Clock ClockType>
LIMES_EXPORT [[nodiscard]] std::tm toTimeObj (const Point<ClockType>& inputTime)
{
	const auto timeT = SystemClock::to_time_t (toSystemTime (inputTime));

	return *std::localtime (&timeT);
}

/** Converts a \c std::time_t object to a time point on the given Clock type. */
template <Clock ClockType>
LIMES_EXPORT [[nodiscard]] Point<ClockType> fromTimeObj (std::time_t time)
{
	const auto systemTimePoint = std::chrono::system_clock::from_time_t (time);

	return convertClockTimePoints<SystemClock, ClockType> (systemTimePoint);
}

/** Converts a \c std::tm object to a time point on the given Clock type. */
template <Clock ClockType>
LIMES_EXPORT [[nodiscard]] Point<ClockType> fromTimeObj (std::tm& timeObj)
{
	return fromTimeObj<ClockType> (std::mktime (&timeObj));
}

}  // namespace time

LIMES_END_NAMESPACE
