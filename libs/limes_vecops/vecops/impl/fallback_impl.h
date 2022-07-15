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

// IWYU pragma: private
// IWYU pragma: friend limes_vecops/vecops/impl/*

#pragma once

#include <algorithm>		 // for max, max_element, min, min_element, minmax_element, reverse, sort, distance
#include <limes_export.h>	 // for LIMES_NO_EXPORT
#include <limes_platform.h>	 // for LIMES_FORCE_INLINE
#include <cmath>			 // for sqrt, cos, abs
#include <cstring>			 // for memset, memcpy, size_t
#include <limits>			 // for numeric_limits
#include <numeric>			 // for accumulate
#include "./phasor.h"		 // for phasor
#include <limes_namespace.h>
#include <cstdint>
#include <limes_core.h>

#if (LIMES_ARM_NEON || LIMES_SSE)
#	include "./pommier/pommier_wrapper.h"	// IWYU pragma: export
#endif

/** @file
	This file contains the fallback implementations of the vecops functions.
	This file contains a raw C++ implementation of every function defined in limes_vecops.h.
	This file is included by every other implementation header file, and any functions not provided by the SIMD libraries will fall back to the functions in this file.
	@ingroup limes_vecops
	@see limes_vecops.h
 */

LIMES_BEGIN_NAMESPACE

/** This namespace contains raw C++ implementations of all the vecops functions.
	Each function in this namespace has the exact same name and signature as the function found in the vecops namespace.
	Any implementation whose SIMD library doesn't provide a function will fall back to using the function from this namespace.
	@ingroup limes_vecops
 */
namespace vecops::fb
{

/// @cond internals

template <typename T>
concept Scalar = std::is_scalar_v<T>;

template <typename T>
concept Integral = std::is_integral_v<T>;

#pragma mark Basic functions

/** @ingroup limes_vecops
	@{
 */

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void fill (DataType* const data, SizeType size, DataType constantToFill) noexcept
{
	std::memset (data, static_cast<int> (constantToFill), static_cast<size_t> (size) * sizeof (DataType));
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void clear (DataType* const data, SizeType size) noexcept
{
	fill (data, size, DataType (0));
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void copy (DataType* const dest, const DataType* const source, SizeType size) noexcept
{
	std::memcpy (dest, source, static_cast<size_t> (size) * sizeof (DataType));
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void swap (DataType* const vecA, DataType* const vecB, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
	{
		const auto elem_a = vecA[i];

		vecA[i] = vecB[i];
		vecB[i] = elem_a;
	}
}

/** @} */

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark -
#pragma mark Arithmetic functions

/** @ingroup vec_arithmetic
	@{
 */

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void recip (DataType* const data, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
	{
		const auto thisData = data[i];

		if (thisData != DataType (0))
			data[i] = DataType (1) / thisData;
	}
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void recipAndCopy (DataType* const dest, const DataType* const origData, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
	{
		const auto thisData = origData[i];

		if (thisData == DataType (0))
			dest[i] = DataType (0);
		else
			dest[i] = DataType (1) / thisData;
	}
}

/** @} */

#pragma mark Addition

/*-----  ADDITION  -----*/

/** @ingroup vec_addition
	@{
 */

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void add (DataType* const data, SizeType size, DataType constantToAdd) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		data[i] += constantToAdd;
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void add (DataType* const dataAndDest, SizeType size, const DataType* const dataToAdd) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] += dataToAdd[i];
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void addAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToAdd) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = origData[i] + constantToAdd;
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void addAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToAdd) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = origData[i] + dataToAdd[i];
}

/** @} */

#pragma mark Subtraction

/*-----  SUBTRACTION  -----*/

/** @ingroup vec_subtraction
	@{
 */

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void subtract (DataType* const data, SizeType size, DataType constantToSubtract) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		data[i] -= constantToSubtract;
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void subtract (DataType* const dataAndDest, SizeType size, const DataType* const dataToSubtract) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] -= dataToSubtract[i];
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void subtractAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToSubtract) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = origData[i] - constantToSubtract;
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void subtractAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToSubtract) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = origData[i] - dataToSubtract[i];
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void subtractInv (DataType* const data, SizeType size, DataType constantToSubtractFrom) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		data[i] = constantToSubtractFrom - data[i];
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void subtractInvAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToSubtractFrom) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = constantToSubtractFrom - origData[i];
}

