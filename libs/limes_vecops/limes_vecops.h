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

#include "util/Platform.h"
#include "util/Constants.h"
#include <type_traits>
#include <string>


static_assert (sizeof (float) == 4, "float is not 32-bits wide");
static_assert (sizeof (double) == 8, "double is not 64-bits wide");


#if (LIMES_VECOPS_USE_VDSP && LIMES_VECOPS_USE_IPP) || (LIMES_VECOPS_USE_VDSP && LIMES_VECOPS_USE_MIPP) || (LIMES_VECOPS_USE_IPP && LIMES_VECOPS_USE_MIPP)
#	error "Only one of LIMES_VECOPS_USE_VDSP, LIMES_VECOPS_USE_IPP, or LIMES_VECOPS_USE_MIPP may be set to 1!"
#endif


#ifndef LIMES_VECOPS_USE_VDSP
#	if (LIMES_VECOPS_USE_IPP || LIMES_VECOPS_USE_MIPP)
#		define LIMES_VECOPS_USE_VDSP 0
#	else
#		if LIMES_APPLE
#			define LIMES_VECOPS_USE_VDSP 1
#		else
#			define LIMES_VECOPS_USE_VDSP 0
#		endif
#	endif
#endif

#ifndef LIMES_VECOPS_USE_IPP
#	if (LIMES_VECOPS_USE_VDSP || LIMES_VECOPS_USE_MIPP)
#		define LIMES_VECOPS_USE_IPP 0
#	else
#		if LIMES_INTEL && __has_include(<ipps.h>)
#			define LIMES_VECOPS_USE_IPP 1
#		else
#			define LIMES_VECOPS_USE_IPP 0
#		endif
#	endif
#endif

#ifndef LIMES_VECOPS_USE_MIPP
#	if (LIMES_VECOPS_USE_VDSP || LIMES_VECOPS_USE_IPP)
#		define LIMES_VECOPS_USE_MIPP 0
#	else
#		if (LIMES_SSE || LIMES_AVX || LIMES_AVX_512 || LIMES_ARM_NEON) && __has_include(<mipp.h>)
#			define LIMES_VECOPS_USE_MIPP 1
#		else
#			define LIMES_VECOPS_USE_MIPP 0
#		endif
#	endif
#endif


namespace limes::vecops
{

template <typename T>
concept Integral = Scalar<T> && std::is_integral_v<T>;


template <Scalar DataType, Integral SizeType>
void fill (DataType* const data, SizeType size, DataType constantToFill);

template <Scalar DataType, Integral SizeType>
void clear (DataType* const data, SizeType size);

template <Scalar DataType, Integral SizeType>
void copy (DataType* const dest, const DataType* const source, SizeType size);

template <Scalar DataType, Integral SizeType>
void swap (DataType* const vecA, DataType* const vecB, SizeType size);


template <Scalar DataType1, Scalar DataType2>
static void convert (DataType1* const dest, const DataType2* const source, int size)
{
	for (auto i = 0; i < size; ++i)
		dest[i] = static_cast<DataType1> (source[i]);
}

template <Scalar DataType1, Scalar DataType2, Integral SizeType>
static void convert (DataType1* const dest, const DataType2* const source, SizeType size)
{
	convert (dest, source, static_cast<int> (size));
}

/*---------------------------------------------------------------------------------------------------------------------------*/


/*-----  ADDITION  -----*/

template <Scalar DataType, Integral SizeType>
void add (DataType* const data, SizeType size, DataType constantToAdd);

template <Scalar DataType, Integral SizeType>
void add (DataType* const dataAndDest, SizeType size, const DataType* const dataToAdd);

template <Scalar DataType, Integral SizeType>
void addAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToAdd);

template <Scalar DataType, Integral SizeType>
void addAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToAdd);


/*-----  SUBTRACTION  -----*/

template <Scalar DataType, Integral SizeType>
void subtract (DataType* const data, SizeType size, DataType constantToSubtract);

template <Scalar DataType, Integral SizeType>
void subtract (DataType* const dataAndDest, SizeType size, const DataType* const dataToSubtract);

template <Scalar DataType, Integral SizeType>
void subtractAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToSubtract);

template <Scalar DataType, Integral SizeType>
void subtractAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToSubtract);


/*-----  MULTIPLICATION  -----*/

template <Scalar DataType, Integral SizeType>
void multiply (DataType* const data, SizeType size, DataType constantToMultiply);

template <Scalar DataType, Integral SizeType>
void multiply (DataType* const dataAndDest, SizeType size, const DataType* const dataToMultiply);

