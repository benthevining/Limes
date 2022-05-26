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

#include <limes_namespace.h>
#include <cmath>
#include <type_traits>
#include <limes_export.h>
#include <algorithm>
#include "../system/limes_assert.h"
#include "../system/compiler_defs.h"
#include "../misc/preprocessor.h"

#if LIMES_HAS_INCLUDE(<numbers>)
#	include <numbers>
#endif

/** @defgroup math Math
	Math utilities.
	@ingroup limes_core
 */

/** @file
	This file contains some math utility functions.
	@ingroup math
 */

LIMES_BEGIN_NAMESPACE

/** This namespace contains math utilities.
	@ingroup math
 */
namespace math
{

/** @ingroup math
	@{
 */

/** @concept Scalar
	A scalar type.
 */
template <typename T>
concept Scalar = std::is_scalar_v<T>;

/** @concept Integral
	An integral type.
 */
template <typename T>
concept Integral = std::is_integral_v<T>;

/** A constexpr-enabled absolute value function. */
template <Scalar T>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION constexpr T abs (T val) noexcept;

/** A constexpr-enabled negation function. */
template <Scalar T>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION constexpr T negate (T val) noexcept;

/** A constexpr-enabled rounding function. */
template <Scalar T>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION constexpr int round (T val) noexcept;

/** A constexpr-enabled clipping function. */
template <Scalar T>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION constexpr T limit (T input, T min, T max) noexcept;

/** A constexpr-enabled power function. */
template <Integral T>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION constexpr T power (T number, T power) noexcept;

/** A constexpr-enabled mapping function. */
template <Scalar T>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION constexpr T map (T input, T sourceRangeMin, T sourceRangeMax, T targetRangeMin, T targetRangeMax) noexcept;

/** Returns true if \c val is 2^something. */
template <Integral Integer>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION constexpr bool isPowerOf2 (Integer val) noexcept;

/** Returns true if the number is divisible by the divisor with no remainder. */
template <Integral Integer>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION constexpr bool isDivisibleBy (Integer number, Integer divisor) noexcept;

/** Returns true if the number is evenly divisible by 2. */
template <Integral Integer>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION constexpr bool numberIsEven (Integer number) noexcept;

/** Returns true if the number is a prime number. */
template <Integral Integer>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION constexpr bool isPrime (Integer number) noexcept;

/** Computes the factorial of the number. */
template <Integral T>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION constexpr T factorial (T number) noexcept;

/** Returns the median of three values. */
template <Scalar T>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION constexpr T middleOfThree (T a, T b, T c) noexcept;

/** Returns the period in samples of a frequency at the specified samplerate. */
template <Scalar FreqType>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION constexpr int periodInSamples (double samplerate, FreqType freqHz) noexcept;

/** Returns the frequency in Hz with a given period in samples at the specified samplerate. */
template <Scalar PeriodType>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION constexpr PeriodType freqFromPeriod (double samplerate, PeriodType period) noexcept;

/** Converts a number of samples to milliseconds at the specified samplerate. */
template <Integral Integer>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION constexpr double sampsToMs (double samplerate, Integer numSamples) noexcept;

/** Converts a number of milliseconds to the closest integer number of samples at the specified samplerate. */
template <Scalar msType>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION constexpr int msToSamps (double samplerate, msType ms) noexcept;

/** Converts a MIDI note to a frequency in Hz. */
template <Scalar T>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION constexpr T midiToFreq (T midiNote) noexcept;

/** Converts a frequency in Hz to a MIDI note. */
template <Scalar T>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION inline T freqToMidi (T freqHz) noexcept;

/*---------------------------------------------------------------------------------------------------------------*/

/** This namespace contains some mathematical constants.
	@ingroup math
 */
namespace constants
{

#if defined(DOXYGEN) || LIMES_HAS_INCLUDE(<numbers>)

/** Pi. */
template <Scalar T>
LIMES_EXPORT static constexpr const T pi = std::numbers::pi_v<T>;

#else

template <Scalar T>
LIMES_EXPORT static constexpr const T pi = static_cast<T> (3.1415916);

#endif

/** Two pi. */
template <Scalar T>
LIMES_EXPORT static constexpr const T two_pi = pi<T>* T (2.);

/** One-half pi. */
template <Scalar T>
LIMES_EXPORT static constexpr const T half_pi = pi<T> / T (2.);

/** The alpha value used for computing default Blackman filters. */
template <Scalar T>
LIMES_EXPORT static constexpr const T blackman_alpha = T (0.16);

}  // namespace constants

/*---------------------------------------------------------------------------------------------------------------*/

template <Scalar T>
constexpr T abs (T val) noexcept
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
constexpr T negate (T val) noexcept
{
	if constexpr (std::is_unsigned<T>::value)
		return val;
	else
		return -val;
}

template <Scalar T>
constexpr int round (T val) noexcept
{
	if constexpr (std::is_integral_v<T>)
		return static_cast<int> (val);
	else
	{
		if (val >= T (0))
			return static_cast<int> (val + 0.5);

		return static_cast<int> (val - 0.5);
	}
}

template <Scalar T>
constexpr T limit (T input, T min, T max) noexcept
{
	LIMES_ASSERT (max > min);

	if (input > max) return max;
	if (input < min) return min;
	return input;
}

template <Integral T>
constexpr T power (T number, T power_) noexcept
{
	if (power_ < T (0))
		return T (1) / power (number, -power_);

	auto result = T (1);

	do
	{
		if (power_ & T (1))
			result = result * number;

		number *= number;
		power_ = power_ >> T (1);
	} while (power_ > T (0));

	return result;
}

template <Scalar T>
constexpr T map (T input, T sourceRangeMin, T sourceRangeMax, T targetRangeMin, T targetRangeMax) noexcept
{
	LIMES_ASSERT (sourceRangeMax != sourceRangeMin);  // mapping from a range of zero will produce NaN!
	LIMES_ASSERT (targetRangeMax != targetRangeMin);

	return targetRangeMin + ((targetRangeMax - targetRangeMin) * (input - sourceRangeMin)) / (sourceRangeMax - sourceRangeMin);
}

template <Integral Integer>
constexpr bool isPowerOf2 (Integer val) noexcept
{
	return val > 0 && (val & (val - 1)) == 0;
}

template <Integral Integer>
constexpr bool isDivisibleBy (Integer number, Integer divisor) noexcept
{
	return number % divisor == 0;
}

template <Integral Integer>
constexpr bool numberIsEven (Integer number) noexcept
{
	return isDivisibleBy (number, Integer (2));
}

template <Integral Integer>
constexpr bool isPrime (Integer number) noexcept
{
	if (number <= 1)
		return false;

	if (number == 2 || number == 3)
		return true;

	if (isDivisibleBy (number, Integer (2)) || isDivisibleBy (number, Integer (3)))
		return false;

	for (auto i = Integer (5); (i * i) <= number; i += 6)
		if (isDivisibleBy (number, i) || isDivisibleBy (number, i + Integer (2)))
			return false;

	return true;
}

template <Integral T>
constexpr T factorial (T number) noexcept
{
	LIMES_ASSERT (number >= 0);

	if (number == 0)
		return 1;

	return number * factorial (number - T (1));
}

template <Scalar T>
constexpr T middleOfThree (T a, T b, T c) noexcept
{
	return std::max ({ std::min (a, b),
					   std::min (std::max (a, b)),
					   c });
}

template <Scalar FreqType>
constexpr int periodInSamples (double samplerate, FreqType freqHz) noexcept
{
	LIMES_ASSERT (freqHz > FreqType (0.));

	const auto val = samplerate / static_cast<double> (freqHz);

	return round (val);
}

template <Scalar PeriodType>
constexpr PeriodType freqFromPeriod (double samplerate, PeriodType period) noexcept
{
	LIMES_ASSERT (period > PeriodType (0.));

	const auto val = samplerate / static_cast<double> (period);

	if constexpr (std::is_integral_v<PeriodType>)
		return static_cast<PeriodType> (round (val));
	else
		return static_cast<PeriodType> (val);
}

template <Integral Integer>
constexpr double sampsToMs (double samplerate, Integer numSamples) noexcept
{
	LIMES_ASSERT (samplerate > 0.);

	const auto val = static_cast<double> (numSamples) / samplerate * 1000.;

	return static_cast<double> (val);
}

template <Scalar msType>
constexpr int msToSamps (double samplerate, msType ms) noexcept
{
	const auto val = samplerate / 1000. * static_cast<double> (ms);

	return round (val);
}

template <Scalar T>
constexpr T midiToFreq (T midiNote) noexcept
{
	const auto pow = static_cast<T> (power (2, round ((round (midiNote) - 69) / 12)));

	const auto val = T (440) * pow;

	if constexpr (std::is_integral_v<T>)
		return static_cast<T> (round (val));
	else
		return static_cast<T> (val);
}

template <Scalar T>
T freqToMidi (T freqHz) noexcept
{
	const auto val = T (69) + T (12) * static_cast<T> (std::log2 (static_cast<double> (freqHz) / 440.));

	if constexpr (std::is_integral_v<T>)
		return static_cast<T> (round (val));
	else
		return static_cast<T> (val);
}

/** @} */

}  // namespace math

LIMES_END_NAMESPACE
