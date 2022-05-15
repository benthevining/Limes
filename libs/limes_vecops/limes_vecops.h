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

/*

limes_vecops
--------------------

The Limes vector operations library.

Dependencies:
- limes_core

Optional dependencies:
- FFTW
and/or one of:
- Apple's Accelerate framework
- Intel IPP
- MIPP

*/

#pragma once

#include <type_traits>
#include <string>
#include <limes_export.h>
#include <limes_platform.h>
#include <limes_core.h>
#include <limes_namespace.h>

// these should never fail, but just as a sanity check:
static_assert (sizeof (float) == 4, "float is not 32-bits wide");
static_assert (sizeof (double) == 8, "double is not 64-bits wide");


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
#		if LIMES_INTEL && LIMES_HAS_INCLUDE(<ipps.h>)
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
#		if (LIMES_SSE || LIMES_AVX || LIMES_AVX_512 || LIMES_ARM_NEON) && LIMES_HAS_INCLUDE(<mipp.h>)
#			define LIMES_VECOPS_USE_MIPP 1
#		else
#			define LIMES_VECOPS_USE_MIPP 0
#		endif
#	endif
#endif

#if (LIMES_VECOPS_USE_VDSP && LIMES_VECOPS_USE_IPP) || (LIMES_VECOPS_USE_VDSP && LIMES_VECOPS_USE_MIPP) || (LIMES_VECOPS_USE_IPP && LIMES_VECOPS_USE_MIPP)
#	error "Only one of LIMES_VECOPS_USE_VDSP, LIMES_VECOPS_USE_IPP, or LIMES_VECOPS_USE_MIPP may be set to 1!"
#endif

#ifndef LIMES_VECOPS_USE_POMMIER
#	if LIMES_ARM_NEON || (LIMES_SSE && ! LIMES_MSVC)  // MSVC doesn't like to compile MMX intrinsics
#		define LIMES_VECOPS_USE_POMMIER 1
#	else
#		define LIMES_VECOPS_USE_POMMIER 0
#	endif
#endif

/*---------------------------------------------------------------------------------------------------------------------------*/

LIMES_BEGIN_NAMESPACE

namespace vecops
{

template <typename T>
concept Scalar = std::is_scalar_v<T>;

template <typename T>
concept Integral = std::is_integral_v<T>;

#pragma mark Basic functions

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void fill (DataType* const data, SizeType size, DataType constantToFill);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void clear (DataType* const data, SizeType size);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void copy (DataType* const dest, const DataType* const source, SizeType size);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void swap (DataType* const vecA, DataType* const vecB, SizeType size);


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

#pragma mark Arithmetic functions

/*-----  ADDITION  -----*/

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void add (DataType* const data, SizeType size, DataType constantToAdd);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void add (DataType* const dataAndDest, SizeType size, const DataType* const dataToAdd);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void addAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToAdd);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void addAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToAdd);


/*-----  SUBTRACTION  -----*/

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void subtract (DataType* const data, SizeType size, DataType constantToSubtract);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void subtract (DataType* const dataAndDest, SizeType size, const DataType* const dataToSubtract);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void subtractAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToSubtract);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void subtractAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToSubtract);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void subtractInv (DataType* const data, SizeType size, DataType constantToSubtractFrom);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void subtractInvAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToSubtractFrom);


/*-----  MULTIPLICATION  -----*/

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void multiply (DataType* const data, SizeType size, DataType constantToMultiply);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void multiply (DataType* const dataAndDest, SizeType size, const DataType* const dataToMultiply);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void multiplyAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToMultiply);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void multiplyAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToMultiply);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT [[nodiscard]] DataType dotProduct (const DataType* const vecA, const DataType* const vecB, SizeType size);

/*-----  DIVISION  -----*/

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void divide (DataType* const data, SizeType size, DataType constantToDivide);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void divide (DataType* const dataAndDest, SizeType size, const DataType* const dataToDivide);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void divideAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToDivide);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void divideAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToDivide);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void divideInv (DataType* const data, SizeType size, DataType constantToDivideFrom);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void divideInvAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToDivideFrom);

/*---------------------------------------------------------------------------------------------------------------------------*/


