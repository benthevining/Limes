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

#ifndef LIMES_VECOPS_USE_VDSP
#	ifdef __APPLE__
#		define LIMES_VECOPS_USE_VDSP 1
#	else
#		define LIMES_VECOPS_USE_VDSP 0
#	endif
#endif

#ifndef LIMES_VECOPS_USE_IPP
#	if __has_include(<ipps.h>)
#		define LIMES_VECOPS_USE_IPP 1
#	else
#		define LIMES_VECOPS_USE_IPP 0
#	endif
#endif

#include "util/Constants.h"

#include <type_traits>
#include <cstring>	// for memcpy
#include <cmath>

namespace lemons::vecops
{

template <typename T>
concept Integral = Scalar<T> && std::is_integral_v<T>;


template <Scalar DataType, Integral SizeType>
void fill (DataType* const data, SizeType size, DataType constantToFill);

template <Scalar DataType, Integral SizeType>
void clear (DataType* const data, SizeType size);

template <Scalar DataType, Integral SizeType>
void copy (DataType* const dest, const DataType* const source, SizeType size)
{
	std::memcpy (dest, source, static_cast<size_t> (size) * sizeof (DataType));
}

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
void square (DataType* const dest, const DataType* const data, SizeType size);

template <Scalar DataType, Integral SizeType>
void squareRoot (DataType* const dataAndDest, SizeType size);

template <Scalar DataType, Integral SizeType>
void squareRoot (DataType* const dest, const DataType* const data, SizeType size);


/*---------------------------------------------------------------------------------------------------------------------------*/


template <Scalar DataType, Integral SizeType>
void reverse (DataType* const dataAndDest, SizeType size);

template <Scalar DataType, Integral SizeType>
void reverse (DataType* const dest, const DataType* const data, SizeType size);

template <Scalar DataType, Integral SizeType>
void sort (DataType* const dataAndDest, SizeType size);

template <Scalar DataType, Integral SizeType>
void sort (DataType* const dest, const DataType* const data, SizeType size);

template <Scalar DataType, Integral SizeType>
void sortReverse (DataType* const dataAndDest, SizeType size);

template <Scalar DataType, Integral SizeType>
void sortReverse (DataType* const dest, const DataType* const data, SizeType size);


/*---------------------------------------------------------------------------------------------------------------------------*/


template <Scalar DataType, Integral SizeType>
void abs (DataType* const dataAndDest, SizeType size);

template <Scalar DataType, Integral SizeType>
void abs (DataType* const dest, const DataType* const data, SizeType size);


template <Scalar DataType, Integral SizeType>
void negate (DataType* const dataAndDest, SizeType size);

template <Scalar DataType, Integral SizeType>
void negate (DataType* const dest, const DataType* const data, SizeType size);


template <Scalar DataType, Integral SizeType>
void clip (DataType* const dataAndDest, SizeType size, DataType lowClip = -1, DataType hiClip = 1);

template <Scalar DataType, Integral SizeType>
void clip (DataType* const dest, const DataType* const data, SizeType size, DataType lowClip = -1, DataType hiClip = 1);


template <Scalar DataType, Integral SizeType>
DataType max (const DataType* const data, SizeType size);

template <Scalar DataType, Integral SizeType, Integral IndexType>
void max (const DataType* const data, SizeType size, DataType& maxValue, IndexType& maxIndex);

template <Scalar DataType, Integral SizeType>
DataType maxAbs (const DataType* const data, SizeType size);

template <Scalar DataType, Integral SizeType, Integral IndexType>
void maxAbs (const DataType* const data, SizeType size, DataType& maxValue, IndexType& maxIndex);


template <Scalar DataType, Integral SizeType>
DataType min (const DataType* const data, SizeType size);

template <Scalar DataType, Integral SizeType, Integral IndexType>
void min (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex);

template <Scalar DataType, Integral SizeType>
DataType minAbs (const DataType* const data, SizeType size);

template <Scalar DataType, Integral SizeType, Integral IndexType>
void minAbs (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex);


template <Scalar DataType, Integral SizeType>
DataType sum (const DataType* const data, SizeType size);

template <Scalar DataType, Integral SizeType>
DataType mean (const DataType* const data, SizeType size);


/*---------------------------------------------------------------------------------------------------------------------------*/


template <Scalar DataType, Integral SizeType>
void generateRamp (DataType* const output, SizeType size, DataType startValue, DataType endValue);

namespace window
{

template <Scalar DataType, Integral SizeType>
void generateBlackman (DataType* const output, SizeType size);

template <Scalar DataType, Integral SizeType>
void generateHamm (DataType* const output, SizeType size);

template <Scalar DataType, Integral SizeType>
void generateHanning (DataType* const output, SizeType size);

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

[[nodiscard]] constexpr bool isUsingFallback() noexcept
{
	return ! (isUsingVDSP() || isUsingIPP());  // cppcheck-suppress knownConditionTrueFalse
}

static_assert (isUsingVDSP() || isUsingIPP() || isUsingFallback());
static_assert (! (isUsingVDSP() && isUsingIPP() && isUsingFallback()));

}  // namespace lemons::vecops

#include "impl/fallback_impl.h"

#if LIMES_VECOPS_USE_VDSP
#	include "impl/vdsp.h"
#elif LIMES_VECOPS_USE_IPP
#	include "impl/ipp.h"
#else
#	include "impl/fallback.h"
#endif
