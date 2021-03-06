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
#include <limes_export.h>
#include <limes_core.h>
#include <type_traits>

/** @dir libs/limes_vecops/vecops
	This directory contains free functions for manipulating vectors.
	@ingroup limes_vecops
 */

/** @file
	This file defines the interface for the limes_vecops free functions.
	@ingroup limes_vecops
 */

// IWYU pragma: begin_exports
#include "./vecops_macros.h"
#include "./fp_status.h"
#include "./check_impl.h"
// IWYU pragma: end_exports

LIMES_BEGIN_NAMESPACE

/** This namespace contains vector operations functions.
	@ingroup limes_vecops
 */
namespace vecops
{

// these should never fail, but just as a sanity check:
static_assert (sizeof (float) == 4, "float is not 32-bits wide");
static_assert (sizeof (double) == 8, "double is not 64-bits wide");

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

/** Fills the vector with the specified value.
	@see clear
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void fill (DataType* const data, SizeType size, DataType constantToFill) noexcept;

/** Fills the vector with zeroes.
	@see fill
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void clear (DataType* const data, SizeType size) noexcept;

/** Copies from one vector to another.
	@see swap
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void copy (DataType* const dest, const DataType* const source, SizeType size) noexcept;

/** Swaps the elements of two vectors.
	@see copy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void swap (DataType* const vecA, DataType* const vecB, SizeType size) noexcept;

/** @} */

template <Scalar DataType1, Scalar DataType2>
static void convert (DataType1* const dest, const DataType2* const source, int size) noexcept
{
	for (auto i = 0; i < size; ++i)
		dest[i] = static_cast<DataType1> (source[i]);
}

template <Scalar DataType1, Scalar DataType2, Integral SizeType>
static void convert (DataType1* const dest, const DataType2* const source, SizeType size) noexcept
{
	convert (dest, source, static_cast<int> (size));
}

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark -
#pragma mark Arithmetic functions

/** @defgroup vec_arithmetic Arithmetic functions
	Basic arithmetic operations on vectors.
	@ingroup limes_vecops
 */

/** @ingroup vec_arithmetic
	@{
 */

/** Replaces each value in the vector with its reciprocal.
	@see recipAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void recip (DataType* const data, SizeType size) noexcept;

/** Writes the reciprocal of each value in the input vector to an output vector.
	@see recip
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void recipAndCopy (DataType* const dest, const DataType* const origData, SizeType size) noexcept;

/** @} */

#pragma mark Addition

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
LIMES_EXPORT void add (DataType* const data, SizeType size, DataType constantToAdd) noexcept;

/** Element-wise addition of two vectors. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void add (DataType* const dataAndDest, SizeType size, const DataType* const dataToAdd) noexcept;

/** Adds a constant to every element of a vector, writing the output to another vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void addAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToAdd) noexcept;

/** Element-wise addition of two vectors, writing the output to a third vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void addAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToAdd) noexcept;

/** @} */

#pragma mark Subtraction

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
LIMES_EXPORT void subtract (DataType* const data, SizeType size, DataType constantToSubtract) noexcept;

/** Element-wise subtraction of two vectors. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void subtract (DataType* const dataAndDest, SizeType size, const DataType* const dataToSubtract) noexcept;

/** Subtracts a constant from every element of a vector, writing the output to another vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void subtractAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToSubtract) noexcept;

/** Element-wise subtraction of two vectors, writing the output to a third vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void subtractAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToSubtract) noexcept;

/** Subtracts every element of a vector from a constant.
	The operation is:
	@code
	for (auto i = 0; i < size; ++i)
		data[i] = constantToSubtractFrom - data[i];
	@endcode
	@see subtractInvAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void subtractInv (DataType* const data, SizeType size, DataType constantToSubtractFrom) noexcept;

/** Subtracts every element of a vector from a constant, writing the output to another vector.
	The operation is:
	@code
	for (auto i = 0; i < size; ++i)
		dest[i] = constantToSubtractFrom - origData[i];
	@endcode
	@see subtractInv
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void subtractInvAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToSubtractFrom) noexcept;

/** @} */

#pragma mark Multiplication

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
LIMES_EXPORT void multiply (DataType* const data, SizeType size, DataType constantToMultiply) noexcept;

/** Element-wise multiplication of two vectors. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void multiply (DataType* const dataAndDest, SizeType size, const DataType* const dataToMultiply) noexcept;

/** Multiplies every element of a vector by a constant, writing the output to another vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void multiplyAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToMultiply) noexcept;

/** Element-wise multiplication of two vectors, writing the output to a third vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void multiplyAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToMultiply) noexcept;

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
LIMES_EXPORT [[nodiscard]] DataType dotProduct (const DataType* const vecA, const DataType* const vecB, SizeType size) noexcept;

/** @} */

#pragma mark Division

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
LIMES_EXPORT void divide (DataType* const data, SizeType size, DataType constantToDivide) noexcept;

/** Element-wise division of two vectors. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void divide (DataType* const dataAndDest, SizeType size, const DataType* const dataToDivide) noexcept;

/** Divides every element of a vector by a constant, writing the output to another vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void divideAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToDivide) noexcept;

/** Element-wise division of two vectors, writing the output to a third vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void divideAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToDivide) noexcept;

/** Divides a constant by every element of a vector.
	The operation is:
	@code
	for (auto i = 0; i < size; ++i)
		data[i] = constantToDivideFrom / data[i];
	@endcode
	@see divideInvAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void divideInv (DataType* const data, SizeType size, DataType constantToDivideFrom) noexcept;

/** Divides a constant by every element of a vector, writing the output to another vector.
	The operation is:
	@code
	for (auto i = 0; i < size; ++i)
		dest[i] = constantToDivideFrom / origData[i];
	@endcode
	@see divideInv
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void divideInvAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToDivideFrom) noexcept;

/** @} */

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark -
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
LIMES_EXPORT void square (DataType* const dataAndDest, SizeType size) noexcept;

/** Squares every element of a vector, writing the output to another vector.
	@see square
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void squareAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept;

/** Replaces every element in the vector with its square root.
	@see squareRootAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void squareRoot (DataType* const dataAndDest, SizeType size) noexcept;

/** Writes the square root of every element in the original vector to the output vector.
	@see squareRoot
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void squareRootAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept;

/** Replaces every element in the vector with its inverse square root.
	@see invSquareRootAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void invSquareRoot (DataType* const dataAndDest, SizeType size) noexcept;

/** Writes the inverse square root of every element in the original vector to the output vector.
	@see invSquareRoot
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void invSquareRootAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept;

/** Replaces every element in the vector with its cube root.
	@see cubeRootAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void cubeRoot (DataType* const dataAndDest, SizeType size) noexcept;

/** Writes the cube root of every element in the original vector to the output vector.
	@see cubeRoot
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void cubeRootAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept;

/** @} */

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark -
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
LIMES_EXPORT void reverse (DataType* const dataAndDest, SizeType size) noexcept;

/** Writes a reversed version of the original vector to the output vector.
	@see reverse
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void reverseAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept;

/** Sorts a vector in-place.
	@see sortAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void sort (DataType* const dataAndDest, SizeType size) noexcept;

/** Writes a sorted version of the original vector to the output vector.
	@see sort
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void sortAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept;

/** Sorts a vector in reverse order in-place.
	@see sortReverseAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void sortReverse (DataType* const dataAndDest, SizeType size) noexcept;

/** Writes a reverse sorted version of the original vector to the output vector.
	@see sortReverse
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void sortReverseAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept;

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
LIMES_EXPORT void interleave (DataType* const output, const DataType* const * const origData, SizeType1 numChannels, SizeType2 numSamples) noexcept;

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
LIMES_EXPORT void deinterleave (DataType* const * const output, const DataType* const interleavedData, SizeType1 numChannels, SizeType2 numSamples) noexcept;

/** @} */

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark -
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
LIMES_EXPORT void abs (DataType* const dataAndDest, SizeType size) noexcept;

/** Writes the absolute value of each element of the original vector to the output vector.
	@see abs
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void absAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept;

/** Flips the sign of every element of the vector.
	@see negateAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void negate (DataType* const dataAndDest, SizeType size) noexcept;

/** Writes the negation of every element of the original vector to the output vector.
	@see negate
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void negateAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept;

/** Clips every element of a vector in-place.
	Any samples that are less than \c lowClip are replaced with \c lowClip , and any samples that are greater than \c hiClip are replaced with \c hiClip .
	@see clipAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void clip (DataType* const dataAndDest, SizeType size, DataType lowClip = -1, DataType hiClip = 1) noexcept;

/** Writes a clipped version of the original vector to the output vector.
	Any samples that are less than \c lowClip are replaced with \c lowClip , and any samples that are greater than \c hiClip are replaced with \c hiClip .
	@see clip
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void clipAndCopy (DataType* const dest, const DataType* const data, SizeType size, DataType lowClip = -1, DataType hiClip = 1) noexcept;

/** Returns the sum of all the values in the vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT [[nodiscard]] DataType sum (const DataType* const data, SizeType size) noexcept;

/** Returns the mean of the values in the vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT [[nodiscard]] DataType mean (const DataType* const data, SizeType size) noexcept;

/** Returns the standard deviation of the values in the vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT [[nodiscard]] DataType standard_deviation (const DataType* const data, SizeType size) noexcept;

/** @} */

/** @defgroup vec_extrema Extrema calculation
	Extrema calculation functions for vectors.
	@ingroup vec_stats
 */

/** @ingroup vec_extrema
	@{
 */

/** Returns the maximum element in a vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT [[nodiscard]] DataType max (const DataType* const data, SizeType size) noexcept;

/** Finds the maximum element in a vector and its index in the vector. */
template <Scalar DataType, Integral SizeType, Integral IndexType>
LIMES_EXPORT void max (const DataType* const data, SizeType size, DataType& maxValue, IndexType& maxIndex) noexcept;

/** Returns the maximum absolute value in a vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT [[nodiscard]] DataType maxAbs (const DataType* const data, SizeType size) noexcept;

/** Finds the maximum absolute value in a vector and its index in the vector. */
template <Scalar DataType, Integral SizeType, Integral IndexType>
LIMES_EXPORT void maxAbs (const DataType* const data, SizeType size, DataType& maxValue, IndexType& maxIndex) noexcept;

/** Returns the minimum element in a vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT [[nodiscard]] DataType min (const DataType* const data, SizeType size) noexcept;

/** Finds the minimum element in a vector and its index in the vector. */
template <Scalar DataType, Integral SizeType, Integral IndexType>
LIMES_EXPORT void min (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex) noexcept;

/** Returns the minimum absolute value in a vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT [[nodiscard]] DataType minAbs (const DataType* const data, SizeType size) noexcept;

/** Finds the minimum absolute value in a vector and its index in the vector. */
template <Scalar DataType, Integral SizeType, Integral IndexType>
LIMES_EXPORT void minAbs (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex) noexcept;

/** Finds both the minimum and maximum values in a vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void minMax (const DataType* const data, SizeType size, DataType& minValue, DataType& maxValue) noexcept;

/** Finds the minimum and maximum values in a vector, and their indices in the vector. */
template <Scalar DataType, Integral SizeType, Integral IndexType>
LIMES_EXPORT void minMax (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex, DataType& maxValue, IndexType& maxIndex) noexcept;

/** Finds both the minimum and maximum absolute values in a vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void minMaxAbs (const DataType* const data, SizeType size, DataType& minValue, DataType& maxValue) noexcept;

/** Finds the minimum and maximum absolute values in a vector, and their indices in the vector. */
template <Scalar DataType, Integral SizeType, Integral IndexType>
LIMES_EXPORT void minMaxAbs (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex, DataType& maxValue, IndexType& maxIndex) noexcept
{
	minAbs (data, size, minValue, minIndex);
	maxAbs (data, size, maxValue, maxIndex);
}

/** A simple struct to hold various info about the extrema in a vector.
	This struct only exists to serve as the return type for \c getVectorExtremaInfo() and \c  .
 */
template <Scalar DataType>
struct LIMES_EXPORT VectorExtremaInfo final
{
	/** The minimum value in the vector. */
	DataType min;

