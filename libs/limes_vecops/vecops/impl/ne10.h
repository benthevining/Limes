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
// IWYU pragma: friend vecops.h

#pragma once

#include "./fallback_impl.h"
#include <limes_namespace.h>
#include <type_traits>
#include <cstdint>
#include <NE10.h>

/** @file
	This file contains implementations of the vecops functions for the NE10 library.
	@ingroup limes_vecops
	@see limes_vecops.h
 */

LIMES_BEGIN_NAMESPACE

namespace vecops
{

static_assert (isUsingNE10());

/// @cond

#pragma mark Basic functions

template <Scalar DataType, Integral SizeType>
void fill (DataType* const data, SizeType size, DataType constantToFill) noexcept
{
	if constexpr (std::is_same_v<DataType, float>)
		ne10_setc_float (data, constantToFill, static_cast<std::uint32_t> (size));
	else
		fb::fill (data, size, constantToFill);
}

template <Scalar DataType, Integral SizeType>
void clear (DataType* const data, SizeType size) noexcept
{
	fill (data, size, DataType (0));
}

template <Scalar DataType, Integral SizeType>
void copy (DataType* const dest, const DataType* const source, SizeType size) noexcept
{
	fb::copy (dest, source, size);
}

template <Scalar DataType, Integral SizeType>
void swap (DataType* const vecA, DataType* const vecB, SizeType size) noexcept
{
	fb::swap (vecA, vecB, size);
}


/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark -
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

#pragma mark Addition

/*-----  ADDITION  -----*/

template <Scalar DataType, Integral SizeType>
void add (DataType* const data, SizeType size, DataType constantToAdd) noexcept
{
	if constexpr (std::is_same_v<DataType, float>)
		ne10_addc_float (data, data, constantToAdd, static_cast<std::uint32_t> (size));
	else
		fb::add (data, size, constantToAdd);
}

template <Scalar DataType, Integral SizeType>
void add (DataType* const dataAndDest, SizeType size, const DataType* const dataToAdd) noexcept
{
	if constexpr (std::is_same_v<DataType, float>)
		ne10_add_float (dataAndDest, dataAndDest, dataToAdd, static_cast<std::uint32_t> (size));
	else
		fb::add (dataAndDest, size, dataToAdd);
}

template <Scalar DataType, Integral SizeType>
void addAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToAdd) noexcept
{
	if constexpr (std::is_same_v<DataType, float>)
		ne10_addc_float (dest, origData, constantToAdd, static_cast<std::uint32_t> (size));
	else
		fb::addAndCopy (dest, origData, size, constantToAdd);
}

template <Scalar DataType, Integral SizeType>
void addAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToAdd) noexcept
{
	if constexpr (std::is_same_v<DataType, float>)
		ne10_add_float (dest, origData, dataToAdd, static_cast<std::uint32_t> (size));
	else
		fb::addAndCopy (dest, origData, size, dataToAdd);
}

#pragma mark Subtraction

/*-----  SUBTRACTION  -----*/

template <Scalar DataType, Integral SizeType>
void subtract (DataType* const data, SizeType size, DataType constantToSubtract) noexcept
{
	if constexpr (std::is_same_v<DataType, float>)
		ne10_subc_float (data, data, constantToSubtract, static_cast<std::uint32_t> (size));
	else
		fb::subtract (data, size, constantToSubtract);
}

template <Scalar DataType, Integral SizeType>
void subtract (DataType* const dataAndDest, SizeType size, const DataType* const dataToSubtract) noexcept
{
	if constexpr (std::is_same_v<DataType, float>)
		ne10_sub_float (dataAndDest, dataAndDest, dataToSubtract, static_cast<std::uint32_t> (size));
	else
		fb::subtract (dataAndDest, size, dataToSubtract);
}

template <Scalar DataType, Integral SizeType>
void subtractAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToSubtract) noexcept
{
	if constexpr (std::is_same_v<DataType, float>)
		ne10_subc_float (dest, origData, constantToSubtract, static_cast<std::uint32_t> (size));
	else
		fb::subtractAndCopy (dest, origData, size, constantToSubtract);
}

template <Scalar DataType, Integral SizeType>
void subtractAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToSubtract) noexcept
{
	if constexpr (std::is_same_v<DataType, float>)
		ne10_sub_float (dest, origData, dataToSubtract, static_cast<std::uint32_t> (size));
	else
		fb::subtractAndCopy (dest, origData, size, dataToSubtract);
}

