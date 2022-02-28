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

static_assert (lemons::vecops::isUsingIPP());

#include <ipps.h>
#include <limits>

namespace lemons::vecops
{

template <bool Value>
using ConditionalType = std::conditional_t<Value, std::true_type, std::false_type>;


template <typename DataType>
using is_float_type = ConditionalType<std::is_same_v<DataType, float>>;

template <typename DataType>
using is_double_type = ConditionalType<std::is_same_v<DataType, double>>;

template <typename DataType>
using is_signed_int = ConditionalType<std::is_integral_v<DataType> && std::is_signed_v<DataType> && std::numeric_limits<DataType>::digits == 32>;

template <typename DataType>
using is_unsigned_int = ConditionalType<std::is_integral_v<DataType> && ! std::is_signed_v<DataType> && std::numeric_limits<DataType>::digits == 32>;


static constexpr auto integer_scale_factor = 0;
static constexpr auto alg_hint_type		   = ippAlgHintFast;


template <Scalar DataType, Integral SizeType>
void fill (DataType* const data, SizeType size, DataType constantToFill)
{
	if constexpr (is_float_type<DataType>())
		ippsSet_32f (constantToFill, data, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsSet_64f (constantToFill, data, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsSet_32s (constantToFill, data, static_cast<int> (size));
	else
	{
	}
}

template <Scalar DataType, Integral SizeType>
void clear (DataType* const data, SizeType size)
{
	if constexpr (is_float_type<DataType>())
		ippsZero_32f (data, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsZero_64f (data, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsZero_32s (data, static_cast<int> (size));
	else
	{
	}
}

template <Scalar DataType, Integral SizeType>
void swap (DataType* const vecA, DataType* const vecB, SizeType size)
{
}


/*---------------------------------------------------------------------------------------------------------------------------*/


/*-----  ADDITION  -----*/

template <Scalar DataType, Integral SizeType>
void add (DataType* const data, SizeType size, DataType constantToAdd)
{
	if constexpr (is_float_type<DataType>())
		ippsAddC_32f_I (constantToAdd, data, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsAddC_64f_I (constantToAdd, data, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsAddC_32s_ISfs (constantToAdd, data, static_cast<int> (size), integer_scale_factor);
	else
	{
	}
}

template <Scalar DataType, Integral SizeType>
void add (DataType* const dataAndDest, SizeType size, const DataType* const dataToAdd)
{
	if constexpr (is_float_type<DataType>())
		ippsAdd_32f_I (dataToAdd, dataAndDest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsAdd_64f_I (dataToAdd, dataAndDest, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsAdd_32s_ISfs (dataToAdd, dataAndDest, static_cast<int> (size), integer_scale_factor);
	else if constexpr (is_unsigned_int<DataType>())
		ippsAdd_32u_I (dataToAdd, dataAndDest, static_cast<int> (size));
	else
	{
	}
}

template <Scalar DataType, Integral SizeType>
void addAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToAdd)
{
	if constexpr (is_float_type<DataType>())
		ippsAddC_32f (origData, constantToAdd, dest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsAddC_64f (origData, constantToAdd, dest, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsAddC_32s_Sfs (origData, constantToAdd, dest, static_cast<int> (size), integer_scale_factor);
	else
	{
	}
}

template <Scalar DataType, Integral SizeType>
void addAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToAdd)
{
	if constexpr (is_float_type<DataType>())
		ippsAdd_32f (origData, dataToAdd, dest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsAdd_64f (origData, dataToAdd, dest, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsAdd_32s_Sfs (origData, dataToAdd, dest, static_cast<int> (size), integer_scale_factor);
	else if constexpr (is_unsigned_int<DataType>())
		ippsAdd_32u (origData, dataToAdd, dest, static_cast<int> (size));
	else
	{
	}
}


/*-----  SUBTRACTION  -----*/

template <Scalar DataType, Integral SizeType>
void subtract (DataType* const data, SizeType size, DataType constantToSubtract)
{
	if constexpr (is_float_type<DataType>())
		ippsSubC_32f_I (constantToSubtract, data, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsSubC_64f_I (constantToSubtract, data, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsSubC_32s_ISfs (constantToSubtract, data, static_cast<int> (size), integer_scale_factor);
	else
	{
	}
}

template <Scalar DataType, Integral SizeType>
void subtract (DataType* const dataAndDest, SizeType size, const DataType* const dataToSubtract)
{
	if constexpr (is_float_type<DataType>())
		ippsSub_32f_I (dataToSubtract, dataAndDest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsSub_64f_I (dataToSubtract, dataAndDest, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsSub_32s_ISfs (dataToSubtract, dataAndDest, static_cast<int> (size), integer_scale_factor);
	else
	{
	}
}

template <Scalar DataType, Integral SizeType>
void subtractAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToSubtract)
{
	if constexpr (is_float_type<DataType>())
		ippsSubC_32f (origData, constantToSubtract, dest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsSubC_64f (origData, constantToSubtract, dest, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsSubC_32s_Sfs (origData, constantToSubtract, dest, static_cast<int> (size), integer_scale_factor);
	else
	{
	}
}

template <Scalar DataType, Integral SizeType>
void subtractAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToSubtract)
{
	if constexpr (is_float_type<DataType>())
		ippsSub_32f (origData, dataToSubtract, dest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsSub_64f (origData, dataToSubtract, dest, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsSub_32s_Sfs (origData, dataToSubtract, dest, static_cast<int> (size), integer_scale_factor);
	else
	{
	}
}


/*-----  MULTIPLICATION  -----*/

template <Scalar DataType, Integral SizeType>
void multiply (DataType* const data, SizeType size, DataType constantToMultiply)
{
	if constexpr (is_float_type<DataType>())
		ippsMulC_32f_I (constantToMultiply, data, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsMulC_64f_I (constantToMultiply, data, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsMulC_32s_ISfs (onstantToMultiply, data, static_cast<int> (size), integer_scale_factor);
	else
	{
	}
}

template <Scalar DataType, Integral SizeType>
void multiply (DataType* const dataAndDest, SizeType size, const DataType* const dataToMultiply)
{
	if constexpr (is_float_type<DataType>())
		ippsMul_32f_I (dataToMultiply, dataAndDest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsMul_64f_I (dataToMultiply, dataAndDest, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsMul_32s_ISfs (dataToMultiply, dataAndDest, static_cast<int> (size), integer_scale_factor);
	else
	{
	}
}

template <Scalar DataType, Integral SizeType>
void multiplyAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToMultiply)
{
	if constexpr (is_float_type<DataType>())
		ippsMulC_32f (origData, constantToMultiply, dest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsMulC_64f (origData, constantToMultiply, dest, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsMulC_32s_Sfs (origData, constantToMultiply, dest, static_cast<int> (size), integer_scale_factor);
	else
	{
	}
}

template <Scalar DataType, Integral SizeType>
void multiplyAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToMultiply)
{
	if constexpr (is_float_type<DataType>())
		ippsMul_32f (origData, dataToMultiply, dest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsMul_64f (origData, dataToMultiply, dest, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsMul_32s_Sfs (origData, dataToMultiply, dest, static_cast<int> (size), integer_scale_factor);
	else
	{
	}
}


/*-----  DIVISION  -----*/

template <Scalar DataType, Integral SizeType>
void divide (DataType* const data, SizeType size, DataType constantToDivide)
{
	if constexpr (is_float_type<DataType>())
		ippsDivC_32f_I (constantToDivide, data, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsDivC_64f_I (constantToDivide, data, static_cast<int> (size));
	else
	{
	}
}

template <Scalar DataType, Integral SizeType>
void divide (DataType* const dataAndDest, SizeType size, const DataType* const dataToDivide)
{
	if constexpr (is_float_type<DataType>())
		ippsDiv_32f_I (dataToDivide, dataAndDest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsDiv_64f_I (dataToDivide, dataAndDest, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsDiv_32s_ISfs (dataToDivide, dataAndDest, static_cast<int> (size), integer_scale_factor);
	else
	{
	}
}

template <Scalar DataType, Integral SizeType>
void divideAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToDivide)
{
	if constexpr (is_float_type<DataType>())
		ippsDivC_32f (origData, constantToDivide, dest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsDivC_64f (origData, constantToDivide, dest, static_cast<int> (size));
	else
	{
	}
}

template <Scalar DataType, Integral SizeType>
void divideAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToDivide)
{
	if constexpr (is_float_type<DataType>())
		ippsDiv_32f (origData, dataToDivide, dest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsDiv_64f (origData, dataToDivide, dest, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsDiv_32s_Sfs (origData, dataToDivide, dest, static_cast<int> (size), integer_scale_factor);
	else
	{
	}
}


/*---------------------------------------------------------------------------------------------------------------------------*/


template <Scalar DataType, Integral SizeType>
void square (DataType* const dataAndDest, SizeType size)
{
	if constexpr (is_float_type<DataType>())
		ippsSqr_32f_I (dataAndDest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsSqr_64f_I (dataAndDest, static_cast<int> (size));
	else
	{
	}
}

template <Scalar DataType, Integral SizeType>
void square (DataType* const dest, const DataType* const data, SizeType size)
{
	if constexpr (is_float_type<DataType>())
		ippsSqr_32f (data, dest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsSqr_64f (data, dest, static_cast<int> (size));
	else
	{
	}
}

template <Scalar DataType, Integral SizeType>
void squareRoot (DataType* const dataAndDest, SizeType size)
{
	if constexpr (is_float_type<DataType>())
		ippsSqrt_32f_I (dataAndDest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsSqrt_64f_I (dataAndDest, static_cast<int> (size));
	else
	{
	}
}

template <Scalar DataType, Integral SizeType>
void squareRoot (DataType* const dest, const DataType* const data, SizeType size)
{
	if constexpr (is_float_type<DataType>())
		ippsSqrt_32f (data, dest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsSqrt_64f (data, dest, static_cast<int> (size));
	else
	{
	}
}


/*---------------------------------------------------------------------------------------------------------------------------*/


template <Scalar DataType, Integral SizeType>
void reverse (DataType* const dataAndDest, SizeType size)
{
	if constexpr (is_float_type<DataType>())
		ippsFlip_32f_I (dataAndDest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsFlip_64f_I (dataAndDest, static_cast<int> (size));
	else
	{
	}
}

template <Scalar DataType, Integral SizeType>
void reverse (DataType* const dest, const DataType* const data, SizeType size)
{
	if constexpr (is_float_type<DataType>())
		ippsFlip_32f (data, dest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsFlip_64f (data, dest, static_cast<int> (size));
	else
	{
	}
}

template <Scalar DataType, Integral SizeType>
void sort (DataType* const dataAndDest, SizeType size)
{
	if constexpr (is_float_type<DataType>())
		ippsSortAscend_32f_I (dataAndDest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsSortAscend_64f_I (dataAndDest, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsSortAscend_32s_I (dataAndDest, static_cast<int> (size));
	else
	{
	}
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
	if constexpr (is_float_type<DataType>())
		ippsSortDescend_32f_I (dataAndDest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsSortDescend_64f_I (dataAndDest, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsSortDescend_32s_I (dataAndDest, static_cast<int> (size));
	else
	{
	}
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
	if constexpr (is_float_type<DataType>())
		ippsAbs_32f_I (dataAndDest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsAbs_64f_I (dataAndDest, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsAbs_32s_I (dataAndDest, static_cast<int> (size));
	else
	{
	}
}

template <Scalar DataType, Integral SizeType>
void abs (DataType* const dest, const DataType* const data, SizeType size)
{
	if constexpr (is_float_type<DataType>())
		ippsAbs_32f (data, dest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsAbs_64f (data, dest, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsAbs_32s (data, dest, static_cast<int> (size));
	else
	{
	}
}


template <Scalar DataType, Integral SizeType>
void negate (DataType* const dataAndDest, SizeType size)
{
	multiply (dataAndDest, size, DataType (-1));
}

template <Scalar DataType, Integral SizeType>
void negate (DataType* const dest, const DataType* const data, SizeType size)
{
	multiplyAndCopy (dest, data, size, DataType (-1));
}


template <Scalar DataType, Integral SizeType>
void clip (DataType* const dataAndDest, SizeType size, DataType lowClip, DataType hiClip)
{
	if constexpr (is_float_type<DataType>())
	{
		ippsThreshold_LT_32f_I (dataAndDest, static_cast<int> (size), lowClip);
		ippsThreshold_GT_32f_I (dataAndDest, static_cast<int> (size), hiClip);
	}
	else if constexpr (is_double_type<DataType>())
	{
		ippsThreshold_LT_64f_I (dataAndDest, static_cast<int> (size), lowClip);
		ippsThreshold_GT_64f_I (dataAndDest, static_cast<int> (size), hiClip);
	}
	else if constexpr (is_signed_int<DataType>())
	{
		ippsThreshold_LT_32s_I (dataAndDest, static_cast<int> (size), lowClip);
		ippsThreshold_GT_32s_I (dataAndDest, static_cast<int> (size), hiClip);
	}
	else
	{
	}
}

template <Scalar DataType, Integral SizeType>
void clip (DataType* const dest, const DataType* const data, SizeType size, DataType lowClip, DataType hiClip)
{
	if constexpr (is_float_type<DataType>())
	{
		ippsThreshold_LT_32f (data, dest, static_cast<int> (size), lowClip);
		ippsThreshold_GT_32f (data, dest, static_cast<int> (size), hiClip);
	}
	else if constexpr (is_double_type<DataType>())
	{
		ippsThreshold_LT_64f (data, dest, static_cast<int> (size), lowClip);
		ippsThreshold_GT_64f (data, dest, static_cast<int> (size), hiClip);
	}
	else if constexpr (is_signed_int<DataType>())
	{
		ippsThreshold_LT_32s (data, dest, static_cast<int> (size), lowClip);
		ippsThreshold_GT_32s (data, dest, static_cast<int> (size), hiClip);
	}
	else
	{
	}
}


template <Scalar DataType, Integral SizeType>
DataType max (const DataType* const data, SizeType size)
{
	DataType maxVal { 0 };

	if constexpr (is_float_type<DataType>())
		ippsMax_32f (data, static_cast<int> (size), &maxVal);
	else if constexpr (is_double_type<DataType>())
		ippsMax_64f (data, static_cast<int> (size), &maxVal);
	else if constexpr (is_signed_int<DataType>())
		ippsMax_32s (data, static_cast<int> (size), &maxVal);
	else
	{
	}

	return maxVal;
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
void max (const DataType* const data, SizeType size, DataType& maxValue, IndexType& maxIndex)
{
	int maxIdx { 0 };

	if constexpr (is_float_type<DataType>())
		ippsMaxIndx_32f (data, static_cast<int> (size), &maxValue, &maxIdx);
	else if constexpr (is_double_type<DataType>())
		ippsMaxIndx_64f (data, static_cast<int> (size), &maxValue, &maxIdx);
	else if constexpr (is_signed_int<DataType>())
		ippsMaxIndx_32s (data, static_cast<int> (size), &maxValue, &maxIdx);
	else
	{
	}

	maxIndex = static_cast<IndexType> (maxIdx);
}

template <Scalar DataType, Integral SizeType>
DataType maxAbs (const DataType* const data, SizeType size)
{
	DataType maxVal { 0 };

	if constexpr (is_float_type<DataType>())
		ippsMaxAbs_32f (data, static_cast<int> (size), &maxVal);
	else if constexpr (is_double_type<DataType>())
		ippsMaxAbs_64f (data, static_cast<int> (size), &maxVal);
	else if constexpr (is_signed_int<DataType>())
		ippsMaxAbs_32s (data, static_cast<int> (size), &maxVal);
	else
	{
	}

	return maxVal;
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
void maxAbs (const DataType* const data, SizeType size, DataType& maxValue, IndexType& maxIndex)
{
	int maxIdx { 0 };

	if constexpr (is_float_type<DataType>())
		ippsMaxAbsIndx_32f (data, static_cast<int> (size), &maxValue, &maxIdx);
	else if constexpr (is_double_type<DataType>())
		ippsMaxAbsIndx_64f (data, static_cast<int> (size), &maxValue, &maxIdx);
	else if constexpr (is_signed_int<DataType>())
		ippsMaxAbsIndx_32s (data, static_cast<int> (size), &maxValue, &maxIdx);
	else
	{
	}

	maxIndex = static_cast<IndexType> (maxIdx);
}

template <Scalar DataType, Integral SizeType>
DataType min (const DataType* const data, SizeType size)
{
	DataType minVal { 0 };

	if constexpr (is_float_type<DataType>())
		ippsMin_32f (data, static_cast<int> (size), &minVal);
	else if constexpr (is_double_type<DataType>())
		ippsMin_64f (data, static_cast<int> (size), &minVal);
	else if constexpr (is_signed_int<DataType>())
		ippsMin_32s (data, static_cast<int> (size), &minVal);
	else
	{
	}

	return minVal;
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
void min (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex)
{
	int minIdx { 0 };

	if constexpr (is_float_type<DataType>())
		ippsMinIndx_32f (data, static_cast<int> (size), &minValue, &minIdx);
	else if constexpr (is_double_type<DataType>())
		ippsMinIndx_64f (data, static_cast<int> (size), &minValue, &minIdx);
	else if constexpr (is_signed_int<DataType>())
		ippsMinIndx_32s (data, static_cast<int> (size), &minValue, &minIdx);
	else
	{
	}

	minIndex = static_cast<IndexType> (minIdx);
}

template <Scalar DataType, Integral SizeType>
DataType minAbs (const DataType* const data, SizeType size)
{
	DataType minVal { 0 };

	if constexpr (is_float_type<DataType>())
		ippsMinAbs_32f (data, static_cast<int> (size), &minVal);
	else if constexpr (is_double_type<DataType>())
		ippsMinAbs_64f (data, static_cast<int> (size), &minVal);
	else if constexpr (is_signed_int<DataType>())
		ippsMinAbs_32s (data, static_cast<int> (size), &minVal);
	else
	{
	}

	return minVal;
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
void minAbs (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex)
{
	int minIdx { 0 };

	if constexpr (is_float_type<DataType>())
		ippsMinAbsIndx_32f (data, static_cast<int> (size), &minValue, &minIdx);
	else if constexpr (is_double_type<DataType>())
		ippsMinAbsIndx_64f (data, static_cast<int> (size), &minValue, &minIdx);
	else if constexpr (is_signed_int<DataType>())
		ippsMinAbsIndx_32s (data, static_cast<int> (size), &minValue, &minIdx);
	else
	{
	}

	minIndex = static_cast<IndexType> (minIdx);
}

template <Scalar DataType, Integral SizeType>
DataType sum (const DataType* const data, SizeType size)
{
	DataType sumVal { 0 };

	if constexpr (is_float_type<DataType>())
		ippsSum_32f (data, static_cast<int> (size), &sumVal, alg_hint_type);
	else if constexpr (is_double_type<DataType>())
		ippsSum_64f (data, static_cast<int> (size), &sumVal);
	else if constexpr (is_signed_int<DataType>())
		ippsSum_32s_Sfs (data, static_cast<int> (size), &sumVal, integer_scale_factor);
	else
	{
	}

	return sumVal;
}

template <Scalar DataType, Integral SizeType>
DataType mean (const DataType* const data, SizeType size)
{
	DataType meanVal { 0 };

	if constexpr (is_float_type<DataType>())
		ippsMean_32f (data, static_cast<int> (size), &meanVal, alg_hint_type);
	else if constexpr (is_double_type<DataType>())
		ippsMean_64f (data, static_cast<int> (size), &meanVal);
	else if constexpr (is_signed_int<DataType>())
		ippsMean_32s_Sfs (data, static_cast<int> (size), &meanVal, integer_scale_factor);
	else
	{
	}

	return meanVal;
}


/*---------------------------------------------------------------------------------------------------------------------------*/


template <Scalar DataType, Integral SizeType>
void generateRamp (DataType* const output, SizeType size, DataType startValue, DataType endValue)
{
}

namespace window
{

template <Scalar DataType, Integral SizeType>
void generateBlackman (DataType* const output, SizeType size)
{
	constexpr auto alpha = DataType (0.16);

	if constexpr (is_float_type<DataType>())
	{
		fill (output, size, DataType (1));
		ippsWinBlackman_32f_I (output, static_cast<int> (size), alpha);
	}
	else if constexpr (is_double_type<DataType>())
	{
		fill (output, size, DataType (1));
		ippsWinBlackman_64f_I (output, static_cast<int> (size), alpha);
	}
	else
	{
	}
}

template <Scalar DataType, Integral SizeType>
void generateHamm (DataType* const output, SizeType size)
{
	if constexpr (is_float_type<DataType>())
	{
		fill (output, size, DataType (1));
		ippsWinHamming_32f_I (output, static_cast<int> (size));
	}
	else if constexpr (is_double_type<DataType>())
	{
		fill (output, size, DataType (1));
		ippsWinHamming_64f_I (output, static_cast<int> (size));
	}
	else
	{
	}
}

template <Scalar DataType, Integral SizeType>
void generateHanning (DataType* const output, SizeType size)
{
	if constexpr (is_float_type<DataType>())
	{
		fill (output, size, DataType (1));
		ippsWinHann_32f_I (output, static_cast<int> (size));
	}
	else if constexpr (is_double_type<DataType>())
	{
		fill (output, size, DataType (1));
		ippsWinHann_64f_I (output, static_cast<int> (size));
	}
	else
	{
	}
}

}  // namespace window

}  // namespace lemons::vecops
