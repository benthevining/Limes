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

/** @file
	This file is a wrapper around the NEON and SSE Pommier SIMD extension functions.
	@ingroup limes_vecops
 */

/// @cond internals

LIMES_BEGIN_NAMESPACE

namespace vecops::pommier
{

LIMES_NO_EXPORT void polarToCartesian (float* const real, float* const imag, const float* const mag, const float* const phase, int size);

LIMES_NO_EXPORT void polarToCartesianInterleaved (float* const dest, const float* const mag, const float* const phase, int size);

}  // namespace vecops::pommier

LIMES_END_NAMESPACE

/// @endcond