template <Scalar DataType, Integral SizeType>
void multiplyAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToMultiply);

template <Scalar DataType, Integral SizeType>
void multiplyAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToMultiply);


/*-----  DIVISION  -----*/

template <Scalar DataType, Integral SizeType>
void divide (DataType* const data, SizeType size, DataType constantToDivide);

template <Scalar DataType, Integral SizeType>
void divide (DataType* const dataAndDest, SizeType size, const DataType* const dataToDivide);

template <Scalar DataType, Integral SizeType>
void divideAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToDivide);

template <Scalar DataType, Integral SizeType>
void divideAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToDivide);


/*---------------------------------------------------------------------------------------------------------------------------*/


template <Scalar DataType, Integral SizeType>
void square (DataType* const dataAndDest, SizeType size);

template <Scalar DataType, Integral SizeType>
void squareAndCopy (DataType* const dest, const DataType* const data, SizeType size);

template <Scalar DataType, Integral SizeType>
void squareRoot (DataType* const dataAndDest, SizeType size);

template <Scalar DataType, Integral SizeType>
void squareRootAndCopy (DataType* const dest, const DataType* const data, SizeType size);


/*---------------------------------------------------------------------------------------------------------------------------*/

/*-----  Sorting and ordering functions  -----*/

template <Scalar DataType, Integral SizeType>
void reverse (DataType* const dataAndDest, SizeType size);

template <Scalar DataType, Integral SizeType>
void reverseAndCopy (DataType* const dest, const DataType* const data, SizeType size);

template <Scalar DataType, Integral SizeType>
void sort (DataType* const dataAndDest, SizeType size);

template <Scalar DataType, Integral SizeType>
void sortAndCopy (DataType* const dest, const DataType* const data, SizeType size);

template <Scalar DataType, Integral SizeType>
void sortReverse (DataType* const dataAndDest, SizeType size);

template <Scalar DataType, Integral SizeType>
void sortReverseAndCopy (DataType* const dest, const DataType* const data, SizeType size);

template <Scalar DataType, Integral SizeType1, Integral SizeType2>
void interleave (DataType* const output, const DataType* const * const origData, SizeType1 numChannels, SizeType2 numSamples);

template <Scalar DataType, Integral SizeType1, Integral SizeType2>
void deinterleave (DataType* const * const output, const DataType* const interleavedData, SizeType1 numChannels, SizeType2 numSamples);


/*---------------------------------------------------------------------------------------------------------------------------*/


template <Scalar DataType, Integral SizeType>
void abs (DataType* const dataAndDest, SizeType size);

template <Scalar DataType, Integral SizeType>
void absAndCopy (DataType* const dest, const DataType* const data, SizeType size);


template <Scalar DataType, Integral SizeType>
void negate (DataType* const dataAndDest, SizeType size);

template <Scalar DataType, Integral SizeType>
void negateAndCopy (DataType* const dest, const DataType* const data, SizeType size);


template <Scalar DataType, Integral SizeType>
void clip (DataType* const dataAndDest, SizeType size, DataType lowClip = -1, DataType hiClip = 1);

template <Scalar DataType, Integral SizeType>
void clipAndCopy (DataType* const dest, const DataType* const data, SizeType size, DataType lowClip = -1, DataType hiClip = 1);


template <Scalar DataType, Integral SizeType>
[[nodiscard]] DataType max (const DataType* const data, SizeType size);

template <Scalar DataType, Integral SizeType, Integral IndexType>
void max (const DataType* const data, SizeType size, DataType& maxValue, IndexType& maxIndex);

template <Scalar DataType, Integral SizeType>
[[nodiscard]] DataType maxAbs (const DataType* const data, SizeType size);

template <Scalar DataType, Integral SizeType, Integral IndexType>
void maxAbs (const DataType* const data, SizeType size, DataType& maxValue, IndexType& maxIndex);


template <Scalar DataType, Integral SizeType>
[[nodiscard]] DataType min (const DataType* const data, SizeType size);

template <Scalar DataType, Integral SizeType, Integral IndexType>
void min (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex);

template <Scalar DataType, Integral SizeType>
[[nodiscard]] DataType minAbs (const DataType* const data, SizeType size);

template <Scalar DataType, Integral SizeType, Integral IndexType>
void minAbs (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex);


template <Scalar DataType, Integral SizeType>
void minMax (const DataType* const data, SizeType size, DataType& minValue, DataType& maxValue);

template <Scalar DataType, Integral SizeType, Integral IndexType>
void minMax (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex, DataType& maxValue, IndexType& maxIndex);

