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
// IWYU pragma: friend limes_vecops.h

#pragma once

#include <ipps.h>
#include <limits>
#include <type_traits>
#include <limes_vecops.h>
#include "./fallback_impl.h"
#include <limes_namespace.h>

/** @file
	This file contains implementations of the vecops functions for the Intel IPP library.
	@ingroup limes_vecops
	@see limes_vecops.h
 */

LIMES_BEGIN_NAMESPACE

namespace vecops
{

/// @cond

static_assert (isUsingIPP());


template <typename DataType>
using is_float_type = std::bool_constant<std::is_same_v<DataType, float>>;

template <typename DataType>
using is_double_type = std::bool_constant<std::is_same_v<DataType, double>>;

template <typename DataType>
using is_signed_int = std::bool_constant<std::is_integral_v<DataType> && std::is_signed_v<DataType> && std::numeric_limits<DataType>::digits == 32>;

template <typename DataType>
using is_unsigned_int = std::bool_constant<std::is_integral_v<DataType> && ! std::is_signed_v<DataType> && std::numeric_limits<DataType>::digits == 32>;


LIMES_NO_EXPORT static constexpr auto integer_scale_factor = 0;
LIMES_NO_EXPORT static constexpr auto alg_hint_type		   = ippAlgHintFast;

#pragma mark Basic functions

template <Scalar DataType, Integral SizeType>
void fill (DataType* const data, SizeType size, DataType constantToFill) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsSet_32f (constantToFill, data, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsSet_64f (constantToFill, data, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsSet_32s (constantToFill, data, static_cast<int> (size));
	else
		fb::fill (data, size, constantToFill);
}

template <Scalar DataType, Integral SizeType>
void clear (DataType* const data, SizeType size) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsZero_32f (data, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsZero_64f (data, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsZero_32s (data, static_cast<int> (size));
	else
		fill (data, size, DataType (0));
}

template <Scalar DataType, Integral SizeType>
void copy (DataType* const dest, const DataType* const source, SizeType size) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsCopy_32f (source, dest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsCopy_64f (source, dest, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsCopy_32s (source, dest, static_cast<int> (size));
	else
		fb::copy (dest, source, size);
}

template <Scalar DataType, Integral SizeType>
void swap (DataType* const vecA, DataType* const vecB, SizeType size) noexcept
{
	fb::swap (vecA, vecB, size);
}


/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark Arithmetic functions

template <Scalar DataType, Integral SizeType>
void recip (DataType* const data, SizeType size) noexcept
{
	fb::recip (data, size);
}

template <Scalar DataType, Integral SizeType>
void recipAndCopy (DataType* const dest, const DataType* const origData, SizeType size) noexcept
{
	fb::recipAndCopy (dest, origData, size);
}

/*-----  ADDITION  -----*/

template <Scalar DataType, Integral SizeType>
void add (DataType* const data, SizeType size, DataType constantToAdd) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsAddC_32f_I (constantToAdd, data, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsAddC_64f_I (constantToAdd, data, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsAddC_32s_ISfs (constantToAdd, data, static_cast<int> (size), integer_scale_factor);
	else
		fb::add (data, size, constantToAdd);
}

template <Scalar DataType, Integral SizeType>
void add (DataType* const dataAndDest, SizeType size, const DataType* const dataToAdd) noexcept
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
		fb::add (dataAndDest, size, dataToAdd);
}

template <Scalar DataType, Integral SizeType>
void addAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToAdd) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsAddC_32f (origData, constantToAdd, dest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsAddC_64f (origData, constantToAdd, dest, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsAddC_32s_Sfs (origData, constantToAdd, dest, static_cast<int> (size), integer_scale_factor);
	else
		fb::addAndCopy (dest, origData, size, constantToAdd);
}

template <Scalar DataType, Integral SizeType>
void addAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToAdd) noexcept
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
		fb::addAndCopy (dest, origData, size, dataToAdd);
}


/*-----  SUBTRACTION  -----*/

template <Scalar DataType, Integral SizeType>
void subtract (DataType* const data, SizeType size, DataType constantToSubtract) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsSubC_32f_I (constantToSubtract, data, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsSubC_64f_I (constantToSubtract, data, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsSubC_32s_ISfs (constantToSubtract, data, static_cast<int> (size), integer_scale_factor);
	else
		fb::subtract (data, size, constantToSubtract);
}

template <Scalar DataType, Integral SizeType>
void subtract (DataType* const dataAndDest, SizeType size, const DataType* const dataToSubtract) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsSub_32f_I (dataToSubtract, dataAndDest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsSub_64f_I (dataToSubtract, dataAndDest, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsSub_32s_ISfs (dataToSubtract, dataAndDest, static_cast<int> (size), integer_scale_factor);
	else
		fb::subtract (dataAndDest, size, dataToSubtract);
}

template <Scalar DataType, Integral SizeType>
void subtractAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToSubtract) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsSubC_32f (origData, constantToSubtract, dest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsSubC_64f (origData, constantToSubtract, dest, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsSubC_32s_Sfs (origData, constantToSubtract, dest, static_cast<int> (size), integer_scale_factor);
	else
		fb::subtractAndCopy (dest, origData, size, constantToSubtract);
}

template <Scalar DataType, Integral SizeType>
void subtractAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToSubtract) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsSub_32f (origData, dataToSubtract, dest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsSub_64f (origData, dataToSubtract, dest, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsSub_32s_Sfs (origData, dataToSubtract, dest, static_cast<int> (size), integer_scale_factor);
	else
		fb::subtractAndCopy (dest, origData, size, dataToSubtract);
}

template <Scalar DataType, Integral SizeType>
void subtractInv (DataType* const data, SizeType size, DataType constantToSubtractFrom) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsSubCRev_32f_I (constantToSubtractFrom, data, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsSubCRev_64f_I (constantToSubtractFrom, data, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsSubCRev_32s_I (constantToSubtractFrom, data, static_cast<int> (size), integer_scale_factor);
	else
		fb::subtractInv (data, size, constantToSubtractFrom);
}

template <Scalar DataType, Integral SizeType>
void subtractInvAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToSubtractFrom) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsSubCRev_32f (origData, constantToSubtractFrom, dest, size);
	else if constexpr (is_double_type<DataType>())
		ippsSubCRev_64f (origData, constantToSubtractFrom, dest, size);
	else if constexpr (is_signed_int<DataType>())
		ippsSubCRev_32s_Sfs (origData, constantToSubtractFrom, dest, size, integer_scale_factor);
	else
		fb::subtractInvAndCopy (dest, origData, size, constantToSubtractFrom);
}


/*-----  MULTIPLICATION  -----*/

template <Scalar DataType, Integral SizeType>
void multiply (DataType* const data, SizeType size, DataType constantToMultiply) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsMulC_32f_I (constantToMultiply, data, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsMulC_64f_I (constantToMultiply, data, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsMulC_32s_ISfs (constantToMultiply, data, static_cast<int> (size), integer_scale_factor);
	else
		fb::multiply (data, size, constantToMultiply);
}

template <Scalar DataType, Integral SizeType>
void multiply (DataType* const dataAndDest, SizeType size, const DataType* const dataToMultiply) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsMul_32f_I (dataToMultiply, dataAndDest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsMul_64f_I (dataToMultiply, dataAndDest, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsMul_32s_ISfs (dataToMultiply, dataAndDest, static_cast<int> (size), integer_scale_factor);
	else
		fb::multiply (dataAndDest, size, dataToMultiply);
}

template <Scalar DataType, Integral SizeType>
void multiplyAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToMultiply) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsMulC_32f (origData, constantToMultiply, dest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsMulC_64f (origData, constantToMultiply, dest, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsMulC_32s_Sfs (origData, constantToMultiply, dest, static_cast<int> (size), integer_scale_factor);
	else
		fb::multiplyAndCopy (dest, origData, size, constantToMultiply);
}

template <Scalar DataType, Integral SizeType>
void multiplyAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToMultiply) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsMul_32f (origData, dataToMultiply, dest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsMul_64f (origData, dataToMultiply, dest, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsMul_32s_Sfs (origData, dataToMultiply, dest, static_cast<int> (size), integer_scale_factor);
	else
		fb::multiplyAndCopy (dest, origData, size, dataToMultiply);
}

template <Scalar DataType, Integral SizeType>
DataType dotProduct (const DataType* const vecA, const DataType* const vecB, SizeType size) noexcept
{
	[[maybe_unused]] DataType dotProd { 0 };

	if constexpr (is_float_type<DataType>())
		ippsDotProd_32f (vecA, vecB, static_cast<int> (size), &dotProd);
	else if constexpr (is_double_type<DataType>())
		ippsDotProd_64f (vecA, vecB, static_cast<int> (size), &dotProd);
	else if constexpr (is_signed_int<DataType>())
		ippsDotProd_32s_Sfs (vecA, vecB, static_cast<int> (size), &dotProd, integer_scale_factor);
	else
		return fb::dotProduct (vecA, vecB, size);

	return dotProd;
}


/*-----  DIVISION  -----*/

template <Scalar DataType, Integral SizeType>
void divide (DataType* const data, SizeType size, DataType constantToDivide) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsDivC_32f_I (constantToDivide, data, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsDivC_64f_I (constantToDivide, data, static_cast<int> (size));
	else
		fb::divide (data, size, constantToDivide);
}

template <Scalar DataType, Integral SizeType>
void divide (DataType* const dataAndDest, SizeType size, const DataType* const dataToDivide) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsDiv_32f_I (dataToDivide, dataAndDest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsDiv_64f_I (dataToDivide, dataAndDest, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsDiv_32s_ISfs (dataToDivide, dataAndDest, static_cast<int> (size), integer_scale_factor);
	else
		fb::divide (dataAndDest, size, dataToDivide);
}

template <Scalar DataType, Integral SizeType>
void divideAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToDivide) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsDivC_32f (origData, constantToDivide, dest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsDivC_64f (origData, constantToDivide, dest, static_cast<int> (size));
	else
		fb::divideAndCopy (dest, origData, size, constantToDivide);
}

template <Scalar DataType, Integral SizeType>
void divideAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToDivide) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsDiv_32f (origData, dataToDivide, dest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsDiv_64f (origData, dataToDivide, dest, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsDiv_32s_Sfs (origData, dataToDivide, dest, static_cast<int> (size), integer_scale_factor);
	else
		fb::divideAndCopy (dest, origData, size, dataToDivide);
}

template <Scalar DataType, Integral SizeType>
void divideInv (DataType* const data, SizeType size, DataType constantToDivideFrom) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsDivCRev_32f_I (constantToDivideFrom, data, static_cast<int> (size));
	else
		fb::divideInv (data, size, constantToDivideFrom);
}

template <Scalar DataType, Integral SizeType>
void divideInvAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToDivideFrom) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsDivCRev_32f (origData, constantToDivideFrom, dest, size);
	else
		fb::divideInvAndCopy (dest, origData, size, constantToDivideFrom);
}


/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark Squaring functions

template <Scalar DataType, Integral SizeType>
void square (DataType* const dataAndDest, SizeType size) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsSqr_32f_I (dataAndDest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsSqr_64f_I (dataAndDest, static_cast<int> (size));
	else
		fb::square (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void squareAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsSqr_32f (data, dest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsSqr_64f (data, dest, static_cast<int> (size));
	else
		fb::squareAndCopy (dest, data, size);
}

template <Scalar DataType, Integral SizeType>
void squareRoot (DataType* const dataAndDest, SizeType size) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsSqrt_32f_I (dataAndDest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsSqrt_64f_I (dataAndDest, static_cast<int> (size));
	else
		fb::squareRoot (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void squareRootAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsSqrt_32f (data, dest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsSqrt_64f (data, dest, static_cast<int> (size));
	else
		fb::squareRootAndCopy (dest, data, size);
}

template <Scalar DataType, Integral SizeType>
void invSquareRoot (DataType* const dataAndDest, SizeType size) noexcept
{
	fb::invSquareRoot (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void invSquareRootAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	fb::invSquareRootAndCopy (dest, data, size);
}

template <Scalar DataType, Integral SizeType>
void cubeRoot (DataType* const dataAndDest, SizeType size) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsCubrt_32f (dataAndDest, dataAndDest, static_cast<int> (size));
	else
		fb::cubeRoot (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void cubeRootAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsCubrt_32f (data, dest, static_cast<int> (size));
	else
		fb::cubeRootAndCopy (dest, data, size);
}

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark Sorting and ordering functions

template <Scalar DataType, Integral SizeType>
void reverse (DataType* const dataAndDest, SizeType size) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsFlip_32f_I (dataAndDest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsFlip_64f_I (dataAndDest, static_cast<int> (size));
	else
		fb::reverse (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void reverseAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsFlip_32f (data, dest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsFlip_64f (data, dest, static_cast<int> (size));
	else
		fb::reverseAndCopy (dest, data, size);
}

template <Scalar DataType, Integral SizeType>
void sort (DataType* const dataAndDest, SizeType size) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsSortAscend_32f_I (dataAndDest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsSortAscend_64f_I (dataAndDest, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsSortAscend_32s_I (dataAndDest, static_cast<int> (size));
	else
		fb::sort (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void sortAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	fb::sortAndCopy (dest, data, size);
}

template <Scalar DataType, Integral SizeType>
void sortReverse (DataType* const dataAndDest, SizeType size) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsSortDescend_32f_I (dataAndDest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsSortDescend_64f_I (dataAndDest, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsSortDescend_32s_I (dataAndDest, static_cast<int> (size));
	else
		fb::sortReverse (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void sortReverseAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	fb::sortReverseAndCopy (dest, data, size);
}

template <Scalar DataType, Integral SizeType1, Integral SizeType2>
void interleave (DataType* const output, const DataType* const * const origData, SizeType1 numChannels, SizeType2 numSamples) noexcept
{
	fb::interleave (output, origData, numChannels, numSamples);
}

template <Scalar DataType, Integral SizeType1, Integral SizeType2>
void deinterleave (DataType* const * const output, const DataType* const interleavedData, SizeType1 numChannels, SizeType2 numSamples) noexcept
{
	fb::deinterleave (output, interleavedData, numChannels, numSamples);
}


/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark Statistical functions

template <Scalar DataType, Integral SizeType>
void abs (DataType* const dataAndDest, SizeType size) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsAbs_32f_I (dataAndDest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsAbs_64f_I (dataAndDest, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsAbs_32s_I (dataAndDest, static_cast<int> (size));
	else
		fb::abs (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void absAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsAbs_32f (data, dest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsAbs_64f (data, dest, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsAbs_32s (data, dest, static_cast<int> (size));
	else
		fb::absAndCopy (dest, data, size);
}


template <Scalar DataType, Integral SizeType>
void negate (DataType* const dataAndDest, SizeType size) noexcept
{
	fb::negate (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void negateAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	fb::negateAndCopy (dest, data, size);
}


template <Scalar DataType, Integral SizeType>
void clip (DataType* const dataAndDest, SizeType size, DataType lowClip, DataType hiClip) noexcept
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
		fb::clip (dataAndDest, size, lowClip, hiClip);
	}
}

template <Scalar DataType, Integral SizeType>
void clipAndCopy (DataType* const dest, const DataType* const data, SizeType size, DataType lowClip, DataType hiClip) noexcept
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
		fb::clipAndCopy (dest, data, size, lowClip, hiClip);
	}
}


template <Scalar DataType, Integral SizeType>
DataType max (const DataType* const data, SizeType size) noexcept
{
	[[maybe_unused]] DataType maxVal { 0 };

	if constexpr (is_float_type<DataType>())
		ippsMax_32f (data, static_cast<int> (size), &maxVal);
	else if constexpr (is_double_type<DataType>())
		ippsMax_64f (data, static_cast<int> (size), &maxVal);
	else if constexpr (is_signed_int<DataType>())
		ippsMax_32s (data, static_cast<int> (size), &maxVal);
	else
		return fb::max (data, size);

	return maxVal;
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
void max (const DataType* const data, SizeType size, DataType& maxValue, IndexType& maxIndex) noexcept
{
	[[maybe_unused]] int maxIdx { 0 };

	if constexpr (is_float_type<DataType>())
		ippsMaxIndx_32f (data, static_cast<int> (size), &maxValue, &maxIdx);
	else if constexpr (is_double_type<DataType>())
		ippsMaxIndx_64f (data, static_cast<int> (size), &maxValue, &maxIdx);
	else if constexpr (is_signed_int<DataType>())
		ippsMaxIndx_32s (data, static_cast<int> (size), &maxValue, &maxIdx);
	else
	{
		fb::max (data, size, maxValue, maxIndex);
		return;
	}

	maxIndex = static_cast<IndexType> (maxIdx);
}

template <Scalar DataType, Integral SizeType>
DataType maxAbs (const DataType* const data, SizeType size) noexcept
{
	[[maybe_unused]] DataType maxVal { 0 };

	if constexpr (is_float_type<DataType>())
		ippsMaxAbs_32f (data, static_cast<int> (size), &maxVal);
	else if constexpr (is_double_type<DataType>())
		ippsMaxAbs_64f (data, static_cast<int> (size), &maxVal);
	else if constexpr (is_signed_int<DataType>())
		ippsMaxAbs_32s (data, static_cast<int> (size), &maxVal);
	else
		return fb::maxAbs (data, size);

	return maxVal;
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
void maxAbs (const DataType* const data, SizeType size, DataType& maxValue, IndexType& maxIndex) noexcept
{
	[[maybe_unused]] int maxIdx { 0 };

	if constexpr (is_float_type<DataType>())
		ippsMaxAbsIndx_32f (data, static_cast<int> (size), &maxValue, &maxIdx);
	else if constexpr (is_double_type<DataType>())
		ippsMaxAbsIndx_64f (data, static_cast<int> (size), &maxValue, &maxIdx);
	else if constexpr (is_signed_int<DataType>())
		ippsMaxAbsIndx_32s (data, static_cast<int> (size), &maxValue, &maxIdx);
	else
	{
		fb::maxAbs (data, size, maxValue, maxIndex);
		return;
	}

	maxIndex = static_cast<IndexType> (maxIdx);
}

template <Scalar DataType, Integral SizeType>
DataType min (const DataType* const data, SizeType size) noexcept
{
	[[maybe_unused]] DataType minVal { 0 };

	if constexpr (is_float_type<DataType>())
		ippsMin_32f (data, static_cast<int> (size), &minVal);
	else if constexpr (is_double_type<DataType>())
		ippsMin_64f (data, static_cast<int> (size), &minVal);
	else if constexpr (is_signed_int<DataType>())
		ippsMin_32s (data, static_cast<int> (size), &minVal);
	else
		return fb::min (data, size);

	return minVal;
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
void min (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex) noexcept
{
	[[maybe_unused]] int minIdx { 0 };

	if constexpr (is_float_type<DataType>())
		ippsMinIndx_32f (data, static_cast<int> (size), &minValue, &minIdx);
	else if constexpr (is_double_type<DataType>())
		ippsMinIndx_64f (data, static_cast<int> (size), &minValue, &minIdx);
	else if constexpr (is_signed_int<DataType>())
		ippsMinIndx_32s (data, static_cast<int> (size), &minValue, &minIdx);
	else
	{
		fb::min (data, size, minValue, minIndex);
		return;
	}

	minIndex = static_cast<IndexType> (minIdx);
}

template <Scalar DataType, Integral SizeType>
DataType minAbs (const DataType* const data, SizeType size) noexcept
{
	[[maybe_unused]] DataType minVal { 0 };

	if constexpr (is_float_type<DataType>())
		ippsMinAbs_32f (data, static_cast<int> (size), &minVal);
	else if constexpr (is_double_type<DataType>())
		ippsMinAbs_64f (data, static_cast<int> (size), &minVal);
	else if constexpr (is_signed_int<DataType>())
		ippsMinAbs_32s (data, static_cast<int> (size), &minVal);
	else
		return fb::minAbs (data, size);

	return minVal;
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
void minAbs (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex) noexcept
{
	[[maybe_unused]] int minIdx { 0 };

	if constexpr (is_float_type<DataType>())
		ippsMinAbsIndx_32f (data, static_cast<int> (size), &minValue, &minIdx);
	else if constexpr (is_double_type<DataType>())
		ippsMinAbsIndx_64f (data, static_cast<int> (size), &minValue, &minIdx);
	else if constexpr (is_signed_int<DataType>())
		ippsMinAbsIndx_32s (data, static_cast<int> (size), &minValue, &minIdx);
	else
	{
		fb::minAbs (data, size, minValue, minIndex);
		return;
	}

	minIndex = static_cast<IndexType> (minIdx);
}

template <Scalar DataType, Integral SizeType>
void minMax (const DataType* const data, SizeType size, DataType& minValue, DataType& maxValue) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsMinMax_32f (data, static_cast<int> (size), &minValue, &maxValue);
	else if constexpr (is_double_type<DataType>())
		ippsMinMax_64f (data, static_cast<int> (size), &minValue, &maxValue);
	else if constexpr (is_signed_int<DataType>())
		ippsMinMax_32s (data, static_cast<int> (size), &minValue, &maxValue);
	else if constexpr (is_unsigned_int<DataType>())
		ippsMinMax_32u (data, static_cast<int> (size), &minValue, &maxValue);
	else
		fb::minMax (data, size, minValue, maxValue);
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
void minMax (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex, DataType& maxValue, IndexType& maxIndex) noexcept
{
	int minIdx { 0 }, maxIdx { 0 };

	if constexpr (is_float_type<DataType>())
		ippsMinMaxIndx_32f (data, static_cast<int> (size), &minValue, &minIdx, &maxValue, &maxIdx);
	else if constexpr (is_double_type<DataType>())
		ippsMinMaxIndx_64f (data, static_cast<int> (size), &minValue, &minIdx, &maxValue, &maxIdx);
	else if constexpr (is_signed_int<DataType>())
		ippsMinMaxIndx_32s (data, static_cast<int> (size), &minValue, &minIdx, &maxValue, &maxIdx);
	else if constexpr (is_unsigned_int<DataType>())
		ippsMinMaxIndx_32u (data, static_cast<int> (size), &minValue, &minIdx, &maxValue, &maxIdx);
	else
	{
		fb::minMax (data, size, minValue, minIndex, maxValue, maxIndex);
		return;
	}

	minIndex = static_cast<IndexType> (minIdx);
	maxIndex = static_cast<IndexType> (maxIdx);
}

template <Scalar DataType, Integral SizeType>
void minMaxAbs (const DataType* const data, SizeType size, DataType& minValue, DataType& maxValue) noexcept
{
	fb::minMaxAbs (data, size, minValue, maxValue);
}

template <Scalar DataType, Integral SizeType>
DataType sum (const DataType* const data, SizeType size) noexcept
{
	[[maybe_unused]] DataType sumVal { 0 };

	if constexpr (is_float_type<DataType>())
		ippsSum_32f (data, static_cast<int> (size), &sumVal, alg_hint_type);
	else if constexpr (is_double_type<DataType>())
		ippsSum_64f (data, static_cast<int> (size), &sumVal);
	else if constexpr (is_signed_int<DataType>())
		ippsSum_32s_Sfs (data, static_cast<int> (size), &sumVal, integer_scale_factor);
	else
		return fb::sum (data, size);

	return sumVal;
}

template <Scalar DataType, Integral SizeType>
DataType mean (const DataType* const data, SizeType size) noexcept
{
	[[maybe_unused]] DataType meanVal { 0 };

	if constexpr (is_float_type<DataType>())
		ippsMean_32f (data, static_cast<int> (size), &meanVal, alg_hint_type);
	else if constexpr (is_double_type<DataType>())
		ippsMean_64f (data, static_cast<int> (size), &meanVal);
	else if constexpr (is_signed_int<DataType>())
		ippsMean_32s_Sfs (data, static_cast<int> (size), &meanVal, integer_scale_factor);
	else
		return fb::mean (data, size);

	return meanVal;
}

template <Scalar DataType, Integral SizeType>
DataType standard_deviation (const DataType* const data, SizeType size) noexcept
{
	[[maybe_unused]] DataType result;

	if constexpr (is_float_type<DataType>())
		ippsStdDev_32f (data, static_cast<int> (size), &result, alg_hint_type);
	else if constexpr (is_double_type<DataType>())
		ippsStdDev_64f (data, static_cast<int> (size), &result, alg_hint_type);
	else
		return fb::standard_deviation (data, size);

	return result;
}

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark Trigonometric functions

template <Scalar DataType, Integral SizeType>
void sinCos (const DataType* const data, SizeType size, DataType* const sinesOut, DataType* const cosinesOut) noexcept
{
	fb::sinCos (data, size, sinesOut, cosinesOut);
}

/* --- sin --- */

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void sine (DataType* const data, SizeType size) noexcept
{
	fb::sine (data, size);
}

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void sineAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	fb::sineAndCopy (dest, data, size);
}

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void arcsine (DataType* const data, SizeType size) noexcept
{
	fb::arcsine (data, size);
}

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void arcsineAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	fb::arcsineAndCopy (dest, data, size);
}

/* --- cos --- */

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void cos (DataType* const data, SizeType size) noexcept
{
	fb::cos (data, size);
}

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void cosAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	fb::cosAndCopy (dest, data, size);
}

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void arccos (DataType* const data, SizeType size) noexcept
{
	fb::arccos (data, size);
}

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void arccosAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	fb::arccosAndCopy (dest, data, size);
}

/* --- tan --- */

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void tan (DataType* const data, SizeType size) noexcept
{
	fb::tan (data, size);
}

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void tanAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	fb::tanAndCopy (dest, data, size);
}

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void arctan (DataType* const data, SizeType size) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsArctan_32f_I (data, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsArctan_64f_I (data, static_cast<int> (size));
	else
		fb::arctan (data, size);
}

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void arctanAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsArctan_32f (data, dest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsArctan_64f (data, dest, static_cast<int> (size));
	else
		fb::arctanAndCopy (dest, data, size);
}

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark Exponential and logarithmic functions

namespace log
{

template <Scalar DataType, Integral SizeType>
void nat (DataType* const data, SizeType size) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsLn_32f_I (data, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsLn_64f_I (data, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsLn_32s_ISfs (data, static_cast<int> (size), integer_scale_factor);
	else
		fb::log::nat (data, size);
}

template <Scalar DataType, Integral SizeType>
void natAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsLn_32f (data, dest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsLn_64f (data, dest, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsLn_32s_Sfs (data, dest, static_cast<int> (size), integer_scale_factor);
	else
		fb::log::natAndCopy (dest, data, size);
}

template <Scalar DataType, Integral SizeType>
void base2 (DataType* const data, SizeType size) noexcept
{
	fb::log::base2 (data, size);
}

template <Scalar DataType, Integral SizeType>
void base2AndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	fb::log::base2AndCopy (dest, data, size);
}

template <Scalar DataType, Integral SizeType>
void base10 (DataType* const data, SizeType size) noexcept
{
	fb::log::base10 (data, size);
}

template <Scalar DataType, Integral SizeType>
void base10AndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	fb::log::base10AndCopy (dest, data, size);
}

}  // namespace log

namespace exp
{

template <Scalar DataType, Integral SizeType>
void e (DataType* const data, SizeType size) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsExp_32f_I (data, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsExp_64f_I (data, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsExp_32s_ISfs (data, static_cast<int> (size), integer_scale_factor);
	else
		fb::exp::e (data, size);
}

template <Scalar DataType, Integral SizeType>
void eAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsExp_32f (data, dest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsExp_64f (data, dest, static_cast<int> (size));
	else if constexpr (is_signed_int<DataType>())
		ippsExp_32s_Sfs (data, dest, static_cast<int> (size), integer_scale_factor);
	else
		fb::exp::eAndCopy (dest, data, size);
}

template <Scalar DataType, Integral SizeType>
void two (DataType* const data, SizeType size) noexcept
{
	fb::exp::two (data, size);
}

template <Scalar DataType, Integral SizeType>
void twoAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	fb::exp::twoAndCopy (dest, data, size);
}

template <Scalar DataType, Integral SizeType>
void pow (DataType* const dataAndDest, const DataType* const exponents, SizeType size) noexcept
{
	fb::exp::pow (dataAndDest, exponents, size);
}

template <Scalar DataType, Integral SizeType>
void powAndCopy (DataType* const dest, const DataType* const data, const DataType* const exponents, SizeType size) noexcept
{
	fb::exp::powAndCopy (dest, data, exponents, size);
}

}  // namespace exp

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark Audio utility functions

template <Scalar DataType, Integral SizeType1, Integral SizeType2>
void mix (DataType* const output, const DataType* const * const origData, SizeType1 numChannels, SizeType2 numSamples) noexcept
{
	fb::mix (output, origData, numChannels, numSamples);
}

template <Scalar DataType, Integral SizeType>
DataType rms (const DataType* const data, SizeType size) noexcept
{
	return fb::rms (data, size);
}

template <Scalar DataType, Integral SizeType>
int countZeroCrossings (const DataType* const data, SizeType size) noexcept
{
	if constexpr (is_float_type<DataType>())
	{
		float numZeroCrossings;
		ippsZeroCrossing_32f (data, static_cast<int> (size), &numZeroCrossings, ippZCR);
		return static_cast<int> (numZeroCrossings);
	}
	else
	{
		return fb::countZeroCrossings (data, size);
	}
}

template <Scalar DataType, Integral SizeType>
void generateRamp (DataType* const output, SizeType size, DataType startValue, DataType endValue) noexcept
{
	const auto increment = (endValue - startValue) / static_cast<DataType> (size);

	if constexpr (is_float_type<DataType>())
		ippsVectorSlope_32f (output, static_cast<int> (size), static_cast<DataType> (startValue), increment);
	else if constexpr (is_double_type<DataType>())
		ippsVectorSlope_64f (output, static_cast<int> (size), static_cast<DataType> (startValue), increment);
	else if constexpr (is_signed_int<DataType>())
		ippsVectorSlope_32s (output, static_cast<int> (size), static_cast<DataType> (startValue), increment);
	else if constexpr (is_unsigned_int<DataType>())
		ippsVectorSlope_32u (output, static_cast<int> (size), static_cast<DataType> (startValue), increment);
	else
		fb::generateRamp (output, size, startValue, endValue);
}

template <Scalar DataType, Integral SizeType>
void applyRamp (DataType* const dataAndDest, SizeType size, DataType startValue, DataType endValue) noexcept
{
	fb::applyRamp (dataAndDest, size, startValue, endValue);
}

template <Scalar DataType, Integral SizeType>
void applyRampAndCopy (DataType* const dest, const DataType* const data, SizeType size, DataType startValue, DataType endValue) noexcept
{
	fb::applyRampAndCopy (dest, data, size, startValue, endValue);
}

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark Windowing functions

namespace window
{

/* --- Blackman --- */

template <Scalar DataType, Integral SizeType>
void generateBlackman (DataType* const output, SizeType size) noexcept
{
	if constexpr (is_float_type<DataType>())
	{
		fill (output, size, DataType (1));
		ippsWinBlackman_32f_I (output, static_cast<int> (size), math::constants::blackman_alpha<DataType>);
	}
	else if constexpr (is_double_type<DataType>())
	{
		fill (output, size, DataType (1));
		ippsWinBlackman_64f_I (output, static_cast<int> (size), math::constants::blackman_alpha<DataType>);
	}
	else
	{
		fb::window::generateBlackman (output, size);
	}
}

template <Scalar DataType, Integral SizeType>
void applyBlackman (DataType* const dataAndDest, SizeType size) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsWinBlackman_32f_I (dataAndDest, static_cast<int> (size), math::constants::blackman_alpha<DataType>);
	else if constexpr (is_double_type<DataType>())
		ippsWinBlackman_64f_I (dataAndDest, static_cast<int> (size), math::constants::blackman_alpha<DataType>);
	else
		fb::window::applyBlackman (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void applyBlackmanAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsWinBlackman_32f (data, dest, static_cast<int> (size), math::constants::blackman_alpha<DataType>);
	else if constexpr (is_double_type<DataType>())
		ippsWinBlackman_64f (data, dest, static_cast<int> (size), math::constants::blackman_alpha<DataType>);
	else
		fb::window::applyBlackmanAndCopy (dest, data, size);
}

/* --- Hamm --- */

template <Scalar DataType, Integral SizeType>
void generateHamm (DataType* const output, SizeType size) noexcept
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
		fb::window::generateHamm (output, size);
	}
}

template <Scalar DataType, Integral SizeType>
void applyHamm (DataType* const dataAndDest, SizeType size) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsWinHamming_32f_I (dataAndDest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsWinHamming_64f_I (dataAndDest, static_cast<int> (size));
	else
		fb::window::generateHamm (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void applyHammAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsWinHamming_32f (data, dest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsWinHamming_64f (data, dest, static_cast<int> (size));
	else
		fb::window::applyHammAndCopy (dest, data, size);
}

/* --- Hanning --- */

template <Scalar DataType, Integral SizeType>
void generateHanning (DataType* const output, SizeType size) noexcept
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
		fb::window::generateHanning (output, size);
	}
}

template <Scalar DataType, Integral SizeType>
void applyHanning (DataType* const dataAndDest, SizeType size) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsWinHann_32f_I (dataAndDest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsWinHann_64f_I (dataAndDest, static_cast<int> (size));
	else
		fb::window::applyHanning (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void applyHanningAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsWinHann_32f (data, dest, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsWinHann_64f (data, dest, static_cast<int> (size));
	else
		fb::window::applyHanningAndCopy (dest, data, size);
}

}  // namespace window

/*---------------------------------------------------------------------------------------------------------------------------*/

template <Scalar DataType, Integral SizeType>
void polarToCartesian (DataType* const real, DataType* const imag, const DataType* const mag, const DataType* const phase, SizeType size) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsPolarToCart_32f (mag, phase, real, imag, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsPolarToCart_64f (mag, phase, real, imag, static_cast<int> (size));
	else
		fb::polarToCartesian (real, imag, mag, phase, size);
}

template <Scalar DataType, Integral SizeType>
void polarToCartesianInterleaved (DataType* const dest, const DataType* const mag, const DataType* const phase, SizeType size) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsPolarToCart_32fc (mag, phase, static_cast<Ipp32fc*> (dest), static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsPolarToCart_64fc (mag, phase, static_cast<Ipp64fc*> (dest), static_cast<int> (size));
	else
		fb::polarToCartesianInterleaved (dest, mag, phase, size);
}

template <Scalar DataType, Integral SizeType>
void cartesianToPolar (DataType* const mag, DataType* const phase, const DataType* const real, const DataType* const imag, SizeType size) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsCartToPolar_32f (real, imag, mag, phase, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsCartToPolar_64f (real, imag, mag, phase, static_cast<int> (size));
	else
		fb::cartesianToPolar (mag, phase, real, imag, size);
}

template <Scalar DataType, Integral SizeType>
void catesianInterleavedToPolar (DataType* const mag, DataType* const phase, const DataType* const src, SizeType size) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsCartToPolar_32fc (static_cast<const Ipp32fc*> (src), mag, phase, size);
	else if constexpr (is_double_type<DataType>())
		ippsCartToPolar_64fc (static_cast<const Ipp64fc*> (src), mag, phase, size);
	else
		fb::catesianInterleavedToPolar (mag, phase, src, size);
}

template <Scalar DataType, Integral SizeType>
void cartesianToMagnitudes (DataType* const mag, const DataType* const real, const DataType* const imag, SizeType size) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsMagnitude_32f (real, imag, mag, static_cast<int> (size));
	else if constexpr (is_double_type<DataType>())
		ippsMagnitude_64f (real, imag, mag, static_cast<int> (size));
	else
		fb::cartesianToMagnitudes (mag, real, imag, size);
}

template <Scalar DataType, Integral SizeType>
void cartesianInterleavedToMagnitudes (DataType* const mag, const DataType* const src, SizeType size) noexcept
{
	if constexpr (is_float_type<DataType>())
		ippsMagnitude_32fc (static_cast<const Ipp32fc*> (src), mag, size);
	else if constexpr (is_double_type<DataType>())
		ippsMagnitude_64fc (static_cast<const Ipp64fc*> (src), mag, size);
	else
		fb::cartesianInterleavedToMagnitudes (mag, src, size);
}

/// @endcond

}  // namespace vecops

LIMES_END_NAMESPACE