	/** The index of \c min in the vector. */
	int minIdx;

	/** The maximum value in the vector. */
	DataType max;

	/** The index of \c max in the vector. */
	int maxIdx;
};

/** Calculates the values and positions of the extrema in the vector and returns them in a single VectorExtremaInfo object.
	@see getAbsVectorExtremaInfo()
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT [[nodiscard]] VectorExtremaInfo<DataType> getVectorExtremaInfo (const DataType* const data, SizeType size) noexcept
{
	VectorExtremaInfo<DataType> info;

	minMax (data, size, info.min, info.minIdx, info.max, info.maxIdx);

	return info;
}

/** Calculates the values and positions of the absolute value extrema in the vector and returns them in a single VectorExtremaInfo object.
	@see getVectorExtremaInfo()
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT [[nodiscard]] VectorExtremaInfo<DataType> getAbsVectorExtremaInfo (const DataType* const data, SizeType size) noexcept
{
	VectorExtremaInfo<DataType> info;

	minMaxAbs (data, size, info.min, info.minIdx, info.max, info.maxIdx);

	return info;
}

/** Returns the difference between the maximum and minimum values in the vector.
	@see rangeAbs
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT [[nodiscard]] DataType range (const DataType* const data, SizeType size) noexcept
{
	DataType minVal, maxVal;

	minMax (data, size, minVal, maxVal);

	return maxVal - minVal;
}

/** Returns the difference between the maximum and minimum absolute values in the vector.
	@see range
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT [[nodiscard]] DataType rangeAbs (const DataType* const data, SizeType size) noexcept
{
	DataType minVal, maxVal;

	minMaxAbs (data, size, minVal, maxVal);

	return maxVal - minVal;
}

/** @} */

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark -
#pragma mark Trigonometric functions

/** @defgroup vec_trig Trigonometric functions
	Trigonometric functions for vectors.
	@ingroup limes_vecops
 */

/** Simultaneously computes the sin and cosine of every value in the vector.
	@ingroup vec_trig
	@see sineAndCopy, cosAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void sinCos (const DataType* const data, SizeType size, DataType* const sinesOut, DataType* const cosinesOut) noexcept;

#pragma mark Sine

/* --- sin --- */

/** @defgroup vec_sin Sine functions
	Sine and arcsine functions for vectors.
	@ingroup vec_trig
 */

/** @ingroup vec_sin
	@{
 */

/** Replaces every value in the vector with its sine.
	@see sineAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void sine (DataType* const data, SizeType size) noexcept;

/** Writes the sine of each element of the input vector to the output vector.
	@see sine
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void sineAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept;

/** Replaces every value in the vector with its arcsine.
	@see arcsineAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void arcsine (DataType* const data, SizeType size) noexcept;

/** Writes the arcsine of each element of the input vector to the output vector.
	@see arcsine
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void arcsineAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept;

/** @} */

#pragma mark Cosine

/* --- cos --- */

/** @defgroup vec_cos Cosine functions
	Cosine and arccosine functions for vectors.
	@ingroup vec_trig
 */

/** @ingroup vec_cos
	@{
 */

/** Replaces every value in the vector with its cosine.
	@see cosAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void cos (DataType* const data, SizeType size) noexcept;

/** Writes the cosine of each element of the input vector to the output vector.
	@see cos
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void cosAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept;

/** Replaces every value in the vector with its arccosine.
	@see arccosAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void arccos (DataType* const data, SizeType size) noexcept;

/** Writes the arccosine of each element of the input vector to the output vector.
	@see arccos
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void arccosAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept;

/** @} */

#pragma mark Tangent

/* --- tan --- */

/** @defgroup vec_tan Tangent functions
	Tangent and arctangent functions for vectors.
	@ingroup vec_trig
 */

/** @ingroup vec_tan
	@{
 */

/** Replaces every value in the vector with its tangent.
	@see tanAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void tan (DataType* const data, SizeType size) noexcept;

/** Writes the tangent of each element of the input vector to the output vector.
	@see tan
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void tanAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept;

/** Replaces every value in the vector with its arctangent.
	@see arctanAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void arctan (DataType* const data, SizeType size) noexcept;

/** Writes the arctangent of each element of the input vector to the output vector.
	@see arctan
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void arctanAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept;

/** @} */

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark -
#pragma mark Exponential and logarithmic functions

#pragma mark Logarithms

/** @defgroup vec_log Logarithmic functions
	Logarithmic functions for vectors.
	@ingroup limes_vecops
 */

/** This namespace contains logarithmic functions for vectors.
	@ingroup vec_log
 */
namespace log
{

/** @ingroup vec_log
	@{
 */

/** Replaces each value in the vector with its natural logarithm.
	@see natAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void nat (DataType* const data, SizeType size) noexcept;

/** Writes the natural logarithm of each value in the input vector to the output vector.
	@see nat
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void natAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept;

/** Replaces each value in the vector with its base 2 logarithm.
	@see base2AndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void base2 (DataType* const data, SizeType size) noexcept;

/** Writes the base 2 logarithm of each value in the input vector to the output vector.
	@see base2
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void base2AndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept;

/** Replaces each value in the vector with its base 10 logarithm.
	@see base10AndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void base10 (DataType* const data, SizeType size) noexcept;

/** Writes the base 10 logarithm of each value in the input vector to the output vector.
	@see base10
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void base10AndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept;

/** @} */

}  // namespace log

#pragma mark Exponents

/** @defgroup vec_exp Exponential functions
	Exponential functions for vectors.
	@ingroup limes_vecops
 */

/** This namespace contains exponential functions for vectors.
	@ingroup vec_exp
 */
namespace exp
{

/** @ingroup vec_exp
	@{
 */

/** Replaces each value in the vector with \c e??value .
	@see eAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void e (DataType* const data, SizeType size) noexcept;

/** For each value in the input vector, writes \c e??value to the output vector.
	@see s
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void eAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept;

/** Replaces each value in the vector with \c 2??value .
	@see twoAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void two (DataType* const data, SizeType size) noexcept;

/** For each value in the input vector, writes \c 2??value to the output vector.
	@see two
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void twoAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept;

/** Raises each value in the input vector to the power of the corresponding element of another vector.
	@see powAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void pow (DataType* const dataAndDest, const DataType* const exponents, SizeType size) noexcept;

/** Raises each value in the input vector to the power of the corresponding element of another vector, then writes the result to an output vector.
	@see pow
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void powAndCopy (DataType* const dest, const DataType* const data, const DataType* const exponents, SizeType size) noexcept;

/** @} */
}  // namespace exp

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark -
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
LIMES_EXPORT void mix (DataType* const output, const DataType* const * const origData, SizeType1 numChannels, SizeType2 numSamples) noexcept;

/** Calculates the RMS value of the vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT [[nodiscard]] DataType rms (const DataType* const data, SizeType size) noexcept;

/** Counts the number of zero crossings present in the vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT [[nodiscard]] int countZeroCrossings (const DataType* const data, SizeType size) noexcept;

/** Generates a ramp of samples, starting from \c startValue and incrementing by a consistent value until \c endValue is reached.
	@see applyRamp, applyRampAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void generateRamp (DataType* const output, SizeType size, DataType startValue, DataType endValue) noexcept;

/** Applies a gain ramp to the samples in the vector.
	@see generateRamp, applyRampAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void applyRamp (DataType* const dataAndDest, SizeType size, DataType startValue, DataType endValue) noexcept;

/** Applies a gain ramp to the samples in the vector, writing the output samples to another vector.
	@see applyRamp, generateRamp
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void applyRampAndCopy (DataType* const dest, const DataType* const data, SizeType size, DataType startValue, DataType endValue) noexcept;

/** @} */

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark -
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

#pragma mark Blackman

/* --- Blackman --- */

/** @defgroup vec_blackman Blackman windowing
	Blackman windowing functions.
	@ingroup vec_window
 */

/** @ingroup vec_blackman
	@{
 */

/** Generates a Blackman window, writing the output samples to a vector.
	@see applyBlackman, applyBlackmanAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void generateBlackman (DataType* const output, SizeType size) noexcept;

/** Applies a Blackman window to the samples in the vector.
	@see generateBlackman, applyBlackmanAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void applyBlackman (DataType* const dataAndDest, SizeType size) noexcept;

/** Applies a Blackman window to the samples in the vector, writing the output samples to another vector.
	@see applyBlackman, generateBlackman
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void applyBlackmanAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept;

/** @} */

#pragma mark Hamm

/* --- Hamm --- */

/** @defgroup vec_hamm Hamm windowing
	Hamm (Hamming) windowing functions.
	@ingroup vec_window
 */

/** @ingroup vec_hamm
	@{
 */

/** Generates a Hamm (Hamming) window, writing the output samples to a vector.
	@see applyHamm, applyHammAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void generateHamm (DataType* const output, SizeType size) noexcept;

/** Applies a Hamm (Hamming) window to the samples in the vector.
	@see generateHamm, applyHammAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void applyHamm (DataType* const dataAndDest, SizeType size) noexcept;

/** Applies a Hamm (Hamming) window to the samples in the vector, writing the output samples to another vector.
	@see applyHamm, generateHamm
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void applyHammAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept;

/** @} */

#pragma mark Hanning

/* --- Hanning --- */

/** @defgroup vec_hanning Hanning windowing
	Hanning windowing functions.
	@ingroup vec_window
 */

/** @ingroup vec_hanning
	@{
 */

/** Generates a Hanning window, writing the output samples to a vector.
	@see applyHanning, applyHanningAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void generateHanning (DataType* const output, SizeType size) noexcept;

/** Applies a Hanning window to the samples in the vector.
	@see generateHanning, applyHanningAndCopy
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void applyHanning (DataType* const dataAndDest, SizeType size) noexcept;

/** Applies a Hanning window to the samples in the vector, writing the output samples to another vector.
	@see applyHanning, generateHanning
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void applyHanningAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept;

/** @} */

#pragma mark General

/* --- General --- */

/** @ingroup vec_window
	@{
 */

/** Represents a type of windowing function. */
enum class LIMES_EXPORT Type
{
	Blackman,
	Hamm,
	Hanning
};

/** Generates a %window function of the desired \c type , writing the output samples to a vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void generate (DataType* const output, SizeType size, Type type) noexcept
{
	switch (type)
	{
		case (Type::Blackman) : return generateBlackman (output, size);
		case (Type::Hamm) : return generateHamm (output, size);
		case (Type::Hanning) : return generateHanning (output, size);
		default : LIMES_UNREACHABLE;
	}
}

/** Applies a %window function of the desired \c type to the samples in the vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void apply (DataType* const output, SizeType size, Type type) noexcept
{
	switch (type)
	{
		case (Type::Blackman) : return applyBlackman (output, size);
		case (Type::Hamm) : return applyHamm (output, size);
		case (Type::Hanning) : return applyHanning (output, size);
		default : LIMES_UNREACHABLE;
	}
}

/** Applies a %window function of the desired \c type to the samples in the vector, writing the output samples to another vector. */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void applyAndCopy (DataType* const dest, const DataType* const data, SizeType size, Type type) noexcept
{
	switch (type)
	{
		case (Type::Blackman) : return applyBlackmanAndCopy (dest, data, size);
		case (Type::Hamm) : return applyHammAndCopy (dest, data, size);
		case (Type::Hanning) : return applyHanningAndCopy (dest, data, size);
		default : LIMES_UNREACHABLE;
	}
}

/** @} */

}  // namespace window


