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

#include <limes_vecops.h>
#include "fallback_impl.h"
#include <limes_namespace.h>

/** @dir libs/limes_vecops/impl
	This directory contains the various implementations of the vecops functions for each SIMD library.
	@ingroup limes_vecops
 */

/** @file
	This file includes the fallback implementations of the vecops functions.
	All the functions in this file simply wrap the functions in fallback_impl.h, which is also included by the other implementations, to cover any functions their libraries may not provide.
	@ingroup limes_vecops
	@see limes_vecops.h
 */

LIMES_BEGIN_NAMESPACE

namespace vecops
{

/// @cond

#pragma mark Basic functions

static_assert (isUsingFallback());

template <Scalar DataType, Integral SizeType>
void fill (DataType* const data, SizeType size, DataType constantToFill) noexcept
{
	fb::fill (data, size, constantToFill);
}

template <Scalar DataType, Integral SizeType>
void clear (DataType* const data, SizeType size) noexcept
{
	fb::clear (data, size);
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
	fb::add (data, size, constantToAdd);
}

template <Scalar DataType, Integral SizeType>
void add (DataType* const dataAndDest, SizeType size, const DataType* const dataToAdd) noexcept
{
	fb::add (dataAndDest, size, dataToAdd);
}

template <Scalar DataType, Integral SizeType>
void addAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToAdd) noexcept
{
	fb::addAndCopy (dest, origData, size, constantToAdd);
}

template <Scalar DataType, Integral SizeType>
void addAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToAdd) noexcept
{
	fb::addAndCopy (dest, origData, size, dataToAdd);
}


/*-----  SUBTRACTION  -----*/

template <Scalar DataType, Integral SizeType>
void subtract (DataType* const data, SizeType size, DataType constantToSubtract) noexcept
{
	fb::subtract (data, size, constantToSubtract);
}

template <Scalar DataType, Integral SizeType>
void subtract (DataType* const dataAndDest, SizeType size, const DataType* const dataToSubtract) noexcept
{
	fb::subtract (dataAndDest, size, dataToSubtract);
}

template <Scalar DataType, Integral SizeType>
void subtractAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToSubtract) noexcept
{
	fb::subtractAndCopy (dest, origData, size, constantToSubtract);
}

template <Scalar DataType, Integral SizeType>
void subtractAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToSubtract) noexcept
{
	fb::subtractAndCopy (dest, origData, size, dataToSubtract);
}

template <Scalar DataType, Integral SizeType>
void subtractInv (DataType* const data, SizeType size, DataType constantToSubtractFrom) noexcept
{
	fb::subtractInv (data, size, constantToSubtractFrom);
}

template <Scalar DataType, Integral SizeType>
void subtractInvAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToSubtractFrom) noexcept
{
	fb::subtractInvAndCopy (dest, origData, size, constantToSubtractFrom);
}


/*-----  MULTIPLICATION  -----*/

template <Scalar DataType, Integral SizeType>
void multiply (DataType* const data, SizeType size, DataType constantToMultiply) noexcept
{
	fb::multiply (data, size, constantToMultiply);
}

template <Scalar DataType, Integral SizeType>
void multiply (DataType* const dataAndDest, SizeType size, const DataType* const dataToMultiply) noexcept
{
	fb::multiply (dataAndDest, size, dataToMultiply);
}

template <Scalar DataType, Integral SizeType>
void multiplyAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToMultiply) noexcept
{
	fb::multiplyAndCopy (dest, origData, size, constantToMultiply);
}

template <Scalar DataType, Integral SizeType>
void multiplyAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToMultiply) noexcept
{
	fb::multiplyAndCopy (dest, origData, size, dataToMultiply);
}

template <Scalar DataType, Integral SizeType>
DataType dotProduct (const DataType* const vecA, const DataType* const vecB, SizeType size) noexcept
{
	return fb::dotProduct (vecA, vecB, size)
}


/*-----  DIVISION  -----*/

template <Scalar DataType, Integral SizeType>
void divide (DataType* const data, SizeType size, DataType constantToDivide) noexcept
{
	fb::divide (data, size, constantToDivide);
}

template <Scalar DataType, Integral SizeType>
void divide (DataType* const dataAndDest, SizeType size, const DataType* const dataToDivide) noexcept
{
	fb::divide (dataAndDest, size, dataToDivide);
}

template <Scalar DataType, Integral SizeType>
void divideAndCopy (DataType* const dest, const DataType* const origData, SizeType size, DataType constantToDivide) noexcept
{
	fb::divideAndCopy (dest, origData, size, constantToDivide);
}

template <Scalar DataType, Integral SizeType>
void divideAndCopy (DataType* const dest, const DataType* const origData, SizeType size, const DataType* const dataToDivide) noexcept
{
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

#pragma mark Statistical functions

template <Scalar DataType, Integral SizeType>
void abs (DataType* const dataAndDest, SizeType size) noexcept
{
	fb::abs (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void absAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
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
DataType min (const DataType* const data, SizeType size)
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

template <Scalar DataType, Integral SizeType, Integral IndexType>
void minMaxAbs (const DataType* const data, SizeType size, DataType& minValue, IndexType& minIndex, DataType& maxValue, IndexType& maxIndex) noexcept
{
	fb::minMaxAbs (data, size, minValue, minIndex, maxValue, maxIndex);
}

template <Scalar DataType, Integral SizeType>
DataType range (const DataType* const data, SizeType size) noexcept
{
	return fb::range (data, size);
}

template <Scalar DataType, Integral SizeType>
DataType rangeAbs (const DataType* const data, SizeType size) noexcept
{
	return fb::rangeAbs (data, size);
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
DataType median (const DataType* const data, SizeType size) noexcept
{
	return fb::median (data, size);
}

template <Scalar DataType, Integral SizeType>
DataType mode (const DataType* const data, SizeType size) noexcept
{
	return fb::mode (data, size);
}

template <Scalar DataType, Integral SizeType>
DataType standard_deviation (const DataType* const data, SizeType size) noexcept
{
	return fb::standard_deviation (data, size);
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
	fb::sine (data, size);
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
	fb::arctan (data, size);
}

template <Scalar DataType, Integral SizeType>
LIMES_EXPORT void arctanAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	fb::arctanAndCopy (dest, data, size);
}

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark Exponential and logarithmic functions

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
	fb::natAndCopy (dest, data, size);
}

template <Scalar DataType, Integral SizeType>
void base2 (DataType* const data, SizeType size) noexcept
{
	fb::base2 (data, size);
}

template <Scalar DataType, Integral SizeType>
void base2AndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	fb::base2AndCopy (dest, data, size);
}

template <Scalar DataType, Integral SizeType>
void base10 (DataType* const data, SizeType size) noexcept
{
	fb::base10 (data, size);
}

template <Scalar DataType, Integral SizeType>
void base10AndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	fb::base10AndCopy (dest, data, size);
}

}  // namespace log

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

#pragma mark Windowing functions

namespace window
{

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

template <Scalar DataType, Integral SizeType>
void generateHamm (DataType* const output, SizeType size) noexcept
{
	fb::window::generateHamm (output, size);
}

template <Scalar DataType, Integral SizeType>
void applyHamm (DataType* const dataAndDest, SizeType size) noexcept
{
	fb::window::applyHamm (dataAndDest, size);
}

template <Scalar DataType, Integral SizeType>
void applyHammAndCopy (DataType* const dest, const DataType* const data, SizeType size) noexcept
{
	fb::window::applyHammAndCopy (dest, data, size);
}

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