/** @} */

#pragma mark Multiplication

/*-----  MULTIPLICATION  -----*/

/** @ingroup vec_multiplication
	@{
 */

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void multiply (DataType* const data, SizeType size, DataType constantToMultiply) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		data[i] *= constantToMultiply;
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void multiply (DataType* const dataAndDest, SizeType size, const DataType* const dataToMultiply) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] *= dataToMultiply[i];
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void multiplyAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToMultiply) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = origData[i] * constantToMultiply;
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void multiplyAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToMultiply) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = origData[i] * dataToMultiply[i];
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT [[nodiscard]] LIMES_FORCE_INLINE DataType dotProduct (const DataType* const vecA, const DataType* const vecB, SizeType size) noexcept
{
	DataType dotProd { 0 };

	for (auto i = SizeType (0); i < size; ++i)
		dotProd += (vecA[i] * vecB[i]);

	return dotProd;
}

/** @} */

#pragma mark Division

/*-----  DIVISION  -----*/

/** @ingroup vec_division
	@{
 */

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void divide (DataType* const data, SizeType size, DataType constantToDivide) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		data[i] /= constantToDivide;
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void divide (DataType* const dataAndDest, SizeType size, const DataType* const dataToDivide) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] /= dataToDivide[i];
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void divideAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToDivide) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = origData[i] / constantToDivide;
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void divideAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToDivide) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = origData[i] / dataToDivide[i];
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void divideInv (DataType* const data, SizeType size, DataType constantToDivideFrom) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
	{
		const auto thisData = data[i];

		if (thisData != DataType (0))
			data[i] = constantToDivideFrom / thisData;
	}
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void divideInvAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToDivideFrom) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
	{
		const auto thisData = origData[i];

		if (thisData == DataType (0))
			dest[i] = DataType (0);
		else
			dest[i] = constantToDivideFrom / thisData;
	}
}

/** @} */

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark -
#pragma mark Squaring functions

/** @ingroup vec_squaring
	@{
 */

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void square (DataType* const dataAndDest, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] *= dataAndDest[i];
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void squareAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = (data[i] * data[i]);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void squareRoot (DataType* const dataAndDest, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] = static_cast<DataType> (std::sqrt (dataAndDest[i]));
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void squareRootAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = static_cast<DataType> (std::sqrt (data[i]));
}


// fast inverse square root from Quake 3
template <Scalar DataType>
LIMES_NO_EXPORT [[nodiscard]] LIMES_FORCE_INLINE DataType quake3_fast_inv_sqrt (DataType number) noexcept;

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void invSquareRoot (DataType* const dataAndDest, SizeType size) noexcept
{
	if constexpr (std::is_same_v<DataType, float> || std::is_same_v<DataType, double>)
	{
		for (auto i = SizeType (0); i < size; ++i)
			dataAndDest[i] = quake3_fast_inv_sqrt (dataAndDest[i]);
	}
	else
	{
		for (auto i = SizeType (0); i < size; ++i)
		{
			const auto denom = static_cast<DataType> (std::sqrt (dataAndDest[i]));

			if (denom == DataType (0))
				dataAndDest[i] = DataType (0);
			else
				dataAndDest[i] = DataType (1) / denom;
		}
	}
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void invSquareRootAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	if constexpr (std::is_same_v<DataType, float> || std::is_same_v<DataType, double>)
	{
		for (auto i = SizeType (0); i < size; ++i)
			dest[i] = quake3_fast_inv_sqrt (data[i]);
	}
	else
	{
		squareRootAndCopy (dest, data, size);
		divideInv (dest, size, DataType (1));
	}
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void cubeRoot (DataType* const dataAndDest, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] = std::cbrt (dataAndDest[i]);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void cubeRootAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = std::cbrt (data[i]);
}

/** @} */

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark -
#pragma mark Sorting and ordering functions

