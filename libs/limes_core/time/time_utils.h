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

LIMES_BEGIN_NAMESPACE

namespace time
{

// NB. this is only needed until all compilers support std::is_clock_v
// clang-format off
template <typename T>
concept Clock = requires (T c)
{
	{ c.now() } -> convertible_to<std::chrono::time_point<T>>;
};
// clang-format on

template <Clock ClockType>
using Point = std::chrono::time_point<ClockType>;

using SystemClock = std::chrono::system_clock;

template <Clock InputClock, Clock OutputClock>
LIMES_EXPORT [[nodiscard]] Point<OutputClock> convertClockTimePoints (const Point<InputClock>& inputTime)
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

template <Clock InputClock>
LIMES_EXPORT [[nodiscard]] Point<SystemClock> toSystemTime (const Point<InputClock>& inputTime)
{
	return convertClockTimePoints<InputClock, SystemClock> (inputTime);
}

template <Clock ClockType>
LIMES_EXPORT [[nodiscard]] std::tm toTimeObj (const Point<ClockType>& inputTime)
{
	const auto timeT = SystemClock::to_time_t (toSystemTime (inputTime));

	return *std::localtime (&timeT);
}

template <Clock ClockType>
LIMES_EXPORT [[nodiscard]] Point<ClockType> fromTimeObj (std::time_t time)
{
	const auto systemTimePoint = std::chrono::system_clock::from_time_t (time);

	return convertClockTimePoints<SystemClock, ClockType> (systemTimePoint);
}

template <Clock ClockType>
LIMES_EXPORT [[nodiscard]] Point<ClockType> fromTimeObj (std::tm& timeObj)
{
	return fromTimeObj<ClockType> (std::mktime (&timeObj));
}

}  // namespace time

LIMES_END_NAMESPACE
