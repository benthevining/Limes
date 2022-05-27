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

/** @defgroup limes_vecops limes_vecops
	The Limes vector operations library.
	All classes and functions in this group are accessible after linking to the limes_vecops library and including limes_vecops.h.
 */

/** @file
	The main header for the limes_vecops library.
	@ingroup limes_vecops
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

#ifdef DOXYGEN

/** @def LIMES_VECOPS_USE_VDSP
	1 if the Apple vDSP library is being used, otherwise 0.
	@ingroup limes_vecops
 */
#	define LIMES_VECOPS_USE_VDSP 0

/** @def LIMES_VECOPS_USE_IPP
	1 if the Intel IPP library is being used, otherwise 0.
	@ingroup limes_vecops
 */
#	define LIMES_VECOPS_USE_IPP 0

/** @def LIMES_VECOPS_USE_MIPP
	1 if the MIPP library is being used, otherwise 0.
	@ingroup limes_vecops
 */
#	define LIMES_VECOPS_USE_MIPP 0

/**	@def LIMES_VECOPS_USE_POMMIER
	1 if the Pommier SIMD extensions are being used, otherwise 0.
	@ingroup limes_vecops
 */
#	define LIMES_VECOPS_USE_POMMIER 0

#endif

/// @cond

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

/// @endcond

/*---------------------------------------------------------------------------------------------------------------------------*/

LIMES_BEGIN_NAMESPACE

/** This namespace contains vector operations functions.
	@ingroup limes_vecops
 */