template <Scalar DataType, Integral SizeType>
void subtractInv (DataType* const data, SizeType size, DataType constantToSubtractFrom) noexcept
{
	if constexpr (std::is_same_v<DataType, float>)
		ne10_rsbc_float (data, data, constantToSubtractFrom, static_cast<std::uint32_t> (size));
	else
		fb::subtractInv (data, size, constantToSubtractFrom);
}

template <Scalar DataType, Integral SizeType>
void subtractInvAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToSubtractFrom) noexcept
{
	if constexpr (std::is_same_v<DataType, float>)
		ne10_rsbc_float (dest, origData, constantToSubtractFrom, static_cast<std::uint32_t> (size));
	else
		fb::subtractInvAndCopy (dest, origData, size, constantToSubtractFrom);
}

#pragma mark Multiplication

/*-----  MULTIPLICATION  -----*/

template <Scalar DataType, Integral SizeType>
void multiply (DataType* const data, SizeType size, DataType constantToMultiply) noexcept
{
	if constexpr (std::is_same_v<DataType, float>)
		ne10_mulc_float (data, data, constantToMultiply, static_cast<std::uint32_t> (size));
	else
		fb::multiply (data, size, constantToMultiply);
}

template <Scalar DataType, Integral SizeType>
void multiply (DataType* const dataAndDest, SizeType size, const DataType* const dataToMultiply) noexcept
{
	if constexpr (std::is_same_v<DataType, float>)
		ne10_mul_float (dataAndDest, dataAndDest, dataToMultiply, static_cast<std::uint32_t> (size));
	else
		fb::multiply (dataAndDest, size, dataToMultiply);
}

template <Scalar DataType, Integral SizeType>
void multiplyAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToMultiply) noexcept
{
	if constexpr (std::is_same_v<DataType, float>)
		ne10_mulc_float (dest, origData, constantToMultiply, static_cast<std::uint32_t> (size));
	else
		fb::multiplyAndCopy (dest, origData, size, constantToMultiply);
}

template <Scalar DataType, Integral SizeType>
void multiplyAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToMultiply) noexcept
{
	if constexpr (std::is_same_v<DataType, float>)
		ne10_mul_float (dest, origData, dataToMultiply, static_cast<std::uint32_t> (size));
	else
		fb::multiplyAndCopy (dest, origData, size, dataToMultiply);
}

template <Scalar DataType, Integral SizeType>
DataType dotProduct (const DataType* const vecA, const DataType* const vecB, SizeType size) noexcept
{
	return fb::dotProduct (vecA, vecB, size);
}

#pragma mark Division

/*-----  DIVISION  -----*/

template <Scalar DataType, Integral SizeType>
void divide (DataType* const data, SizeType size, DataType constantToDivide) noexcept
{
	if constexpr (std::is_same_v<DataType, float>)
		ne10_divc_float (data, data, constantToDivide, static_cast<std::uint32_t> (size));
	else
		fb::divide (data, size, constantToDivide);
}

template <Scalar DataType, Integral SizeType>
void divide (DataType* const dataAndDest, SizeType size, const DataType* const dataToDivide) noexcept
{
	if constexpr (std::is_same_v<DataType, float>)
		ne10_div_float (dataAndDest, dataAndDest, dataToDivide, static_cast<std::uint32_t> (size));
	else
		fb::divide (dataAndDest, size, dataToDivide);
}

template <Scalar DataType, Integral SizeType>
void divideAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToDivide) noexcept
{
	if constexpr (std::is_same_v<DataType, float>)
		ne10_divc_float (dest, origData, constantToDivide, static_cast<std::uint32_t> (size));
	else
		fb::divideAndCopy (dest, origData, size, constantToDivide);
}

template <Scalar DataType, Integral SizeType>
void divideAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToDivide) noexcept
{
	if constexpr (std::is_same_v<DataType, float>)
		ne10_div_float (dest, origData, dataToDivide, static_cast<std::uint32_t> (size));
	else
		fb::divideAndCopy (dest, origData, size, dataToDivide);
}

template <Scalar DataType, Integral SizeType>
void divideInv (DataType* const data, SizeType size, DataType constantToDivideFrom) noexcept
{
	fb::divideInv (data, size, constantToDivideFrom);
}