/** @ingroup vec_sorting
	@{
 */

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void reverse (DataType* const dataAndDest, SizeType size) noexcept
{
	std::reverse (dataAndDest, dataAndDest + size);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void reverseAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	copy (dest, data, size);
	reverse (dest, size);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void sort (DataType* const dataAndDest, SizeType size) noexcept
{
	std::sort (dataAndDest, dataAndDest + size);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void sortAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	copy (dest, data, size);
	sort (dest, size);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void sortReverse (DataType* const dataAndDest, SizeType size) noexcept
{
	std::sort (dataAndDest, dataAndDest + size);
	std::reverse (dataAndDest, dataAndDest + size);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void sortReverseAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	copy (dest, data, size);
	sortReverse (dest, size);
}

template <Scalar DataType, Integral SizeType1, Integral SizeType2>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void interleave (DataType* const output, const DataType* const * const origData, SizeType1 numChannels, SizeType2 numSamples) noexcept
{
	int idx = 0;

	switch (numChannels)
	{
		case 2 :
		{
			// common case, may be vectorized by compiler if hardcoded
			for (auto i = 0; i < static_cast<int> (numSamples); ++i)
				for (auto j = 0; j < 2; ++j)
					output[idx++] = origData[j][i];

			return;
		}
		case 1 :
		{
			copy (output, origData[0], numSamples);
			return;
		}
		default :
		{
			for (auto i = 0; i < static_cast<int> (numSamples); ++i)
				for (auto j = 0; j < static_cast<int> (numChannels); ++j)
					output[idx++] = origData[j][i];
		}
	}
}

template <Scalar DataType, Integral SizeType1, Integral SizeType2>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void deinterleave (DataType* const * const output, const DataType* const interleavedData, SizeType1 numChannels, SizeType2 numSamples) noexcept
{
	int idx = 0;

	switch (numChannels)
	{
		case 2 :
		{
			// common case, may be vectorized by compiler if hardcoded
			for (auto i = 0; i < static_cast<int> (numSamples); ++i)
				for (auto j = 0; j < 2; ++j)
					output[j][i] = interleavedData[idx++];

			return;
		}
		case 1 :
		{
			copy (output[0], interleavedData, numSamples);
			return;
		}
		default :
		{
			for (auto i = 0; i < static_cast<int> (numSamples); ++i)
				for (auto j = 0; j < static_cast<int> (numChannels); ++j)
					output[j][i] = interleavedData[idx++];
		}
	}
}

/** @} */

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark -
#pragma mark Statistical functions

/** @ingroup vec_stats
	@{
 */

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void abs (DataType* const dataAndDest, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] = std::abs (dataAndDest[i]);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void absAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = std::abs (data[i]);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void negate (DataType* const dataAndDest, SizeType size) noexcept
{
	multiply (dataAndDest, size, DataType (-1));
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void negateAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	multiplyAndCopy (dest, data, size, DataType (-1));
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void clip (DataType* const dataAndDest, SizeType size, DataType lowClip, DataType hiClip) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
	{
		const auto curr = dataAndDest[i];

		dataAndDest[i] = std::max (curr, lowClip);
		dataAndDest[i] = std::min (curr, hiClip);
	}
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void clipAndCopy (DataType* const dest, const DataType* const data, SizeType size, DataType lowClip, DataType hiClip) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
	{
		const auto curr = data[i];

		dest[i] = std::max (curr, lowClip);
		dest[i] = std::min (curr, hiClip);
	}
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT [[nodiscard]] LIMES_FORCE_INLINE DataType max (const DataType* const data, SizeType size) noexcept
{
	return *std::max_element (data, data + size);
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void max (const DataType* const data, SizeType size, DataType& maxValue, IndexType& maxIndex) noexcept
{
	const auto max_elem = std::max_element (data, data + size);

	maxValue = *max_elem;
	maxIndex = static_cast<IndexType> (std::distance (data, max_elem));
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT [[nodiscard]] LIMES_FORCE_INLINE DataType maxAbs (const DataType* const data, SizeType size) noexcept
{
	DataType maxVal { 0 };

	for (auto i = SizeType (0); i < size; ++i)
		maxVal = std::max (maxVal, std::abs (data[i]));

	return maxVal;
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void maxAbs (const DataType* const data, SizeType size, DataType& maxValue, IndexType& maxIndex) noexcept
{
	maxValue = 0;

	for (auto i = SizeType (0); i < size; ++i)
	{
		const auto curr = std::abs (data[i]);

		if (curr > maxValue)
		{
			maxValue = curr;
			maxIndex = static_cast<IndexType> (i);
		}
	}
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT [[nodiscard]] LIMES_FORCE_INLINE DataType min (const DataType* const data, SizeType size) noexcept
{
	return *std::min_element (data, data + size);
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void min (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex) noexcept
{
	const auto min_elem = std::min_element (data, data + size);

	minValue = *min_elem;
	minIndex = static_cast<IndexType> (std::distance (data, min_elem));
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT [[nodiscard]] LIMES_FORCE_INLINE DataType minAbs (const DataType* const data, SizeType size) noexcept
{
	auto minVal = std::numeric_limits<DataType>::max();

	for (auto i = SizeType (0); i < size; ++i)
		minVal = std::min (minVal, std::abs (data[i]));

	return minVal;
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void minAbs (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex) noexcept
{
	minValue = std::numeric_limits<DataType>::max();

	for (auto i = SizeType (0); i < size; ++i)
	{
		const auto curr = std::abs (data[i]);

		if (curr < minValue)
		{
			minValue = curr;
			minIndex = static_cast<IndexType> (i);
		}
	}
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void minMax (const DataType* const data, SizeType size, DataType& minValue, DataType& maxValue) noexcept
{
	const auto pair = std::minmax_element (data, data + size);

	minValue = *pair.first;
	maxValue = *pair.second;
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void minMax (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex, DataType& maxValue, IndexType& maxIndex) noexcept
{
	const auto pair = std::minmax_element (data, data + size);

	minValue = *pair.first;
	maxValue = *pair.second;

	minIndex = static_cast<IndexType> (std::distance (data, pair.first));
	maxIndex = static_cast<IndexType> (std::distance (data, pair.second));
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void minMaxAbs (const DataType* const data, SizeType size, DataType& minValue, DataType& maxValue) noexcept
{
	minValue = minAbs (data, size);
	maxValue = maxAbs (data, size);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT [[nodiscard]] LIMES_FORCE_INLINE DataType sum (const DataType* const data, SizeType size) noexcept
{
	return std::accumulate (data, data + size, DataType (0));
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT [[nodiscard]] LIMES_FORCE_INLINE DataType mean (const DataType* const data, SizeType size) noexcept
{
	return sum (data, size) / static_cast<DataType> (size);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT [[nodiscard]] LIMES_FORCE_INLINE DataType standard_deviation (const DataType* const data, SizeType size) noexcept
{
	const auto meanVal = mean (data, size);

	auto sumVal = DataType { 0 };

	for (auto i = SizeType (0); i < size; ++i)
	{
		const auto val = data[i] - meanVal;
		sumVal += (val * val);
	}

	return std::sqrt (sumVal / static_cast<DataType> (size));
}

/** @} */

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark -
#pragma mark Trigonometric functions

namespace detail
{
/// @cond

/** A Pade approximant continued fraction.
	Works best for input in the range -pi, pi.
 */
template <Scalar DataType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE DataType fast_sine (DataType x) noexcept
{
	const auto x2		   = x * x;
	const auto numerator   = -x * (DataType (-11511339840) + x2 * (DataType (1640635920) + x2 * (DataType (-52785432) + x2 * DataType (479249))));
	const auto denominator = DataType (11511339840) + x2 * (DataType (277920720) + x2 * (DataType (3177720) + x2 * DataType (18361)));

	return numerator / denominator;
}

/** A Pade approximant continued fraction.
	Works best for input in the range -pi, pi.
 */
template <Scalar DataType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE DataType fast_cosine (DataType x) noexcept
{
	const auto x2		   = x * x;
	const auto numerator   = -(DataType (-39251520) + x2 * (DataType (18471600) + x2 * (DataType (-1075032) + DataType (14615) * x2)));
	const auto denominator = DataType (39251520) + x2 * (DataType (1154160) + x2 * (DataType (16632) + x2 * DataType (127)));

	return numerator / denominator;
}

/** A Pade approximant continued fraction.
	Works best for input in the range -pi/2, pi/2.
 */
template <Scalar DataType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE DataType fast_tangent (DataType x) noexcept
{
	const auto x2		   = x * x;
	const auto numerator   = x * (DataType (-135135) + x2 * (DataType (17325) + x2 * (DataType (-378) + x2)));
	const auto denominator = DataType (-135135) + x2 * (DataType (62370) + x2 * (DataType (-3150) + DataType (28) * x2));

	return numerator / denominator;
}

/// @endcond
}  // namespace detail

/** @ingroup vec_trig */
template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void sinCos (const DataType* const data, SizeType size, DataType* const sinesOut, DataType* const cosinesOut) noexcept
{
#if (LIMES_ARM_NEON || LIMES_SSE)
	if constexpr (std::is_same_v<DataType, float>)
		pommier::sinCos (data, static_cast<int> (size), sinesOut, cosinesOut);
	else
#endif
		for (auto i = SizeType (0); i < size; ++i)
		{
			const auto thisData = data[i];

			sinesOut[i]	  = detail::fast_sine (thisData);
			cosinesOut[i] = detail::fast_cosine (thisData);
		}
}

#pragma mark Sine

/* --- sin --- */

/** @ingroup vec_sin
	@{
 */

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void sine (DataType* const data, SizeType size) noexcept
{
#if (LIMES_ARM_NEON || LIMES_SSE)
	if constexpr (std::is_same_v<DataType, float>)
		pommier::sine (data, size);
	else
#endif
		for (auto i = SizeType (0); i < size; ++i)
			data[i] = detail::fast_sine (data[i]);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void sineAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
#if (LIMES_ARM_NEON || LIMES_SSE)
	if constexpr (std::is_same_v<DataType, float>)
		pommier::sineAndCopy (dest, data, size);
	else
#endif
		for (auto i = SizeType (0); i < size; ++i)
			dest[i] = detail::fast_sine (data[i]);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void arcsine (DataType* const data, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		data[i] = std::asin (data[i]);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void arcsineAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = std::asin (data[i]);
}

/** @} */

#pragma mark Cosine

/* --- cos --- */

/** @ingroup vec_cos
	@{
 */

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void cos (DataType* const data, SizeType size) noexcept
{
#if (LIMES_ARM_NEON || LIMES_SSE)
	if constexpr (std::is_same_v<DataType, float>)
		pommier::cos (data, size);
	else
#endif
		for (auto i = SizeType (0); i < size; ++i)
			data[i] = detail::fast_cosine (data[i]);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void cosAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
#if (LIMES_ARM_NEON || LIMES_SSE)
	if constexpr (std::is_same_v<DataType, float>)
		pommier::cosAndCopy (data, size);
	else
#endif
		for (auto i = SizeType (0); i < size; ++i)
			dest[i] = detail::fast_cosine (data[i]);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void arccos (DataType* const data, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		data[i] = std::acos (data[i]);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void arccosAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = std::acos (data[i]);
}

/** @} */

#pragma mark Tangent

/* --- tan --- */

/** @ingroup vec_tan
	@{
 */

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void tan (DataType* const data, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		data[i] = detail::fast_tangent (data[i]);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void tanAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = detail::fast_tangent (data[i]);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void arctan (DataType* const data, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		data[i] = std::atan (data[i]);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void arctanAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = std::atan (data[i]);
}

/** @} */

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark -
#pragma mark Exponential and logarithmic functions

#pragma mark Logarithms

/** This namespace contains implementations of the logarithmic functions in raw C++.
	@ingroup vec_log
 */
namespace log
{

/** @ingroup vec_log
	@{
 */

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void nat (DataType* const data, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		data[i] = std::log (data[i]);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void natAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = std::log (data[i]);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void base2 (DataType* const data, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		data[i] = std::log2 (data[i]);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void base2AndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = std::log2 (data[i]);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void base10 (DataType* const data, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		data[i] = std::log10 (data[i]);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void base10AndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = std::log10 (data[i]);
}

/** @} */

}  // namespace log

#pragma mark Exponents

/** This namespace contains implementations of the exponential functions in raw C++.
	@ingroup vec_exp
 */
namespace exp
{

/** @ingroup vec_exp
	@{
 */

namespace detail
{

/// @cond internals

/** A Pade approximant continued fraction.
	Works best for input in range -6, +4
 */
template <Scalar DataType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE DataType fast_exp (DataType x) noexcept
{
	const auto numerator   = DataType (1680) + x * (DataType (840) + x * (DataType (180) + x * (DataType (20) + x)));
	const auto denominator = DataType (1680) + x * (DataType (-840) + x * (DataType (180) + x * (DataType (-20) + x)));

	return numerator / denominator;
}

/// @endcond

}  // namespace detail

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void e (DataType* const data, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		data[i] = detail::fast_exp (data[i]);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void eAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = detail::fast_exp (data[i]);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void two (DataType* const data, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		data[i] = std::exp2 (data[i]);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void twoAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = std::exp2 (data[i]);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void pow (DataType* const dataAndDest, const DataType* const exponents, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] = std::pow (dataAndDest[i], exponents[i]);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void powAndCopy (DataType* const dest, const DataType* const data, const DataType* const exponents, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = std::pow (data[i], exponents[i]);
}

/** @} */

}  // namespace exp

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark -
#pragma mark Audio utility functions

/** @ingroup vec_audio
	@{
 */

template <Scalar DataType, Integral SizeType1, Integral SizeType2>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void mix (DataType* const output, const DataType* const * const origData, SizeType1 numChannels, SizeType2 numSamples) noexcept
{
	copy (output, origData[0], numSamples);

	if (numChannels == 1)
		return;

	for (auto c = 1; c < static_cast<int> (numChannels); ++c)
		add (output, numSamples, origData[c]);

	multiply (output, numSamples, DataType (1.) / static_cast<DataType> (numChannels));
}


template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT [[nodiscard]] LIMES_FORCE_INLINE DataType rms (const DataType* const data, SizeType size) noexcept
{
	DataType t = DataType (0);

	if (size == 0)
		return t;

	for (auto i = 0; i < static_cast<int> (size); ++i)
		t += (data[i] * data[i]);

	t /= static_cast<DataType> (size);

	return std::sqrt (t);
}


template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT [[nodiscard]] LIMES_FORCE_INLINE int countZeroCrossings (const DataType* const data, SizeType size) noexcept
{
	auto numCrossings = 0;

	auto sign = data[0] > DataType (0);

	for (auto i = 1; i < static_cast<int> (size); ++i)
	{
		const auto currSign = data[i] > DataType (0);

		if (currSign != sign)
		{
			sign = currSign;
			++numCrossings;
		}
	}

	return numCrossings;
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void generateRamp (DataType* const output, SizeType size, DataType startValue, DataType endValue) noexcept
{
	const auto increment = (endValue - startValue) / static_cast<DataType> (size);

	for (auto i = SizeType (0); i < size; ++i)
		output[i] = (startValue + (increment * static_cast<DataType> (i)));
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void applyRamp (DataType* const dataAndDest, SizeType size, DataType startValue, DataType endValue) noexcept
{
	const auto increment = (endValue - startValue) / static_cast<DataType> (size);

	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] *= (startValue + (increment * static_cast<DataType> (i)));
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void applyRampAndCopy (DataType* const dest, const DataType* const data, SizeType size, DataType startValue, DataType endValue) noexcept
{
	generateRamp (dest, size, startValue, endValue);
	multiply (dest, size, data);
}

/** @} */

/// @endcond

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark -
#pragma mark Windowing functions

/** This namespace contains the implementations of the windowing functions in raw C++.
	@ingroup vec_window
 */
namespace window
{

/** This namespace contains implementation details for windowing functions.
	@ingroup vec_window
 */
namespace detail
{

/// @cond internals

/** @ingroup vec_window */
template <Scalar ValueType, Integral SizeType>
LIMES_NO_EXPORT [[nodiscard]] LIMES_FORCE_INLINE ValueType ncos (SizeType order, SizeType i, SizeType size) noexcept
{
	return std::cos (static_cast<ValueType> (order * i)
					 * math::constants::pi<ValueType> / static_cast<ValueType> (size - 1));
}

/** @ingroup vec_blackman */
template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT [[nodiscard]] LIMES_FORCE_INLINE DataType getBlackmanSample (SizeType size, SizeType i) noexcept
{
	const auto cos2 = ncos<DataType> (SizeType (2), i, size);
	const auto cos4 = ncos<DataType> (SizeType (4), i, size);

	return static_cast<DataType> (0.5 * (1. - math::constants::blackman_alpha<DataType>) -0.5 * cos2 + 0.5 * math::constants::blackman_alpha<DataType> * cos4);
}

/** @ingroup vec_hamm */
template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT [[nodiscard]] LIMES_FORCE_INLINE DataType getHammSample (SizeType size, SizeType i) noexcept
{
	return static_cast<DataType> (0.54 - 0.46 * ncos<DataType> (SizeType (2), i, size));
}

/** @ingroup vec_hanning */
template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT [[nodiscard]] LIMES_FORCE_INLINE DataType getHanningSample (SizeType size, SizeType i) noexcept
{
	return static_cast<DataType> (0.5 - 0.5 * detail::ncos<DataType> (SizeType (2), i, size));
}

/// @endcond

}  // namespace detail

/// @cond internals

#pragma mark Blackman

/* --- Blackman --- */

/** @ingroup vec_blackman
	@{
 */

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void generateBlackman (DataType* const output, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		output[i] = detail::getBlackmanSample<DataType> (size, i);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void applyBlackman (DataType* const dataAndDest, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] *= detail::getBlackmanSample<DataType> (size, i);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void applyBlackmanAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	window::generateBlackman (dest, size);
	multiply (dest, size, data);
}

/** @} */

#pragma mark Hamm

/* --- Hamm --- */

/** @ingroup vec_hamm
	@{
 */

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void generateHamm (DataType* const output, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		output[i] = detail::getHammSample<DataType> (size, i);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void applyHamm (DataType* const dataAndDest, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] *= detail::getHammSample<DataType> (size, i);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void applyHammAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	window::generateHamm (dest, size);
	multiply (dest, size, data);
}

/** @} */

#pragma mark Hanning

/* --- Hanning --- */

/** @ingroup vec_hanning
	@{
 */

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void generateHanning (DataType* const output, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		output[i] = detail::getHanningSample<DataType> (size, i);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void applyHanning (DataType* const dataAndDest, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] *= detail::getHanningSample<DataType> (size, i);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void applyHanningAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	window::generateHanning (dest, size);
	multiply (dest, size, data);
}

/** @} */

/// @endcond

}  // namespace window

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark -
#pragma mark Complex conversions

/** This namespace contains implementation details for the vecops implementations in raw C++.
	@ingroup limes_vecops
 */
namespace detail
{

/// @cond internals

/** @ingroup limes_vecops */
template <Scalar T>
LIMES_NO_EXPORT void magphase (T* const mag, T* const phase, T real, T imag) noexcept;

/// @endcond

}  // namespace detail

/// @cond internals

/** @ingroup vec_complex
	@{
 */

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void polarToCartesian (DataType* const real, DataType* const imag, const DataType* const mag, const DataType* const phase, SizeType size) noexcept
{
#if (LIMES_ARM_NEON || LIMES_SSE)
	if constexpr (std::is_same_v<DataType, float>)
	{
		pommier::polarToCartesian (real, imag, mag, phase, static_cast<int> (size));
		return;
	}
#endif

	for (auto i = SizeType (0); i < size; ++i)
		vecops::detail::phasor<DataType> (real + i, imag + i, phase[i]);

	multiply (real, size, mag);
	multiply (imag, size, mag);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void polarToCartesianInterleaved (DataType* const dest, const DataType* const mag, const DataType* const phase, SizeType size) noexcept
{
#if (LIMES_ARM_NEON || LIMES_SSE)
	if constexpr (std::is_same_v<DataType, float>)
	{
		pommier::polarToCartesianInterleaved (dest, mag, phase, size);
		return;
	}
#endif

	DataType real, imag;

	for (auto i = SizeType (0); i < size; ++i)
	{
		vecops::detail::phasor<DataType> (&real, &imag, phase[i]);

		real *= mag[i];
		imag *= mag[i];

		const auto i2 = i * 2;

		dest[i2]	 = real;
		dest[i2 + 1] = imag;
	}
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void cartesianToPolar (DataType* const mag, DataType* const phase, const DataType* const real, const DataType* const imag, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		detail::magphase<DataType> (mag + i, phase + i, real[i], imag[i]);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void catesianInterleavedToPolar (DataType* const mag, DataType* const phase, const DataType* const src, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
	{
		const auto i2 = i * 2;
		detail::magphase<DataType> (mag + i, phase + i, src[i2], src[i2 + 1]);
	}
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void cartesianToMagnitudes (DataType* const mag, const DataType* const real, const DataType* const imag, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
		mag[i] = static_cast<DataType> (std::sqrt (real[i] * real[i] + imag[i] * imag[i]));
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void cartesianInterleavedToMagnitudes (DataType* const mag, const DataType* const src, SizeType size) noexcept
{
	for (auto i = SizeType (0); i < size; ++i)
	{
		const auto i2 = i * 2;
		mag[i]		  = static_cast<DataType> (std::sqrt (src[i2] * src[i2] + src[i2 + 1] * src[i2 + 1]));
	}
}

/** @} */

/// @endcond

}  // namespace vecops::fb

LIMES_END_NAMESPACE
