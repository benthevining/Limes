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

static_assert (lemons::vecops::isUsingFallback());

#include <cstring>	// for memset
#include <cmath>
#include <algorithm>
#include <numeric>
#include <limits>

namespace lemons::vecops
{

template <Scalar DataType, Integral SizeType>
void fill (DataType* const data, SizeType size, DataType constantToFill)
{
	std::memset (data, static_cast<int> (constantToFill), static_cast<size_t> (size) * sizeof (DataType));
}

template <Scalar DataType, Integral SizeType>
void clear (DataType* const data, SizeType size)
{
	std::memset (data, 0, static_cast<size_t> (size) * sizeof (DataType));
}

template <Scalar DataType, Integral SizeType>
void swap (DataType* const vecA, DataType* const vecB, SizeType size)
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
void add (DataType* const data, SizeType size, DataType constantToAdd)
{
	for (auto i = SizeType (0); i < size; ++i)
		data[i] += constantToAdd;
}

template <Scalar DataType, Integral SizeType>
void add (DataType* const dataAndDest, SizeType size, const DataType* const dataToAdd)
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] += dataToAdd[i];
}

template <Scalar DataType, Integral SizeType>
void addAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToAdd)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = origData[i] + constantToAdd;
}

template <Scalar DataType, Integral SizeType>
void addAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToAdd)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = origData[i] + dataToAdd[i];
}


/*-----  SUBTRACTION  -----*/

template <Scalar DataType, Integral SizeType>
void subtract (DataType* const data, SizeType size, DataType constantToSubtract)
{
	for (auto i = SizeType (0); i < size; ++i)
		data[i] -= constantToSubtract;
}

template <Scalar DataType, Integral SizeType>
void subtract (DataType* const dataAndDest, SizeType size, const DataType* const dataToSubtract)
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] -= dataToSubtract[i];
}

template <Scalar DataType, Integral SizeType>
void subtractAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToSubtract)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = origData[i] - constantToSubtract;
}

template <Scalar DataType, Integral SizeType>
void subtractAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToSubtract)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = origData[i] - dataToSubtract[i];
}


/*-----  MULTIPLICATION  -----*/

template <Scalar DataType, Integral SizeType>
void multiply (DataType* const data, SizeType size, DataType constantToMultiply)
{
	for (auto i = SizeType (0); i < size; ++i)
		data[i] *= constantToMultiply;
}

template <Scalar DataType, Integral SizeType>
void multiply (DataType* const dataAndDest, SizeType size, const DataType* const dataToMultiply)
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] *= dataToMultiply[i];
}

template <Scalar DataType, Integral SizeType>
void multiplyAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToMultiply)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = origData[i] * constantToMultiply;
}

template <Scalar DataType, Integral SizeType>
void multiplyAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToMultiply)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = origData[i] * dataToMultiply[i];
}


/*-----  DIVISION  -----*/

template <Scalar DataType, Integral SizeType>
void divide (DataType* const data, SizeType size, DataType constantToDivide)
{
	for (auto i = SizeType (0); i < size; ++i)
		data[i] /= constantToDivide;
}

template <Scalar DataType, Integral SizeType>
void divide (DataType* const dataAndDest, SizeType size, const DataType* const dataToDivide)
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] /= dataToDivide[i];
}

template <Scalar DataType, Integral SizeType>
void divideAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToDivide)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = origData[i] / constantToDivide;
}

template <Scalar DataType, Integral SizeType>
void divideAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToDivide)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = origData[i] / dataToDivide[i];
}


/*---------------------------------------------------------------------------------------------------------------------------*/


template <Scalar DataType, Integral SizeType>
void square (DataType* const dataAndDest, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] *= dataAndDest[i];
}

template <Scalar DataType, Integral SizeType>
void square (DataType* const dest, const DataType* const data, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = (data[i] * data[i]);
}

template <Scalar DataType, Integral SizeType>
void squareRoot (DataType* const dataAndDest, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] = static_cast<DataType> (std::sqrt (dataAndDest[i]));
}

template <Scalar DataType, Integral SizeType>
void squareRoot (DataType* const dest, const DataType* const data, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = static_cast<DataType> (std::sqrt (data[i]));
}


/*---------------------------------------------------------------------------------------------------------------------------*/


template <Scalar DataType, Integral SizeType>
void reverse (DataType* const dataAndDest, SizeType size)
{
	std::reverse (dataAndDest, dataAndDest + size);
}

template <Scalar DataType, Integral SizeType>
void reverse (DataType* const dest, const DataType* const data, SizeType size)
{
	copy (dest, data, size);
	reverse (dest, size);
}

template <Scalar DataType, Integral SizeType>
void sort (DataType* const dataAndDest, SizeType size)
{
	std::sort (dataAndDest, dataAndDest + size);
}

template <Scalar DataType, Integral SizeType>
void sort (DataType* const dest, const DataType* const data, SizeType size)
{
	copy (dest, data, size);
	sort (dest, size);
}

