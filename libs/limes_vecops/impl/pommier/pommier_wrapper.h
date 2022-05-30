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

#include <limes_platform.h>

/// @cond
#if ! (LIMES_ARM_NEON || LIMES_SSE)
#	error
#endif

#if ! LIMES_VECOPS_USE_POMMIER
#	error
#endif
/// @endcond

#include <limes_export.h>
#include <limes_namespace.h>

/// @cond internals

/** @defgroup vec_pommier Pommier SIMD extension functions
	Pommier SIMD extension functions.
	@ingroup limes_vecops
 */

/** @dir libs/limes_vecops/impl/pommier
	This directory contains the implementations of the Pommier SIMD extension functions.
	@ingroup vec_pommier
 */

/** @file
	This file is a wrapper around the NEON and SSE Pommier SIMD extension functions.
	@ingroup vec_pommier
 */

LIMES_BEGIN_NAMESPACE

/** This namespace contains the Pommier SIMD extension function wrappers, which call the NEON or SSE implementations found in the pommier namespace.
	@see pommier
	@ingroup vec_pommier
 */
namespace vecops::pommier
{

LIMES_NO_EXPORT void polarToCartesian (float* const real, float* const imag, const float* const mag, const float* const phase, int size);

LIMES_NO_EXPORT void polarToCartesianInterleaved (float* const dest, const float* const mag, const float* const phase, int size);

LIMES_NO_EXPORT void sine (float* const data, int size);

LIMES_NO_EXPORT void sineAndCopy (float* const dest, const float* const data, int size);

LIMES_NO_EXPORT void cos (float* const data, int size);

LIMES_NO_EXPORT void cosAndCopy (float* const dest, const float* const data, int size);

LIMES_NO_EXPORT void sinCos (const DataType* const data, SizeType size, DataType* const sinesOut, DataType* const cosinesOut);

}  // namespace vecops::pommier

LIMES_END_NAMESPACE

/// @endcond
