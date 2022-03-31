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

#include <mipp.h>
#include <limits>

namespace limes::vecops
{

static_assert (isUsingMIPP());

namespace detail
{

template <Scalar DataType, Integral SizeType>
[[nodiscard]] constexpr int getVecLoopSize (SizeType size)
{
	return static_cast<int> ((static_cast<decltype (mipp::N<DataType>())> (size) / mipp::N<DataType>()) * mipp::N<DataType>());
}

}  // namespace detail

template <Scalar DataType, Integral SizeType>
void fill (DataType* const data, SizeType size, DataType constantToFill)
{
	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	const auto constant = mipp::set1<DataType> (constantToFill);

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
		mipp::store (data + i, constant);

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
		data[i] = constantToFill;
}

template <Scalar DataType, Integral SizeType>
void clear (DataType* const data, SizeType size)
{
	fill (data, size, DataType (0));
}

template <Scalar DataType, Integral SizeType>
void copy (DataType* const dest, const DataType* const source, SizeType size)
{
	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> data;

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		data.load (&source[i]);
		data.store (&dest[i]);
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
		dest[i] = source[i];
}

template <Scalar DataType, Integral SizeType>
void swap (DataType* const vecA, DataType* const vecB, SizeType size)
{
	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> dataA, dataB;

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		dataA.load (&vecA[i]);
		dataB.load (&vecB[i]);

		dataA.store (&vecB[i]);
		dataB.store (&vecA[i]);
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
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
	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> dataRegister;

	const auto constant = mipp::set1<DataType> (constantToAdd);

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		dataRegister.load (&data[i]);
		dataRegister += constant;
		dataRegister.store (&data[i]);
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
		data[i] += constantToAdd;
}

template <Scalar DataType, Integral SizeType>
void add (DataType* const dataAndDest, SizeType size, const DataType* const dataToAdd)
{
	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> dataInOut, dataAdding;

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		dataInOut.load (&dataAndDest[i]);
		dataAdding.load (&dataToAdd[i]);

		dataInOut += dataAdding;

		dataInOut.store (&dataAndDest[i]);
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
		dataAndDest[i] += dataToAdd[i];
}

template <Scalar DataType, Integral SizeType>
void addAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToAdd)
{
	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> dataRegister;

	const auto constant = mipp::set1<DataType> (constantToAdd);

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		dataRegister.load (&origData[i]);
		dataRegister += constant;
		dataRegister.store (&dest[i]);
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
		dest[i] = origData[i] + constantToAdd;
}

template <Scalar DataType, Integral SizeType>
void addAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToAdd)
{
	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> dataIn, dataAdding;

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		dataIn.load (&origData[i]);
		dataAdding.load (&dataToAdd[i]);

		dataIn += dataAdding;

		dataIn.store (&dest[i]);
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
		dest[i] = origData[i] + dataToAdd[i];
}


/*-----  SUBTRACTION  -----*/

template <Scalar DataType, Integral SizeType>
void subtract (DataType* const data, SizeType size, DataType constantToSubtract)
{
	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> dataRegister;

	const auto constant = mipp::set1<DataType> (constantToSubtract);

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		dataRegister.load (&data[i]);
		dataRegister -= constant;
		dataRegister.store (&data[i]);
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
		data[i] -= constantToSubtract;
}

template <Scalar DataType, Integral SizeType>
void subtract (DataType* const dataAndDest, SizeType size, const DataType* const dataToSubtract)
{
	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> dataInOut, dataAdding;

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		dataInOut.load (&dataAndDest[i]);
		dataAdding.load (&dataToSubtract[i]);

		dataInOut -= dataAdding;

		dataInOut.store (&dataAndDest[i]);
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
		dataAndDest[i] -= dataToSubtract[i];
}

template <Scalar DataType, Integral SizeType>
void subtractAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToSubtract)
{
	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> dataRegister;

	const auto constant = mipp::set1<DataType> (constantToSubtract);

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		dataRegister.load (&origData[i]);
		dataRegister -= constant;
		dataRegister.store (&dest[i]);
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
		dest[i] = origData[i] - constantToSubtract;
}

