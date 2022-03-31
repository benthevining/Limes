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

#include <cstring>	// for memset, memcpy
#include <cmath>
#include <algorithm>
#include <numeric>
#include <limits>

namespace limes::vecops::fb
{

template <Scalar DataType, Integral SizeType>
inline void fill (DataType* const data, SizeType size, DataType constantToFill)
{
	std::memset (data, static_cast<int> (constantToFill), static_cast<size_t> (size) * sizeof (DataType));
}

template <Scalar DataType, Integral SizeType>
inline void clear (DataType* const data, SizeType size)
{
	std::memset (data, 0, static_cast<size_t> (size) * sizeof (DataType));
}

template <Scalar DataType, Integral SizeType>
inline void copy (DataType* const dest, const DataType* const source, SizeType size)
{
	std::memcpy (dest, source, static_cast<size_t> (size) * sizeof (DataType));
}

template <Scalar DataType, Integral SizeType>
inline void swap (DataType* const vecA, DataType* const vecB, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
	{
		const auto elem_a = vecA[i];

		vecA[i] = vecB[i];
		vecB[i] = elem_a;
	}
}


/*---------------------------------------------------------------------------------------------------------------------------*/


/*-----  ADDITION  -----*/

template <Scalar DataType, Integral SizeType>
inline void add (DataType* const data, SizeType size, DataType constantToAdd)
{
	for (auto i = SizeType (0); i < size; ++i)
		data[i] += constantToAdd;
}

template <Scalar DataType, Integral SizeType>
inline void add (DataType* const dataAndDest, SizeType size, const DataType* const dataToAdd)
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] += dataToAdd[i];
}

template <Scalar DataType, Integral SizeType>
inline void addAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToAdd)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = origData[i] + constantToAdd;
}

template <Scalar DataType, Integral SizeType>
inline void addAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToAdd)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = origData[i] + dataToAdd[i];
}


/*-----  SUBTRACTION  -----*/

template <Scalar DataType, Integral SizeType>
inline void subtract (DataType* const data, SizeType size, DataType constantToSubtract)
{
	for (auto i = SizeType (0); i < size; ++i)
		data[i] -= constantToSubtract;
}

template <Scalar DataType, Integral SizeType>
inline void subtract (DataType* const dataAndDest, SizeType size, const DataType* const dataToSubtract)
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] -= dataToSubtract[i];
}

template <Scalar DataType, Integral SizeType>
inline void subtractAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToSubtract)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = origData[i] - constantToSubtract;
}

template <Scalar DataType, Integral SizeType>
inline void subtractAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToSubtract)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = origData[i] - dataToSubtract[i];
}


/*-----  MULTIPLICATION  -----*/

template <Scalar DataType, Integral SizeType>
inline void multiply (DataType* const data, SizeType size, DataType constantToMultiply)
{
	for (auto i = SizeType (0); i < size; ++i)
		data[i] *= constantToMultiply;
}

template <Scalar DataType, Integral SizeType>
inline void multiply (DataType* const dataAndDest, SizeType size, const DataType* const dataToMultiply)
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] *= dataToMultiply[i];
}

template <Scalar DataType, Integral SizeType>
inline void multiplyAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToMultiply)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = origData[i] * constantToMultiply;
}

template <Scalar DataType, Integral SizeType>
inline void multiplyAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToMultiply)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = origData[i] * dataToMultiply[i];
}


/*-----  DIVISION  -----*/

template <Scalar DataType, Integral SizeType>
inline void divide (DataType* const data, SizeType size, DataType constantToDivide)
{
	for (auto i = SizeType (0); i < size; ++i)
		data[i] /= constantToDivide;
}

template <Scalar DataType, Integral SizeType>
inline void divide (DataType* const dataAndDest, SizeType size, const DataType* const dataToDivide)
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] /= dataToDivide[i];
}

template <Scalar DataType, Integral SizeType>
inline void divideAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToDivide)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = origData[i] / constantToDivide;
}

template <Scalar DataType, Integral SizeType>
inline void divideAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToDivide)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = origData[i] / dataToDivide[i];
}


/*---------------------------------------------------------------------------------------------------------------------------*/