template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void square (DataType* const dataAndDest, SizeType size);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void squareAndCopy (DataType* const dest, const DataType* const data, SizeType size);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void squareRoot (DataType* const dataAndDest, SizeType size);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void squareRootAndCopy (DataType* const dest, const DataType* const data, SizeType size);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void invSquareRoot (DataType* const dataAndDest, SizeType size);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void invSquareRootAndCopy (DataType* const dest, const DataType* const data, SizeType size);


/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark Sorting and ordering functions

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void reverse (DataType* const dataAndDest, SizeType size);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void reverseAndCopy (DataType* const dest, const DataType* const data, SizeType size);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void sort (DataType* const dataAndDest, SizeType size);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void sortAndCopy (DataType* const dest, const DataType* const data, SizeType size);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void sortReverse (DataType* const dataAndDest, SizeType size);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void sortReverseAndCopy (DataType* const dest, const DataType* const data, SizeType size);

template <Scalar DataType, Integral SizeType1, Integral SizeType2>
LIMES_EXPORT void interleave (DataType* const output, const DataType* const * const origData, SizeType1 numChannels, SizeType2 numSamples);

template <Scalar DataType, Integral SizeType1, Integral SizeType2>
LIMES_EXPORT void deinterleave (DataType* const * const output, const DataType* const interleavedData, SizeType1 numChannels, SizeType2 numSamples);


/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark Statistical functions

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void abs (DataType* const dataAndDest, SizeType size);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void absAndCopy (DataType* const dest, const DataType* const data, SizeType size);


template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void negate (DataType* const dataAndDest, SizeType size);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void negateAndCopy (DataType* const dest, const DataType* const data, SizeType size);


template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void clip (DataType* const dataAndDest, SizeType size, DataType lowClip = -1, DataType hiClip = 1);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void clipAndCopy (DataType* const dest, const DataType* const data, SizeType size, DataType lowClip = -1, DataType hiClip = 1);


template <Scalar DataType, Integral SizeType>
LIMES_EXPORT [[nodiscard]] DataType max (const DataType* const data, SizeType size);

template <Scalar DataType, Integral SizeType, Integral IndexType>
LIMES_EXPORT void max (const DataType* const data, SizeType size, DataType& maxValue, IndexType& maxIndex);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT [[nodiscard]] DataType maxAbs (const DataType* const data, SizeType size);

template <Scalar DataType, Integral SizeType, Integral IndexType>
LIMES_EXPORT void maxAbs (const DataType* const data, SizeType size, DataType& maxValue, IndexType& maxIndex);


template <Scalar DataType, Integral SizeType>
LIMES_EXPORT [[nodiscard]] DataType min (const DataType* const data, SizeType size);

