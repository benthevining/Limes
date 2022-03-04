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

static_assert (lemons::vecops::isUsingVDSP());

#include <Accelerate/Accelerate.h>
#include <limits>
#include <cstring>	// for memset
#include <cmath>

namespace lemons::vecops
{

template <bool Value>
using ConditionalType = std::conditional_t<Value, std::true_type, std::false_type>;


template <typename DataType>
using is_float_type = ConditionalType<std::is_same_v<DataType, float>>;

template <typename DataType>
using is_double_type = ConditionalType<std::is_same_v<DataType, double>>;

template <typename DataType>
using is_signed_32_bit_type = ConditionalType<std::is_integral_v<DataType> && std::is_signed_v<DataType> && std::numeric_limits<DataType>::digits == 32>;


template <Scalar DataType, Integral SizeType>
void fill (DataType* const data, SizeType size, DataType constantToFill)
{
	if constexpr (is_float_type<DataType>())
		vDSP_vfill (&constantToFill, data, vDSP_Stride (1), vDSP_Length (size));
	else if constexpr (is_double_type<DataType>())
		vDSP_vfillD (&constantToFill, data, vDSP_Stride (1), vDSP_Length (size));
	else if constexpr (is_signed_32_bit_type<DataType>())
		vDSP_vfilli (&constantToFill, data, vDSP_Stride (1), vDSP_Length (size));
	else
		fb::fill (data, size, constantToFill);
}

template <Scalar DataType, Integral SizeType>
void clear (DataType* const data, SizeType size)
{
	if constexpr (is_float_type<DataType>())
		vDSP_vclr (data, vDSP_Stride (1), vDSP_Length (size));
	else if constexpr (is_double_type<DataType>())
		vDSP_vclrD (data, vDSP_Stride (1), vDSP_Length (size));
	else
		fb::clear (data, size);
}

template <Scalar DataType, Integral SizeType>
void copy (DataType* const dest, const DataType* const source, SizeType size)
{
	fb::copy (dest, source, size);
}

template <Scalar DataType, Integral SizeType>
void swap (DataType* const vecA, DataType* const vecB, SizeType size)
{
	if constexpr (is_float_type<DataType>())
		vDSP_vswap (vecA, vDSP_Stride (1), vecB, vDSP_Stride (1), vDSP_Length (size));
	else if constexpr (is_double_type<DataType>())
		vDSP_vswapD (vecA, vDSP_Stride (1), vecB, vDSP_Stride (1), vDSP_Length (size));
	else
		fb::swap (vecA, vecB, size);
}


template <>
[[maybe_unused]] void convert (float* const dest, const double* const source, int size)
{
	vDSP_vdpsp (source, vDSP_Stride (1), dest, vDSP_Stride (1), vDSP_Length (size));
}

template <>
[[maybe_unused]] void convert (double* const dest, const float* const source, int size)
{
	vDSP_vspdp (source, vDSP_Stride (1), dest, vDSP_Stride (1), vDSP_Length (size));
}


/*---------------------------------------------------------------------------------------------------------------------------*/


/*-----  ADDITION  -----*/

template <Scalar DataType, Integral SizeType>
void add (DataType* const data, SizeType size, DataType constantToAdd)
{
	if constexpr (is_float_type<DataType>())
		vDSP_vsadd (data, vDSP_Stride (1), &constantToAdd, data, vDSP_Stride (1), vDSP_Length (size));
	else if constexpr (is_double_type<DataType>())
		vDSP_vsaddD (data, vDSP_Stride (1), &constantToAdd, data, vDSP_Stride (1), vDSP_Length (size));
	else if constexpr (is_signed_32_bit_type<DataType>())
		vDSP_vsaddi (data, vDSP_Stride (1), &constantToAdd, data, vDSP_Stride (1), vDSP_Length (size));
	else
		fb::add (data, size, constantToAdd);
}

template <Scalar DataType, Integral SizeType>
void add (DataType* const dataAndDest, SizeType size, const DataType* const dataToAdd)
{
	if constexpr (is_float_type<DataType>())
		vDSP_vadd (dataAndDest, vDSP_Stride (1), dataToAdd, vDSP_Stride (1), dataAndDest, vDSP_Stride (1), vDSP_Length (size));
	else if constexpr (is_double_type<DataType>())
		vDSP_vaddD (dataAndDest, vDSP_Stride (1), dataToAdd, vDSP_Stride (1), dataAndDest, vDSP_Stride (1), vDSP_Length (size));
	else if constexpr (is_signed_32_bit_type<DataType>())
		vDSP_vaddi (dataAndDest, vDSP_Stride (1), dataToAdd, vDSP_Stride (1), dataAndDest, vDSP_Stride (1), vDSP_Length (size));
	else
		fb::add (dataAndDest, size, dataToAdd);
}

template <Scalar DataType, Integral SizeType>
void addAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToAdd)
{
	if constexpr (is_float_type<DataType>())
		vDSP_vsadd (origData, vDSP_Stride (1), &constantToAdd, dest, vDSP_Stride (1), vDSP_Length (size));
	else if constexpr (is_double_type<DataType>())
		vDSP_vsaddD (origData, vDSP_Stride (1), &constantToAdd, dest, vDSP_Stride (1), vDSP_Length (size));
	else if constexpr (is_signed_32_bit_type<DataType>())
		vDSP_vsaddi (origData, vDSP_Stride (1), &constantToAdd, dest, vDSP_Stride (1), vDSP_Length (size));
	else
		fb::addAndCopy (dest, origData, size, constantToAdd);
}