template <Scalar DataType, Integral SizeType>
void subtractAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToSubtract)
{
	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> dataIn, dataAdding;

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		dataIn.load (&origData[i]);
		dataAdding.load (&dataToSubtract[i]);

		dataIn -= dataAdding;

		dataIn.store (&dest[i]);
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
		dest[i] = origData[i] - dataToSubtract[i];
}


/*-----  MULTIPLICATION  -----*/

template <Scalar DataType, Integral SizeType>
void multiply (DataType* const data, SizeType size, DataType constantToMultiply)
{
	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> dataRegister;

	const auto constant = mipp::set1<DataType> (constantToMultiply);

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		dataRegister.load (&data[i]);
		dataRegister *= constant;
		dataRegister.store (&data[i]);
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
		data[i] *= constantToMultiply;
}

template <Scalar DataType, Integral SizeType>
void multiply (DataType* const dataAndDest, SizeType size, const DataType* const dataToMultiply)
{
	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> dataInOut, dataAdding;

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		dataInOut.load (&dataAndDest[i]);
		dataAdding.load (&dataToMultiply[i]);

		dataInOut *= dataAdding;

		dataInOut.store (&dataAndDest[i]);
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
		dataAndDest[i] *= dataToMultiply[i];
}

template <Scalar DataType, Integral SizeType>
void multiplyAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToMultiply)
{
	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> dataRegister;

	const auto constant = mipp::set1<DataType> (constantToMultiply);

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		dataRegister.load (&origData[i]);
		dataRegister *= constant;
		dataRegister.store (&dest[i]);
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
		dest[i] = origData[i] * constantToMultiply;
}

template <Scalar DataType, Integral SizeType>
void multiplyAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToMultiply)
{
	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> dataIn, dataAdding;

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		dataIn.load (&origData[i]);
		dataAdding.load (&dataToMultiply[i]);

		dataIn *= dataAdding;

		dataIn.store (&dest[i]);
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
		dest[i] = origData[i] * dataToMultiply[i];
}


/*-----  DIVISION  -----*/

template <Scalar DataType, Integral SizeType>
void divide (DataType* const data, SizeType size, DataType constantToDivide)
{
	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> dataRegister;

	const auto constant = mipp::set1<DataType> (constantToDivide);

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		dataRegister.load (&data[i]);
		dataRegister /= constant;
		dataRegister.store (&data[i]);
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
		data[i] /= constantToDivide;
}

template <Scalar DataType, Integral SizeType>
void divide (DataType* const dataAndDest, SizeType size, const DataType* const dataToDivide)
{
	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> dataInOut, dataAdding;

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		dataInOut.load (&dataAndDest[i]);
		dataAdding.load (&dataToDivide[i]);

		dataInOut /= dataAdding;

		dataInOut.store (&dataAndDest[i]);
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
		dataAndDest[i] /= dataToDivide[i];
}

template <Scalar DataType, Integral SizeType>
void divideAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToDivide)
{
	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> dataRegister;

	const auto constant = mipp::set1<DataType> (constantToDivide);

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		dataRegister.load (&origData[i]);
		dataRegister /= constant;
		dataRegister.store (&dest[i]);
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
		dest[i] = origData[i] / constantToDivide;
}

template <Scalar DataType, Integral SizeType>
void divideAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToDivide)
{
	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> dataIn, dataAdding;

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		dataIn.load (&origData[i]);
		dataAdding.load (&dataToDivide[i]);

		dataIn /= dataAdding;

		dataIn.store (&dest[i]);
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
		dest[i] = origData[i] / dataToDivide[i];
}


/*---------------------------------------------------------------------------------------------------------------------------*/


template <Scalar DataType, Integral SizeType>
void square (DataType* const dataAndDest, SizeType size)
{
	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> data;

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		data.load (&dataAndDest[i]);
		data *= data;
		data.store (&dataAndDest[i]);
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
		dataAndDest[i] *= dataAndDest[i];
}

template <Scalar DataType, Integral SizeType>
void squareAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> dataRegister;

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		dataRegister.load (&data[i]);
		dataRegister *= dataRegister;
		dataRegister.store (&dest[i]);
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
		dest[i] *= data[i];
}

template <Scalar DataType, Integral SizeType>
void squareRoot (DataType* const dataAndDest, SizeType size)
{
	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> dataRegister;

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		dataRegister.load (&dataAndDest[i]);
		dataRegister = mipp::sqrt (dataRegister);
		dataRegister.store (&dataAndDest[i]);
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
		dataAndDest[i] = static_cast<DataType> (std::sqrt (dataAndDest[i]));
}

