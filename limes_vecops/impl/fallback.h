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

namespace lemons::vecops
{

template <Scalar DataType, Integral SizeType>
void fill (DataType* const data, SizeType size, DataType constantToFill)
{
	fb::fill (data, size, constantToFill);
}

template <Scalar DataType, Integral SizeType>
void clear (DataType* const data, SizeType size)
{
	fb::clear (data, size);
}

template <Scalar DataType, Integral SizeType>
void swap (DataType* const vecA, DataType* const vecB, SizeType size)
{
	fb::swap (vecA, vecB, size);
}


/*---------------------------------------------------------------------------------------------------------------------------*/


/*-----  ADDITION  -----*/

template <Scalar DataType, Integral SizeType>
void add (DataType* const data, SizeType size, DataType constantToAdd)
{
	fb::add (data, size, constantToAdd);
}

template <Scalar DataType, Integral SizeType>
void add (DataType* const dataAndDest, SizeType size, const DataType* const dataToAdd)
{
	fb::add (dataAndDest, size, dataToAdd);
}

template <Scalar DataType, Integral SizeType>
void addAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToAdd)
{
	fb::addAndCopy (dest, origData, size, constantToAdd);
}

template <Scalar DataType, Integral SizeType>
void addAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToAdd)
{
	fb::addAndCopy (dest, origData, size, dataToAdd);
}


/*-----  SUBTRACTION  -----*/

template <Scalar DataType, Integral SizeType>
void subtract (DataType* const data, SizeType size, DataType constantToSubtract)
{
	fb::subtract (data, size, constantToSubtract);
}

template <Scalar DataType, Integral SizeType>
void subtract (DataType* const dataAndDest, SizeType size, const DataType* const dataToSubtract)
{
	fb::subtract (dataAndDest, size, dataToSubtract);
}

template <Scalar DataType, Integral SizeType>
void subtractAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToSubtract)
{
	fb::subtractAndCopy (dest, origData, size, constantToSubtract);
}

template <Scalar DataType, Integral SizeType>
void subtractAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToSubtract)
{
	fb::subtractAndCopy (dest, origData, size, dataToSubtract);
}


/*-----  MULTIPLICATION  -----*/

template <Scalar DataType, Integral SizeType>
void multiply (DataType* const data, SizeType size, DataType constantToMultiply)
{
	fb::multiply (data, size, constantToMultiply);
}

template <Scalar DataType, Integral SizeType>
void multiply (DataType* const dataAndDest, SizeType size, const DataType* const dataToMultiply)
{
	fb::multiply (dataAndDest, size, dataToMultiply);
}

template <Scalar DataType, Integral SizeType>
void multiplyAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToMultiply)
{
	fb::multiplyAndCopy (dest, origData, size, constantToMultiply);
}

template <Scalar DataType, Integral SizeType>
void multiplyAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToMultiply)
{
	fb::multiplyAndCopy (dest, origData, size, dataToMultiply);
}


/*-----  DIVISION  -----*/

template <Scalar DataType, Integral SizeType>
void divide (DataType* const data, SizeType size, DataType constantToDivide)
{
	fb::divide (data, size, constantToDivide);
}

template <Scalar DataType, Integral SizeType>
void divide (DataType* const dataAndDest, SizeType size, const DataType* const dataToDivide)
{
	fb::divide (dataAndDest, size, dataToDivide);
}

template <Scalar DataType, Integral SizeType>
void divideAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToDivide)
{
	fb::divideAndCopy (dest, origData, size, constantToDivide);
}

template <Scalar DataType, Integral SizeType>
void divideAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToDivide)
{
	fb::divideAndCopy (dest, origData, size, dataToDivide);
}


/*---------------------------------------------------------------------------------------------------------------------------*/