template <Scalar DataType, Integral SizeType>
void addAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToAdd)
{
	if constexpr (is_float_type<DataType>())
		vDSP_vadd (origData, vDSP_Stride (1), dataToAdd, vDSP_Stride (1), dest, vDSP_Stride (1), vDSP_Length (size));
	else if constexpr (is_double_type<DataType>())
		vDSP_vaddD (origData, vDSP_Stride (1), dataToAdd, vDSP_Stride (1), dest, vDSP_Stride (1), vDSP_Length (size));
	else if constexpr (is_signed_32_bit_type<DataType>())
		vDSP_vaddi (origData, vDSP_Stride (1), dataToAdd, vDSP_Stride (1), dest, vDSP_Stride (1), vDSP_Length (size));
	else
		fb::addAndCopy (dest, origData, size, dataToAdd);
}


/*-----  SUBTRACTION  -----*/

template <Scalar DataType, Integral SizeType>
void subtract (DataType* const data, SizeType size, DataType constantToSubtract)
{
	add (data, size, -constantToSubtract);
}

template <Scalar DataType, Integral SizeType>
void subtract (DataType* const dataAndDest, SizeType size, const DataType* const dataToSubtract)
{
	if constexpr (is_float_type<DataType>())
		vDSP_vsub (dataAndDest, vDSP_Stride (1), dataToSubtract, vDSP_Stride (1), dataAndDest, vDSP_Stride (1), vDSP_Length (size));
	else if constexpr (is_double_type<DataType>())
		vDSP_vsubD (dataAndDest, vDSP_Stride (1), dataToSubtract, vDSP_Stride (1), dataAndDest, vDSP_Stride (1), vDSP_Length (size));
	else
		fb::subtract (dataAndDest, size, dataToSubtract);
}

template <Scalar DataType, Integral SizeType>
void subtractAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToSubtract)
{
	addAndCopy (dest, origData, size, -constantToSubtract);
}

template <Scalar DataType, Integral SizeType>
void subtractAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToSubtract)
{
	if constexpr (is_float_type<DataType>())
		vDSP_vsub (origData, vDSP_Stride (1), dataToSubtract, vDSP_Stride (1), dest, vDSP_Stride (1), vDSP_Length (size));
	else if constexpr (is_double_type<DataType>())
		vDSP_vsubD (origData, vDSP_Stride (1), dataToSubtract, vDSP_Stride (1), dest, vDSP_Stride (1), vDSP_Length (size));
	else
		fb::subtractAndCopy (dest, origData, size, dataToSubtract);
}


