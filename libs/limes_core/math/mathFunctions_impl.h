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
#include <algorithm>
#include "../system/limes_assert.h"

/** @file
	This file contains implementation details for the math utility functions.
	@ingroup core_math
 */

LIMES_BEGIN_NAMESPACE

namespace math
{

/// @cond

#pragma mark Utility functions

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
	if (power_ == 0)
		return T (1);

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

template <Integral T>
constexpr T cantorPair (T a, T b) noexcept
{
	const auto A = a >= T (0) ? T (2) * a : T (-2) * a - T (1);
	const auto B = b >= T (0) ? T (2) * b : T (-2) * b - T (1);

	return (A + B) * (A + B + T (1)) / T (2) + A;
}

template <Integral T>
constexpr T szudzikPair (T a, T b) noexcept
{
	const auto A = a >= T (0) ? T (2) * a : T (-2) * a - T (1);
	const auto B = b >= T (0) ? T (2) * b : T (-2) * b - T (1);

	if (A >= B)
		return A * A + A + B;

	return A + B * B;
}

template <Integral T>
constexpr T numDigits (T num) noexcept
{
	T count = 0;

	while (num != 0)
	{
		num /= 10;
		++count;
	}

	return count;
}

/*---------------------------------------------------------------------------------------------------------------*/

#pragma mark Audio utility functions

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

/// @endcond

}  // namespace math

LIMES_END_NAMESPACE