template <Scalar DataType, Integral SizeType>
void square (DataType* const dataAndDest, SizeType size)
{
	fb::square (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void square (DataType* const dest, const DataType* const data, SizeType size)
{
	fb::square (dest, data, size);
}

template <Scalar DataType, Integral SizeType>
void squareRoot (DataType* const dataAndDest, SizeType size)
{
	fb::squareRoot (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void squareRoot (DataType* const dest, const DataType* const data, SizeType size)
{
	fb::squareRoot (dest, data, size);
}


/*---------------------------------------------------------------------------------------------------------------------------*/


template <Scalar DataType, Integral SizeType>
void reverse (DataType* const dataAndDest, SizeType size)
{
	fb::reverse (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void reverse (DataType* const dest, const DataType* const data, SizeType size)
{
	fb::reverse (dest, data, size);
}

template <Scalar DataType, Integral SizeType>
void sort (DataType* const dataAndDest, SizeType size)
{
	fb::sort (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void sort (DataType* const dest, const DataType* const data, SizeType size)
{
	fb::sort (dest, data, size);
}

template <Scalar DataType, Integral SizeType>
void sortReverse (DataType* const dataAndDest, SizeType size)
{
	fb::sortReverse (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void sortReverse (DataType* const dest, const DataType* const data, SizeType size)
{
	fb::sortReverse (dest, data, size);
}


/*---------------------------------------------------------------------------------------------------------------------------*/


template <Scalar DataType, Integral SizeType>
void abs (DataType* const dataAndDest, SizeType size)
{
	fb::abs (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void abs (DataType* const dest, const DataType* const data, SizeType size)
{
	fb::abs (dest, data, size);
}

template <Scalar DataType, Integral SizeType>
void negate (DataType* const dataAndDest, SizeType size)
{
	fb::negate (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void negate (DataType* const dest, const DataType* const data, SizeType size)
{
	fb::negate (dest, data, size);
}

template <Scalar DataType, Integral SizeType>
void clip (DataType* const dataAndDest, SizeType size, DataType lowClip, DataType hiClip)
{
	fb::clip (dataAndDest, size, lowClip, hiClip);
}

template <Scalar DataType, Integral SizeType>
void clip (DataType* const dest, const DataType* const data, SizeType size, DataType lowClip, DataType hiClip)
{
	fb::clip (dest, data, size, lowClip, hiClip);
}

template <Scalar DataType, Integral SizeType>
DataType max (const DataType* const data, SizeType size)
{
	return fb::max (data, size);
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
void max (const DataType* const data, SizeType size, DataType& maxValue, IndexType& maxIndex)
{
	fb::max (data, size, maxValue, maxIndex);
}

template <Scalar DataType, Integral SizeType>
DataType maxAbs (const DataType* const data, SizeType size)
{
	return fb::maxAbs (data, size);
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
void maxAbs (const DataType* const data, SizeType size, DataType& maxValue, IndexType& maxIndex)
{
	fb::maxAbs (data, size, maxValue, maxIndex);
}

template <Scalar DataType, Integral SizeType>
DataType min (const DataType* const data, SizeType size)
{
	return fb::min (data, size);
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
void min (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex)
{
	fb::min (data, size, minValue, minIndex);
}

template <Scalar DataType, Integral SizeType>
DataType minAbs (const DataType* const data, SizeType size)
{
	return fb::minAbs (data, size);
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
void minAbs (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex)
{
	fb::minAbs (data, size, minValue, minIndex);
}

template <Scalar DataType, Integral SizeType>
DataType sum (const DataType* const data, SizeType size)
{
	return fb::sum (data, size);
}

template <Scalar DataType, Integral SizeType>
DataType mean (const DataType* const data, SizeType size)
{
	return fb::mean (data, size);
}


/*---------------------------------------------------------------------------------------------------------------------------*/


template <Scalar DataType, Integral SizeType>
void generateRamp (DataType* const output, SizeType size, DataType startValue, DataType endValue)
{
	fb::generateRamp (output, size, startValue, endValue);
}

namespace window
{

template <Scalar DataType, Integral SizeType>
void generateBlackman (DataType* const output, SizeType size)
{
	fb::window::generateBlackman (output, size);
}

template <Scalar DataType, Integral SizeType>
void generateHamm (DataType* const output, SizeType size)
{
	fb::window::generateHamm (output, size);
}

template <Scalar DataType, Integral SizeType>
void generateHanning (DataType* const output, SizeType size)
{
	fb::window::generateHanning (output, size);
}

}  // namespace window

}  // namespace lemons::vecops