template <Scalar DataType, Integral SizeType>
void divideInvAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToDivideFrom) noexcept
{
	fb::divideInvAndCopy (dest, origData, size, constantToDivideFrom);
}


/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark -
#pragma mark Squaring functions

template <Scalar DataType, Integral SizeType>
void square (DataType* const dataAndDest, SizeType size) noexcept
{
	fb::square (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void squareAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	fb::squareAndCopy (dest, data, size);
}

template <Scalar DataType, Integral SizeType>
void squareRoot (DataType* const dataAndDest, SizeType size) noexcept
{
	fb::squareRoot (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void squareRootAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
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
	fb::cubeRoot (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void cubeRootAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	fb::cubeRootAndCopy (dest, data, size);
}

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark -
#pragma mark Sorting and ordering functions

template <Scalar DataType, Integral SizeType>
void reverse (DataType* const dataAndDest, SizeType size) noexcept
{
	fb::reverse (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void reverseAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	fb::reverseAndCopy (dest, data, size);
}

template <Scalar DataType, Integral SizeType>
void sort (DataType* const dataAndDest, SizeType size) noexcept
{
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

#pragma mark -
#pragma mark Statistical functions

template <Scalar DataType, Integral SizeType>
void abs (DataType* const dataAndDest, SizeType size) noexcept
{
	if constexpr (std::is_same_v<DataType, float>)
		ne10_abs_float (dataAndDest, dataAndDest, static_cast<std::uint32_t> (size));
	else
		fb::abs (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void absAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	if constexpr (std::is_same_v<DataType, float>)
		ne10_abs_float (dest, data, static_cast<std::uint32_t> (size));
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
	fb::clip (dataAndDest, size, lowClip, hiClip);
}

template <Scalar DataType, Integral SizeType>
void clipAndCopy (DataType* const dest, const DataType* const data, SizeType size, DataType lowClip, DataType hiClip) noexcept
{
	fb::clipAndCopy (dest, data, size, lowClip, hiClip);
}


template <Scalar DataType, Integral SizeType>
DataType max (const DataType* const data, SizeType size) noexcept
{
	return fb::max (data, size);
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
void max (const DataType* const data, SizeType size, DataType& maxValue, IndexType& maxIndex) noexcept
{
	fb::max (data, size, maxValue, maxIndex);
}

template <Scalar DataType, Integral SizeType>
DataType maxAbs (const DataType* const data, SizeType size) noexcept
{
	return fb::maxAbs (data, size);
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
void maxAbs (const DataType* const data, SizeType size, DataType& maxValue, IndexType& maxIndex) noexcept
{
	fb::maxAbs (data, size, maxValue, maxIndex);
}

template <Scalar DataType, Integral SizeType>
DataType min (const DataType* const data, SizeType size) noexcept
{
	return fb::min (data, size);
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
void min (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex) noexcept
{
	fb::min (data, size, minValue, minIndex);
}

template <Scalar DataType, Integral SizeType>
DataType minAbs (const DataType* const data, SizeType size) noexcept
{
	return fb::minAbs (data, size);
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
void minAbs (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex) noexcept
{
	fb::minAbs (data, size, minValue, minIndex);
}

template <Scalar DataType, Integral SizeType>
void minMax (const DataType* const data, SizeType size, DataType& minValue, DataType& maxValue) noexcept
{
	fb::minMax (data, size, minValue, maxValue);
}

template <Scalar DataType, Integral SizeType, Integral IndexType>
void minMax (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex, DataType& maxValue, IndexType& maxIndex) noexcept
{
	fb::minMax (data, size, minValue, minIndex, maxValue, maxIndex);
}

template <Scalar DataType, Integral SizeType>
void minMaxAbs (const DataType* const data, SizeType size, DataType& minValue, DataType& maxValue) noexcept
{
	fb::minMaxAbs (data, size, minValue, maxValue);
}

template <Scalar DataType, Integral SizeType>
DataType sum (const DataType* const data, SizeType size) noexcept
{
	return fb::sum (data, size);
}

template <Scalar DataType, Integral SizeType>
DataType mean (const DataType* const data, SizeType size) noexcept
{
	return fb::mean (data, size);
}

template <Scalar DataType, Integral SizeType>
DataType standard_deviation (const DataType* const data, SizeType size) noexcept
{
	return fb::standard_deviation (data, size);
}

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark -
#pragma mark Trigonometric functions

template <Scalar DataType, Integral SizeType>
void sinCos (const DataType* const data, SizeType size, DataType* const sinesOut, DataType* const cosinesOut) noexcept
{
	fb::sinCos (data, size, sinesOut, cosinesOut);
}

#pragma mark Sine

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

#pragma mark Cosine

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

#pragma mark Tangent

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
	fb::arctan (data, size);
}

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void arctanAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	fb::arctanAndCopy (dest, data, size);
}

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark -
#pragma mark Exponential and logarithmic functions

#pragma mark Logarithms

namespace log
{

template <Scalar DataType, Integral SizeType>
void nat (DataType* const data, SizeType size) noexcept
{
	fb::log::nat (data, size);
}

template <Scalar DataType, Integral SizeType>
void natAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
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

#pragma mark Exponents

namespace exp
{

template <Scalar DataType, Integral SizeType>
void e (DataType* const data, SizeType size) noexcept
{
	fb::exp::e (data, size);
}

template <Scalar DataType, Integral SizeType>
void eAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
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

#pragma mark -
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
	return fb::countZeroCrossings (data, size);
}

template <Scalar DataType, Integral SizeType>
void generateRamp (DataType* const output, SizeType size, DataType startValue, DataType endValue) noexcept
{
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

#pragma mark -
#pragma mark Windowing functions

namespace window
{

#pragma mark Blackman

/* --- Blackman --- */

template <Scalar DataType, Integral SizeType>
void generateBlackman (DataType* const output, SizeType size) noexcept
{
	fb::window::generateBlackman (output, size);
}

template <Scalar DataType, Integral SizeType>
void applyBlackman (DataType* const dataAndDest, SizeType size) noexcept
{
	fb::window::applyBlackman (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void applyBlackmanAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	fb::window::applyBlackmanAndCopy (dest, data, size);
}

#pragma mark Hamm

/* --- Hamm --- */

template <Scalar DataType, Integral SizeType>
void generateHamm (DataType* const output, SizeType size) noexcept
{
	fb::window::generateHamm (output, size);
}

template <Scalar DataType, Integral SizeType>
void applyHamm (DataType* const dataAndDest, SizeType size) noexcept
{
	fb::window::generateHamm (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void applyHammAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	fb::window::applyHammAndCopy (dest, data, size);
}

#pragma mark Hanning

/* --- Hanning --- */

template <Scalar DataType, Integral SizeType>
void generateHanning (DataType* const output, SizeType size) noexcept
{
	fb::window::generateHanning (output, size);
}

template <Scalar DataType, Integral SizeType>
void applyHanning (DataType* const dataAndDest, SizeType size) noexcept
{
	fb::window::applyHanning (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void applyHanningAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	fb::window::applyHanningAndCopy (dest, data, size);
}

}  // namespace window

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark -
#pragma mark Complex conversions

template <Scalar DataType, Integral SizeType>
void polarToCartesian (DataType* const real, DataType* const imag, const DataType* const mag, const DataType* const phase, SizeType size) noexcept
{
	fb::polarToCartesian (real, imag, mag, phase, size);
}

template <Scalar DataType, Integral SizeType>
void polarToCartesianInterleaved (DataType* const dest, const DataType* const mag, const DataType* const phase, SizeType size) noexcept
{
	fb::polarToCartesianInterleaved (dest, mag, phase, size);
}

template <Scalar DataType, Integral SizeType>
void cartesianToPolar (DataType* const mag, DataType* const phase, const DataType* const real, const DataType* const imag, SizeType size) noexcept
{
	fb::cartesianToPolar (mag, phase, real, imag, size);
}

template <Scalar DataType, Integral SizeType>
void catesianInterleavedToPolar (DataType* const mag, DataType* const phase, const DataType* const src, SizeType size) noexcept
{
	fb::catesianInterleavedToPolar (mag, phase, src, size);
}

template <Scalar DataType, Integral SizeType>
void cartesianToMagnitudes (DataType* const mag, const DataType* const real, const DataType* const imag, SizeType size) noexcept
{
	fb::cartesianToMagnitudes (mag, real, imag, size);
}

template <Scalar DataType, Integral SizeType>
void cartesianInterleavedToMagnitudes (DataType* const mag, const DataType* const src, SizeType size) noexcept
{
	fb::cartesianInterleavedToMagnitudes (mag, src, size);
}

/// @endcond

}  // namespace vecops

LIMES_END_NAMESPACE