/*-----  MULTIPLICATION  -----*/

template <Scalar DataType, Integral SizeType>
void multiply (DataType* const data, SizeType size, DataType constantToMultiply)
{
	if constexpr (is_float_type<DataType>())
		vDSP_vsmul (data, vDSP_Stride (1), &constantToMultiply, data, vDSP_Stride (1), vDSP_Length (size));
	else if constexpr (is_double_type<DataType>())
		vDSP_vsmulD (data, vDSP_Stride (1), &constantToMultiply, data, vDSP_Stride (1), vDSP_Length (size));
	else
		fb::multiply (data, size, constantToMultiply);
}

template <Scalar DataType, Integral SizeType>
void multiply (DataType* const dataAndDest, SizeType size, const DataType* const dataToMultiply)
{
	if constexpr (is_float_type<DataType>())
		vDSP_vmul (dataAndDest, vDSP_Stride (1), dataToMultiply, vDSP_Stride (1), dataAndDest, vDSP_Stride (1), vDSP_Length (size));
	else if constexpr (is_double_type<DataType>())
		vDSP_vmulD (dataAndDest, vDSP_Stride (1), dataToMultiply, vDSP_Stride (1), dataAndDest, vDSP_Stride (1), vDSP_Length (size));
	else
		fb::multiply (dataAndDest, size, dataToMultiply);
}

template <Scalar DataType, Integral SizeType>
void multiplyAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToMultiply)
{
	if constexpr (is_float_type<DataType>())
		vDSP_vsmul (origData, vDSP_Stride (1), &constantToMultiply, dest, vDSP_Stride (1), vDSP_Length (size));
	else if constexpr (is_double_type<DataType>())
		vDSP_vsmulD (origData, vDSP_Stride (1), &constantToMultiply, dest, vDSP_Stride (1), vDSP_Length (size));
	else
		fb::multiplyAndCopy (dest, origData, size, constantToMultiply);
}

template <Scalar DataType, Integral SizeType>
void multiplyAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToMultiply)
{
	if constexpr (is_float_type<DataType>())
		vDSP_vmul (origData, vDSP_Stride (1), dataToMultiply, vDSP_Stride (1), dest, vDSP_Stride (1), vDSP_Length (size));
	else if constexpr (is_double_type<DataType>())
		vDSP_vmulD (origData, vDSP_Stride (1), dataToMultiply, vDSP_Stride (1), dest, vDSP_Stride (1), vDSP_Length (size));
	else
		fb::multiplyAndCopy (dest, origData, size, dataToMultiply);
}


/*-----  DIVISION  -----*/

template <Scalar DataType, Integral SizeType>
void divide (DataType* const data, SizeType size, DataType constantToDivide)
{
	if constexpr (is_float_type<DataType>())
		vDSP_vsdiv (data, vDSP_Stride (1), &constantToDivide, data, vDSP_Stride (1), vDSP_Length (size));
	else if constexpr (is_double_type<DataType>())
		vDSP_vsdivD (data, vDSP_Stride (1), &constantToDivide, data, vDSP_Stride (1), vDSP_Length (size));
	else if constexpr (is_signed_32_bit_type<DataType>())
		vDSP_vsdivi (data, vDSP_Stride (1), &constantToDivide, data, vDSP_Stride (1), vDSP_Length (size));
	else
		fb::divide (data, size, constantToDivide);
}

template <Scalar DataType, Integral SizeType>
void divide (DataType* const dataAndDest, SizeType size, const DataType* const dataToDivide)
{
	if constexpr (is_float_type<DataType>())
		vDSP_vdiv (dataToDivide, vDSP_Stride (1), dataAndDest, vDSP_Stride (1), dataAndDest, vDSP_Stride (1), vDSP_Length (size));
	else if constexpr (is_double_type<DataType>())
		vDSP_vdivD (dataToDivide, vDSP_Stride (1), dataAndDest, vDSP_Stride (1), dataAndDest, vDSP_Stride (1), vDSP_Length (size));
	else if constexpr (is_signed_32_bit_type<DataType>())
		vDSP_vdivi (dataToDivide, vDSP_Stride (1), dataAndDest, vDSP_Stride (1), dataAndDest, vDSP_Stride (1), vDSP_Length (size));
	else
		fb::divide (dataAndDest, size, dataToDivide);
}