template <Scalar DataType, Integral SizeType>
void squareRootAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> dataRegister;

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		dataRegister.load (&data[i]);
		dataRegister = mipp::sqrt (dataRegister);
		dataRegister.store (&dest[i]);
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
		dest[i] = static_cast<DataType> (std::sqrt (data[i]));
}


/*---------------------------------------------------------------------------------------------------------------------------*/


template <Scalar DataType, Integral SizeType>
void reverse (DataType* const dataAndDest, SizeType size)
{
	fb::reverse (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void reverseAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	copy (dest, data, size);
	reverse (dest, size);
}

template <Scalar DataType, Integral SizeType>
void sort (DataType* const dataAndDest, SizeType size)
{
	fb::sort (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void sortAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	copy (dest, data, size);
	sort (dest, size);
}

template <Scalar DataType, Integral SizeType>
void sortReverse (DataType* const dataAndDest, SizeType size)
{
	sort (dataAndDest, size);
	reverse (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void sortReverseAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	sortAndCopy (dest, data, size);
	reverse (dest, size);
}

template <Scalar DataType, Integral SizeType1, Integral SizeType2>
void interleave (DataType* const output, const DataType* const * const origData, SizeType1 numChannels, SizeType2 numSamples)
{
	// TO DO: MIPP does provide interleave/deinterleave operations, but it's a bit complex to wrap my head around how to implement it properly...
	fb::interleave (output, origData, numChannels, numSamples);
}

template <Scalar DataType, Integral SizeType1, Integral SizeType2>
void deinterleave (DataType* const * const output, const DataType* const interleavedData, SizeType1 numChannels, SizeType2 numSamples)
{
	fb::deinterleave (output, interleavedData, numChannels, numSamples);
}


/*---------------------------------------------------------------------------------------------------------------------------*/


template <Scalar DataType, Integral SizeType>
void abs (DataType* const dataAndDest, SizeType size)
{
	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> dataRegister;

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		dataRegister.load (&dataAndDest[i]);
		dataRegister = mipp::abs (dataRegister);
		dataRegister.store (&dataAndDest[i]);
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
		dataAndDest[i] = std::abs (dataAndDest[i]);
}

template <Scalar DataType, Integral SizeType>
void absAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> dataRegister;

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		dataRegister.load (&data[i]);
		dataRegister = mipp::abs (dataRegister);
		dataRegister.store (&dest[i]);
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
		dest[i] = std::abs (data[i]);
}


template <Scalar DataType, Integral SizeType>
void negate (DataType* const dataAndDest, SizeType size)
{
	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> dataRegister;

	const auto negationMask = mipp::set0<mipp::Msk<mipp::N<DataType>>>();

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		dataRegister.load (&dataAndDest[i]);

		mipp::neg (dataRegister, negationMask);

		dataRegister.store (&dataAndDest[i]);
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
		dataAndDest[i] = -dataAndDest[i];
}

template <Scalar DataType, Integral SizeType>
void negateAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> dataRegister;

	const auto negationMask = mipp::set0<mipp::Msk<mipp::N<DataType>>>();

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		dataRegister.load (&data[i]);

		mipp::neg (dataRegister, negationMask);

		dataRegister.store (&dest[i]);
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
		dest[i] = -data[i];
}


template <Scalar DataType, Integral SizeType>
void clip (DataType* const dataAndDest, SizeType size, DataType lowClip, DataType hiClip)
{
	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> dataRegister;

	const auto lowRegister	= mipp::set1<DataType> (lowClip);
	const auto highRegister = mipp::set1<DataType> (hiClip);

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		dataRegister.load (&dataAndDest[i]);
		dataRegister = mipp::max (dataRegister, highRegister);
		dataRegister = mipp::min (dataRegister, lowRegister);
		dataRegister.store (&dataAndDest[i]);
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
	{
		const auto curr = dataAndDest[i];

		dataAndDest[i] = std::max (curr, lowClip);
		dataAndDest[i] = std::min (curr, hiClip);
	}
}

template <Scalar DataType, Integral SizeType>
void clipAndCopy (DataType* const dest, const DataType* const data, SizeType size, DataType lowClip, DataType hiClip)
{
	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> dataRegister;

	const auto lowRegister	= mipp::set1<DataType> (lowClip);
	const auto highRegister = mipp::set1<DataType> (hiClip);

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		dataRegister.load (&data[i]);
		dataRegister = mipp::max (dataRegister, highRegister);
		dataRegister = mipp::min (dataRegister, lowRegister);
		dataRegister.store (&dest[i]);
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
	{
		const auto curr = data[i];

		dest[i] = std::max (curr, lowClip);
		dest[i] = std::min (curr, hiClip);
	}
}


template <Scalar DataType, Integral SizeType>
DataType max (const DataType* const data, SizeType size)
{
	DataType result { 0 };

	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> dataRegister;

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		dataRegister.load (&data[i]);

		result = std::max (result, mipp::hmax (dataRegister));
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
		result = std::max (result, data[i]);

	return result;
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
void max (const DataType* const data, SizeType size, DataType& maxValue, IndexType& maxIndex)
{
	fb::max (data, size, maxValue, maxIndex);
}

template <Scalar DataType, Integral SizeType>
DataType maxAbs (const DataType* const data, SizeType size)
{
	DataType result { 0 };

	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> dataRegister;

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		dataRegister.load (&data[i]);
		dataRegister = mipp::abs (dataRegister);

		result = std::max (result, mipp::hmax (dataRegister));
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
		result = std::max (result, std::abs (data[i]));

	return result;
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
void maxAbs (const DataType* const data, SizeType size, DataType& maxValue, IndexType& maxIndex)
{
	fb::maxAbs (data, size, maxValue, maxIndex);
}


template <Scalar DataType, Integral SizeType>
DataType min (const DataType* const data, SizeType size)
{
	DataType result { std::numeric_limits<DataType>::max() };

	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> dataRegister;

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		dataRegister.load (&data[i]);

		result = std::min (result, mipp::hmin (dataRegister));
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
		result = std::min (result, data[i]);

	return result;
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
void min (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex)
{
	fb::min (data, size, minValue, minIndex);
}

template <Scalar DataType, Integral SizeType>
DataType minAbs (const DataType* const data, SizeType size)
{
	DataType result { std::numeric_limits<DataType>::max() };

	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> dataRegister;

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		dataRegister.load (&data[i]);
		dataRegister = mipp::abs (dataRegister);

		result = std::min (result, mipp::hmin (dataRegister));
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
		result = std::min (result, std::abs (data[i]));

	return result;
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
void minAbs (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex)
{
	fb::maxAbs (data, size, minValue, minIndex);
}


template <Scalar DataType, Integral SizeType>
void minMax (const DataType* const data, SizeType size, DataType& minValue, DataType& maxValue)
{
	maxValue = 0;
	minValue = std::numeric_limits<DataType>::max();

	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> dataRegister;

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		dataRegister.load (&data[i]);

		maxValue = std::max (maxValue, mipp::hmax (dataRegister));
		minValue = std::min (minValue, mipp::hmin (dataRegister));
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
	{
		const auto curr = data[i];

		maxValue = std::max (maxValue, curr);
		minValue = std::min (minValue, curr);
	}
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
void minMax (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex, DataType& maxValue, IndexType& maxIndex)
{
	fb::minMax (data, size, minValue, minIndex, maxValue, maxIndex);
}

template <Scalar DataType, Integral SizeType>
void minMaxAbs (const DataType* const data, SizeType size, DataType& minValue, DataType& maxValue)
{
	maxValue = 0;
	minValue = std::numeric_limits<DataType>::max();

	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> dataRegister;

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		dataRegister.load (&data[i]);
		dataRegister = mipp::abs (dataRegister);

		maxValue = std::max (maxValue, mipp::hmax (dataRegister));
		minValue = std::min (minValue, mipp::hmin (dataRegister));
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
	{
		const auto curr = std::abs (data[i]);

		maxValue = std::max (maxValue, curr);
		minValue = std::min (minValue, curr);
	}
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
void minMaxAbs (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex, DataType& maxValue, IndexType& maxIndex)
{
	fb::minMaxAbs (data, size, minValue, minIndex, maxValue, maxIndex);
}


template <Scalar DataType, Integral SizeType>
DataType range (const DataType* const data, SizeType size)
{
	DataType minValue, maxValue;

	minMax (data, size, minValue, maxValue);

	return maxValue - minValue;
}

template <Scalar DataType, Integral SizeType>
DataType rangeAbs (const DataType* const data, SizeType size)
{
	DataType minValue, maxValue;

	minMaxAbs (data, size, minValue, maxValue);

	return maxValue - minValue;
}


template <Scalar DataType, Integral SizeType>
DataType sum (const DataType* const data, SizeType size)
{
	DataType result { 0 };

	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> dataRegister;

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		dataRegister.load (&data[i]);

		result += mipp::sum (dataRegister);
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
		result += data[i];

	return result;
}

template <Scalar DataType, Integral SizeType>
DataType mean (const DataType* const data, SizeType size)
{
	DataType result { 0 };

	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> dataRegister;

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		dataRegister.load (&data[i]);

		result *= mipp::hmul (dataRegister);
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
		result *= data[i];

	return result / static_cast<DataType> (size);
}


/*---------------------------------------------------------------------------------------------------------------------------*/


template <Scalar DataType, Integral SizeType1, Integral SizeType2>
void mix (DataType* const output, const DataType* const * const origData, SizeType1 numChannels, SizeType2 numSamples)
{
	const auto multiplier = DataType (1.0) / static_cast<DataType> (numChannels);

	const auto vecLoopSize = detail::getVecLoopSize<DataType> (numSamples);

	mipp::Reg<DataType> dataRegister, channelRegister;

	const auto zeroReg		 = mipp::set1<DataType> (0);
	const auto multiplierReg = mipp::set1<DataType> (multiplier);

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		zeroReg.store (&output[i]);

		for (int c = 0; c < static_cast<int> (numChannels); ++c)
		{
			dataRegister.load (&output[i]);
			channelRegister.load (&origData[c][i]);

			dataRegister += channelRegister;

			dataRegister.store (&output[i]);
		}

		// dataRegister.load (&output[i]);
		dataRegister *= multiplierReg;
		dataRegister.store (&output[i]);
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (numSamples); ++i)
	{
		output[i] = 0;

		for (int c = 0; c < static_cast<int> (numChannels); ++c)
			output[i] += origData[c][i];

		output[i] *= multiplier;
	}
}

template <Scalar DataType, Integral SizeType>
DataType rms (const DataType* const data, SizeType size)
{
	if (size == 0) return 0;

	DataType result { 0 };

	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> dataRegister;

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		dataRegister.load (&data[i]);
		dataRegister = dataRegister * dataRegister;
		result += (mipp::sum (dataRegister));
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
		result += (data[i] * data[i]);

	result /= static_cast<DataType> (size);

	return std::sqrt (result);
}

template <Scalar DataType, Integral SizeType>
int countZeroCrossings (const DataType* const data, SizeType size)
{
	return fb::countZeroCrossings (data, size);
}


/*---------------------------------------------------------------------------------------------------------------------------*/


template <Scalar DataType, Integral SizeType>
void generateRamp (DataType* const output, SizeType size, DataType startValue, DataType endValue)
{
	fb::generateRamp (output, size, startValue, endValue);
}

template <Scalar DataType, Integral SizeType>
void applyRamp (DataType* const dataAndDest, SizeType size, DataType startValue, DataType endValue)
{
	fb::applyRamp (dataAndDest, size, startValue, endValue);
}

template <Scalar DataType, Integral SizeType>
void applyRampAndCopy (DataType* const dest, const DataType* const data, SizeType size, DataType startValue, DataType endValue)
{
	generateRamp (dest, size, startValue, endValue);
	multiply (dest, size, data);
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

}  // namespace detail

template <Scalar DataType, Integral SizeType>
void generateBlackman (DataType* const output, SizeType size)
{
	const auto vecLoopSize = vecops::detail::getVecLoopSize<DataType> (size);

	const auto pi_register		= mipp::set1<DataType> (constants::pi<DataType>);
	const auto size_register	= mipp::set1<DataType> (static_cast<DataType> (size - 1));
	const auto order_2_register = mipp::set1<DataType> (DataType (2));
	const auto order_4_register = mipp::set1<DataType> (DataType (4));
	const auto pnt_5_register	= mipp::set1<DataType> (DataType (0.5));
	const auto alpha_register	= mipp::set1<DataType> (constants::blackman_alpha<DataType>);

	const auto one_minus_alpha_register = mipp::set1<DataType> (DataType (1) - constants::blackman_alpha<DataType>);

	mipp::Reg<DataType> i_register;

	auto reg_ncos = [&i_register, &pi_register, &size_register] (const mipp::Reg<DataType>& order)
	{
		return mipp::cos (order * i_register) * pi_register / size_register;
	};

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		i_register = mipp::set1<DataType> (static_cast<DataType> (i));

		const auto cos2 = reg_ncos (order_2_register);
		const auto cos4 = reg_ncos (order_4_register);

		i_register = pnt_5_register * one_minus_alpha_register - pnt_5_register * cos2 + pnt_5_register * alpha_register * cos4;

		i_register.store (&output[i]);
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
	{
		const auto cos2 = detail::ncos<DataType> (SizeType (2), i, size);
		const auto cos4 = detail::ncos<DataType> (SizeType (4), i, size);

		output[i] = static_cast<DataType> (0.5 * (1. - constants::blackman_alpha<DataType>) -0.5 * cos2 + 0.5 * constants::blackman_alpha<DataType> * cos4);
	}
}

template <Scalar DataType, Integral SizeType>
void applyBlackman (DataType* const dataAndDest, SizeType size)
{
	fb::window::applyBlackman (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void applyBlackmanAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	generateBlackman (dest, size);
	multiply (dest, size, data);
}

template <Scalar DataType, Integral SizeType>
void generateHamm (DataType* const output, SizeType size)
{
	const auto vecLoopSize = vecops::detail::getVecLoopSize<DataType> (size);

	const auto pnt_54_register = mipp::set1<DataType> (DataType (0.54));
	const auto pnt_46_register = mipp::set1<DataType> (DataType (0.46));
	const auto order_register  = mipp::set1<DataType> (DataType (2));
	const auto pi_register	   = mipp::set1<DataType> (constants::pi<DataType>);
	const auto size_register   = mipp::set1<DataType> (static_cast<DataType> (size - 1));

	mipp::Reg<DataType> i_register;

	auto reg_ncos = [&order_register, &i_register, &pi_register, &size_register]
	{
		return mipp::cos (order_register * i_register) * pi_register / size_register;
	};

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		i_register = mipp::set1<DataType> (static_cast<DataType> (i));

		i_register = reg_ncos();  // cppcheck-suppress redundantAssignment

		i_register = pnt_54_register - pnt_46_register * i_register;

		i_register.store (&output[i]);
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
		output[i] = static_cast<DataType> (0.54 - 0.46 * detail::ncos<DataType> (SizeType (2), i, size));
}

template <Scalar DataType, Integral SizeType>
void applyHamm (DataType* const dataAndDest, SizeType size)
{
	fb::window::applyHamm (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void applyHammAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	generateHamm (dest, size);
	multiply (dest, size, data);
}

template <Scalar DataType, Integral SizeType>
void generateHanning (DataType* const output, SizeType size)
{
	const auto vecLoopSize = vecops::detail::getVecLoopSize<DataType> (size);

	const auto order_register = mipp::set1<DataType> (DataType (2));
	const auto pi_register	  = mipp::set1<DataType> (constants::pi<DataType>);
	const auto size_register  = mipp::set1<DataType> (static_cast<DataType> (size - 1));
	const auto pnt_5_register = mipp::set1<DataType> (0.5);

	mipp::Reg<DataType> i_register;

	auto reg_ncos = [&order_register, &i_register, &pi_register, &size_register]
	{
		return mipp::cos (order_register * i_register) * pi_register / size_register;
	};

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		i_register = mipp::set1<DataType> (static_cast<DataType> (i));
		i_register = pnt_5_register - pnt_5_register * reg_ncos();	// cppcheck-suppress redundantAssignment
		i_register.store (&output[i]);
	}

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
		output[i] = static_cast<DataType> (0.5 - 0.5 * detail::ncos<DataType> (SizeType (2), i, size));
}

template <Scalar DataType, Integral SizeType>
void applyHanning (DataType* const dataAndDest, SizeType size)
{
	fb::window::applyHanning (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void applyHanningAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	generateHanning (dest, size);
	multiply (dest, size, data);
}

}  // namespace window

}  // namespace limes::vecops