template <Scalar DataType, Integral SizeType>
void sortReverse (DataType* const dataAndDest, SizeType size)
{
	std::sort (dataAndDest, dataAndDest + size);
	std::reverse (dataAndDest, dataAndDest + size);
}

template <Scalar DataType, Integral SizeType>
void sortReverse (DataType* const dest, const DataType* const data, SizeType size)
{
	copy (dest, data, size);
	sortReverse (dest, size);
}


/*---------------------------------------------------------------------------------------------------------------------------*/


template <Scalar DataType, Integral SizeType>
void abs (DataType* const dataAndDest, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] = std::abs (dataAndDest[i]);
}

template <Scalar DataType, Integral SizeType>
void abs (DataType* const dest, const DataType* const data, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = std::abs (data[i]);
}

template <Scalar DataType, Integral SizeType>
void negate (DataType* const dataAndDest, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		dataAndDest[i] = -dataAndDest[i];
}

template <Scalar DataType, Integral SizeType>
void negate (DataType* const dest, const DataType* const data, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		dest[i] = -data[i];
}

template <Scalar DataType, Integral SizeType>
void clip (DataType* const dataAndDest, SizeType size, DataType lowClip, DataType hiClip)
{
	for (auto i = SizeType (0); i < size; ++i)
	{
		const auto curr = dataAndDest[i];

		dataAndDest[i] = std::max (curr, lowClip);
		dataAndDest[i] = std::min (curr, hiClip);
	}
}

template <Scalar DataType, Integral SizeType>
void clip (DataType* const dest, const DataType* const data, SizeType size, DataType lowClip, DataType hiClip)
{
	for (auto i = SizeType (0); i < size; ++i)
	{
		const auto curr = data[i];

		dest[i] = std::max (curr, lowClip);
		dest[i] = std::min (curr, hiClip);
	}
}

template <Scalar DataType, Integral SizeType>
DataType max (const DataType* const data, SizeType size)
{
	return *std::max_element (data, data + size);
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
void max (const DataType* const data, SizeType size, DataType& maxValue, IndexType& maxIndex)
{
	const auto max_elem = std::max_element (data, data + size);

	maxValue = *max_elem;
	maxIndex = static_cast<IndexType> (std::distance (data, max_elem));
}

template <Scalar DataType, Integral SizeType>
DataType maxAbs (const DataType* const data, SizeType size)
{
	DataType maxVal { 0 };

	for (auto i = SizeType (0); i < size; ++i)
		maxVal = std::max (maxVal, std::abs (data[i]));

	return maxVal;
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
void maxAbs (const DataType* const data, SizeType size, DataType& maxValue, IndexType& maxIndex)
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
DataType min (const DataType* const data, SizeType size)
{
	return *std::min_element (data, data + size);
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
void min (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex)
{
	const auto min_elem = std::min_element (data, data + size);

	minValue = *min_elem;
	minIndex = static_cast<IndexType> (std::distance (data, min_elem));
}

template <Scalar DataType, Integral SizeType>
DataType minAbs (const DataType* const data, SizeType size)
{
	auto minVal = std::numeric_limits<DataType>::max();

	for (auto i = SizeType (0); i < size; ++i)
		minVal = std::min (minVal, std::abs (data[i]));

	return minVal;
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
void minAbs (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex)
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
DataType sum (const DataType* const data, SizeType size)
{
	return std::accumulate (data, data + size, DataType (0));
}

template <Scalar DataType, Integral SizeType>
DataType mean (const DataType* const data, SizeType size)
{
	return sum (data, size) / static_cast<DataType> (size);
}


/*---------------------------------------------------------------------------------------------------------------------------*/


template <Scalar DataType, Integral SizeType>
void generateRamp (DataType* const output, SizeType size, DataType startValue, DataType endValue)
{
	const auto increment = (endValue - startValue) / static_cast<DataType> (size);

	for (auto i = SizeType (0); i < size; ++i)
		output[i] = startValue + (increment * static_cast<DataType> (i));
}

namespace window
{

template <Scalar DataType, Integral SizeType>
void generateBlackman (DataType* const output, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
	{
		constexpr auto alpha = DataType (0.16);

		const auto cos2 = detail::ncos<DataType> (2, i, size);
		const auto cos4 = detail::ncos<DataType> (4, i, size);

		output[i] = static_cast<DataType> (0.5 * (1 - alpha) - 0.5 * cos2 + 0.5 * alpha * cos4);
	}
}

template <Scalar DataType, Integral SizeType>
void generateHamm (DataType* const output, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		output[i] = static_cast<DataType> (0.54 - 0.46 * detail::ncos<DataType> (2, i, size));
}

template <Scalar DataType, Integral SizeType>
void generateHanning (DataType* const output, SizeType size)
{
	for (auto i = SizeType (0); i < size; ++i)
		output[i] = static_cast<DataType> (0.5 - 0.5 * detail::ncos<DataType> (2, i, size));
}

}  // namespace window

}  // namespace lemons::vecops