namespace vecops
{

/** @ingroup limes_vecops
	@{
 */

/** @concept Scalar
	Any scalar type.
 */
template <typename T>
concept Scalar = std::is_scalar_v<T>;

/** @concept Integral
	Any integral type.
 */
template <typename T>
concept Integral = std::is_integral_v<T>;

#pragma mark Basic functions

/** Fills the vector with the specified value. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void fill (DataType* const data, SizeType size, DataType constantToFill);

/** Fills the vector with zeroes. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void clear (DataType* const data, SizeType size);

/** Copies from one vector to another. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void copy (DataType* const dest, const DataType* const source, SizeType size);

/** Swaps the elements of two vectors. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void swap (DataType* const vecA, DataType* const vecB, SizeType size);

/** @} */

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

/** @defgroup vec_arithmetic Arithmetic functions
	Basic arithmetic operations on vectors.
	@ingroup limes_vecops
 */

/*-----  ADDITION  -----*/

/** @defgroup vec_addition Addition
	Vector addition
	@ingroup vec_arithmetic
 */

/** @ingroup vec_addition
	@{
 */

/** Adds a constant to every element of the vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void add (DataType* const data, SizeType size, DataType constantToAdd);

/** Element-wise addition of two vectors. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void add (DataType* const dataAndDest, SizeType size, const DataType* const dataToAdd);

/** Adds a constant to every element of a vector, writing the output to another vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void addAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToAdd);

/** Element-wise addition of two vectors, writing the output to a third vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void addAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToAdd);

/** @} */

/*-----  SUBTRACTION  -----*/

/** @defgroup vec_subtraction Subtraction
	Vector subtraction
	@ingroup vec_arithmetic
 */

/** @ingroup vec_subtraction
	@{
 */

/** Subtracts a constant from every element of the vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void subtract (DataType* const data, SizeType size, DataType constantToSubtract);

/** Element-wise subtraction of two vectors. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void subtract (DataType* const dataAndDest, SizeType size, const DataType* const dataToSubtract);

/** Subtracts a constant from every element of a vector, writing the output to another vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void subtractAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToSubtract);

/** Element-wise subtraction of two vectors, writing the output to a third vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void subtractAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToSubtract);

/** Subtracts every element of a vector from a constant.
	The operation is:
	@code
	for (auto i = 0; i < size; ++i)
		data[i] = constantToSubtractFrom - data[i];
	@endcode
	@see subtractInvAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void subtractInv (DataType* const data, SizeType size, DataType constantToSubtractFrom);

/** Subtracts every element of a vector from a constant, writing the output to another vector.
	The operation is:
	@code
	for (auto i = 0; i < size; ++i)
		dest[i] = constantToSubtractFrom - origData[i];
	@endcode
	@see subtractInv
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void subtractInvAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToSubtractFrom);

/** @} */

/*-----  MULTIPLICATION  -----*/

/** @defgroup vec_multiplication Multiplication
	Vector multiplication
	@ingroup vec_arithmetic
 */

/** @ingroup vec_multiplication
	@{
 */

/** Multiplies every element of a vector by a constant. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void multiply (DataType* const data, SizeType size, DataType constantToMultiply);

/** Element-wise multiplication of two vectors. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void multiply (DataType* const dataAndDest, SizeType size, const DataType* const dataToMultiply);

/** Multiplies every element of a vector by a constant, writing the output to another vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void multiplyAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToMultiply);

/** Element-wise multiplication of two vectors, writing the output to a third vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void multiplyAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToMultiply);

/** Computes the dot product of two vectors.
	The operation is:
	@code
	DataType dotProd { 0 };

	for (auto i = 0; i < size; ++i)
		dotProd += (vecA[i] * vecB[i]);

	return dotProd;
	@endcode
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT [[nodiscard]] DataType dotProduct (const DataType* const vecA, const DataType* const vecB, SizeType size);

/** @} */

/*-----  DIVISION  -----*/

/** @defgroup vec_division Division
	Vector division
	@ingroup vec_arithmetic
 */

/** @ingroup vec_division
	@{
 */

/** Divides every element of a vector by a constant. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void divide (DataType* const data, SizeType size, DataType constantToDivide);

/** Element-wise division of two vectors. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void divide (DataType* const dataAndDest, SizeType size, const DataType* const dataToDivide);

/** Divides every element of a vector by a constant, writing the output to another vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void divideAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToDivide);

/** Element-wise division of two vectors, writing the output to a third vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void divideAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToDivide);

/** Divides a constant by every element of a vector.
	The operation is:
	@code
	for (auto i = 0; i < size; ++i)
		data[i] = constantToDivideFrom / data[i];
	@endcode
	@see divideInvAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void divideInv (DataType* const data, SizeType size, DataType constantToDivideFrom);

/** Divides a constant by every element of a vector, writing the output to another vector.
	The operation is:
	@code
	for (auto i = 0; i < size; ++i)
		dest[i] = constantToDivideFrom / origData[i];
	@endcode
	@see divideInv
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void divideInvAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToDivideFrom);

/** @} */

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark Squaring functions

/** @defgroup vec_squaring Squaring functions
	Squaring and square root functions on vectors.
	@ingroup limes_vecops
 */

/** @ingroup vec_squaring
	@{
 */

/** Squares every element of a vector.
	@see squareAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void square (DataType* const dataAndDest, SizeType size);

/** Squares every element of a vector, writing the output to another vector.
	@see square
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void squareAndCopy (DataType* const dest, const DataType* const data, SizeType size);

/** Replaces every element in the vector with its square root.
	@see squareRootAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void squareRoot (DataType* const dataAndDest, SizeType size);

/** Writes the square root of every element in the original vector to the output vector.
	@see squareRoot
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void squareRootAndCopy (DataType* const dest, const DataType* const data, SizeType size);

/** Replaces every element in the vector with its inverse square root.
	@see invSquareRootAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void invSquareRoot (DataType* const dataAndDest, SizeType size);

/** Writes the inverse square root of every element in the original vector to the output vector.
	@see invSquareRoot
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void invSquareRootAndCopy (DataType* const dest, const DataType* const data, SizeType size);

/** @} */

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark Sorting and ordering functions

/** @defgroup vec_sorting Sorting functions
	Sorting and ordering functions on vectors.
	@ingroup limes_vecops
 */

/** @ingroup vec_sorting
	@{
 */

/** Reverses a vector in-place.
	@see reverseAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void reverse (DataType* const dataAndDest, SizeType size);

/** Writes a reversed version of the original vector to the output vector.
	@see reverse
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void reverseAndCopy (DataType* const dest, const DataType* const data, SizeType size);

/** Sorts a vector in-place.
	@see sortAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void sort (DataType* const dataAndDest, SizeType size);

/** Writes a sorted version of the original vector to the output vector.
	@see sort
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void sortAndCopy (DataType* const dest, const DataType* const data, SizeType size);

/** Sorts a vector in reverse order in-place.
	@see sortReverseAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void sortReverse (DataType* const dataAndDest, SizeType size);

/** Writes a reverse sorted version of the original vector to the output vector.
	@see sortReverse
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void sortReverseAndCopy (DataType* const dest, const DataType* const data, SizeType size);

/** Interleaves multichannel data into a single output vector.
	The size of the output vector must be at least \c numChannels*numSamples .
	The operation is:
	@code
	int idx = 0;

	for (auto i = 0; i < numSamples; ++i)
		for (auto j = 0; j < numChannels; ++j)
			output[idx++] = origData[j][i];
	@endcode
	@see deinterleave
 */
template <Scalar DataType, Integral SizeType1, Integral SizeType2>
LIMES_EXPORT void interleave (DataType* const output, const DataType* const * const origData, SizeType1 numChannels, SizeType2 numSamples);

/** Deinterleaves single channel data into multiple output vectors.
	The operation is:
	@code
	int idx = 0;

	for (auto i = 0; i < numSamples; ++i)
		for (auto j = 0; j < numChannels; ++j)
			output[j][i] = interleavedData[idx++];
	@endcode
	@see interleave
 */
template <Scalar DataType, Integral SizeType1, Integral SizeType2>
LIMES_EXPORT void deinterleave (DataType* const * const output, const DataType* const interleavedData, SizeType1 numChannels, SizeType2 numSamples);

/** @} */

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark Statistical functions

/** @defgroup vec_stats Statistical functions
	Statistical functions on vectors.
	@ingroup limes_vecops
 */

/** @ingroup vec_stats
	@{
 */

/** Replaces each element of the vector with its absolute value.
	@see absAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void abs (DataType* const dataAndDest, SizeType size);

/** Writes the absolute value of each element of the original vector to the output vector.
	@see abs
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void absAndCopy (DataType* const dest, const DataType* const data, SizeType size);

/** Flips the sign of every element of the vector.
	@see negateAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void negate (DataType* const dataAndDest, SizeType size);

/** Writes the negation of every element of the original vector to the output vector.
	@see negate
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void negateAndCopy (DataType* const dest, const DataType* const data, SizeType size);

/** Clips every element of a vector in-place.
	Any samples that are less than \c lowClip are replaced with \c lowClip , and any samples that are greater than \c hiClip are replaced with \c hiClip .
	@see clipAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void clip (DataType* const dataAndDest, SizeType size, DataType lowClip = -1, DataType hiClip = 1);

/** Writes a clipped version of the original vector to the output vector.
	Any samples that are less than \c lowClip are replaced with \c lowClip , and any samples that are greater than \c hiClip are replaced with \c hiClip .
	@see clip
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void clipAndCopy (DataType* const dest, const DataType* const data, SizeType size, DataType lowClip = -1, DataType hiClip = 1);

/** Returns the maximum element in a vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT [[nodiscard]] DataType max (const DataType* const data, SizeType size);

/** Finds the maximum element in a vector and its index in the vector. */
template <Scalar DataType, Integral SizeType, Integral IndexType>
LIMES_EXPORT void max (const DataType* const data, SizeType size, DataType& maxValue, IndexType& maxIndex);

/** Returns the maximum absolute value in a vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT [[nodiscard]] DataType maxAbs (const DataType* const data, SizeType size);

/** Finds the maximum absolute value in a vector and its index in the vector. */
template <Scalar DataType, Integral SizeType, Integral IndexType>
LIMES_EXPORT void maxAbs (const DataType* const data, SizeType size, DataType& maxValue, IndexType& maxIndex);

/** Returns the minimum element in a vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT [[nodiscard]] DataType min (const DataType* const data, SizeType size);

/** Finds the minimum element in a vector and its index in the vector. */
template <Scalar DataType, Integral SizeType, Integral IndexType>
LIMES_EXPORT void min (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex);

/** Returns the minimum absolute value in a vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT [[nodiscard]] DataType minAbs (const DataType* const data, SizeType size);

/** Finds the minimum absolute value in a vector and its index in the vector. */
template <Scalar DataType, Integral SizeType, Integral IndexType>
LIMES_EXPORT void minAbs (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex);

/** Finds both the minimum and maximum values in a vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void minMax (const DataType* const data, SizeType size, DataType& minValue, DataType& maxValue);

/** Finds the minimum and maximum values in a vector, and their indices in the vector. */
template <Scalar DataType, Integral SizeType, Integral IndexType>
LIMES_EXPORT void minMax (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex, DataType& maxValue, IndexType& maxIndex);

/** Finds both the minimum and maximum absolute values in a vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void minMaxAbs (const DataType* const data, SizeType size, DataType& minValue, DataType& maxValue);

/** Finds the minimum and maximum absolute values in a vector, and their indices in the vector. */
template <Scalar DataType, Integral SizeType, Integral IndexType>
LIMES_EXPORT void minMaxAbs (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex, DataType& maxValue, IndexType& maxIndex);

/** Returns the difference between the maximum and minimum values in the vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT [[nodiscard]] DataType range (const DataType* const data, SizeType size);

/** Returns the difference between the maximum and minimum absolute values in the vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT [[nodiscard]] DataType rangeAbs (const DataType* const data, SizeType size);

/** Returns the sum of all the values in the vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT [[nodiscard]] DataType sum (const DataType* const data, SizeType size);

/** Returns the mean of the values in the vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT [[nodiscard]] DataType mean (const DataType* const data, SizeType size);

/** @} */

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark Audio utility functions

/** @defgroup vec_audio Audio utility functions
	Audio utility functions for vectors.
	@ingroup limes_vecops
 */

/** @ingroup vec_audio
	@{
 */

/** Mixes together multiple channels of audio into one output vector. */
template <Scalar DataType, Integral SizeType1, Integral SizeType2>
LIMES_EXPORT void mix (DataType* const output, const DataType* const * const origData, SizeType1 numChannels, SizeType2 numSamples);

/** Calculates the RMS value of the vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT [[nodiscard]] DataType rms (const DataType* const data, SizeType size);

/** Counts the number of zero crossings present in the vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT [[nodiscard]] int countZeroCrossings (const DataType* const data, SizeType size);

/** Generates a ramp of samples, starting from \c startValue and incrementing by a consistent value until \c endValue is reached.
	@see applyRamp, applyRampAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void generateRamp (DataType* const output, SizeType size, DataType startValue, DataType endValue);

/** Applies a gain ramp to the samples in the vector.
	@see generateRamp, applyRampAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void applyRamp (DataType* const dataAndDest, SizeType size, DataType startValue, DataType endValue);

/** Applies a gain ramp to the samples in the vector, writing the output samples to another vector.
	@see applyRamp, generateRamp
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void applyRampAndCopy (DataType* const dest, const DataType* const data, SizeType size, DataType startValue, DataType endValue);

/** @} */

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark Windowing functions

/** @defgroup vec_window Windowing functions
	Windowing functions for vectors.
	@ingroup limes_vecops
 */

/** This namespace contains windowing functions.
	@ingroup vec_window
 */
namespace window
{

/** @ingroup vec_window
	@{
 */

/** Generates a Blackman window, writing the output samples to a vector.
	@see applyBlackman, applyBlackmanAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void generateBlackman (DataType* const output, SizeType size);

/** Applies a Blackman window to the samples in the vector.
	@see generateBlackman, applyBlackmanAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void applyBlackman (DataType* const dataAndDest, SizeType size);

/** Applies a Blackman window to the samples in the vector, writing the output samples to another vector.
	@see applyBlackman, generateBlackman
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void applyBlackmanAndCopy (DataType* const dest, const DataType* const data, SizeType size);

/** Generates a Hamm (Hamming) window, writing the output samples to a vector.
	@see applyHamm, applyHammAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void generateHamm (DataType* const output, SizeType size);

/** Applies a Hamm (Hamming) window to the samples in the vector.
	@see generateHamm, applyHammAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void applyHamm (DataType* const dataAndDest, SizeType size);

/** Applies a Hamm (Hamming) window to the samples in the vector, writing the output samples to another vector.
	@see applyHamm, generateHamm
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void applyHammAndCopy (DataType* const dest, const DataType* const data, SizeType size);

/** Generates a Hanning window, writing the output samples to a vector.
	@see applyHanning, applyHanningAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void generateHanning (DataType* const output, SizeType size);

/** Applies a Hanning window to the samples in the vector.
	@see generateHanning, applyHanningAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void applyHanning (DataType* const dataAndDest, SizeType size);

/** Applies a Hanning window to the samples in the vector, writing the output samples to another vector.
	@see applyHanning, generateHanning
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void applyHanningAndCopy (DataType* const dest, const DataType* const data, SizeType size);

/** @} */

}  // namespace window


/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark Complex conversions

/** @defgroup vec_complex Windowing functions
	Windowing functions for vectors.
	@ingroup limes_vecops
 */

/** @ingroup vec_complex
	@{
 */

/** Converts polar data to cartesian data.
	Note that this function is only intended for use with float or double samples.
	@param real Real signal out
	@param imag Imaginary signal out
	@param mag Magnitude in
	@param phase Phase in
	@param size Data size
	@see polarToCartesianInterleaved
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void polarToCartesian (DataType* const real, DataType* const imag, const DataType* const mag, const DataType* const phase, SizeType size);

/** Converts polar data to interleaved cartesian data.
	Note that this function is only intended for use with float or double samples.
	@param dest Interleaved cartesian signal out. The size of this vector should be \c 2*size .
	@param mag Magnitude in
	@param phase Phase in
	@size Data size
	@see polarToCartesian
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void polarToCartesianInterleaved (DataType* const dest, const DataType* const mag, const DataType* const phase, SizeType size);

/** Converts cartesian data to polar data.
	Note that this function is only intended for use with float or double samples.
	@param mag Magnitude out
	@param phase Phase out
	@param real Real signal in
	@param imag Imaginary signal in
	@param size Data size
	@see catesianInterleavedToPolar
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void cartesianToPolar (DataType* const mag, DataType* const phase, const DataType* const real, const DataType* const imag, SizeType size);

/** Converts interleaved cartesian data to polar data.
	Note that this function is only intended for use with float or double samples.
	@param mag Magnitude out
	@param phase Phase out
	@param src Interleaved cartesian signal in. The size of this vector should be \c 2*size .
	@param size Data size
	@see cartesianToPolar
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void catesianInterleavedToPolar (DataType* const mag, DataType* const phase, const DataType* const src, SizeType size);

/** Converts cartesian data to polar magnitudes.
	Note that this function is only intended for use with float or double samples.
	@param mag Magnitude out
	@param real Real signal in
	@param imag Imaginary signal in
	@param size Data size
	@see cartesianInterleavedToMagnitudes
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void cartesianToMagnitudes (DataType* const mag, const DataType* const real, const DataType* const imag, SizeType size);

/** Converts interleaved cartesian data to polar magnitudes.
	Note that this function is only intended for use with float or double samples.
	@param mag Magnitude out
	@param src Interleaved cartesian signal in. The size of this vector should be \c 2*size .
	@param size Data size
	@see cartesianToMagnitudes
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void cartesianInterleavedToMagnitudes (DataType* const mag, const DataType* const src, SizeType size);

/** @} */

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark Floating point mode functions

/** @defgroup vec_fpm Floating point mode functions
	Floating point mode functions.
	@ingroup limes_vecops
 */

/** @ingroup vec_fpm
	@{
 */

/** Controls whether denormalized numbers are enabled or disabled.
	@see areDenormalsDisabled
 */
LIMES_EXPORT void disableDenormalisedNumberSupport (bool shouldDisable = true) noexcept;

/** Returns true if denormalized numbers are currently enabled.
	@see disableDenormalisedNumberSupport
 */
LIMES_EXPORT [[nodiscard]] bool areDenormalsDisabled() noexcept;

/** Controls whether flush to zero mode is enabled or disabled.
	@see isFlushToZeroEnabled
 */
LIMES_EXPORT void enableFlushToZeroMode (bool shouldEnable = true) noexcept;

/** Returns true if flush to zero mode is enabled.
	@see enableFlushToZeroMode
 */
LIMES_EXPORT [[nodiscard]] bool isFlushToZeroEnabled() noexcept;

/** An RAII class that disables denormalized numbers when it is constructed, and resets the denormalized number state when it is destructed.
 */
class LIMES_EXPORT ScopedNoDenormals final
{
public:

