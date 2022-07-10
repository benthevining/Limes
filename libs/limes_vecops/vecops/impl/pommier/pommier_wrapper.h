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
// IWYU pragma: friend fallback_impl.h

#pragma once

#include <limes_platform.h>

/// @cond
#if ! (LIMES_ARM_NEON || LIMES_SSE)
#	error
#endif
/// @endcond

#include <limes_export.h>
#include <limes_namespace.h>

/** @defgroup vec_pommier Pommier SIMD extension functions
	Pommier SIMD extension functions.
	This group contains sin and cos SIMD functions written by Julien Pommier for NEON and SSE.
	Limes contains a simple wrapper that forwards function calls to the appropriate Pommier function.
	@ingroup limes_vecops
 */

/** @dir libs/limes_vecops/vecops/impl/pommier
	This directory contains the implementations of the Pommier SIMD extension functions.
	@ingroup vec_pommier
 */

/** @file
	This file is a wrapper around the NEON and SSE Pommier SIMD extension functions.
	@ingroup vec_pommier
	@see sse_mathfun.h, neon_mathfun.h
 */

LIMES_BEGIN_NAMESPACE

/** This namespace contains the Pommier SIMD extension function wrappers, which call the NEON or SSE implementations found in the pommier namespace.
	@see pommier
	@ingroup vec_pommier
 */
namespace vecops::pommier
{

/// @cond internals

LIMES_NO_EXPORT void polarToCartesian (float* const real, float* const imag,
									   const float* const mag, const float* const phase,
									   int size) noexcept;

LIMES_NO_EXPORT void polarToCartesianInterleaved (float* const		 dest,
												  const float* const mag, const float* const phase,
												  int size) noexcept;

LIMES_NO_EXPORT void sine (float* const data, int size) noexcept;

LIMES_NO_EXPORT void sineAndCopy (float* const dest, const float* const data, int size) noexcept;

LIMES_NO_EXPORT void cos (float* const data, int size) noexcept;

LIMES_NO_EXPORT void cosAndCopy (float* const dest, const float* const data, int size) noexcept;

LIMES_NO_EXPORT void sinCos (const float* const data,
							 int				size,
							 float* const sinesOut, float* const cosinesOut) noexcept;

/// @endcond

}  // namespace vecops::pommier

LIMES_END_NAMESPACE