template <Scalar DataType, Integral SizeType, Integral IndexType>
LIMES_EXPORT void min (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT [[nodiscard]] DataType minAbs (const DataType* const data, SizeType size);

template <Scalar DataType, Integral SizeType, Integral IndexType>
LIMES_EXPORT void minAbs (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex);


template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void minMax (const DataType* const data, SizeType size, DataType& minValue, DataType& maxValue);

template <Scalar DataType, Integral SizeType, Integral IndexType>
LIMES_EXPORT void minMax (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex, DataType& maxValue, IndexType& maxIndex);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void minMaxAbs (const DataType* const data, SizeType size, DataType& minValue, DataType& maxValue);

template <Scalar DataType, Integral SizeType, Integral IndexType>
LIMES_EXPORT void minMaxAbs (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex, DataType& maxValue, IndexType& maxIndex);


template <Scalar DataType, Integral SizeType>
LIMES_EXPORT [[nodiscard]] DataType range (const DataType* const data, SizeType size);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT [[nodiscard]] DataType rangeAbs (const DataType* const data, SizeType size);


template <Scalar DataType, Integral SizeType>
LIMES_EXPORT [[nodiscard]] DataType sum (const DataType* const data, SizeType size);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT [[nodiscard]] DataType mean (const DataType* const data, SizeType size);


/*---------------------------------------------------------------------------------------------------------------------------*/


template <Scalar DataType, Integral SizeType1, Integral SizeType2>
LIMES_EXPORT void mix (DataType* const output, const DataType* const * const origData, SizeType1 numChannels, SizeType2 numSamples);


template <Scalar DataType, Integral SizeType>
LIMES_EXPORT [[nodiscard]] DataType rms (const DataType* const data, SizeType size);


template <Scalar DataType, Integral SizeType>
LIMES_EXPORT [[nodiscard]] int countZeroCrossings (const DataType* const data, SizeType size);


/*---------------------------------------------------------------------------------------------------------------------------*/


template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void generateRamp (DataType* const output, SizeType size, DataType startValue, DataType endValue);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void applyRamp (DataType* const dataAndDest, SizeType size, DataType startValue, DataType endValue);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void applyRampAndCopy (DataType* const dest, const DataType* const data, SizeType size, DataType startValue, DataType endValue);

#pragma mark Windowing functions

namespace window
{

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void generateBlackman (DataType* const output, SizeType size);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void applyBlackman (DataType* const dataAndDest, SizeType size);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void applyBlackmanAndCopy (DataType* const dest, const DataType* const data, SizeType size);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void generateHamm (DataType* const output, SizeType size);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void applyHamm (DataType* const dataAndDest, SizeType size);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void applyHammAndCopy (DataType* const dest, const DataType* const data, SizeType size);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void generateHanning (DataType* const output, SizeType size);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void applyHanning (DataType* const dataAndDest, SizeType size);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void applyHanningAndCopy (DataType* const dest, const DataType* const data, SizeType size);

}  // namespace window


/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark Complex conversions

/* Only intended for float or double samples */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void polarToCartesian (DataType* const real, DataType* const imag, const DataType* const mag, const DataType* const phase, SizeType size);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void polarToCartesianInterleaved (DataType* const dest, const DataType* const mag, const DataType* const phase, SizeType size);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void cartesianToPolar (DataType* const mag, DataType* const phase, const DataType* const real, const DataType* const imag, SizeType size);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void catesianInterleavedToPolar (DataType* const mag, DataType* const phase, const DataType* const src, SizeType size);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void cartesianToMagnitudes (DataType* const mag, const DataType* const real, const DataType* const imag, SizeType size);

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void cartesianInterleavedToMagnitudes (DataType* const mag, const DataType* const src, SizeType size);

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark Floating point mode functions

LIMES_EXPORT void disableDenormalisedNumberSupport (bool shouldDisable = true) noexcept;

LIMES_EXPORT [[nodiscard]] bool areDenormalsDisabled() noexcept;

LIMES_EXPORT void enableFlushToZeroMode (bool shouldEnable = true) noexcept;

LIMES_EXPORT [[nodiscard]] bool isFlushToZeroEnabled() noexcept;


class LIMES_EXPORT ScopedNoDenormals final
{
public:

	ScopedNoDenormals() noexcept;
	~ScopedNoDenormals() noexcept;

private:

	const intptr_t fpsr;
};


class LIMES_EXPORT ScopedFlushToZero final
{
public:

	ScopedFlushToZero() noexcept;
	~ScopedFlushToZero() noexcept;

private:

	const intptr_t fpsr;
};

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark Implementation kind checking

LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION consteval bool isUsingVDSP() noexcept
{
#if LIMES_VECOPS_USE_VDSP
	return true;
#else
	return false;
#endif
}

LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION consteval bool isUsingIPP() noexcept
{
#if LIMES_VECOPS_USE_IPP
	return true;
#else
	return false;
#endif
}

LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION consteval bool isUsingMIPP() noexcept
{
#if LIMES_VECOPS_USE_MIPP
	return true;
#else
	return false;
#endif
}

LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION consteval bool isUsingFallback() noexcept
{
	return ! (isUsingVDSP() || isUsingIPP() || isUsingMIPP());	// cppcheck-suppress knownConditionTrueFalse
}

static_assert (isUsingVDSP() || isUsingIPP() || isUsingMIPP() || isUsingFallback());


LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION static consteval const char* getImplementationName() noexcept
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

LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION consteval bool isUsingPommierExtensions() noexcept
{
#if LIMES_VECOPS_USE_POMMIER
	return true;
#else
	return false;
#endif
}

}  // namespace vecops

LIMES_END_NAMESPACE

#include "impl/fallback_impl.h"

// IWYU pragma: begin_exports
#if LIMES_VECOPS_USE_VDSP
#	include "impl/vdsp.h"
#elif LIMES_VECOPS_USE_IPP
#	include "impl/ipp.h"
#elif LIMES_VECOPS_USE_MIPP
#	include "impl/mipp.h"
#else
#	include "impl/fallback.h"
#endif
// IWYU pragma: end_exports

#include "fft/limes_fft.h"