/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark -
#pragma mark Complex conversions

/** @defgroup vec_complex Complex conversions
	Complex number conversion functions for vectors.
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
LIMES_EXPORT void polarToCartesian (DataType* const real, DataType* const imag, const DataType* const mag, const DataType* const phase, SizeType size) noexcept;

/** Converts polar data to interleaved cartesian data.
	Note that this function is only intended for use with float or double samples.
	@param dest Interleaved cartesian signal out. The size of this vector should be \c 2*size .
	@param mag Magnitude in
	@param phase Phase in
	@param size Data size
	@see polarToCartesian
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void polarToCartesianInterleaved (DataType* const dest, const DataType* const mag, const DataType* const phase, SizeType size) noexcept;

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
LIMES_EXPORT void cartesianToPolar (DataType* const mag, DataType* const phase, const DataType* const real, const DataType* const imag, SizeType size) noexcept;

/** Converts interleaved cartesian data to polar data.
	Note that this function is only intended for use with float or double samples.
	@param mag Magnitude out
	@param phase Phase out
	@param src Interleaved cartesian signal in. The size of this vector should be \c 2*size .
	@param size Data size
	@see cartesianToPolar
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void catesianInterleavedToPolar (DataType* const mag, DataType* const phase, const DataType* const src, SizeType size) noexcept;

/** Converts cartesian data to polar magnitudes.
	Note that this function is only intended for use with float or double samples.
	@param mag Magnitude out
	@param real Real signal in
	@param imag Imaginary signal in
	@param size Data size
	@see cartesianInterleavedToMagnitudes
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void cartesianToMagnitudes (DataType* const mag, const DataType* const real, const DataType* const imag, SizeType size) noexcept;

/** Converts interleaved cartesian data to polar magnitudes.
	Note that this function is only intended for use with float or double samples.
	@param mag Magnitude out
	@param src Interleaved cartesian signal in. The size of this vector should be \c 2*size .
	@param size Data size
	@see cartesianToMagnitudes
 */
template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void cartesianInterleavedToMagnitudes (DataType* const mag, const DataType* const src, SizeType size) noexcept;

/** @} */

}  // namespace vecops

LIMES_END_NAMESPACE


/// @cond

// IWYU pragma: begin_exports

#include "./impl/fallback_impl.h"

#if LIMES_VECOPS_USE_VDSP
#	include "./impl/vdsp.h"
#elif LIMES_VECOPS_USE_IPP
#	include "./impl/ipp.h"
#elif LIMES_VECOPS_USE_MIPP
#	include "./impl/mipp.h"
#elif LIMES_VECOPS_USE_NE10
#	include "./impl/ne10.h"
#else
#	include "./impl/fallback.h"
#endif

// IWYU pragma: end_exports

/// @endcond