template <Scalar DataType, Integral SizeType>
inline void square (DataType* const dataAndDest, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] *= dataAndDest[i];
}

template <Scalar DataType, Integral SizeType>
inline void squareAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = (data[i] * data[i]);
}

template <Scalar DataType, Integral SizeType>
inline void squareRoot (DataType* const dataAndDest, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] = static_cast<DataType> (std::sqrt (dataAndDest[i]));
}

template <Scalar DataType, Integral SizeType>
inline void squareRootAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = static_cast<DataType> (std::sqrt (data[i]));
}


/*---------------------------------------------------------------------------------------------------------------------------*/


template <Scalar DataType, Integral SizeType>
inline void reverse (DataType* const dataAndDest, SizeType size)
{
	std::reverse (dataAndDest, dataAndDest + size);
}

template <Scalar DataType, Integral SizeType>
inline void reverseAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	copy (dest, data, size);
	reverse (dest, size);
}

template <Scalar DataType, Integral SizeType>
inline void sort (DataType* const dataAndDest, SizeType size)
{
	std::sort (dataAndDest, dataAndDest + size);
}

template <Scalar DataType, Integral SizeType>
inline void sortAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	copy (dest, data, size);
	sort (dest, size);
}

template <Scalar DataType, Integral SizeType>
inline void sortReverse (DataType* const dataAndDest, SizeType size)
{
	std::sort (dataAndDest, dataAndDest + size);
	std::reverse (dataAndDest, dataAndDest + size);
}

template <Scalar DataType, Integral SizeType>
inline void sortReverseAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	copy (dest, data, size);
	sortReverse (dest, size);
}

template <Scalar DataType, Integral SizeType1, Integral SizeType2>
inline void interleave (DataType* const output, const DataType* const * const origData, SizeType1 numChannels, SizeType2 numSamples)
{
	int idx = 0;

	switch (numChannels)
	{
		case 2 :
		{
			// common case, may be vectorized by compiler if hardcoded
			for (int i = 0; i < static_cast<int> (numSamples); ++i)
				for (int j = 0; j < 2; ++j)
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
			for (int i = 0; i < static_cast<int> (numSamples); ++i)
				for (int j = 0; j < static_cast<int> (numChannels); ++j)
					output[idx++] = origData[j][i];
		}
	}
}

template <Scalar DataType, Integral SizeType1, Integral SizeType2>
inline void deinterleave (DataType* const * const output, const DataType* const interleavedData, SizeType1 numChannels, SizeType2 numSamples)
{
	int idx = 0;

	switch (numChannels)
	{
		case 2 :
		{
			// common case, may be vectorized by compiler if hardcoded
			for (int i = 0; i < static_cast<int> (numSamples); ++i)
				for (int j = 0; j < 2; ++j)
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
			for (int i = 0; i < static_cast<int> (numSamples); ++i)
				for (int j = 0; j < static_cast<int> (numChannels); ++j)
					output[j][i] = interleavedData[idx++];
		}
	}
}


/*---------------------------------------------------------------------------------------------------------------------------*/


template <Scalar DataType, Integral SizeType>
inline void abs (DataType* const dataAndDest, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] = std::abs (dataAndDest[i]);
}

template <Scalar DataType, Integral SizeType>
inline void absAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = std::abs (data[i]);
}

template <Scalar DataType, Integral SizeType>
inline void negate (DataType* const dataAndDest, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] = -dataAndDest[i];
}

template <Scalar DataType, Integral SizeType>
inline void negateAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = -data[i];
}

template <Scalar DataType, Integral SizeType>
inline void clip (DataType* const dataAndDest, SizeType size, DataType lowClip, DataType hiClip)
{
	for (auto i = SizeType (0); i < size; ++i)
	{
		const auto curr = dataAndDest[i];

		dataAndDest[i] = std::max (curr, lowClip);
		dataAndDest[i] = std::min (curr, hiClip);
	}
}

template <Scalar DataType, Integral SizeType>
inline void clipAndCopy (DataType* const dest, const DataType* const data, SizeType size, DataType lowClip, DataType hiClip)
{
	for (auto i = SizeType (0); i < size; ++i)
	{
		const auto curr = data[i];

		dest[i] = std::max (curr, lowClip);
		dest[i] = std::min (curr, hiClip);
	}
}

