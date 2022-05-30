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
	This file contains the Pommier SIMD extension functions for SSE.

	@warning I've had trouble compiling these implementations with MSVC, and apparently MSVC compiling MMX intrinsics is a known issue.
	Prefer another implementation when compiling with MSVC.

	@ingroup vec_pommier
	@see pommier_wrapper.h

	@par The original license:
	SIMD (SSE1+MMX or SSE2) implementation of sin, cos, exp and log \n
	Inspired by Intel Approximate Math library, and based on the corresponding algorithms of the cephes math library \n
	The default is to use the SSE1 version. If you define USE_SSE2 the the SSE2 intrinsics will be used in place of the MMX intrinsics.
	Do not expect any significant performance improvement with SSE2. \n
	Copyright (C) 2007  Julien Pommier \n
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
#if ! LIMES_SSE
#	error
#endif
/// @endcond

#include <xmmintrin.h>

namespace pommier
{

/// @cond

#if LIMES_MSVC
#	define ALIGN16_BEG __declspec(align (16))
#	define ALIGN16_END
#else
#	define ALIGN16_BEG
#	define ALIGN16_END __attribute__ ((aligned (16)))
#endif

using v4sf = __m128;  // vector of 4 float (sse1)

#ifdef USE_SSE2
#	include <emmintrin.h>
using v4si = __m128i;  // vector of 4 int (sse2)
#else
using v2si = __m64;	 // vector of 2 int (mmx)
#endif

#if defined(__MINGW32__)

/* the ugly part below: many versions of gcc used to be completely buggy with respect to some intrinsics
	The movehl_ps is fixed in mingw 3.4.5, but I found out that all the _mm_cmp* intrinsics were completely
	broken on my mingw gcc 3.4.5 ...

	Note that the bug on _mm_cmp* does occur only at -O0 optimization level
*/

inline __m128 my_movehl_ps (__m128 a, const __m128 b)
{
	asm(
		"movhlps %2,%0\n\t"
		: "=x"(a)
		: "0"(a), "x"(b));
	return a;
}
#	warning "redefined _mm_movehl_ps (see gcc bug 21179)"
#	define _mm_movehl_ps my_movehl_ps

inline __m128 my_cmplt_ps (__m128 a, const __m128 b)
{
	asm(
		"cmpltps %2,%0\n\t"
		: "=x"(a)
		: "0"(a), "x"(b));
	return a;
}

inline __m128 my_cmpgt_ps (__m128 a, const __m128 b)
{
	asm(
		"cmpnleps %2,%0\n\t"
		: "=x"(a)
		: "0"(a), "x"(b));
	return a;
}

inline __m128 my_cmpeq_ps (__m128 a, const __m128 b)
{
	asm(
		"cmpeqps %2,%0\n\t"
		: "=x"(a)
		: "0"(a), "x"(b));
	return a;
}
#	warning "redefined _mm_cmpxx_ps functions..."
#	define _mm_cmplt_ps my_cmplt_ps
#	define _mm_cmpgt_ps my_cmpgt_ps
#	define _mm_cmpeq_ps my_cmpeq_ps

#endif /* __MINGW32__ */


/** natural logarithm computed for 4 simultaneous floats
	@returns NaN for x <= 0
*/
v4sf log_ps (v4sf x);


v4sf exp_ps (v4sf x);


/** Evaluation of 4 sines at once, using only SSE1+MMX intrinsics so
	it runs also on old athlons XPs and the pentium III of your grandmother.

	The code is the exact rewriting of the cephes sinf function.
	Precision is excellent as long as x < 8192 (I did not bother to
	take into account the special handling they have for greater values
	-- it does not return garbage for arguments over 8192, though, but
	the extra precision is missing).

	Note that it is such that \c sinf((float)M_PI)=8.74e-8, which is the
	surprising but correct result.

	Performance is also surprisingly good, 1.33 times faster than the
	macos vsinf SSE2 function, and 1.5 times faster than the
	__vrs4_sinf of amd's ACML (which is only available in 64 bits). Not
	too bad for an SSE1 function (with no special tuning) !
	However the latter libraries probably have a much better handling of NaN,
	Inf, denormalized and other special arguments..

	On my core 1 duo, the execution of this function takes approximately 95 cycles.

	From what I have observed on the experiments with Intel AMath lib, switching to an
	SSE2 version would improve the perf by only 10%.

	Since it is based on SSE intrinsics, it has to be compiled at -O2 to
	deliver full speed.
*/
v4sf sin_ps (v4sf x);


/** almost the same as sin_ps */
v4sf cos_ps (v4sf x);

/** Since sin_ps and cos_ps are almost identical, sincos_ps could replace both of them.
	It is almost as fast, and gives you a free cosine with your sine */
void sincos_ps (v4sf x, v4sf* s, v4sf* c);

/// @endcond

}  // namespace pommier
