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
#include <type_traits>
#include <limes_export.h>
#include "../system/compiler_defs.h"
#include "../misc/preprocessor.h"

#if LIMES_HAS_INCLUDE(<numbers>)
#	include <numbers>
#endif

/** @defgroup core_math Math
	Math utilities.
	@ingroup limes_core
 */

/** @dir libs/limes_core/math
	This directory contains math utilities.
	@ingroup core_math
 */

/** @file
	This file contains some math utility functions.
	@ingroup core_math
 */

LIMES_BEGIN_NAMESPACE

/** This namespace contains math utilities.
	@ingroup core_math
 */
namespace math
{

/** @ingroup core_math
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

#pragma mark Utility functions

/** A constexpr-enabled max function. */
template <Scalar T>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION constexpr T max (T a, T b) noexcept
{
	return a > b ? a : b;
}

/** A constexpr-enabled min function. */
template <Scalar T>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION constexpr T min (T a, T b) noexcept
{
	return a < b ? a : b;
}

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

/** A constexpr-enabled power function.
	The formula is @f$ n=number^{power} @f$.
 */
template <Integral T>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION constexpr T power (T number, T power) noexcept;

/** A constexpr-enabled mapping function. */
template <Scalar T>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION constexpr T map (T input, T sourceRangeMin, T sourceRangeMax, T targetRangeMin, T targetRangeMax) noexcept;

/** Returns true if \c val is 2^something. */
template <Integral Integer>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION constexpr bool isPowerOf2 (Integer val) noexcept;

/** Returns true if the \c number is divisible by the \c divisor with no remainder. */
template <Integral Integer>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION constexpr bool isDivisibleBy (Integer number, Integer divisor) noexcept;

/** Returns true if the \c number is evenly divisible by 2. */
template <Integral Integer>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION constexpr bool numberIsEven (Integer number) noexcept;

/** Returns true if the \c number is a prime number. */
template <Integral Integer>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION constexpr bool isPrime (Integer number) noexcept;

/** Computes the factorial of the \c number. */
template <Integral T>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION constexpr T factorial (T number) noexcept;

/** Returns the median of three values. */
template <Scalar T>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION constexpr T middleOfThree (T a, T b, T c) noexcept;

/** Calculates the Cantor pairing function from two integers.
	This is a computationally efficient way to create a single hash value that combines two others.
	The formula is:
	@f[
		c = (a+b)*\frac{a+b+1}{2}+a
	@f]
	@see szudzikPair
 */
template <Integral T>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION constexpr T cantorPair (T a, T b) noexcept;

/** Calculates the Szudzik pairing function from two integers.
	This is a computationally efficient way to create a single hash value that combines two others.
	@see cantorPair
 */
template <Integral T>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION constexpr T szudzikPair (T a, T b) noexcept;

/*---------------------------------------------------------------------------------------------------------------*/

#pragma mark Audio utility functions

/** Returns the period in samples of a frequency at the specified samplerate.
	The formula is @f$ T=\frac{samplerate}{freqHz} @f$.
	@see freqFromPeriod
 */
template <Scalar FreqType>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION constexpr int periodInSamples (double samplerate, FreqType freqHz) noexcept;

/** Returns the frequency in Hz with a given period in samples at the specified samplerate.
	The formula is @f$ f=\frac{samplerate}{period} @f$.
	@see periodInSamples
 */
template <Scalar PeriodType>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION constexpr PeriodType freqFromPeriod (double samplerate, PeriodType period) noexcept;

/** Converts a number of samples to milliseconds at the specified samplerate.
	The formula is @f$ ms=\frac{numSamples}{samplerate}*1000 @f$.
	@see msToSamps
 */
template <Integral Integer>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION constexpr double sampsToMs (double samplerate, Integer numSamples) noexcept;

/** Converts a number of milliseconds to the closest integer number of samples at the specified samplerate.
	The formula is @f$ s=\frac{samplerate}{1000}*ms @f$.
	@see sampsToMs
 */
template <Scalar msType>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION constexpr int msToSamps (double samplerate, msType ms) noexcept;

/** Converts a MIDI note to a frequency in Hz.
	The formula is @f$ f=440*2^{\frac{midiNote-69}{12}} @f$.
	@see freqToMidi
 */
template <Scalar T>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION constexpr T midiToFreq (T midiNote) noexcept;

/** Converts a frequency in Hz to a MIDI note.
	The formula is @f$ midi=69+12*\log_2\frac{freqHz}{440} @f$.
	@see midiToFreq
 */
template <Scalar T>
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION inline T freqToMidi (T freqHz) noexcept;

/*---------------------------------------------------------------------------------------------------------------*/

#pragma mark Constants

/** This namespace contains some mathematical constants.
	@ingroup core_math
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

/** @} */

}  // namespace math

LIMES_END_NAMESPACE

#include "mathFunctions_impl.h"	 // IWYU pragma: export