template <Scalar DataType, Integral SizeType>
void divideAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToDivide)
{
	if constexpr (is_float_type<DataType>())
		vDSP_vsdiv (origData, vDSP_Stride (1), &constantToDivide, dest, vDSP_Stride (1), vDSP_Length (size));
	else if constexpr (is_double_type<DataType>())
		vDSP_vsdivD (origData, vDSP_Stride (1), &constantToDivide, dest, vDSP_Stride (1), vDSP_Length (size));
	else if constexpr (is_signed_32_bit_type<DataType>())
		vDSP_vsdivi (origData, vDSP_Stride (1), &constantToDivide, dest, vDSP_Stride (1), vDSP_Length (size));
	else
		fb::divideAndCopy (dest, origData, size, constantToDivide);
}

template <Scalar DataType, Integral SizeType>
void divideAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToDivide)
{
	if constexpr (is_float_type<DataType>())
		vDSP_vdiv (dataToDivide, vDSP_Stride (1), origData, vDSP_Stride (1), dest, vDSP_Stride (1), vDSP_Length (size));
	else if constexpr (is_double_type<DataType>())
		vDSP_vdivD (dataToDivide, vDSP_Stride (1), origData, vDSP_Stride (1), dest, vDSP_Stride (1), vDSP_Length (size));
	else if constexpr (is_signed_32_bit_type<DataType>())
		vDSP_vdivi (dataToDivide, vDSP_Stride (1), origData, vDSP_Stride (1), dest, vDSP_Stride (1), vDSP_Length (size));
	else
		fb::divideAndCopy (dest, origData, size, dataToDivide);
}


/*---------------------------------------------------------------------------------------------------------------------------*/


