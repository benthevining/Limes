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
#include "../misc/TypeTraits.h"

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

using SystemClock = std::chrono::system_clock;

template <Clock InputClock>
LIMES_EXPORT [[nodiscard]] std::chrono::time_point<SystemClock> toSystemTime (const std::chrono::time_point<InputClock>& inputTime)
{
	if constexpr (std::is_same_v<InputClock, SystemClock>)
		return inputTime;
	else
	{
		const auto inputNow	 = InputClock::now();
		const auto systemNow = SystemClock::now();

		using SystemDuration = typename SystemClock::duration;

		const auto inputDuration = std::chrono::duration_cast<SystemDuration> (inputTime - inputNow);

		return std::chrono::time_point_cast<SystemDuration> (inputDuration + systemNow);
	}
}

template <Clock ClockType>
LIMES_EXPORT [[nodiscard]] std::tm toTimeObj (const std::chrono::time_point<ClockType>& inputTime)
{
	const auto timeT = SystemClock::to_time_t (toSystemTime (inputTime));

	return *std::localtime (&timeT);
}

}  // namespace time

LIMES_END_NAMESPACE