template <Scalar DataType, Integral SizeType>
void minMaxAbs (const DataType* const data, SizeType size, DataType& minValue, DataType& maxValue);

template <Scalar DataType, Integral SizeType, Integral IndexType>
void minMaxAbs (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex, DataType& maxValue, IndexType& maxIndex);


template <Scalar DataType, Integral SizeType>
[[nodiscard]] DataType range (const DataType* const data, SizeType size);

template <Scalar DataType, Integral SizeType>
[[nodiscard]] DataType rangeAbs (const DataType* const data, SizeType size);


template <Scalar DataType, Integral SizeType>
[[nodiscard]] DataType sum (const DataType* const data, SizeType size);

template <Scalar DataType, Integral SizeType>
[[nodiscard]] DataType mean (const DataType* const data, SizeType size);


/*---------------------------------------------------------------------------------------------------------------------------*/


template <Scalar DataType, Integral SizeType1, Integral SizeType2>
void mix (DataType* const output, const DataType* const * const origData, SizeType1 numChannels, SizeType2 numSamples);


template <Scalar DataType, Integral SizeType>
DataType rms (const DataType* const data, SizeType size);


template <Scalar DataType, Integral SizeType>
[[nodiscard]] int countZeroCrossings (const DataType* const data, SizeType size);


/*---------------------------------------------------------------------------------------------------------------------------*/


template <Scalar DataType, Integral SizeType>
void generateRamp (DataType* const output, SizeType size, DataType startValue, DataType endValue);

template <Scalar DataType, Integral SizeType>
void applyRamp (DataType* const dataAndDest, SizeType size, DataType startValue, DataType endValue);

template <Scalar DataType, Integral SizeType>
void applyRampAndCopy (DataType* const dest, const DataType* const data, SizeType size, DataType startValue, DataType endValue);


namespace window
{

template <Scalar DataType, Integral SizeType>
void generateBlackman (DataType* const output, SizeType size);

template <Scalar DataType, Integral SizeType>
void applyBlackman (DataType* const dataAndDest, SizeType size);

template <Scalar DataType, Integral SizeType>
void applyBlackmanAndCopy (DataType* const dest, const DataType* const data, SizeType size);

template <Scalar DataType, Integral SizeType>
void generateHamm (DataType* const output, SizeType size);

template <Scalar DataType, Integral SizeType>
void applyHamm (DataType* const dataAndDest, SizeType size);

template <Scalar DataType, Integral SizeType>
void applyHammAndCopy (DataType* const dest, const DataType* const data, SizeType size);

template <Scalar DataType, Integral SizeType>
void generateHanning (DataType* const output, SizeType size);

template <Scalar DataType, Integral SizeType>
void applyHanning (DataType* const dataAndDest, SizeType size);

template <Scalar DataType, Integral SizeType>
void applyHanningAndCopy (DataType* const dest, const DataType* const data, SizeType size);

}  // namespace window


/*---------------------------------------------------------------------------------------------------------------------------*/


[[nodiscard]] constexpr bool isUsingVDSP() noexcept
{
#if LIMES_VECOPS_USE_VDSP
	return true;
#else
	return false;
#endif
}

[[nodiscard]] constexpr bool isUsingIPP() noexcept
{
#if LIMES_VECOPS_USE_IPP
	return true;
#else
	return false;
#endif
}

[[nodiscard]] constexpr bool isUsingMIPP() noexcept
{
#if LIMES_VECOPS_USE_MIPP
	return true;
#else
	return false;
#endif
}

[[nodiscard]] constexpr bool isUsingFallback() noexcept
{
	return ! (isUsingVDSP() || isUsingIPP() || isUsingMIPP());	// cppcheck-suppress knownConditionTrueFalse
}

static_assert (isUsingVDSP() || isUsingIPP() || isUsingMIPP() || isUsingFallback());


[[nodiscard]] static constexpr const char* const getImplementationName()
{
	if constexpr (isUsingVDSP())
		return "Apple vDSP";
	else if constexpr (isUsingIPP())
		return "Intel IPP";
	else if constexpr (isUsingMIPP())
		return "MIPP";
	else
		return "Fallback";
}

}  // namespace limes::vecops

#include "impl/fallback_impl.h"

#if LIMES_VECOPS_USE_VDSP
#	include "impl/vdsp.h"
#elif LIMES_VECOPS_USE_IPP
#	include "impl/ipp.h"
#elif LIMES_VECOPS_USE_MIPP
#	include "impl/mipp.h"
#else
#	include "impl/fallback.h"
#endif

#include "fft/fft.h"