template <Scalar DataType, Integral SizeType>
void square (DataType* const dataAndDest, SizeType size)
{
	square (dataAndDest, dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void square (DataType* const dest, const DataType* const data, SizeType size)
{
	if constexpr (is_float_type<DataType>())
		vDSP_vsq (data, vDSP_Stride (1), dest, vDSP_Stride (1), vDSP_Length (size));
	else if constexpr (is_double_type<DataType>())
		vDSP_vsqD (data, vDSP_Stride (1), dest, vDSP_Stride (1), vDSP_Length (size));
	else
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
	if constexpr (is_float_type<DataType>())
		vDSP_vrvrs (dataAndDest, vDSP_Stride (1), vDSP_Length (size));
	else if constexpr (is_double_type<DataType>())
		vDSP_vrvrsD (dataAndDest, vDSP_Stride (1), vDSP_Length (size));
	else
		fb::reverse (dataAndDest, size);
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
	if constexpr (is_float_type<DataType>())
		vDSP_vsort (dataAndDest, vDSP_Stride (1), vDSP_Length (size));
	else if constexpr (is_double_type<DataType>())
		vDSP_vsortD (dataAndDest, vDSP_Stride (1), vDSP_Length (size));
	else
		fb::sort (dataAndDest, size);
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
	sort (dataAndDest, size);
	reverse (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void sortReverse (DataType* const dest, const DataType* const data, SizeType size)
{
	sort (dest, data, size);
	reverse (dest, size);
}


/*---------------------------------------------------------------------------------------------------------------------------*/


template <Scalar DataType, Integral SizeType>
void abs (DataType* const dataAndDest, SizeType size)
{
	abs (dataAndDest, dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void abs (DataType* const dest, const DataType* const data, SizeType size)
{
	if constexpr (is_float_type<DataType>())
		vDSP_vabs (data, vDSP_Stride (1), dest, vDSP_Stride (1), vDSP_Length (size));
	else if constexpr (is_double_type<DataType>())
		vDSP_vabsD (data, vDSP_Stride (1), dest, vDSP_Stride (1), vDSP_Length (size));
	else if constexpr (is_signed_32_bit_type<DataType>())
		vDSP_vabsi (data, vDSP_Stride (1), dest, vDSP_Stride (1), vDSP_Length (size));
	else
		fb::abs (dest, data, size);
}


template <Scalar DataType, Integral SizeType>
void negate (DataType* const dataAndDest, SizeType size)
{
	negate (dataAndDest, dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void negate (DataType* const dest, const DataType* const data, SizeType size)
{
	if constexpr (is_float_type<DataType>())
		vDSP_vneg (data, vDSP_Stride (1), dest, vDSP_Stride (1), vDSP_Length (size));
	else if constexpr (is_double_type<DataType>())
		vDSP_vnegD (data, vDSP_Stride (1), dest, vDSP_Stride (1), vDSP_Length (size));
	else
		fb::negate (dest, data, size);
}


template <Scalar DataType, Integral SizeType>
void clip (DataType* const dataAndDest, SizeType size, DataType lowClip, DataType hiClip)
{
	clip (dataAndDest, dataAndDest, size, lowClip, hiClip);
}

template <Scalar DataType, Integral SizeType>
void clip (DataType* const dest, const DataType* const data, SizeType size, DataType lowClip, DataType hiClip)
{
	if constexpr (is_float_type<DataType>())
		vDSP_vclip (data, vDSP_Stride (1), &lowClip, &hiClip, dest, vDSP_Stride (1), vDSP_Length (size));
	else if constexpr (is_double_type<DataType>())
		vDSP_vclipD (data, vDSP_Stride (1), &lowClip, &hiClip, dest, vDSP_Stride (1), vDSP_Length (size));
	else
		fb::clip (dest, data, size, lowClip, hiClip);
}

template <Scalar DataType, Integral SizeType>
DataType max (const DataType* const data, SizeType size)
{
	DataType maxVal { 0 };

	if constexpr (is_float_type<DataType>())
		vDSP_maxv (data, vDSP_Stride (1), &maxVal, vDSP_Length (size));
	else if constexpr (is_double_type<DataType>())
		vDSP_maxvD (data, vDSP_Stride (1), &maxVal, vDSP_Length (size));
	else
		return fb::max (data, size);

	return maxVal;
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
void max (const DataType* const data, SizeType size, DataType& maxValue, IndexType& maxIndex)
{
	vDSP_Length maxIdx { 0 };

	if constexpr (is_float_type<DataType>())
		vDSP_maxvi (data, vDSP_Stride (1), &maxValue, &maxIdx, vDSP_Length (size));
	else if constexpr (is_double_type<DataType>())
		vDSP_maxviD (data, vDSP_Stride (1), &maxValue, &maxIdx, vDSP_Length (size));
	else
	{
		fb::max (data, size, maxValue, maxIndex);
		return;
	}

	maxIndex = static_cast<IndexType> (maxIdx);
}

template <Scalar DataType, Integral SizeType>
DataType maxAbs (const DataType* const data, SizeType size)
{
	DataType maxVal { 0 };

	if constexpr (is_float_type<DataType>())
		vDSP_maxmgv (data, vDSP_Stride (1), &maxVal, vDSP_Length (size));
	else if constexpr (is_double_type<DataType>())
		vDSP_maxmgvD (data, vDSP_Stride (1), &maxVal, vDSP_Length (size));
	else
		return fb::maxAbs (data, size);

	return maxVal;
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
void maxAbs (const DataType* const data, SizeType size, DataType& maxValue, IndexType& maxIndex)
{
	vDSP_Length maxIdx { 0 };

	if constexpr (is_float_type<DataType>())
		vDSP_maxmgvi (data, vDSP_Stride (1), &maxValue, &maxIdx, vDSP_Length (size));
	else if constexpr (is_double_type<DataType>())
		vDSP_maxmgviD (data, vDSP_Stride (1), &maxValue, &maxIdx, vDSP_Length (size));
	else
	{
		fb::maxAbs (data, size, maxValue, maxIndex);
		return;
	}

	maxIndex = static_cast<IndexType> (maxIdx);
}

template <Scalar DataType, Integral SizeType>
DataType min (const DataType* const data, SizeType size)
{
	DataType minVal { 0 };

	if constexpr (is_float_type<DataType>())
		vDSP_minv (data, vDSP_Stride (1), &minVal, vDSP_Length (size));
	else if constexpr (is_double_type<DataType>())
		vDSP_minvD (data, vDSP_Stride (1), &minVal, vDSP_Length (size));
	else
		return fb::min (data, size);

	return minVal;
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
void min (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex)
{
	vDSP_Length minIdx { 0 };

	if constexpr (is_float_type<DataType>())
		vDSP_minvi (data, vDSP_Stride (1), &minValue, &minIdx, vDSP_Length (size));
	else if constexpr (is_double_type<DataType>())
		vDSP_minviD (data, vDSP_Stride (1), &minValue, &minIdx, vDSP_Length (size));
	else
	{
		fb::min (data, size, minValue, minIndex);
		return;
	}

	minIndex = static_cast<IndexType> (minIdx);
}

template <Scalar DataType, Integral SizeType>
DataType minAbs (const DataType* const data, SizeType size)
{
	DataType minVal { 0 };

	if constexpr (is_float_type<DataType>())
		vDSP_minmgv (data, vDSP_Stride (1), &minVal, vDSP_Length (size));
	else if constexpr (is_double_type<DataType>())
		vDSP_minmgvD (data, vDSP_Stride (1), &minVal, vDSP_Length (size));
	else
		return fb::minAbs (data, size);

	return minVal;
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
void minAbs (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex)
{
	vDSP_Length minIdx { 0 };

	if constexpr (is_float_type<DataType>())
		vDSP_minmgvi (data, vDSP_Stride (1), &minValue, &minIdx, vDSP_Length (size));
	else if constexpr (is_double_type<DataType>())
		vDSP_minmgviD (data, vDSP_Stride (1), &minValue, &minIdx, vDSP_Length (size));
	else
	{
		fb::minAbs (data, size, minValue, minIndex);
		return;
	}

	minIndex = static_cast<IndexType> (minIdx);
}

template <Scalar DataType, Integral SizeType>
void minMax (const DataType* const data, SizeType size, DataType& minValue, DataType& maxValue)
{
	minValue = min (data, size);
	maxValue = max (data, size);
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
void minMax (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex, DataType& maxValue, IndexType& maxIndex)
{
	min (data, size, minValue, minIndex);
	max (data, size, maxValue, maxIndex);
}

template <Scalar DataType, Integral SizeType>
void minMaxAbs (const DataType* const data, SizeType size, DataType& minValue, DataType& maxValue)
{
	minValue = minAbs (data, size);
	maxValue = maxAbs (data, size);
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
void minMaxAbs (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex, DataType& maxValue, IndexType& maxIndex)
{
	minAbs (data, size, minValue, minIndex);
	maxAbs (data, size, maxValue, maxIndex);
}

template <Scalar DataType, Integral SizeType>
DataType range (const DataType* const data, SizeType size)
{
	return fb::range (data, size);
}

template <Scalar DataType, Integral SizeType>
DataType rangeAbs (const DataType* const data, SizeType size)
{
	return fb::rangeAbs (data, size);
}

template <Scalar DataType, Integral SizeType>
DataType sum (const DataType* const data, SizeType size)
{
	DataType sumVal { 0 };

	if constexpr (is_float_type<DataType>())
		vDSP_sve (data, vDSP_Stride (1), &sumVal, vDSP_Length (size));
	else if constexpr (is_double_type<DataType>())
		vDSP_sveD (data, vDSP_Stride (1), &sumVal, vDSP_Length (size));
	else
		return fb::sum (data, size);

	return sumVal;
}

template <Scalar DataType, Integral SizeType>
DataType mean (const DataType* const data, SizeType size)
{
	DataType meanVal { 0 };

	if constexpr (is_float_type<DataType>())
		vDSP_meanv (data, vDSP_Stride (1), &meanVal, vDSP_Length (size));
	else if constexpr (is_double_type<DataType>())
		vDSP_meanvD (data, vDSP_Stride (1), &meanVal, vDSP_Length (size));
	else
		return fb::mean (data, size);

	return meanVal;
}


/*---------------------------------------------------------------------------------------------------------------------------*/


template <Scalar DataType, Integral SizeType>
void generateRamp (DataType* const output, SizeType size, DataType startValue, DataType endValue)
{
	const auto increment = (endValue - startValue) / static_cast<DataType> (size);

	if constexpr (is_float_type<DataType>())
		vDSP_vramp (&startValue, &increment, output, vDSP_Stride (1), vDSP_Length (size));
	else if constexpr (is_double_type<DataType>())
		vDSP_vrampD (&startValue, &increment, output, vDSP_Stride (1), vDSP_Length (size));
	else
		fb::generateRamp (output, size, startValue, endValue);
}

template <Scalar DataType, Integral SizeType>
void applyRamp (DataType* const dataAndDest, SizeType size, DataType startValue, DataType endValue)
{
	fb::applyRamp (dataAndDest, size, startValue, endValue);
}

template <Scalar DataType, Integral SizeType>
void applyRamp (DataType* const dest, const DataType* const data, SizeType size, DataType startValue, DataType endValue)
{
	generateRamp (dest, size, startValue, endValue);
	multiply (dest, size, data);
}


namespace window
{

template <Scalar DataType, Integral SizeType>
void generateBlackman (DataType* const output, SizeType size)
{
	if constexpr (is_float_type<DataType>())
		vDSP_blkman_window (output, vDSP_Length (size), 0);
	else if constexpr (is_double_type<DataType>())
		vDSP_blkman_windowD (output, vDSP_Length (size), 0);
	else
		fb::window::generateBlackman (output, size);
}

template <Scalar DataType, Integral SizeType>
void applyBlackman (DataType* const dataAndDest, SizeType size)
{
	fb::window::applyBlackman (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void applyBlackman (DataType* const dest, const DataType* const data, SizeType size)
{
	generateBlackman (dest, size);
	multiply (dest, size, data);
}

template <Scalar DataType, Integral SizeType>
void generateHamm (DataType* const output, SizeType size)
{
	if constexpr (is_float_type<DataType>())
		vDSP_hamm_window (output, vDSP_Length (size), 0);
	else if constexpr (is_double_type<DataType>())
		vDSP_hamm_windowD (output, vDSP_Length (size), 0);
	else
		fb::window::generateHamm (output, size);
}

template <Scalar DataType, Integral SizeType>
void applyHamm (DataType* const dataAndDest, SizeType size)
{
	fb::window::applyHamm (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void applyHamm (DataType* const dest, const DataType* const data, SizeType size)
{
	generateHamm (dest, size);
	multiply (dest, size, data);
}

template <Scalar DataType, Integral SizeType>
void generateHanning (DataType* const output, SizeType size)
{
	if constexpr (is_float_type<DataType>())
		vDSP_hann_window (output, vDSP_Length (size), vDSP_HANN_NORM);
	else if constexpr (is_double_type<DataType>())
		vDSP_hann_windowD (output, vDSP_Length (size), vDSP_HANN_NORM);
	else
		fb::window::generateHanning (output, size);
}

template <Scalar DataType, Integral SizeType>
void applyHanning (DataType* const dataAndDest, SizeType size)
{
	fb::window::applyHanning (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void applyHanning (DataType* const dest, const DataType* const data, SizeType size)
{
	generateHanning (dest, size);
	multiply (dest, size, data);
}

}  // namespace window

}  // namespace lemons::vecops