	ScopedNoDenormals() noexcept;
	~ScopedNoDenormals() noexcept;

private:

	const intptr_t fpsr;
};

/** An RAII class that enables flush to zero mode when it is constructed, and resets the flush to zero state when it is destructed.
 */
class LIMES_EXPORT ScopedFlushToZero final
{
public:

	ScopedFlushToZero() noexcept;
	~ScopedFlushToZero() noexcept;

private:

	const intptr_t fpsr;
};

/** @} */

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark Implementation kind checking

/** @defgroup vec_impl Implementation kind checking
	Implementation kind checking functions.
	@ingroup limes_vecops
 */

/** @ingroup vec_impl
	@{
 */

/** Returns true if the implementation being used is Apple vDSP. */
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION consteval bool isUsingVDSP() noexcept
{
#if LIMES_VECOPS_USE_VDSP
	return true;
#else
	return false;
#endif
}

/** Returns true if the implementation being used is Intel IPP. */
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION consteval bool isUsingIPP() noexcept
{
#if LIMES_VECOPS_USE_IPP
	return true;
#else
	return false;
#endif
}

/** Returns true if the implementation being used is MIPP. */
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION consteval bool isUsingMIPP() noexcept
{
#if LIMES_VECOPS_USE_MIPP
	return true;
#else
	return false;
#endif
}

/** Returns true if the fallback implementation is being used. */
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION consteval bool isUsingFallback() noexcept
{
	return ! (isUsingVDSP() || isUsingIPP() || isUsingMIPP());	// cppcheck-suppress knownConditionTrueFalse
}

static_assert (isUsingVDSP() || isUsingIPP() || isUsingMIPP() || isUsingFallback());

/** Returns a string literal with the name of the implementation being used. */
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

/** Returns true if the Pommier SIMD extension functions are being used. */
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION consteval bool isUsingPommierExtensions() noexcept
{
#if LIMES_VECOPS_USE_POMMIER
	return true;
#else
	return false;
#endif
}

/** @} */

}  // namespace vecops

LIMES_END_NAMESPACE

/// @cond

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

/// @endcond
