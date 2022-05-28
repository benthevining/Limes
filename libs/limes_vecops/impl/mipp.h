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

#include <limes_core.h>

/** @file
	This file contains implementations of the vecops functions for the MIPP library.
	@ingroup limes_vecops
	@see limes_vecops.h
 */

LIMES_DISABLE_ALL_COMPILER_WARNINGS

#include <mipp.h>

LIMES_REENABLE_ALL_COMPILER_WARNINGS

#include <limits>
#include <cmath>
#include <limes_vecops.h>
#include "fallback_impl.h"
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace vecops
{

/// @cond

static_assert (isUsingMIPP());

namespace detail
{

template <Scalar DataType, Integral SizeType>
LIMES_NO_EXPORT [[nodiscard]] LIMES_FORCE_INLINE constexpr int getVecLoopSize (SizeType size)
{
	constexpr const auto n = mipp::N<DataType>();

	return static_cast<int> ((static_cast<decltype (n)> (size) / n) * n);
}


template <typename DataType, typename VecorizedOp, typename ScalarOp, Integral SizeType>
LIMES_NO_EXPORT LIMES_FORCE_INLINE void perform (SizeType size, VecorizedOp&& vectorOp, ScalarOp&& scalarOp)
{
	const auto vecLoopSize = getVecLoopSize<DataType> (size);

	if (size >= mipp::N<DataType>())
		for (auto i = decltype (vecLoopSize) (0); i < vecLoopSize; i += mipp::N<DataType>())
			vectorOp (i);

	for (auto i = vecLoopSize; i < static_cast<decltype (i)> (size); ++i)
		scalarOp (i);
}

}  // namespace detail

#pragma mark Basic functions

template <Scalar DataType, Integral SizeType>
void fill (DataType* const data, SizeType size, DataType constantToFill)
{
	const auto constant = mipp::set1<DataType> (constantToFill);

	detail::perform<DataType> (
		size,
		[data, &constant] (auto i)
		{ mipp::store (data + i, constant); },
		[data, constantToFill] (auto i)
		{ data[i] = constantToFill; });
}

template <Scalar DataType, Integral SizeType>
void clear (DataType* const data, SizeType size)
{
	fb::clear (data, size);
}

template <Scalar DataType, Integral SizeType>
void copy (DataType* const dest, const DataType* const source, SizeType size)
{
	mipp::Reg<DataType> data;

	detail::perform<DataType> (
		size,
		[&data, dest, source] (auto i)
		{ data.load (&source[i]);
		data.store (&dest[i]); },
		[dest, source] (auto i)
		{ dest[i] = source[i]; });
}

template <Scalar DataType, Integral SizeType>
void swap (DataType* const vecA, DataType* const vecB, SizeType size)
{
	mipp::Reg<DataType> dataA, dataB;

	const auto vecOp = [&dataA, &dataB, vecA, vecB] (auto i)
	{
		dataA.load (&vecA[i]);
		dataB.load (&vecB[i]);

		dataA.store (&vecB[i]);
		dataB.store (&vecA[i]);
	};

	const auto scalarOp = [vecA, vecB] (auto i)
	{
		const auto elem_a = vecA[i];

		vecA[i] = vecB[i];
		vecB[i] = elem_a;
	};

	detail::perform<DataType> (size,
							   std::move (vecOp),
							   std::move (scalarOp));
}


/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark Arithmetic functions

/*-----  ADDITION  -----*/

template <Scalar DataType, Integral SizeType>
void add (DataType* const data, SizeType size, DataType constantToAdd)
{
	const auto constant = mipp::set1<DataType> (constantToAdd);

	mipp::Reg<DataType> dataRegister;

	const auto vecOp = [&dataRegister, &constant, data] (auto i)
	{
		dataRegister.load (&data[i]);
		dataRegister += constant;
		dataRegister.store (&data[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   [data, constantToAdd] (auto i)
							   { data[i] += constantToAdd; });
}

template <Scalar DataType, Integral SizeType>
void add (DataType* const dataAndDest, SizeType size, const DataType* const dataToAdd)
{
	mipp::Reg<DataType> dataInOut, dataAdding;

	const auto vecOp = [&dataInOut, &dataAdding, dataAndDest, &dataToAdd] (auto i)
	{
		dataInOut.load (&dataAndDest[i]);
		dataAdding.load (&dataToAdd[i]);

		dataInOut += dataAdding;

		dataInOut.store (&dataAndDest[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   [dataAndDest, dataToAdd] (auto i)
							   { dataAndDest[i] += dataToAdd[i]; });
}

template <Scalar DataType, Integral SizeType>
void addAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToAdd)
{
	mipp::Reg<DataType> dataRegister;

	const auto constant = mipp::set1<DataType> (constantToAdd);

	const auto vecOp = [&dataRegister, &constant, origData, dest] (auto i)
	{
		dataRegister.load (&origData[i]);
		dataRegister += constant;
		dataRegister.store (&dest[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   [dest, origData, constantToAdd] (auto i)
							   { dest[i] = origData[i] + constantToAdd; });
}

template <Scalar DataType, Integral SizeType>
void addAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToAdd)
{
	mipp::Reg<DataType> dataIn, dataAdding;

	const auto vecOp = [&dataIn, &dataAdding, origData, dataToAdd, dest] (auto i)
	{
		dataIn.load (&origData[i]);
		dataAdding.load (&dataToAdd[i]);

		dataIn += dataAdding;

		dataIn.store (&dest[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   [dest, origData, dataToAdd] (auto i)
							   { dest[i] = origData[i] + dataToAdd[i]; });
}


/*-----  SUBTRACTION  -----*/

template <Scalar DataType, Integral SizeType>
void subtract (DataType* const data, SizeType size, DataType constantToSubtract)
{
	mipp::Reg<DataType> dataRegister;

	const auto constant = mipp::set1<DataType> (constantToSubtract);

	const auto vecOp = [&dataRegister, &constant, data] (auto i)
	{
		dataRegister.load (&data[i]);
		dataRegister -= constant;
		dataRegister.store (&data[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   [data, constantToSubtract] (auto i)
							   { data[i] -= constantToSubtract; });
}

template <Scalar DataType, Integral SizeType>
void subtract (DataType* const dataAndDest, SizeType size, const DataType* const dataToSubtract)
{
	mipp::Reg<DataType> dataInOut, dataAdding;

	const auto vecOp = [&dataInOut, &dataAdding, dataAndDest, dataToSubtract] (auto i)
	{
		dataInOut.load (&dataAndDest[i]);
		dataAdding.load (&dataToSubtract[i]);

		dataInOut -= dataAdding;

		dataInOut.store (&dataAndDest[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   [dataAndDest, dataToSubtract] (auto i)
							   { dataAndDest[i] -= dataToSubtract[i]; });
}

template <Scalar DataType, Integral SizeType>
void subtractAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToSubtract)
{
	mipp::Reg<DataType> dataRegister;

	const auto constant = mipp::set1<DataType> (constantToSubtract);

	const auto vecOp = [&dataRegister, &constant, origData, dest] (auto i)
	{
		dataRegister.load (&origData[i]);
		dataRegister -= constant;
		dataRegister.store (&dest[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   [dest, origData, constantToSubtract] (auto i)
							   { dest[i] = origData[i] - constantToSubtract; });
}

template <Scalar DataType, Integral SizeType>
void subtractAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToSubtract)
{
	mipp::Reg<DataType> dataIn, dataAdding;

	const auto vecOp = [&dataIn, &dataAdding, origData, dest, dataToSubtract] (auto i)
	{
		dataIn.load (&origData[i]);
		dataAdding.load (&dataToSubtract[i]);

		dataIn -= dataAdding;

		dataIn.store (&dest[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   [dest, origData, dataToSubtract] (auto i)
							   { dest[i] = origData[i] - dataToSubtract[i]; });
}

template <Scalar DataType, Integral SizeType>
void subtractInv (DataType* const data, SizeType size, DataType constantToSubtractFrom)
{
	mipp::Reg<DataType> dataRegister;

	const auto constant = mipp::set1<DataType> (constantToSubtractFrom);

	const auto vecOp = [&dataRegister, &constant, data] (auto i)
	{
		dataRegister.load (&data[i]);
		dataRegister = constant - dataRegister;
		dataRegister.store (&data[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   [data, constantToSubtractFrom] (auto i)
							   { data[i] = constantToSubtractFrom - data[i]; });
}

template <Scalar DataType, Integral SizeType>
void subtractInvAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToSubtractFrom)
{
	mipp::Reg<DataType> dataRegister;

	const auto constant = mipp::set1<DataType> (constantToSubtractFrom);

	const auto vecOp = [&dataRegister, &constant, origData, dest] (auto i)
	{
		dataRegister.load (&origData[i]);
		dataRegister = constant - dataRegister;
		dataRegister.store (&dest[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   [dest, origData, constantToSubtractFrom] (auto i)
							   { dest[i] = constantToSubtractFrom - origData[i]; });
}


/*-----  MULTIPLICATION  -----*/

template <Scalar DataType, Integral SizeType>
void multiply (DataType* const data, SizeType size, DataType constantToMultiply)
{
	mipp::Reg<DataType> dataRegister;

	const auto constant = mipp::set1<DataType> (constantToMultiply);

	const auto vecOp = [&dataRegister, &constant, data] (auto i)
	{
		dataRegister.load (&data[i]);
		dataRegister *= constant;
		dataRegister.store (&data[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   [data, constantToMultiply] (auto i)
							   { data[i] *= constantToMultiply; });
}

template <Scalar DataType, Integral SizeType>
void multiply (DataType* const dataAndDest, SizeType size, const DataType* const dataToMultiply)
{
	mipp::Reg<DataType> dataInOut, dataAdding;

	const auto vecOp = [&dataInOut, &dataAdding, dataAndDest, dataToMultiply] (auto i)
	{
		dataInOut.load (&dataAndDest[i]);
		dataAdding.load (&dataToMultiply[i]);

		dataInOut *= dataAdding;

		dataInOut.store (&dataAndDest[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   [dataAndDest, dataToMultiply] (auto i)
							   { dataAndDest[i] *= dataToMultiply[i]; });
}

template <Scalar DataType, Integral SizeType>
void multiplyAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToMultiply)
{
	mipp::Reg<DataType> dataRegister;

	const auto constant = mipp::set1<DataType> (constantToMultiply);

	const auto vecOp = [&dataRegister, &constant, origData, dest] (auto i)
	{
		dataRegister.load (&origData[i]);
		dataRegister *= constant;
		dataRegister.store (&dest[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   [dest, origData, constantToMultiply] (auto i)
							   { dest[i] = origData[i] * constantToMultiply; });
}

template <Scalar DataType, Integral SizeType>
void multiplyAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToMultiply)
{
	mipp::Reg<DataType> dataIn, dataAdding;

	const auto vecOp = [&dataIn, &dataAdding, origData, dataToMultiply, dest] (auto i)
	{
		dataIn.load (&origData[i]);
		dataAdding.load (&dataToMultiply[i]);

		dataIn *= dataAdding;

		dataIn.store (&dest[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   [dest, origData, dataToMultiply] (auto i)
							   { dest[i] = origData[i] * dataToMultiply[i]; });
}

template <Scalar DataType, Integral SizeType>
DataType dotProduct (const DataType* const vecA, const DataType* const vecB, SizeType size)
{
	DataType dotProd { 0 };

	mipp::Reg<DataType> dataA, dataB;

	const auto vecOp = [&dotProd, &dataA, &dataB, vecA, vecB] (auto i)
	{
		dataA.load (&vecA[i]);
		dataB.load (&vecB[i]);

		dataA *= dataB;

		dotProd += mipp::sum (dataA);
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   [&dotProd, vecA, vecB] (auto i)
							   { dotProd += (vecA[i] * vecB[i]); });

	return dotProd;
}


/*-----  DIVISION  -----*/

template <Scalar DataType, Integral SizeType>
void divide (DataType* const data, SizeType size, DataType constantToDivide)
{
	mipp::Reg<DataType> dataRegister;

	const auto constant = mipp::set1<DataType> (constantToDivide);

	const auto vecOp = [&dataRegister, &constant, data] (auto i)
	{
		dataRegister.load (&data[i]);
		dataRegister /= constant;
		dataRegister.store (&data[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   [data, constantToDivide] (auto i)
							   { data[i] /= constantToDivide; });
}

template <Scalar DataType, Integral SizeType>
void divide (DataType* const dataAndDest, SizeType size, const DataType* const dataToDivide)
{
	mipp::Reg<DataType> dataInOut, dataAdding;

	const auto vecOp = [&dataInOut, &dataAdding, dataAndDest, dataToDivide] (auto i)
	{
		dataInOut.load (&dataAndDest[i]);
		dataAdding.load (&dataToDivide[i]);

		dataInOut /= dataAdding;

		dataInOut.store (&dataAndDest[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   [dataAndDest, dataToDivide] (auto i)
							   { dataAndDest[i] /= dataToDivide[i]; });
}

template <Scalar DataType, Integral SizeType>
void divideAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToDivide)
{
	mipp::Reg<DataType> dataRegister;

	const auto constant = mipp::set1<DataType> (constantToDivide);

	const auto vecOp = [&dataRegister, &constant, origData, dest] (auto i)
	{
		dataRegister.load (&origData[i]);
		dataRegister /= constant;
		dataRegister.store (&dest[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   [dest, origData, constantToDivide] (auto i)
							   { dest[i] = origData[i] / constantToDivide; });
}

template <Scalar DataType, Integral SizeType>
void divideAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToDivide)
{
	mipp::Reg<DataType> dataIn, dataAdding;

	const auto vecOp = [&dataIn, &dataAdding, origData, dataToDivide, dest] (auto i)
	{
		dataIn.load (&origData[i]);
		dataAdding.load (&dataToDivide[i]);

		dataIn /= dataAdding;

		dataIn.store (&dest[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   [dest, origData, dataToDivide] (auto i)
							   { dest[i] = origData[i] / dataToDivide[i]; });
}

template <Scalar DataType, Integral SizeType>
void divideInv (DataType* const data, SizeType size, DataType constantToDivideFrom)
{
	mipp::Reg<DataType> dataRegister;

	const auto constant = mipp::set1<DataType> (constantToDivideFrom);

	const auto vecOp = [&dataRegister, &constant, data] (auto i)
	{
		dataRegister.load (&data[i]);
		dataRegister = constant / dataRegister;
		dataRegister.store (&data[i]);
	};

	const auto scalarOp = [data, constantToDivideFrom] (auto i)
	{
		const auto thisData = data[i];

		if (thisData == DataType (0))
			data[i] = DataType (0);
		else
			data[i] = constantToDivideFrom / thisData;
	};

	detail::perform<DataType> (size, std::move (vecOp), std::move (scalarOp));
}

template <Scalar DataType, Integral SizeType>
void divideInvAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToDivideFrom)
{
	mipp::Reg<DataType> dataRegister;

	const auto constant = mipp::set1<DataType> (constantToDivideFrom);

	const auto vecOp = [&dataRegister, &constant, origData, dest] (auto i)
	{
		dataRegister.load (&origData[i]);
		dataRegister = constant / dataRegister;
		dataRegister.store (&dest[i]);
	};

	const auto scalarOp = [origData, dest, constantToDivideFrom] (auto i)
	{
		const auto thisData = origData[i];

		if (thisData == DataType (0))
			dest[i] = DataType (0);
		else
			dest[i] = constantToDivideFrom / thisData;
	};

	detail::perform<DataType> (size, std::move (vecOp), std::move (scalarOp));
}


/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark Squaring functions

template <Scalar DataType, Integral SizeType>
void square (DataType* const dataAndDest, SizeType size)
{
	mipp::Reg<DataType> data;

	const auto vecOp = [&data, dataAndDest] (auto i)
	{
		data.load (&dataAndDest[i]);
		data *= data;
		data.store (&dataAndDest[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   [dataAndDest] (auto i)
							   { dataAndDest[i] *= dataAndDest[i]; });
}

template <Scalar DataType, Integral SizeType>
void squareAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	mipp::Reg<DataType> dataRegister;

	const auto vecOp = [&dataRegister, data, dest] (auto i)
	{
		dataRegister.load (&data[i]);
		dataRegister *= dataRegister;
		dataRegister.store (&dest[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   [dest, data] (auto i)
							   { dest[i] *= data[i]; });
}

template <Scalar DataType, Integral SizeType>
void squareRoot (DataType* const dataAndDest, SizeType size)
{
	mipp::Reg<DataType> dataRegister;

	const auto vecOp = [&dataRegister, dataAndDest] (auto i)
	{
		dataRegister.load (&dataAndDest[i]);
		dataRegister = mipp::sqrt (dataRegister);
		dataRegister.store (&dataAndDest[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   [dataAndDest] (auto i)
							   { dataAndDest[i] = static_cast<DataType> (std::sqrt (dataAndDest[i])); });
}

template <Scalar DataType, Integral SizeType>
void squareRootAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	mipp::Reg<DataType> dataRegister;

	const auto vecOp = [&dataRegister, data, dest] (auto i)
	{
		dataRegister.load (&data[i]);
		dataRegister = mipp::sqrt (dataRegister);
		dataRegister.store (&dest[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   [data, dest] (auto i)
							   { dest[i] = static_cast<DataType> (std::sqrt (data[i])); });
}

template <Scalar DataType, Integral SizeType>
void invSquareRoot (DataType* const dataAndDest, SizeType size)
{
	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> dataRegister;

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		dataRegister.load (&dataAndDest[i]);
		dataRegister = mipp::rsqrt (dataRegister);
		dataRegister.store (&dataAndDest[i]);
	}

	const auto numLeft = size - vecLoopSize;

	if (numLeft > 0)
		fb::invSquareRoot (dataAndDest + vecLoopSize, numLeft);
}

template <Scalar DataType, Integral SizeType>
void invSquareRootAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	const auto vecLoopSize = detail::getVecLoopSize<DataType> (size);

	mipp::Reg<DataType> dataRegister;

	for (auto i = 0; i < vecLoopSize; i += mipp::N<DataType>())
	{
		dataRegister.load (&data[i]);
		dataRegister = mipp::rsqrt (dataRegister);
		dataRegister.store (&dest[i]);
	}

	const auto numLeft = size - vecLoopSize;

	if (numLeft > 0)
		fb::invSquareRootAndCopy (dest + vecLoopSize, data + vecLoopSize, numLeft);
}


/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark Sorting and ordering functions

template <Scalar DataType, Integral SizeType>
void reverse (DataType* const dataAndDest, SizeType size)
{
	fb::reverse (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void reverseAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	fb::reverseAndCopy (dest, data, size);
}

template <Scalar DataType, Integral SizeType>
void sort (DataType* const dataAndDest, SizeType size)
{
	fb::sort (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void sortAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	fb::sortAndCopy (dest, data, size);
}

template <Scalar DataType, Integral SizeType>
void sortReverse (DataType* const dataAndDest, SizeType size)
{
	fb::sortReverse (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void sortReverseAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	fb::sortReverseAndCopy (dest, data, size);
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

#pragma mark Statistical functions

template <Scalar DataType, Integral SizeType>
void abs (DataType* const dataAndDest, SizeType size)
{
	mipp::Reg<DataType> dataRegister;

	const auto vecOp = [&dataRegister, dataAndDest] (auto i)
	{
		dataRegister.load (&dataAndDest[i]);
		dataRegister = mipp::abs (dataRegister);
		dataRegister.store (&dataAndDest[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   [dataAndDest] (auto i)
							   { dataAndDest[i] = std::abs (dataAndDest[i]); });
}

template <Scalar DataType, Integral SizeType>
void absAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	mipp::Reg<DataType> dataRegister;

	const auto vecOp = [&dataRegister, data, dest] (auto i)
	{
		dataRegister.load (&data[i]);
		dataRegister = mipp::abs (dataRegister);
		dataRegister.store (&dest[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   [dest, data] (auto i)
							   { dest[i] = std::abs (data[i]); });
}


template <Scalar DataType, Integral SizeType>
void negate (DataType* const dataAndDest, SizeType size)
{
	mipp::Reg<DataType> dataRegister;

	const auto negationMask = mipp::set0<mipp::Msk<mipp::N<DataType>>>();

	const auto vecOp = [&dataRegister, &negationMask, dataAndDest] (auto i)
	{
		dataRegister.load (&dataAndDest[i]);

		mipp::neg (dataRegister, negationMask);

		dataRegister.store (&dataAndDest[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   [dataAndDest] (auto i)
							   { dataAndDest[i] = -dataAndDest[i]; });
}

template <Scalar DataType, Integral SizeType>
void negateAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	mipp::Reg<DataType> dataRegister;

	const auto negationMask = mipp::set0<mipp::Msk<mipp::N<DataType>>>();

	const auto vecOp = [&dataRegister, &negationMask, data, dest] (auto i)
	{
		dataRegister.load (&data[i]);

		mipp::neg (dataRegister, negationMask);

		dataRegister.store (&dest[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   [dest, data] (auto i)
							   { dest[i] = -data[i]; });
}


template <Scalar DataType, Integral SizeType>
void clip (DataType* const dataAndDest, SizeType size, DataType lowClip, DataType hiClip)
{
	mipp::Reg<DataType> dataRegister;

	const auto lowRegister	= mipp::set1<DataType> (lowClip);
	const auto highRegister = mipp::set1<DataType> (hiClip);

	const auto vecOp = [&dataRegister, &lowRegister, &highRegister, dataAndDest] (auto i)
	{
		dataRegister.load (&dataAndDest[i]);
		dataRegister = mipp::sat (dataRegister, lowRegister, highRegister);
		dataRegister.store (&dataAndDest[i]);
	};

	const auto scalarOp = [dataAndDest, lowClip, hiClip] (auto i)
	{
		const auto curr = dataAndDest[i];

		dataAndDest[i] = std::max (curr, lowClip);
		dataAndDest[i] = std::min (curr, hiClip);
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   std::move (scalarOp));
}

template <Scalar DataType, Integral SizeType>
void clipAndCopy (DataType* const dest, const DataType* const data, SizeType size, DataType lowClip, DataType hiClip)
{
	mipp::Reg<DataType> dataRegister;

	const auto lowRegister	= mipp::set1<DataType> (lowClip);
	const auto highRegister = mipp::set1<DataType> (hiClip);

	const auto vecOp = [&dataRegister, &lowRegister, &highRegister, data, dest] (auto i)
	{
		dataRegister.load (&data[i]);
		dataRegister = mipp::sat (dataRegister, lowRegister, highRegister);
		dataRegister.store (&dest[i]);
	};

	const auto scalarOp = [data, dest, lowClip, hiClip] (auto i)
	{
		const auto curr = data[i];

		dest[i] = std::max (curr, lowClip);
		dest[i] = std::min (curr, hiClip);
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   std::move (scalarOp));
}


template <Scalar DataType, Integral SizeType>
DataType max (const DataType* const data, SizeType size)
{
	DataType result { 0 };

	mipp::Reg<DataType> dataRegister;

	const auto vecOp = [&result, &dataRegister, data] (auto i)
	{
		dataRegister.load (&data[i]);

		result = std::max (result, mipp::hmax (dataRegister));
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   [&result, data] (auto i)
							   { result = std::max (result, data[i]); });

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

	mipp::Reg<DataType> dataRegister;

	const auto vecOp = [&result, &dataRegister, data] (auto i)
	{
		dataRegister.load (&data[i]);
		dataRegister = mipp::abs (dataRegister);

		result = std::max (result, mipp::hmax (dataRegister));
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   [&result, data] (auto i)
							   { result = std::max (result, std::abs (data[i])); });

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

	mipp::Reg<DataType> dataRegister;

	const auto vecOp = [&result, &dataRegister, data] (auto i)
	{
		dataRegister.load (&data[i]);

		result = std::min (result, mipp::hmin (dataRegister));
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   [&result, data] (auto i)
							   { result = std::min (result, data[i]); });

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

	mipp::Reg<DataType> dataRegister;

	const auto vecOp = [&result, &dataRegister, data] (auto i)
	{
		dataRegister.load (&data[i]);
		dataRegister = mipp::abs (dataRegister);

		result = std::min (result, mipp::hmin (dataRegister));
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   [&result, data] (auto i)
							   { result = std::min (result, std::abs (data[i])); });

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

	mipp::Reg<DataType> dataRegister;

	const auto vecOp = [&maxValue, &minValue, &dataRegister, data] (auto i)
	{
		dataRegister.load (&data[i]);

		maxValue = std::max (maxValue, mipp::hmax (dataRegister));
		minValue = std::min (minValue, mipp::hmin (dataRegister));
	};

	const auto scalarOp = [&maxValue, &minValue, data] (auto i)
	{
		const auto curr = data[i];

		maxValue = std::max (maxValue, curr);
		minValue = std::min (minValue, curr);
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   std::move (scalarOp));
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

	mipp::Reg<DataType> dataRegister;

	const auto vecOp = [&maxValue, &minValue, &dataRegister, data] (auto i)
	{
		dataRegister.load (&data[i]);
		dataRegister = mipp::abs (dataRegister);

		maxValue = std::max (maxValue, mipp::hmax (dataRegister));
		minValue = std::min (minValue, mipp::hmin (dataRegister));
	};

	const auto scalarOp = [&maxValue, &minValue, data] (auto i)
	{
		const auto curr = std::abs (data[i]);

		maxValue = std::max (maxValue, curr);
		minValue = std::min (minValue, curr);
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   std::move (scalarOp));
}

template <Scalar DataType, Integral SizeType>
DataType sum (const DataType* const data, SizeType size)
{
	DataType result { 0 };

	mipp::Reg<DataType> dataRegister;

	const auto vecOp = [&result, &dataRegister, data] (auto i)
	{
		dataRegister.load (&data[i]);

		result += mipp::sum (dataRegister);
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   [&result, data] (auto i)
							   { result += data[i]; });

	return result;
}

template <Scalar DataType, Integral SizeType>
DataType mean (const DataType* const data, SizeType size)
{
	DataType result { 0 };

	mipp::Reg<DataType> dataRegister;

	const auto vecOp = [&result, &dataRegister, data] (auto i)
	{
		dataRegister.load (&data[i]);

		result *= mipp::hmul (dataRegister);
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   [&result, data] (auto i)
							   { result *= data[i]; });

	return result / static_cast<DataType> (size);
}

template <Scalar DataType, Integral SizeType>
DataType standard_deviation (const DataType* const data, SizeType size)
{
	const auto meanVal = mean (data, size);

	auto sumVal = DataType { 0 };

	mipp::Reg<DataType> dataRegister;

	const auto meanRegister = mipp::set1<DataType> (meanVal);

	const auto vecOp = [data, &sumVal, &dataRegister, &meanRegister] (auto i)
	{
		dataRegister.load (&data[i]);

		dataRegister -= meanRegister;

		dataRegister *= dataRegister;

		sumVal += mipp::sum (dataRegister);
	};

	const auto scalarOp = [data, meanVal, &sumVal] (auto i)
	{
		const auto val = data[i] - meanVal;
		sumVal += (val * val);
	};

	detail::perform<DataType> (size, std::move (vecOp), std::move (scalarOp));

	return std::sqrt (sumVal / static_cast<DataType> (size));
}

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark Trigonometric functions

template <Scalar DataType, Integral SizeType>
void sinCos (const DataType* const data, SizeType size, DataType* const sinesOut, DataType* const cosinesOut)
{
	mipp::Reg<DataType> dataRegister, sinRegister, cosRegister;

	const auto vecOp = [&dataRegister, &sinRegister, &cosRegister, data, sinesOut, cosinesOut] (auto i)
	{
		dataRegister.load (&data[i]);

		mipp::sincos (dataRegister, sinRegister, cosRegister);

		sinRegister.store (&sinesOut[i]);
		cosRegister.store (&cosinesOut[i]);
	};

	const auto scalarOp = [data, sinesOut, cosinesOut] (auto i)
	{
		const auto thisData = data[i];

		sinesOut[i]	  = std::sin (thisData);
		cosinesOut[i] = std::sin (thisData);
	};

	detail::perform<DataType> (size, std::move (vecOp), std::move (scalarOp));
}

/* --- sin --- */

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void sine (DataType* const data, SizeType size)
{
	mipp::Reg<DataType> dataRegister;

	const auto vecOp = [data, &dataRegister] (auto i)
	{
		dataRegister.load (&data[i]);

		dataRegister = mipp::sin (dataRegister);

		dataRegister.store (&data[i]);
	};

	const auto scalarOp = [data] (auto i)
	{
		data[i] = std::sin (data[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp), std::move (scalarOp));
}

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void sineAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	mipp::Reg<DataType> dataRegister;

	const auto vecOp = [dest, data, &dataRegister] (auto i)
	{
		dataRegister.load (&data[i]);

		dataRegister = mipp::sin (dataRegister);

		dataRegister.store (&dest[i]);
	};

	const auto scalarOp = [dest, data] (auto i)
	{
		dest[i] = std::sin (data[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp), std::move (scalarOp));
}

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void arcsine (DataType* const data, SizeType size)
{
	mipp::Reg<DataType> dataRegister;

	const auto oneRegister = mipp::set1 (DataType (1));

	const auto vecOp = [data, &dataRegister, &oneRegister] (auto i)
	{
		dataRegister.load (&data[i]);

		dataRegister = mipp::sin (dataRegister);

		dataRegister = oneRegister / dataRegister;

		dataRegister.store (&data[i]);
	};

	const auto scalarOp = [data] (auto i)
	{
		data[i] = std::asin (data[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp), std::move (scalarOp));
}

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void arcsineAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	mipp::Reg<DataType> dataRegister;

	const auto oneRegister = mipp::set1 (DataType (1));

	const auto vecOp = [dest, data, &dataRegister, &oneRegister] (auto i)
	{
		dataRegister.load (&data[i]);

		dataRegister = mipp::sin (dataRegister);

		dataRegister = oneRegister / dataRegister;

		dataRegister.store (&dest[i]);
	};

	const auto scalarOp = [dest, data] (auto i)
	{
		dest[i] = std::asin (data[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp), std::move (scalarOp));
}

/* --- cos --- */

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void cos (DataType* const data, SizeType size)
{
	mipp::Reg<DataType> dataRegister;

	const auto vecOp = [data, &dataRegister] (auto i)
	{
		dataRegister.load (&data[i]);

		dataRegister = mipp::cos (dataRegister);

		dataRegister.store (&data[i]);
	};

	const auto scalarOp = [data] (auto i)
	{
		data[i] = std::cos (data[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp), std::move (scalarOp));
}

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void cosAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	mipp::Reg<DataType> dataRegister;

	const auto vecOp = [dest, data, &dataRegister] (auto i)
	{
		dataRegister.load (&data[i]);

		dataRegister = mipp::cos (dataRegister);

		dataRegister.store (&dest[i]);
	};

	const auto scalarOp = [dest, data] (auto i)
	{
		dest[i] = std::cos (data[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp), std::move (scalarOp));
}

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void arccos (DataType* const data, SizeType size)
{
	mipp::Reg<DataType> dataRegister;

	const auto oneRegister = mipp::set1 (DataType (1));

	const auto vecOp = [data, &dataRegister, &oneRegister] (auto i)
	{
		dataRegister.load (&data[i]);

		dataRegister = mipp::cos (dataRegister);

		dataRegister = oneRegister / dataRegister;

		dataRegister.store (&data[i]);
	};

	const auto scalarOp = [data] (auto i)
	{
		data[i] = std::acos (data[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp), std::move (scalarOp));
}

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void arccosAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	mipp::Reg<DataType> dataRegister;

	const auto oneRegister = mipp::set1 (DataType (1));

	const auto vecOp = [dest, data, &dataRegister, &oneRegister] (auto i)
	{
		dataRegister.load (&data[i]);

		dataRegister = mipp::cos (dataRegister);

		dataRegister = oneRegister / dataRegister;

		dataRegister.store (&dest[i]);
	};

	const auto scalarOp = [dest, data] (auto i)
	{
		dest[i] = std::acos (data[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp), std::move (scalarOp));
}

/* --- tan --- */

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void tan (DataType* const data, SizeType size)
{
	mipp::Reg<DataType> dataRegister;

	const auto vecOp = [data, &dataRegister] (auto i)
	{
		dataRegister.load (&data[i]);

		dataRegister = mipp::tan (dataRegister);

		dataRegister.store (&data[i]);
	};

	const auto scalarOp = [data] (auto i)
	{
		data[i] = std::tan (data[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp), std::move (scalarOp));
}

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void tanAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	mipp::Reg<DataType> dataRegister;

	const auto vecOp = [dest, data, &dataRegister] (auto i)
	{
		dataRegister.load (&data[i]);

		dataRegister = mipp::tan (dataRegister);

		dataRegister.store (&dest[i]);
	};

	const auto scalarOp = [dest, data] (auto i)
	{
		dest[i] = std::tan (data[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp), std::move (scalarOp));
}

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void arctan (DataType* const data, SizeType size)
{
	mipp::Reg<DataType> dataRegister;

	const auto oneRegister = mipp::set1 (DataType (1));

	const auto vecOp = [data, &dataRegister, &oneRegister] (auto i)
	{
		dataRegister.load (&data[i]);

		dataRegister = mipp::tan (dataRegister);

		dataRegister = oneRegister / dataRegister;

		dataRegister.store (&data[i]);
	};

	const auto scalarOp = [data] (auto i)
	{
		data[i] = std::atan (data[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp), std::move (scalarOp));
}

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void arctanAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	mipp::Reg<DataType> dataRegister;

	const auto oneRegister = mipp::set1 (DataType (1));

	const auto vecOp = [dest, data, &dataRegister, &oneRegister] (auto i)
	{
		dataRegister.load (&data[i]);

		dataRegister = mipp::tan (dataRegister);

		dataRegister = oneRegister / dataRegister;

		dataRegister.store (&dest[i]);
	};

	const auto scalarOp = [dest, data] (auto i)
	{
		dest[i] = std::atan (data[i]);
	};

	detail::perform<DataType> (size, std::move (vecOp), std::move (scalarOp));
}

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark Audio utility functions

template <Scalar DataType, Integral SizeType1, Integral SizeType2>
void mix (DataType* const output, const DataType* const * const origData, SizeType1 numChannels, SizeType2 numSamples)
{
	const auto multiplier = DataType (1.0) / static_cast<DataType> (numChannels);

	mipp::Reg<DataType> dataRegister, channelRegister;

	const auto zeroReg		 = mipp::set1<DataType> (0);
	const auto multiplierReg = mipp::set1<DataType> (multiplier);

	const auto vecOp = [&dataRegister, &channelRegister, &zeroReg, &multiplierReg, multiplier, origData, output, numChannels] (auto i)
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
	};

	const auto scalarOp = [output, origData, numChannels, multiplier] (auto i)
	{
		output[i] = 0;

		for (int c = 0; c < static_cast<int> (numChannels); ++c)
			output[i] += origData[c][i];

		output[i] *= multiplier;
	};

	detail::perform<DataType> (numSamples, std::move (vecOp), std::move (scalarOp));
}

template <Scalar DataType, Integral SizeType>
DataType rms (const DataType* const data, SizeType size)
{
	if (size == 0) return 0;

	DataType result { 0 };

	mipp::Reg<DataType> dataRegister;

	const auto vecOp = [&result, &dataRegister, data] (auto i)
	{
		dataRegister.load (&data[i]);
		dataRegister = dataRegister * dataRegister;
		result += (mipp::sum (dataRegister));
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   [&result, data] (auto i)
							   { result += (data[i] * data[i]); });

	result /= static_cast<DataType> (size);

	return std::sqrt (result);
}

template <Scalar DataType, Integral SizeType>
int countZeroCrossings (const DataType* const data, SizeType size)
{
	return fb::countZeroCrossings (data, size);
}

template <Scalar DataType, Integral SizeType>
void generateRamp (DataType* const output, SizeType size, DataType startValue, DataType endValue)
{
	const auto increment = (endValue - startValue) / static_cast<DataType> (size);

	const auto initial_incs = [increment]
	{
		DataType incs[mipp::N<DataType>()];

		for (auto i = 0; i < mipp::N<DataType>(); ++i)
			incs[i] = increment * static_cast<DataType> (i);
	}();

	auto incReg = mipp::set<DataType> (initial_incs);

	const auto i_reg = mipp::set1 (static_cast<DataType> (mipp::N<DataType>()));

	const auto vecOp = [&incReg, &i_reg, output] (auto i)
	{
		incReg.store (&output[i]);
		incReg += i_reg;
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   [output, startValue, increment] (auto i)
							   { output[i] = (startValue + (increment * static_cast<DataType> (i))); });
}

template <Scalar DataType, Integral SizeType>
void applyRamp (DataType* const dataAndDest, SizeType size, DataType startValue, DataType endValue)
{
	const auto increment = (endValue - startValue) / static_cast<DataType> (size);

	const auto initial_incs = [increment]
	{
		DataType incs[mipp::N<DataType>()];

		for (auto i = 0; i < mipp::N<DataType>(); ++i)
			incs[i] = increment * static_cast<DataType> (i);
	}();

	auto incReg = mipp::set<DataType> (initial_incs);

	const auto i_reg = mipp::set1 (static_cast<DataType> (mipp::N<DataType>()));

	mipp::Reg<DataType> dataRegister;

	const auto vecOp = [&dataRegister, &incReg, &i_reg, dataAndDest] (auto i)
	{
		dataRegister.load (&dataAndDest[i]);
		dataRegister *= incReg;
		dataRegister.store (&dataAndDest[i]);

		incReg += i_reg;
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   [dataAndDest, startValue, increment] (auto i)
							   { dataAndDest[i] *= (startValue + (increment * static_cast<DataType> (i))); });
}

template <Scalar DataType, Integral SizeType>
void applyRampAndCopy (DataType* const dest, const DataType* const data, SizeType size, DataType startValue, DataType endValue)
{
	const auto increment = (endValue - startValue) / static_cast<DataType> (size);

	const auto initial_incs = [increment]
	{
		DataType incs[mipp::N<DataType>()];

		for (auto i = 0; i < mipp::N<DataType>(); ++i)
			incs[i] = increment * static_cast<DataType> (i);
	}();

	auto incReg = mipp::set<DataType> (initial_incs);

	const auto i_reg = mipp::set1 (static_cast<DataType> (mipp::N<DataType>()));

	mipp::Reg<DataType> dataRegister;

	const auto vecOp = [&dataRegister, &incReg, &i_reg, data, dest] (auto i)
	{
		dataRegister.load (&data[i]);
		dataRegister *= incReg;
		dataRegister.store (&dest[i]);

		incReg += i_reg;
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   [dest, data, startValue, increment] (auto i)
							   { dest[i] = data[i] * (startValue + (increment * static_cast<DataType> (i))); });
}

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark Windowing functions

namespace window
{

/* --- Blackman --- */

template <Scalar DataType, Integral SizeType>
void generateBlackman (DataType* const output, SizeType size)
{
	const auto pi_register		= mipp::set1<DataType> (math::constants::pi<DataType>);
	const auto size_register	= mipp::set1<DataType> (static_cast<DataType> (size - 1));
	const auto order_2_register = mipp::set1<DataType> (DataType (2));
	const auto order_4_register = mipp::set1<DataType> (DataType (4));
	const auto pnt_5_register	= mipp::set1<DataType> (DataType (0.5));
	const auto alpha_register	= mipp::set1<DataType> (math::constants::blackman_alpha<DataType>);

	const auto one_minus_alpha_register = mipp::set1<DataType> (DataType (1) - math::constants::blackman_alpha<DataType>);

	mipp::Reg<DataType> i_register;

	const auto reg_ncos = [&i_register, &pi_register, &size_register] (const mipp::Reg<DataType>& order)
	{
		return mipp::cos (order * i_register) * pi_register / size_register;
	};

	const auto vecOp = [&, output] (auto i)
	{
		i_register = mipp::set1<DataType> (static_cast<DataType> (i));

		const auto cos2 = reg_ncos (order_2_register);
		const auto cos4 = reg_ncos (order_4_register);

		i_register = pnt_5_register * one_minus_alpha_register - pnt_5_register * cos2 + pnt_5_register * alpha_register * cos4;

		i_register.store (&output[i]);
	};

	const auto scalarOp = [size, output] (auto i)
	{
		const auto cos2 = fb::window::detail::ncos<DataType> (SizeType (2), i, size);
		const auto cos4 = fb::window::detail::ncos<DataType> (SizeType (4), i, size);

		output[i] = static_cast<DataType> (0.5 * (1. - math::constants::blackman_alpha<DataType>) -0.5 * cos2 + 0.5 * math::constants::blackman_alpha<DataType> * cos4);
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   std::move (scalarOp));
}

template <Scalar DataType, Integral SizeType>
void applyBlackman (DataType* const dataAndDest, SizeType size)
{
	fb::window::applyBlackman (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void applyBlackmanAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	fb::window::applyBlackmanAndCopy (dest, data, size);
}

/* --- Hamm --- */

template <Scalar DataType, Integral SizeType>
void generateHamm (DataType* const output, SizeType size)
{
	const auto pnt_54_register = mipp::set1<DataType> (DataType (0.54));
	const auto pnt_46_register = mipp::set1<DataType> (DataType (0.46));
	const auto order_register  = mipp::set1<DataType> (DataType (2));
	const auto pi_register	   = mipp::set1<DataType> (math::constants::pi<DataType>);
	const auto size_register   = mipp::set1<DataType> (static_cast<DataType> (size - 1));

	mipp::Reg<DataType> i_register;

	const auto reg_ncos = [&order_register, &i_register, &pi_register, &size_register]
	{
		return mipp::cos (order_register * i_register) * pi_register / size_register;
	};

	const auto vecOp = [&, output] (auto i)
	{
		i_register = mipp::set1<DataType> (static_cast<DataType> (i));

		i_register = reg_ncos();  // cppcheck-suppress redundantAssignment

		i_register = pnt_54_register - pnt_46_register * i_register;

		i_register.store (&output[i]);
	};

	const auto scalarOp = [output, size] (auto i)
	{
		output[i] = static_cast<DataType> (0.54 - 0.46 * fb::window::detail::ncos<DataType> (SizeType (2), i, size));
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   std::move (scalarOp));
}

template <Scalar DataType, Integral SizeType>
void applyHamm (DataType* const dataAndDest, SizeType size)
{
	fb::window::applyHamm (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void applyHammAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	fb::window::applyHammAndCopy (dest, data, size);
}

/* --- Hanning --- */

template <Scalar DataType, Integral SizeType>
void generateHanning (DataType* const output, SizeType size)
{
	// NB. MIPP had compiler errors when declaring these as 'auto', for some reason......
	const mipp::Reg<DataType> order_register = mipp::set1<DataType> (DataType (2));
	const mipp::Reg<DataType> pi_register	 = mipp::set1<DataType> (math::constants::pi<DataType>);
	const mipp::Reg<DataType> size_register	 = mipp::set1<DataType> (static_cast<DataType> (size - 1));
	const mipp::Reg<DataType> pnt_5_register = mipp::set1<DataType> (0.5);

	mipp::Reg<DataType> i_register;

	auto reg_ncos = [&order_register, &i_register, &pi_register, &size_register]
	{
		return mipp::cos (order_register * i_register) * pi_register / size_register;
	};

	const auto vecOp = [&, output] (auto i)
	{
		i_register = mipp::set1<DataType> (static_cast<DataType> (i));
		i_register = pnt_5_register - pnt_5_register * reg_ncos();	// cppcheck-suppress redundantAssignment
		i_register.store (&output[i]);
	};

	const auto scalarOp = [output, size] (auto i)
	{
		output[i] = static_cast<DataType> (0.5 - 0.5 * fb::window::detail::ncos<DataType> (SizeType (2), i, size));
	};

	detail::perform<DataType> (size, std::move (vecOp),
							   std::move (scalarOp));
}

template <Scalar DataType, Integral SizeType>
void applyHanning (DataType* const dataAndDest, SizeType size)
{
	fb::window::applyHanning (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void applyHanningAndCopy (DataType* const dest, const DataType* const data, SizeType size)
{
	fb::window::applyHanningAndCopy (dest, data, size);
}

}  // namespace window

/*---------------------------------------------------------------------------------------------------------------------------*/

template <Scalar DataType, Integral SizeType>
void polarToCartesian (DataType* const real, DataType* const imag, const DataType* const mag, const DataType* const phase, SizeType size)
{
	fb::polarToCartesian (real, imag, mag, phase, size);
}

template <Scalar DataType, Integral SizeType>
void polarToCartesianInterleaved (DataType* const dest, const DataType* const mag, const DataType* const phase, SizeType size)
{
	fb::polarToCartesianInterleaved (dest, mag, phase, size);
}

template <Scalar DataType, Integral SizeType>
void cartesianToPolar (DataType* const mag, DataType* const phase, const DataType* const real, const DataType* const imag, SizeType size)
{
	fb::cartesianToPolar (mag, phase, real, imag, size);
}

template <Scalar DataType, Integral SizeType>
void catesianInterleavedToPolar (DataType* const mag, DataType* const phase, const DataType* const src, SizeType size)
{
	fb::catesianInterleavedToPolar (mag, phase, src, size);
}

template <Scalar DataType, Integral SizeType>
void cartesianToMagnitudes (DataType* const mag, const DataType* const real, const DataType* const imag, SizeType size)
{
	fb::cartesianToMagnitudes (mag, real, imag, size);
}

template <Scalar DataType, Integral SizeType>
void cartesianInterleavedToMagnitudes (DataType* const mag, const DataType* const src, SizeType size)
{
	fb::cartesianInterleavedToMagnitudes (mag, src, size);
}

/// @endcond

}  // namespace vecops

LIMES_END_NAMESPACE
