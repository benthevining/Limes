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

#include <algorithm>		 // for max, max_element, min, min_element, minmax_element, reverse, sort, distance
#include <limes_export.h>	 // for LIMES_NO_EXPORT
#include <limes_platform.h>	 // for LIMES_FORCE_INLINE
#include <cmath>			 // for sqrt, cos, abs
#include <cstring>			 // for memset, memcpy, size_t
#include <limits>			 // for numeric_limits
#include <numeric>			 // for accumulate
#include "phasor.h"			 // for phasor
#include <limes_namespace.h>

#if LIMES_VECOPS_USE_IPP
#	include <ipps.h>
#endif

#if LIMES_VECOPS_USE_POMMIER
#	include "pommier/pommier_wrapper.h"
#endif

LIMES_BEGIN_NAMESPACE

namespace vecops::fb
{

#pragma mark Basic functions

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void fill (DataType* const data, SizeType size, DataType constantToFill)
{
	std::memset (data, static_cast<int> (constantToFill), static_cast<size_t> (size) * sizeof (DataType));
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void clear (DataType* const data, SizeType size)
{
	fill (data, size, DataType (0));
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void copy (DataType* const dest, const DataType* const source, SizeType size)
{
	std::memcpy (dest, source, static_cast<size_t> (size) * sizeof (DataType));
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void swap (DataType* const vecA, DataType* const vecB, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
	{
		const auto elem_a = vecA[i];

		vecA[i] = vecB[i];
		vecB[i] = elem_a;
	}
}


/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark Arithmetic functions

/*-----  ADDITION  -----*/

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void add (DataType* const data, SizeType size, DataType constantToAdd)
{
	for (auto i = SizeType (0); i < size; ++i)
		data[i] += constantToAdd;
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void add (DataType* const dataAndDest, SizeType size, const DataType* const dataToAdd)
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] += dataToAdd[i];
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void addAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToAdd)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = origData[i] + constantToAdd;
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void addAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToAdd)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = origData[i] + dataToAdd[i];
}


/*-----  SUBTRACTION  -----*/

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void subtract (DataType* const data, SizeType size, DataType constantToSubtract)
{
	for (auto i = SizeType (0); i < size; ++i)
		data[i] -= constantToSubtract;
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void subtract (DataType* const dataAndDest, SizeType size, const DataType* const dataToSubtract)
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] -= dataToSubtract[i];
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void subtractAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToSubtract)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = origData[i] - constantToSubtract;
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void subtractAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToSubtract)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = origData[i] - dataToSubtract[i];
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void subtractInv (DataType* const data, SizeType size, DataType constantToSubtractFrom)
{
	for (auto i = SizeType (0); i < size; ++i)
		data[i] = constantToSubtractFrom - data[i];
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void subtractInvAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToSubtractFrom)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = constantToSubtractFrom - origData[i];
}


/*-----  MULTIPLICATION  -----*/

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void multiply (DataType* const data, SizeType size, DataType constantToMultiply)
{
	for (auto i = SizeType (0); i < size; ++i)
		data[i] *= constantToMultiply;
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void multiply (DataType* const dataAndDest, SizeType size, const DataType* const dataToMultiply)
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] *= dataToMultiply[i];
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void multiplyAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToMultiply)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = origData[i] * constantToMultiply;
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void multiplyAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToMultiply)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = origData[i] * dataToMultiply[i];
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT [[nodiscard]] LIMES_FORCE_INLINE DataType dotProduct (const DataType* const vecA, const DataType* const vecB, SizeType size)
{
	DataType dotProd { 0 };

	for (auto i = SizeType (0); i < size; ++i)
		dotProd += (vecA[i] * vecB[i]);

	return dotProd;
}


/*-----  DIVISION  -----*/

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void divide (DataType* const data, SizeType size, DataType constantToDivide)
{
	for (auto i = SizeType (0); i < size; ++i)
		data[i] /= constantToDivide;
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void divide (DataType* const dataAndDest, SizeType size, const DataType* const dataToDivide)
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] /= dataToDivide[i];
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void divideAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToDivide)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = origData[i] / constantToDivide;
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void divideAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToDivide)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = origData[i] / dataToDivide[i];
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void divideInv (DataType* const data, SizeType size, DataType constantToDivideFrom)
{
	for (auto i = SizeType (0); i < size; ++i)
	{
		const auto thisData = data[i];

		if (thisData != DataType (0))
			data[i] = constantToDivideFrom / thisData;
	}
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void divideInvAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToDivideFrom)
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


