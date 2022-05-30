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

/** @file
	This file contains the Pommier SIMD extension functions for ARM NEON.
	@ingroup vec_pommier
	@see pommier_wrapper.h

	@par The original license:
	NEON implementation of sin, cos, exp and log \n
	Inspired by Intel Approximate Math library, and based on the corresponding algorithms of the cephes math library \n
	Copyright (C) 2011  Julien Pommier \n
	This software is provided 'as-is', without any express or implied warranty.
	In no event will the authors be held liable for any damages arising from the use of this software. \n
	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions: \n
	1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software.
	If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required. \n
	2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software. \n
	3. This notice may not be removed or altered from any source distribution. \n
	(this is the zlib license)
 */

#pragma once

#include <limes_platform.h>

/// @cond
#if ! LIMES_ARM_NEON
#	error
#endif
/// @endcond

#include <arm_neon.h>
#include <limes_export.h>

/** This namespace contains the implementations of the Pommier SIMD extension functions.
	@see vecops::pommier
	@ingroup vec_pommier
 */
namespace pommier
{

/// @cond internals

/** A vector of 4 floats. */
using v4sf = float32x4_t;

/** A vector of 4 uint32s. */
using v4su = uint32x4_t;

/** A vector of 4 uint32s. */
using v4si = int32x4_t;


/** Natural logarithm computed for 4 simultaneous floats
	@return NaN for x <= 0
*/
LIMES_NO_EXPORT v4sf log_ps (v4sf x);


/** %exp() computed for 4 floats at once */
LIMES_NO_EXPORT v4sf exp_ps (v4sf x);


/** Evaluation of 4 sines & cosines at once.

	The code is the exact rewriting of the cephes sinf function.
	Precision is excellent as long as x < 8192 (I did not bother to
	take into account the special handling they have for greater values
	-- it does not return garbage for arguments over 8192, though, but
	the extra precision is missing).

	Note that it is such that \c sinf((float)M_PI)=8.74e-8 , which is the
	surprising but correct result.

	Note also that when you compute \c sin(x) , \c cos(x) is available at
	almost no extra price so both sin_ps and cos_ps make use of
	sincos_ps..
*/
LIMES_NO_EXPORT void sincos_ps (v4sf x, v4sf* ysin, v4sf* ycos);

LIMES_NO_EXPORT v4sf sin_ps (v4sf x);

LIMES_NO_EXPORT v4sf cos_ps (v4sf x);

/// @endcond

}  // namespace pommier