template <Scalar DataType, Integral SizeType>
[[nodiscard]] inline DataType max (const DataType* const data, SizeType size)
{
	return *std::max_element (data, data + size);
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
inline void max (const DataType* const data, SizeType size, DataType& maxValue, IndexType& maxIndex)
{
	const auto max_elem = std::max_element (data, data + size);

	maxValue = *max_elem;
	maxIndex = static_cast<IndexType> (std::distance (data, max_elem));
}

template <Scalar DataType, Integral SizeType>
[[nodiscard]] inline DataType maxAbs (const DataType* const data, SizeType size)
{
	DataType maxVal { 0 };

	for (auto i = SizeType (0); i < size; ++i)
		maxVal = std::max (maxVal, std::abs (data[i]));

	return maxVal;
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
inline void maxAbs (const DataType* const data, SizeType size, DataType& maxValue, IndexType& maxIndex)
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
[[nodiscard]] inline DataType min (const DataType* const data, SizeType size)
{
	return *std::min_element (data, data + size);
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
inline void min (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex)
{
	const auto min_elem = std::min_element (data, data + size);

	minValue = *min_elem;
	minIndex = static_cast<IndexType> (std::distance (data, min_elem));
}

template <Scalar DataType, Integral SizeType>
[[nodiscard]] inline DataType minAbs (const DataType* const data, SizeType size)
{
	auto minVal = std::numeric_limits<DataType>::max();

	for (auto i = SizeType (0); i < size; ++i)
		minVal = std::min (minVal, std::abs (data[i]));

	return minVal;
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
inline void minAbs (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex)
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
inline void minMax (const DataType* const data, SizeType size, DataType& minValue, DataType& maxValue)
{
	const auto pair = std::minmax_element (data, data + size);

	minValue = *pair.first;
	maxValue = *pair.second;
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
inline void minMax (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex, DataType& maxValue, IndexType& maxIndex)
{
	const auto pair = std::minmax_element (data, data + size);

	minValue = *pair.first;
	maxValue = *pair.second;

	minIndex = static_cast<IndexType> (std::distance (data, pair.first));
	maxIndex = static_cast<IndexType> (std::distance (data, pair.second));
}

template <Scalar DataType, Integral SizeType>
inline void minMaxAbs (const DataType* const data, SizeType size, DataType& minValue, DataType& maxValue)
{
	minValue = minAbs (data, size);
	maxValue = maxAbs (data, size);
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
inline void minMaxAbs (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex, DataType& maxValue, IndexType& maxIndex)
{
	minAbs (data, size, minValue, minIndex);
	maxAbs (data, size, maxValue, maxIndex);
}

template <Scalar DataType, Integral SizeType>
[[nodiscard]] inline DataType range (const DataType* const data, SizeType size)
{
	DataType minVal, maxVal;

	minMax (data, size, minVal, maxVal);

	return maxVal - minVal;
}

template <Scalar DataType, Integral SizeType>
[[nodiscard]] inline DataType rangeAbs (const DataType* const data, SizeType size)
{
	DataType minVal, maxVal;

	minMaxAbs (data, size, minVal, maxVal);

	return maxVal - minVal;
}

template <Scalar DataType, Integral SizeType>
[[nodiscard]] inline DataType sum (const DataType* const data, SizeType size)
{
	return std::accumulate (data, data + size, DataType (0));
}

template <Scalar DataType, Integral SizeType>
[[nodiscard]] inline DataType mean (const DataType* const data, SizeType size)
{
	return sum (data, size) / static_cast<DataType> (size);
}


/*---------------------------------------------------------------------------------------------------------------------------*/


template <Scalar DataType, Integral SizeType1, Integral SizeType2>
inline void mix (DataType* const output, const DataType* const * const origData, SizeType1 numChannels, SizeType2 numSamples)
{
	clear (output, numSamples);

	for (int c = 0; c < static_cast<int> (numChannels); ++c)
		add (output, numSamples, origData[c]);

	multiply (output, numSamples, DataType (1.0) / static_cast<DataType> (numChannels));
}


template <Scalar DataType, Integral SizeType>
[[nodiscard]] inline DataType rms (const DataType* const data, SizeType size)
{
	DataType t = DataType (0);

	if (size == 0) return t;

	for (int i = 0; i < static_cast<int> (size); ++i)
		t += (data[i] * data[i]);

	t /= static_cast<DataType> (size);

	return std::sqrt (t);
}


template <Scalar DataType, Integral SizeType>
[[nodiscard]] inline int countZeroCrossings (const DataType* const data, SizeType size)
{
	int numCrossings { 0 };

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
inline void generateRamp (DataType* const output, SizeType size, DataType startValue, DataType endValue)
{
	const auto increment = (endValue - startValue) / static_cast<DataType> (size);

	for (auto i = SizeType (0); i < size; ++i)
		output[i] = (startValue + (increment * static_cast<DataType> (i)));
}

template <Scalar DataType, Integral SizeType>
inline void applyRamp (DataType* const dataAndDest, SizeType size, DataType startValue, DataType endValue)
{
	const auto increment = (endValue - startValue) / static_cast<DataType> (size);

	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] *= (startValue + (increment * static_cast<DataType> (i)));
}

template <Scalar DataType, Integral SizeType>
inline void applyRampAndCopy (DataType* const dest, const DataType* const data, SizeType size, DataType startValue, DataType endValue)
{
	const auto increment = (endValue - startValue) / static_cast<DataType> (size);

	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = data[i] * (startValue + (increment * static_cast<DataType> (i)));
}


namespace window
{

namespace detail
{

template <Scalar ValueType, Integral SizeType>
[[nodiscard]] inline ValueType ncos (SizeType order, SizeType i, SizeType size) noexcept
{
	return std::cos (static_cast<ValueType> (order * i)
					 * constants::pi<ValueType> / static_cast<ValueType> (size - 1));
}

template <Scalar DataType, Integral SizeType>
[[nodiscard]] inline DataType getBlackmanSample (SizeType size, SizeType i) noexcept
{
	const auto cos2 = ncos<DataType> (SizeType (2), i, size);
	const auto cos4 = ncos<DataType> (SizeType (4), i, size);

	return static_cast<DataType> (0.5 * (1. - constants::blackman_alpha<DataType>) -0.5 * cos2 + 0.5 * constants::blackman_alpha<DataType> * cos4);
}

template <Scalar DataType, Integral SizeType>
[[nodiscard]] inline DataType getHammSample (SizeType size, SizeType i) noexcept
{
	return static_cast<DataType> (0.54 - 0.46 * ncos<DataType> (SizeType (2), i, size));
}

template <Scalar DataType, Integral SizeType>
[[nodiscard]] inline DataType getHanningSample (SizeType size, SizeType i) noexcept
{
	return static_cast<DataType> (0.5 - 0.5 * detail::ncos<DataType> (SizeType (2), i, size));
}

}  // namespace detail

template <Scalar DataType, Integral SizeType>
inline void generateBlackman (DataType* const output, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		output[i] = detail::getBlackmanSample<DataType> (size, i);
}

template <Scalar DataType, Integral SizeType>
inline void applyBlackman (DataType* const dataAndDest, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] *= detail::getBlackmanSample<DataType> (size, i);
}

template <Scalar DataType, Integral SizeType>
inline void applyBlackmanAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = data[i] * detail::getBlackmanSample<DataType> (size, i);
}

template <Scalar DataType, Integral SizeType>
inline void generateHamm (DataType* const output, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		output[i] = detail::getHammSample<DataType> (size, i);
}

template <Scalar DataType, Integral SizeType>
inline void applyHamm (DataType* const dataAndDest, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] *= detail::getHammSample<DataType> (size, i);
}

template <Scalar DataType, Integral SizeType>
inline void applyHammAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = data[i] * detail::getHammSample<DataType> (size, i);
}

template <Scalar DataType, Integral SizeType>
inline void generateHanning (DataType* const output, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		output[i] = detail::getHanningSample<DataType> (size, i);
}

template <Scalar DataType, Integral SizeType>
inline void applyHanning (DataType* const dataAndDest, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] *= detail::getHanningSample<DataType> (size, i);
}

template <Scalar DataType, Integral SizeType>
inline void applyHanningAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = data[i] * detail::getHanningSample<DataType> (size, i);
}

}  // namespace window

}  // namespace limes::vecops::fb