/*---------------------------------------------------------------------------------------------------------------------------*/


template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void square (DataType* const dataAndDest, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] *= dataAndDest[i];
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void squareAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = (data[i] * data[i]);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void squareRoot (DataType* const dataAndDest, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] = static_cast<DataType> (std::sqrt (dataAndDest[i]));
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void squareRootAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = static_cast<DataType> (std::sqrt (data[i]));
}


// fast inverse square root from Quake 3
LIMES_NO_EXPORT [[nodiscard]] LIMES_FORCE_INLINE float quake3_fast_inv_sqrt (float number)
{
	const auto x2 = number * 0.5f;

	// cppcheck-suppress invalidPointerCast
	long i = *(long*) &number;	// NOLINT

	i = 0x5f3759df - (i >> 1);	// what the fuck?

	// cppcheck-suppress invalidPointerCast
	float y = *(float*) &i;	 // NOLINT

	constexpr auto threehalfs = 1.5f;

	y *= (threehalfs - (x2 * y * y));  // 1st iteration

	//	y  *= ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}


template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void invSquareRoot (DataType* const dataAndDest, SizeType size)
{
	if constexpr (std::is_same_v<DataType, float>)
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
LIMES_NO_EXPORT LIMES_FORCE_INLINE void invSquareRootAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	if constexpr (std::is_same_v<DataType, float>)
	{
		for (auto i = SizeType (0); i < size; ++i)
			dest[i] = quake3_fast_inv_sqrt (data[i]);
	}
	else
	{
		for (auto i = SizeType (0); i < size; ++i)
		{
			const auto denom = static_cast<DataType> (std::sqrt (data[i]));

			if (denom == DataType (0))
				dest[i] = DataType (0);
			else
				dest[i] = DataType (1) / denom;
		}
	}
}


/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark Sorting and ordering functions

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void reverse (DataType* const dataAndDest, SizeType size)
{
	std::reverse (dataAndDest, dataAndDest + size);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void reverseAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	copy (dest, data, size);
	reverse (dest, size);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void sort (DataType* const dataAndDest, SizeType size)
{
	std::sort (dataAndDest, dataAndDest + size);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void sortAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	copy (dest, data, size);
	sort (dest, size);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void sortReverse (DataType* const dataAndDest, SizeType size)
{
	std::sort (dataAndDest, dataAndDest + size);
	std::reverse (dataAndDest, dataAndDest + size);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void sortReverseAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	copy (dest, data, size);
	sortReverse (dest, size);
}

template <Scalar DataType, Integral SizeType1, Integral SizeType2>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void interleave (DataType* const output, const DataType* const * const origData, SizeType1 numChannels, SizeType2 numSamples)
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
LIMES_NO_EXPORT LIMES_FORCE_INLINE void deinterleave (DataType* const * const output, const DataType* const interleavedData, SizeType1 numChannels, SizeType2 numSamples)
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


/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark Statistical functions

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void abs (DataType* const dataAndDest, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] = std::abs (dataAndDest[i]);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void absAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = std::abs (data[i]);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void negate (DataType* const dataAndDest, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] = -dataAndDest[i];
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void negateAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = -data[i];
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void clip (DataType* const dataAndDest, SizeType size, DataType lowClip, DataType hiClip)
{
	for (auto i = SizeType (0); i < size; ++i)
	{
		const auto curr = dataAndDest[i];

		dataAndDest[i] = std::max (curr, lowClip);
		dataAndDest[i] = std::min (curr, hiClip);
	}
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void clipAndCopy (DataType* const dest, const DataType* const data, SizeType size, DataType lowClip, DataType hiClip)
{
	for (auto i = SizeType (0); i < size; ++i)
	{
		const auto curr = data[i];

		dest[i] = std::max (curr, lowClip);
		dest[i] = std::min (curr, hiClip);
	}
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT [[nodiscard]] LIMES_FORCE_INLINE DataType max (const DataType* const data, SizeType size)
{
	return *std::max_element (data, data + size);
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void max (const DataType* const data, SizeType size, DataType& maxValue, IndexType& maxIndex)
{
	const auto max_elem = std::max_element (data, data + size);

	maxValue = *max_elem;
	maxIndex = static_cast<IndexType> (std::distance (data, max_elem));
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT [[nodiscard]] LIMES_FORCE_INLINE DataType maxAbs (const DataType* const data, SizeType size)
{
	DataType maxVal { 0 };

	for (auto i = SizeType (0); i < size; ++i)
		maxVal = std::max (maxVal, std::abs (data[i]));

	return maxVal;
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void maxAbs (const DataType* const data, SizeType size, DataType& maxValue, IndexType& maxIndex)
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
LIMES_NO_EXPORT [[nodiscard]] LIMES_FORCE_INLINE DataType min (const DataType* const data, SizeType size)
{
	return *std::min_element (data, data + size);
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void min (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex)
{
	const auto min_elem = std::min_element (data, data + size);

	minValue = *min_elem;
	minIndex = static_cast<IndexType> (std::distance (data, min_elem));
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT [[nodiscard]] LIMES_FORCE_INLINE DataType minAbs (const DataType* const data, SizeType size)
{
	auto minVal = std::numeric_limits<DataType>::max();

	for (auto i = SizeType (0); i < size; ++i)
		minVal = std::min (minVal, std::abs (data[i]));

	return minVal;
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void minAbs (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex)
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
LIMES_NO_EXPORT LIMES_FORCE_INLINE void minMax (const DataType* const data, SizeType size, DataType& minValue, DataType& maxValue)
{
	const auto pair = std::minmax_element (data, data + size);

	minValue = *pair.first;
	maxValue = *pair.second;
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void minMax (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex, DataType& maxValue, IndexType& maxIndex)
{
	const auto pair = std::minmax_element (data, data + size);

	minValue = *pair.first;
	maxValue = *pair.second;

	minIndex = static_cast<IndexType> (std::distance (data, pair.first));
	maxIndex = static_cast<IndexType> (std::distance (data, pair.second));
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void minMaxAbs (const DataType* const data, SizeType size, DataType& minValue, DataType& maxValue)
{
	minValue = minAbs (data, size);
	maxValue = maxAbs (data, size);
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void minMaxAbs (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex, DataType& maxValue, IndexType& maxIndex)
{
	minAbs (data, size, minValue, minIndex);
	maxAbs (data, size, maxValue, maxIndex);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT [[nodiscard]] LIMES_FORCE_INLINE DataType range (const DataType* const data, SizeType size)
{
	DataType minVal, maxVal;

	minMax (data, size, minVal, maxVal);

	return maxVal - minVal;
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT [[nodiscard]] LIMES_FORCE_INLINE DataType rangeAbs (const DataType* const data, SizeType size)
{
	DataType minVal, maxVal;

	minMaxAbs (data, size, minVal, maxVal);

	return maxVal - minVal;
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT [[nodiscard]] LIMES_FORCE_INLINE DataType sum (const DataType* const data, SizeType size)
{
	return std::accumulate (data, data + size, DataType (0));
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT [[nodiscard]] LIMES_FORCE_INLINE DataType mean (const DataType* const data, SizeType size)
{
	return sum (data, size) / static_cast<DataType> (size);
}


/*---------------------------------------------------------------------------------------------------------------------------*/


template <Scalar DataType, Integral SizeType1, Integral SizeType2>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void mix (DataType* const output, const DataType* const * const origData, SizeType1 numChannels, SizeType2 numSamples)
{
	copy (output, origData[0], numSamples);

	if (numChannels == 1)
		return;

	for (auto c = 1; c < static_cast<int> (numChannels); ++c)
		add (output, numSamples, origData[c]);

	multiply (output, numSamples, DataType (1.) / static_cast<DataType> (numChannels));
}


template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT [[nodiscard]] LIMES_FORCE_INLINE DataType rms (const DataType* const data, SizeType size)
{
	DataType t = DataType (0);

	if (size == 0) return t;

	for (auto i = 0; i < static_cast<int> (size); ++i)
		t += (data[i] * data[i]);

	t /= static_cast<DataType> (size);

	return std::sqrt (t);
}


template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT [[nodiscard]] LIMES_FORCE_INLINE int countZeroCrossings (const DataType* const data, SizeType size)
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


/*---------------------------------------------------------------------------------------------------------------------------*/

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void generateRamp (DataType* const output, SizeType size, DataType startValue, DataType endValue)
{
	const auto increment = (endValue - startValue) / static_cast<DataType> (size);

	for (auto i = SizeType (0); i < size; ++i)
		output[i] = (startValue + (increment * static_cast<DataType> (i)));
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void applyRamp (DataType* const dataAndDest, SizeType size, DataType startValue, DataType endValue)
{
	const auto increment = (endValue - startValue) / static_cast<DataType> (size);

	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] *= (startValue + (increment * static_cast<DataType> (i)));
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void applyRampAndCopy (DataType* const dest, const DataType* const data, SizeType size, DataType startValue, DataType endValue)
{
	const auto increment = (endValue - startValue) / static_cast<DataType> (size);

	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = data[i] * (startValue + (increment * static_cast<DataType> (i)));
}

#pragma mark Windowing functions

namespace window
{

namespace detail
{

template <Scalar ValueType, Integral SizeType>
LIMES_NO_EXPORT [[nodiscard]] LIMES_FORCE_INLINE ValueType ncos (SizeType order, SizeType i, SizeType size) noexcept
{
	return std::cos (static_cast<ValueType> (order * i)
					 * math::constants::pi<ValueType> / static_cast<ValueType> (size - 1));
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT [[nodiscard]] LIMES_FORCE_INLINE DataType getBlackmanSample (SizeType size, SizeType i) noexcept
{
	const auto cos2 = ncos<DataType> (SizeType (2), i, size);
	const auto cos4 = ncos<DataType> (SizeType (4), i, size);

	return static_cast<DataType> (0.5 * (1. - math::constants::blackman_alpha<DataType>) -0.5 * cos2 + 0.5 * math::constants::blackman_alpha<DataType> * cos4);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT [[nodiscard]] LIMES_FORCE_INLINE DataType getHammSample (SizeType size, SizeType i) noexcept
{
	return static_cast<DataType> (0.54 - 0.46 * ncos<DataType> (SizeType (2), i, size));
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT [[nodiscard]] LIMES_FORCE_INLINE DataType getHanningSample (SizeType size, SizeType i) noexcept
{
	return static_cast<DataType> (0.5 - 0.5 * detail::ncos<DataType> (SizeType (2), i, size));
}

}  // namespace detail

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void generateBlackman (DataType* const output, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		output[i] = detail::getBlackmanSample<DataType> (size, i);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void applyBlackman (DataType* const dataAndDest, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] *= detail::getBlackmanSample<DataType> (size, i);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void applyBlackmanAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = data[i] * detail::getBlackmanSample<DataType> (size, i);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void generateHamm (DataType* const output, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		output[i] = detail::getHammSample<DataType> (size, i);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void applyHamm (DataType* const dataAndDest, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] *= detail::getHammSample<DataType> (size, i);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void applyHammAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = data[i] * detail::getHammSample<DataType> (size, i);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void generateHanning (DataType* const output, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		output[i] = detail::getHanningSample<DataType> (size, i);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void applyHanning (DataType* const dataAndDest, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] *= detail::getHanningSample<DataType> (size, i);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void applyHanningAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = data[i] * detail::getHanningSample<DataType> (size, i);
}

}  // namespace window

/*---------------------------------------------------------------------------------------------------------------------------*/

namespace detail
{

template <Scalar T>
LIMES_NO_EXPORT void magphase (T* const mag, T* const phase, T real, T imag);

}  // namespace detail

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void polarToCartesian (DataType* const real, DataType* const imag, const DataType* const mag, const DataType* const phase, SizeType size)
{
#if LIMES_VECOPS_USE_POMMIER
	if constexpr (std::is_same_v<DataType, float>)
	{
		pommier::polarToCartesian (real, imag, mag, phase, static_cast<int> (size));
		return;
	}
#endif

	for (auto i = SizeType (0); i < size; ++i)
		vecops::detail::phasor<DataType> (real + i, imag + i, phase[i]);

	vecops::multiply (real, size, mag);
	vecops::multiply (imag, size, mag);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void polarToCartesianInterleaved (DataType* const dest, const DataType* const mag, const DataType* const phase, SizeType size)
{
#if LIMES_VECOPS_USE_POMMIER
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
LIMES_NO_EXPORT LIMES_FORCE_INLINE void cartesianToPolar (DataType* const mag, DataType* const phase, const DataType* const real, const DataType* const imag, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		detail::magphase<DataType> (mag + i, phase + i, real[i], imag[i]);
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void catesianInterleavedToPolar (DataType* const mag, DataType* const phase, const DataType* const src, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
	{
		const auto i2 = i * 2;
		detail::magphase<DataType> (mag + i, phase + i, src[i2], src[i2 + 1]);
	}
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void cartesianToMagnitudes (DataType* const mag, const DataType* const real, const DataType* const imag, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		mag[i] = static_cast<DataType> (std::sqrt (real[i] * real[i] + imag[i] * imag[i]));
}

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void cartesianInterleavedToMagnitudes (DataType* const mag, const DataType* const src, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
	{
		const auto i2 = i * 2;
		mag[i]		  = static_cast<DataType> (std::sqrt (src[i2] * src[i2] + src[i2 + 1] * src[i2 + 1]));
	}
}

}  // namespace vecops::fb

LIMES_END_NAMESPACE
