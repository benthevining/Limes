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

#include <cmath>
#include "../misc/Platform.h"

namespace lemons::math
{

template <typename T>
concept Scalar = std::is_scalar_v<T>;

template <typename T>
concept Integral = std::is_integral_v<T>;

/** @addtogroup lemons_math Math
	@ingroup lemons_core
	Math utility functions.

 @{
 */

/** A constexpr-enabled absolute value function. */
template <Scalar T>
[[nodiscard]] constexpr T abs (T val) noexcept
{
	if (val < T (0))
		return -val;

	return val;
}


template <Scalar T>
[[nodiscard]] constexpr T negate (T val) noexcept
{
	if constexpr (std::is_unsigned<T>::value)
		return val;
	else
	{
		if (val < T (0))
			return -val;

		return val;
	}
}


template <Scalar T>
[[nodiscard]] constexpr int round (T val) noexcept
{
	if constexpr (std::is_integral_v<T>)
		return static_cast<int> (val);
	else
		return (val >= T (0)) ? static_cast<int> (val + 0.5) : static_cast<int> (val - 0.5);
}


/** Returns true if val is 2^something.
 */
template <Integral Integer>
[[nodiscard]] constexpr bool isPowerOf2 (Integer val) noexcept
{
	return val > 0 && (val & (val - 1)) == 0;
}


/** Returns true if the number is divisible by the divisor with no remainder. */
[[nodiscard]] constexpr bool isDivisibleBy (int number, int divisor) noexcept
{
	return number % divisor == 0;
}


/** Returns true if the number is evenly divisible by 2. */
[[nodiscard]] constexpr bool numberIsEven (int number) noexcept
{
	return isDivisibleBy (number, 2);
}


/** Returns the period in samples of a frequency at the specified samplerate. */
template <Scalar FreqType>
[[nodiscard]] inline int periodInSamples (double samplerate, FreqType freqHz) noexcept
{
	LIMES_ASSERT (freqHz > FreqType (0.));

	const auto val = samplerate / static_cast<double> (freqHz);

	return round (val);
}

/** Returns the frequency in Hz with a given period in samples at the specified samplerate. */
template <Scalar PeriodType>
[[nodiscard]] inline PeriodType freqFromPeriod (double samplerate, PeriodType period) noexcept
{
	LIMES_ASSERT (period > PeriodType (0.));

	const auto val = samplerate / static_cast<double> (period);

	if constexpr (std::is_same_v<PeriodType, int>)
		return round (val);
	else
		return static_cast<PeriodType> (val);
}


/** Converts a number of samples to milliseconds at the specified samplerate. */
[[nodiscard]] constexpr double sampsToMs (double samplerate, int numSamples) noexcept
{
	LIMES_ASSERT (samplerate > 0.);

	const auto val = static_cast<double> (numSamples) / samplerate * 1000.;

	return static_cast<double> (val);
}

/** Converts a number of milliseconds to the closest integer number of samples at the specified samplerate. */
template <Scalar msType>
[[nodiscard]] inline int msToSamps (double samplerate, msType ms) noexcept
{
	const auto val = samplerate / 1000. * static_cast<double> (ms);

	return round (val);
}

/** Converts a MIDI note to a frequency in Hz. */
template <Scalar T>
[[nodiscard]] inline T midiToFreq (T midiNote) noexcept
{
	const auto val = 440. * std::pow (2., (static_cast<double> (midiNote) - 69.) / 12.);

	if constexpr (std::is_same_v<T, int>)
		return round (val);
	else
		return static_cast<T> (val);
}

/** Converts a frequency in Hz to a MIDI note. */
template <Scalar T>
[[nodiscard]] inline T freqToMidi (T freqHz) noexcept
{
	const auto val = 69. + 12. * std::log2 (static_cast<double> (freqHz) / 440.);

	if constexpr (std::is_same_v<T, int>)
		return round (val);
	else
		return static_cast<T> (val);
}

///@}

}  // namespace lemons::math
