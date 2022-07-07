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

#include "./sse_mathfun.h"
#include <mmintrin.h>
#include <immintrin.h>

namespace pommier
{

/* declare some SSE constants -- why can't I figure a better way to do that? */
#define _PS_CONST(Name, Val) \
	static constexpr ALIGN16_BEG float _ps_##Name[4] ALIGN16_END = { Val, Val, Val, Val }
#define _PI32_CONST(Name, Val) \
	static constexpr ALIGN16_BEG int _pi32_##Name[4] ALIGN16_END = { Val, Val, Val, Val }
#define _PS_CONST_TYPE(Name, Type, Val) \
	static constexpr ALIGN16_BEG Type _ps_##Name[4] ALIGN16_END = { (Type) Val, (Type) Val, (Type) Val, (Type) Val }

_PS_CONST (1, 1.0f);
_PS_CONST (0p5, 0.5f);
/* the smallest non denormalized float number */
_PS_CONST_TYPE (min_norm_pos, int, 0x00800000);
_PS_CONST_TYPE (mant_mask, int, 0x7f800000);
_PS_CONST_TYPE (inv_mant_mask, int, ~0x7f800000);

_PS_CONST_TYPE (sign_mask, int, 0x80000000);
_PS_CONST_TYPE (inv_sign_mask, int, ~0x80000000);

_PI32_CONST (1, 1);
_PI32_CONST (inv1, ~1);
_PI32_CONST (2, 2);
_PI32_CONST (4, 4);
_PI32_CONST (0x7f, 0x7f);

_PS_CONST (cephes_SQRTHF, 0.707106781186547524);
_PS_CONST (cephes_log_p0, 7.0376836292E-2);
_PS_CONST (cephes_log_p1, -1.1514610310E-1);
_PS_CONST (cephes_log_p2, 1.1676998740E-1);
_PS_CONST (cephes_log_p3, -1.2420140846E-1);
_PS_CONST (cephes_log_p4, +1.4249322787E-1);
_PS_CONST (cephes_log_p5, -1.6668057665E-1);
_PS_CONST (cephes_log_p6, +2.0000714765E-1);
_PS_CONST (cephes_log_p7, -2.4999993993E-1);
_PS_CONST (cephes_log_p8, +3.3333331174E-1);
_PS_CONST (cephes_log_q1, -2.12194440e-4);
_PS_CONST (cephes_log_q2, 0.693359375);

#undef _PI32_CONST
#undef _PS_CONST_TYPE

#ifndef USE_SSE2
typedef union xmm_mm_union
{
	__m128 xmm;
	__m64  mm[2];
} xmm_mm_union;

#	define COPY_XMM_TO_MM(xmm_, mm0_, mm1_) \
		{                                    \
			xmm_mm_union u;                  \
			u.xmm = xmm_;                    \
			mm0_  = u.mm[0];                 \
			mm1_  = u.mm[1];                 \
		}

#	define COPY_MM_TO_XMM(mm0_, mm1_, xmm_) \
		{                                    \
			xmm_mm_union u;                  \
			u.mm[0] = mm0_;                  \
			u.mm[1] = mm1_;                  \
			xmm_	= u.xmm;                 \
		}

#endif	// USE_SSE2


v4sf log_ps (v4sf x) noexcept
{
#ifdef USE_SSE2
	v4si emm0;
#else
	v2si mm0, mm1;
#endif

	x = _mm_max_ps (x, *reinterpret_cast<const v4sf*> (_ps_min_norm_pos)); /* cut off denormalized stuff */

#ifndef USE_SSE2
	/* part 1: x = frexpf(x, &e); */
	COPY_XMM_TO_MM (x, mm0, mm1);
	mm0 = _mm_srli_pi32 (mm0, 23);
	mm1 = _mm_srli_pi32 (mm1, 23);
#else
	emm0   = _mm_srli_epi32 (_mm_castps_si128 (x), 23);
#endif

	/* keep only the fractional part */
	x = _mm_and_ps (x, *reinterpret_cast<const v4sf*> (_ps_inv_mant_mask));
	x = _mm_or_ps (x, *reinterpret_cast<const v4sf*> (_ps_0p5));

#ifndef USE_SSE2
	/* now e=mm0:mm1 contain the really base-2 exponent */
	mm0	   = _mm_sub_pi32 (mm0, *reinterpret_cast<const v2si*> (_pi32_0x7f));
	mm1	   = _mm_sub_pi32 (mm1, *reinterpret_cast<const v2si*> (_pi32_0x7f));
	v4sf e = _mm_cvtpi32x2_ps (mm0, mm1);
	_mm_empty(); /* bye bye mmx */
#else
	emm0   = _mm_sub_epi32 (emm0, *reinterpret_cast<const v4si*> (_pi32_0x7f));
	v4sf e = _mm_cvtepi32_ps (emm0);
#endif

	const v4sf one = *reinterpret_cast<const v4sf*> (_ps_1);

	e = _mm_add_ps (e, one);

	/* part2:
	   if( x < SQRTHF ) {
		 e -= 1;
		 x = x + x - 1.0;
	   } else { x = x - 1.0; }
	*/
	const v4sf mask = _mm_cmplt_ps (x, *reinterpret_cast<const v4sf*> (_ps_cephes_SQRTHF));

	v4sf tmp = _mm_and_ps (x, mask);
	x		 = _mm_sub_ps (x, one);
	e		 = _mm_sub_ps (e, _mm_and_ps (one, mask));
	x		 = _mm_add_ps (x, tmp);

	v4sf y = *reinterpret_cast<const v4sf*> (_ps_cephes_log_p0);

	y = _mm_mul_ps (y, x);
	y = _mm_add_ps (y, *reinterpret_cast<const v4sf*> (_ps_cephes_log_p1));
	y = _mm_mul_ps (y, x);
	y = _mm_add_ps (y, *reinterpret_cast<const v4sf*> (_ps_cephes_log_p2));
	y = _mm_mul_ps (y, x);
	y = _mm_add_ps (y, *reinterpret_cast<const v4sf*> (_ps_cephes_log_p3));
	y = _mm_mul_ps (y, x);
	y = _mm_add_ps (y, *reinterpret_cast<const v4sf*> (_ps_cephes_log_p4));
	y = _mm_mul_ps (y, x);
	y = _mm_add_ps (y, *reinterpret_cast<const v4sf*> (_ps_cephes_log_p5));
	y = _mm_mul_ps (y, x);
	y = _mm_add_ps (y, *reinterpret_cast<const v4sf*> (_ps_cephes_log_p6));
	y = _mm_mul_ps (y, x);
	y = _mm_add_ps (y, *reinterpret_cast<const v4sf*> (_ps_cephes_log_p7));
	y = _mm_mul_ps (y, x);
	y = _mm_add_ps (y, *reinterpret_cast<const v4sf*> (_ps_cephes_log_p8));
	y = _mm_mul_ps (y, x);

	const v4sf z = _mm_mul_ps (x, x);

	y = _mm_mul_ps (y, z);

	tmp = _mm_mul_ps (e, *reinterpret_cast<const v4sf*> (_ps_cephes_log_q1));
	y	= _mm_add_ps (y, tmp);

	tmp = _mm_mul_ps (z, *reinterpret_cast<const v4sf*> (_ps_0p5));
	y	= _mm_sub_ps (y, tmp);

	tmp = _mm_mul_ps (e, *reinterpret_cast<const v4sf*> (_ps_cephes_log_q2));
	x	= _mm_add_ps (x, y);
	x	= _mm_add_ps (x, tmp);

	const v4sf invalid_mask = _mm_cmple_ps (x, _mm_setzero_ps());

	return _mm_or_ps (x, invalid_mask);	 // negative arg will be NAN
}


_PS_CONST (exp_hi, 88.3762626647949f);
_PS_CONST (exp_lo, -88.3762626647949f);

_PS_CONST (cephes_LOG2EF, 1.44269504088896341);
_PS_CONST (cephes_exp_C1, 0.693359375);
_PS_CONST (cephes_exp_C2, -2.12194440e-4);

_PS_CONST (cephes_exp_p0, 1.9875691500E-4);
_PS_CONST (cephes_exp_p1, 1.3981999507E-3);
_PS_CONST (cephes_exp_p2, 8.3334519073E-3);
_PS_CONST (cephes_exp_p3, 4.1665795894E-2);
_PS_CONST (cephes_exp_p4, 1.6666665459E-1);
_PS_CONST (cephes_exp_p5, 5.0000001201E-1);


v4sf exp_ps (v4sf x) noexcept
{
	v4sf tmp = _mm_setzero_ps(), fx;

#ifdef USE_SSE2
	v4si emm0;
#else
	v2si mm0, mm1;
#endif

	x = _mm_min_ps (x, *reinterpret_cast<const v4sf*> (_ps_exp_hi));
	x = _mm_max_ps (x, *reinterpret_cast<const v4sf*> (_ps_exp_lo));

	/* express exp(x) as exp(g + n*log(2)) */
	fx = _mm_mul_ps (x, *reinterpret_cast<const v4sf*> (_ps_cephes_LOG2EF));
	fx = _mm_add_ps (fx, *reinterpret_cast<const v4sf*> (_ps_0p5));

	/* how to perform a floorf with SSE: just below */
#ifndef USE_SSE2
	/* step 1 : cast to int */
	tmp = _mm_movehl_ps (tmp, fx);
	mm0 = _mm_cvttps_pi32 (fx);
	mm1 = _mm_cvttps_pi32 (tmp);
	/* step 2 : cast back to float */
	tmp = _mm_cvtpi32x2_ps (mm0, mm1);
#else
	emm0	   = _mm_cvttps_epi32 (fx);
	tmp		   = _mm_cvtepi32_ps (emm0);
#endif

	/* if greater, subtract 1 */
	v4sf mask = _mm_cmpgt_ps (tmp, fx);

	const v4sf one = *reinterpret_cast<const v4sf*> (_ps_1);

	mask = _mm_and_ps (mask, one);
	fx	 = _mm_sub_ps (tmp, mask);

	tmp	   = _mm_mul_ps (fx, *reinterpret_cast<const v4sf*> (_ps_cephes_exp_C1));
	v4sf z = _mm_mul_ps (fx, *reinterpret_cast<const v4sf*> (_ps_cephes_exp_C2));
	x	   = _mm_sub_ps (x, tmp);
	x	   = _mm_sub_ps (x, z);

	z = _mm_mul_ps (x, x);

	v4sf y = *reinterpret_cast<const v4sf*> (_ps_cephes_exp_p0);

	y = _mm_mul_ps (y, x);
	y = _mm_add_ps (y, *reinterpret_cast<const v4sf*> (_ps_cephes_exp_p1));
	y = _mm_mul_ps (y, x);
	y = _mm_add_ps (y, *reinterpret_cast<const v4sf*> (_ps_cephes_exp_p2));
	y = _mm_mul_ps (y, x);
	y = _mm_add_ps (y, *reinterpret_cast<const v4sf*> (_ps_cephes_exp_p3));
	y = _mm_mul_ps (y, x);
	y = _mm_add_ps (y, *reinterpret_cast<const v4sf*> (_ps_cephes_exp_p4));
	y = _mm_mul_ps (y, x);
	y = _mm_add_ps (y, *reinterpret_cast<const v4sf*> (_ps_cephes_exp_p5));
	y = _mm_mul_ps (y, z);
	y = _mm_add_ps (y, x);
	y = _mm_add_ps (y, one);

	/* build 2^n */
#ifndef USE_SSE2
	z	= _mm_movehl_ps (z, fx);
	mm0 = _mm_cvttps_pi32 (fx);
	mm1 = _mm_cvttps_pi32 (z);
	mm0 = _mm_add_pi32 (mm0, *reinterpret_cast<const v2si*> (_pi32_0x7f));
	mm1 = _mm_add_pi32 (mm1, *reinterpret_cast<const v2si*> (_pi32_0x7f));
	mm0 = _mm_slli_pi32 (mm0, 23);
	mm1 = _mm_slli_pi32 (mm1, 23);

	v4sf pow2n;
	COPY_MM_TO_XMM (mm0, mm1, pow2n);
	_mm_empty();
#else
	emm0	   = _mm_cvttps_epi32 (fx);
	emm0	   = _mm_add_epi32 (emm0, *reinterpret_cast<const v4si*> (_pi32_0x7f));
	emm0	   = _mm_slli_epi32 (emm0, 23);
	v4sf pow2n = _mm_castsi128_ps (emm0);
#endif

	return _mm_mul_ps (y, pow2n);
}


_PS_CONST (minus_cephes_DP1, -0.78515625);
_PS_CONST (minus_cephes_DP2, -2.4187564849853515625e-4);
_PS_CONST (minus_cephes_DP3, -3.77489497744594108e-8);
_PS_CONST (sincof_p0, -1.9515295891E-4);
_PS_CONST (sincof_p1, 8.3321608736E-3);
_PS_CONST (sincof_p2, -1.6666654611E-1);
_PS_CONST (coscof_p0, 2.443315711809948E-005);
_PS_CONST (coscof_p1, -1.388731625493765E-003);
_PS_CONST (coscof_p2, 4.166664568298827E-002);
_PS_CONST (cephes_FOPI, 1.27323954473516);	// 4 / M_PI

#undef _PS_CONST


v4sf sin_ps (v4sf x) noexcept
{
	v4sf xmm1, xmm2 = _mm_setzero_ps(), xmm3, sign_bit = x, y;

#ifdef USE_SSE2
	v4si emm0, emm2;
#else
	v2si mm0, mm1, mm2, mm3;
#endif

	/* take the absolute value */
	x = _mm_and_ps (x, *reinterpret_cast<const v4sf*> (_ps_inv_sign_mask));

	/* extract the sign bit (upper one) */
	sign_bit = _mm_and_ps (sign_bit, *reinterpret_cast<const v4sf*> (_ps_sign_mask));

	/* scale by 4/Pi */
	y = _mm_mul_ps (x, *reinterpret_cast<const v4sf*> (_ps_cephes_FOPI));

#ifdef USE_SSE2
	/* store the integer part of y in mm0 */
	emm2 = _mm_cvttps_epi32 (y);

	/* j=(j+1) & (~1) (see the cephes sources) */
	emm2 = _mm_add_epi32 (emm2, *reinterpret_cast<const v4si*> (_pi32_1));
	emm2 = _mm_and_si128 (emm2, *reinterpret_cast<const v4si*> (_pi32_inv1));
	y	 = _mm_cvtepi32_ps (emm2);

	/* get the swap sign flag */
	emm0 = _mm_and_si128 (emm2, *reinterpret_cast<const v4si*> (_pi32_4));
	emm0 = _mm_slli_epi32 (emm0, 29);

	/* get the polynom selection mask
	   there is one polynom for 0 <= x <= Pi/4
	   and another one for Pi/4<x<=Pi/2

	   Both branches will be computed.
	*/
	emm2 = _mm_and_si128 (emm2, *reinterpret_cast<const v4si*> (_pi32_2));
	emm2 = _mm_cmpeq_epi32 (emm2, _mm_setzero_si128());

	v4sf swap_sign_bit = _mm_castsi128_ps (emm0);
	v4sf poly_mask	   = _mm_castsi128_ps (emm2);
	sign_bit		   = _mm_xor_ps (sign_bit, swap_sign_bit);
#else
	/* store the integer part of y in mm0:mm1 */
	xmm2 = _mm_movehl_ps (xmm2, y);
	mm2	 = _mm_cvttps_pi32 (y);
	mm3	 = _mm_cvttps_pi32 (xmm2);

	/* j=(j+1) & (~1) (see the cephes sources) */
	mm2 = _mm_add_pi32 (mm2, *reinterpret_cast<const v2si*> (_pi32_1));
	mm3 = _mm_add_pi32 (mm3, *reinterpret_cast<const v2si*> (_pi32_1));
	mm2 = _mm_and_si64 (mm2, *reinterpret_cast<const v2si*> (_pi32_inv1));
	mm3 = _mm_and_si64 (mm3, *reinterpret_cast<const v2si*> (_pi32_inv1));
	y	= _mm_cvtpi32x2_ps (mm2, mm3);

	/* get the swap sign flag */
	mm0 = _mm_and_si64 (mm2, *reinterpret_cast<const v2si*> (_pi32_4));
	mm1 = _mm_and_si64 (mm3, *reinterpret_cast<const v2si*> (_pi32_4));
	mm0 = _mm_slli_pi32 (mm0, 29);
	mm1 = _mm_slli_pi32 (mm1, 29);

	/* get the polynom selection mask */
	mm2 = _mm_and_si64 (mm2, *reinterpret_cast<const v2si*> (_pi32_2));
	mm3 = _mm_and_si64 (mm3, *reinterpret_cast<const v2si*> (_pi32_2));
	mm2 = _mm_cmpeq_pi32 (mm2, _mm_setzero_si64());
	mm3 = _mm_cmpeq_pi32 (mm3, _mm_setzero_si64());

	v4sf swap_sign_bit, poly_mask;
	COPY_MM_TO_XMM (mm0, mm1, swap_sign_bit);
	COPY_MM_TO_XMM (mm2, mm3, poly_mask);

	sign_bit = _mm_xor_ps (sign_bit, swap_sign_bit);
	_mm_empty(); /* good-bye mmx */
#endif

	/* The magic pass: "Extended precision modular arithmetic"
	   x = ((x - y * DP1) - y * DP2) - y * DP3; */
	xmm1 = *reinterpret_cast<const v4sf*> (_ps_minus_cephes_DP1);
	xmm2 = *reinterpret_cast<const v4sf*> (_ps_minus_cephes_DP2);
	xmm3 = *reinterpret_cast<const v4sf*> (_ps_minus_cephes_DP3);
	xmm1 = _mm_mul_ps (y, xmm1);
	xmm2 = _mm_mul_ps (y, xmm2);
	xmm3 = _mm_mul_ps (y, xmm3);
	x	 = _mm_add_ps (x, xmm1);
	x	 = _mm_add_ps (x, xmm2);
	x	 = _mm_add_ps (x, xmm3);

	/* Evaluate the first polynom  (0 <= x <= Pi/4) */
	y = *reinterpret_cast<const v4sf*> (_ps_coscof_p0);

	const v4sf z = _mm_mul_ps (x, x);

	y = _mm_mul_ps (y, z);
	y = _mm_add_ps (y, *reinterpret_cast<const v4sf*> (_ps_coscof_p1));
	y = _mm_mul_ps (y, z);
	y = _mm_add_ps (y, *reinterpret_cast<const v4sf*> (_ps_coscof_p2));
	y = _mm_mul_ps (y, z);
	y = _mm_mul_ps (y, z);

	const v4sf tmp = _mm_mul_ps (z, *reinterpret_cast<const v4sf*> (_ps_0p5));

	y = _mm_sub_ps (y, tmp);
	y = _mm_add_ps (y, *reinterpret_cast<const v4sf*> (_ps_1));

	/* Evaluate the second polynom  (Pi/4 <= x <= 0) */

	v4sf y2 = *reinterpret_cast<const v4sf*> (_ps_sincof_p0);

	y2 = _mm_mul_ps (y2, z);
	y2 = _mm_add_ps (y2, *reinterpret_cast<const v4sf*> (_ps_sincof_p1));
	y2 = _mm_mul_ps (y2, z);
	y2 = _mm_add_ps (y2, *reinterpret_cast<const v4sf*> (_ps_sincof_p2));
	y2 = _mm_mul_ps (y2, z);
	y2 = _mm_mul_ps (y2, x);
	y2 = _mm_add_ps (y2, x);

	/* select the correct result from the two polynoms */
	xmm3 = poly_mask;
	y2	 = _mm_and_ps (xmm3, y2);  //, xmm3);
	y	 = _mm_andnot_ps (xmm3, y);
	y	 = _mm_add_ps (y, y2);

	return _mm_xor_ps (y, sign_bit);
}


v4sf cos_ps (v4sf x) noexcept
{
	v4sf xmm1, xmm2 = _mm_setzero_ps(), xmm3, y;

#ifdef USE_SSE2
	v4si emm0, emm2;
#else
	v2si mm0, mm1, mm2, mm3;
#endif

	/* take the absolute value */
	x = _mm_and_ps (x, *reinterpret_cast<const v4sf*> (_ps_inv_sign_mask));

	/* scale by 4/Pi */
	y = _mm_mul_ps (x, *(reinterpret_cast<const v4sf*> (_ps_cephes_FOPI)));

#ifdef USE_SSE2
	/* store the integer part of y in mm0 */
	emm2 = _mm_cvttps_epi32 (y);

	/* j=(j+1) & (~1) (see the cephes sources) */
	emm2 = _mm_add_epi32 (emm2, *reinterpret_cast<const v4si*> (_pi32_1));
	emm2 = _mm_and_si128 (emm2, *reinterpret_cast<const v4si*> (_pi32_inv1));
	y	 = _mm_cvtepi32_ps (emm2);

	emm2 = _mm_sub_epi32 (emm2, *reinterpret_cast<const v4si*> (_pi32_2));

	/* get the swap sign flag */
	emm0 = _mm_andnot_si128 (emm2, *reinterpret_cast<const v4si*> (_pi32_4));
	emm0 = _mm_slli_epi32 (emm0, 29);

	/* get the polynom selection mask */
	emm2 = _mm_and_si128 (emm2, *reinterpret_cast<const v4si*> (_pi32_2));
	emm2 = _mm_cmpeq_epi32 (emm2, _mm_setzero_si128());

	v4sf sign_bit  = _mm_castsi128_ps (emm0);
	v4sf poly_mask = _mm_castsi128_ps (emm2);
#else
	/* store the integer part of y in mm0:mm1 */
	xmm2 = _mm_movehl_ps (xmm2, y);
	mm2	 = _mm_cvttps_pi32 (y);
	mm3	 = _mm_cvttps_pi32 (xmm2);

	/* j=(j+1) & (~1) (see the cephes sources) */
	mm2 = _mm_add_pi32 (mm2, *reinterpret_cast<const v2si*> (_pi32_1));
	mm3 = _mm_add_pi32 (mm3, *reinterpret_cast<const v2si*> (_pi32_1));
	mm2 = _mm_and_si64 (mm2, *reinterpret_cast<const v2si*> (_pi32_inv1));
	mm3 = _mm_and_si64 (mm3, *reinterpret_cast<const v2si*> (_pi32_inv1));

	y = _mm_cvtpi32x2_ps (mm2, mm3);

	mm2 = _mm_sub_pi32 (mm2, *reinterpret_cast<const v2si*> (_pi32_2));
	mm3 = _mm_sub_pi32 (mm3, *reinterpret_cast<const v2si*> (_pi32_2));

	/* get the swap sign flag in mm0:mm1 and the
	   polynom selection mask in mm2:mm3 */

	mm0 = _mm_andnot_si64 (mm2, *reinterpret_cast<const v2si*> (_pi32_4));
	mm1 = _mm_andnot_si64 (mm3, *reinterpret_cast<const v2si*> (_pi32_4));
	mm0 = _mm_slli_pi32 (mm0, 29);
	mm1 = _mm_slli_pi32 (mm1, 29);

	mm2 = _mm_and_si64 (mm2, *reinterpret_cast<const v2si*> (_pi32_2));
	mm3 = _mm_and_si64 (mm3, *reinterpret_cast<const v2si*> (_pi32_2));

	mm2 = _mm_cmpeq_pi32 (mm2, _mm_setzero_si64());
	mm3 = _mm_cmpeq_pi32 (mm3, _mm_setzero_si64());

	v4sf sign_bit, poly_mask;
	COPY_MM_TO_XMM (mm0, mm1, sign_bit);
	COPY_MM_TO_XMM (mm2, mm3, poly_mask);
	_mm_empty(); /* good-bye mmx */
#endif

	/* The magic pass: "Extended precision modular arithmetic"
	   x = ((x - y * DP1) - y * DP2) - y * DP3; */
	xmm1 = *reinterpret_cast<const v4sf*> (_ps_minus_cephes_DP1);
	xmm2 = *reinterpret_cast<const v4sf*> (_ps_minus_cephes_DP2);
	xmm3 = *reinterpret_cast<const v4sf*> (_ps_minus_cephes_DP3);
	xmm1 = _mm_mul_ps (y, xmm1);
	xmm2 = _mm_mul_ps (y, xmm2);
	xmm3 = _mm_mul_ps (y, xmm3);
	x	 = _mm_add_ps (x, xmm1);
	x	 = _mm_add_ps (x, xmm2);
	x	 = _mm_add_ps (x, xmm3);

	/* Evaluate the first polynom  (0 <= x <= Pi/4) */
	y = *reinterpret_cast<const v4sf*> (_ps_coscof_p0);

	const v4sf z = _mm_mul_ps (x, x);

	y = _mm_mul_ps (y, z);
	y = _mm_add_ps (y, *reinterpret_cast<const v4sf*> (_ps_coscof_p1));
	y = _mm_mul_ps (y, z);
	y = _mm_add_ps (y, *reinterpret_cast<const v4sf*> (_ps_coscof_p2));
	y = _mm_mul_ps (y, z);
	y = _mm_mul_ps (y, z);

	const v4sf tmp = _mm_mul_ps (z, *reinterpret_cast<const v4sf*> (_ps_0p5));

	y = _mm_sub_ps (y, tmp);
	y = _mm_add_ps (y, *reinterpret_cast<const v4sf*> (_ps_1));

	/* Evaluate the second polynom  (Pi/4 <= x <= 0) */

	v4sf y2 = *reinterpret_cast<const v4sf*> (_ps_sincof_p0);
	y2		= _mm_mul_ps (y2, z);
	y2		= _mm_add_ps (y2, *reinterpret_cast<const v4sf*> (_ps_sincof_p1));
	y2		= _mm_mul_ps (y2, z);
	y2		= _mm_add_ps (y2, *reinterpret_cast<const v4sf*> (_ps_sincof_p2));
	y2		= _mm_mul_ps (y2, z);
	y2		= _mm_mul_ps (y2, x);
	y2		= _mm_add_ps (y2, x);

	/* select the correct result from the two polynoms */
	xmm3 = poly_mask;
	y2	 = _mm_and_ps (xmm3, y2);  //, xmm3);
	y	 = _mm_andnot_ps (xmm3, y);
	y	 = _mm_add_ps (y, y2);

	return _mm_xor_ps (y, sign_bit);
}


void sincos_ps (v4sf x, v4sf* s, v4sf* c) noexcept
{
	v4sf xmm1, xmm2, xmm3 = _mm_setzero_ps(), sign_bit_sin, y;

#ifdef USE_SSE2
	v4si emm0, emm2, emm4;
#else
	v2si mm0, mm1, mm2, mm3, mm4, mm5;
#endif

	sign_bit_sin = x;

	/* take the absolute value */
	x = _mm_and_ps (x, *reinterpret_cast<const v4sf*> (_ps_inv_sign_mask));

	/* extract the sign bit (upper one) */
	sign_bit_sin = _mm_and_ps (sign_bit_sin, *reinterpret_cast<const v4sf*> (_ps_sign_mask));

	/* scale by 4/Pi */
	y = _mm_mul_ps (x, *reinterpret_cast<const v4sf*> (_ps_cephes_FOPI));

#ifdef USE_SSE2
	/* store the integer part of y in emm2 */
	emm2 = _mm_cvttps_epi32 (y);

	/* j=(j+1) & (~1) (see the cephes sources) */
	emm2 = _mm_add_epi32 (emm2, *reinterpret_cast<const v4si*> (_pi32_1));
	emm2 = _mm_and_si128 (emm2, *reinterpret_cast<const v4si*> (_pi32_inv1));
	y	 = _mm_cvtepi32_ps (emm2);

	emm4 = emm2;

	/* get the swap sign flag for the sine */
	emm0				   = _mm_and_si128 (emm2, *reinterpret_cast<const v4si*> (_pi32_4));
	emm0				   = _mm_slli_epi32 (emm0, 29);
	v4sf swap_sign_bit_sin = _mm_castsi128_ps (emm0);

	/* get the polynom selection mask for the sine*/
	emm2		   = _mm_and_si128 (emm2, *reinterpret_cast<const v4si*> (_pi32_2));
	emm2		   = _mm_cmpeq_epi32 (emm2, _mm_setzero_si128());
	v4sf poly_mask = _mm_castsi128_ps (emm2);
#else
	/* store the integer part of y in mm2:mm3 */
	xmm3 = _mm_movehl_ps (xmm3, y);
	mm2	 = _mm_cvttps_pi32 (y);
	mm3	 = _mm_cvttps_pi32 (xmm3);

	/* j=(j+1) & (~1) (see the cephes sources) */
	mm2 = _mm_add_pi32 (mm2, *reinterpret_cast<const v2si*> (_pi32_1));
	mm3 = _mm_add_pi32 (mm3, *reinterpret_cast<const v2si*> (_pi32_1));
	mm2 = _mm_and_si64 (mm2, *reinterpret_cast<const v2si*> (_pi32_inv1));
	mm3 = _mm_and_si64 (mm3, *reinterpret_cast<const v2si*> (_pi32_inv1));

	y = _mm_cvtpi32x2_ps (mm2, mm3);

	mm4 = mm2;
	mm5 = mm3;

	/* get the swap sign flag for the sine */
	mm0 = _mm_and_si64 (mm2, *reinterpret_cast<const v2si*> (_pi32_4));
	mm1 = _mm_and_si64 (mm3, *reinterpret_cast<const v2si*> (_pi32_4));
	mm0 = _mm_slli_pi32 (mm0, 29);
	mm1 = _mm_slli_pi32 (mm1, 29);
	v4sf swap_sign_bit_sin;
	COPY_MM_TO_XMM (mm0, mm1, swap_sign_bit_sin);

	/* get the polynom selection mask for the sine */

	mm2 = _mm_and_si64 (mm2, *reinterpret_cast<const v2si*> (_pi32_2));
	mm3 = _mm_and_si64 (mm3, *reinterpret_cast<const v2si*> (_pi32_2));
	mm2 = _mm_cmpeq_pi32 (mm2, _mm_setzero_si64());
	mm3 = _mm_cmpeq_pi32 (mm3, _mm_setzero_si64());
	v4sf poly_mask;
	COPY_MM_TO_XMM (mm2, mm3, poly_mask);
#endif

	/* The magic pass: "Extended precision modular arithmetic"
	   x = ((x - y * DP1) - y * DP2) - y * DP3; */
	xmm1 = *reinterpret_cast<const v4sf*> (_ps_minus_cephes_DP1);
	xmm2 = *reinterpret_cast<const v4sf*> (_ps_minus_cephes_DP2);
	xmm3 = *reinterpret_cast<const v4sf*> (_ps_minus_cephes_DP3);
	xmm1 = _mm_mul_ps (y, xmm1);
	xmm2 = _mm_mul_ps (y, xmm2);
	xmm3 = _mm_mul_ps (y, xmm3);
	x	 = _mm_add_ps (x, xmm1);
	x	 = _mm_add_ps (x, xmm2);
	x	 = _mm_add_ps (x, xmm3);

#ifdef USE_SSE2
	emm4			  = _mm_sub_epi32 (emm4, *reinterpret_cast<const v4si*> (_pi32_2));
	emm4			  = _mm_andnot_si128 (emm4, *reinterpret_cast<const v4si*> (_pi32_4));
	emm4			  = _mm_slli_epi32 (emm4, 29);
	v4sf sign_bit_cos = _mm_castsi128_ps (emm4);
#else
	/* get the sign flag for the cosine */
	mm4 = _mm_sub_pi32 (mm4, *reinterpret_cast<const v2si*> (_pi32_2));
	mm5 = _mm_sub_pi32 (mm5, *reinterpret_cast<const v2si*> (_pi32_2));
	mm4 = _mm_andnot_si64 (mm4, *reinterpret_cast<const v2si*> (_pi32_4));
	mm5 = _mm_andnot_si64 (mm5, *reinterpret_cast<const v2si*> (_pi32_4));
	mm4 = _mm_slli_pi32 (mm4, 29);
	mm5 = _mm_slli_pi32 (mm5, 29);
	v4sf sign_bit_cos;
	COPY_MM_TO_XMM (mm4, mm5, sign_bit_cos);
	_mm_empty(); /* good-bye mmx */
#endif

	sign_bit_sin = _mm_xor_ps (sign_bit_sin, swap_sign_bit_sin);

	/* Evaluate the first polynom  (0 <= x <= Pi/4) */
	const v4sf z = _mm_mul_ps (x, x);

	y = *reinterpret_cast<const v4sf*> (_ps_coscof_p0);

	y = _mm_mul_ps (y, z);
	y = _mm_add_ps (y, *reinterpret_cast<const v4sf*> (_ps_coscof_p1));
	y = _mm_mul_ps (y, z);
	y = _mm_add_ps (y, *reinterpret_cast<const v4sf*> (_ps_coscof_p2));
	y = _mm_mul_ps (y, z);
	y = _mm_mul_ps (y, z);

	const v4sf tmp = _mm_mul_ps (z, *reinterpret_cast<const v4sf*> (_ps_0p5));

	y = _mm_sub_ps (y, tmp);
	y = _mm_add_ps (y, *reinterpret_cast<const v4sf*> (_ps_1));

	/* Evaluate the second polynom  (Pi/4 <= x <= 0) */

	v4sf y2 = *reinterpret_cast<const v4sf*> (_ps_sincof_p0);

	y2 = _mm_mul_ps (y2, z);
	y2 = _mm_add_ps (y2, *reinterpret_cast<const v4sf*> (_ps_sincof_p1));
	y2 = _mm_mul_ps (y2, z);
	y2 = _mm_add_ps (y2, *reinterpret_cast<const v4sf*> (_ps_sincof_p2));
	y2 = _mm_mul_ps (y2, z);
	y2 = _mm_mul_ps (y2, x);
	y2 = _mm_add_ps (y2, x);

	/* select the correct result from the two polynoms */
	xmm3	   = poly_mask;
	v4sf ysin2 = _mm_and_ps (xmm3, y2);
	v4sf ysin1 = _mm_andnot_ps (xmm3, y);
	y2		   = _mm_sub_ps (y2, ysin2);
	y		   = _mm_sub_ps (y, ysin1);

	xmm1 = _mm_add_ps (ysin1, ysin2);
	xmm2 = _mm_add_ps (y, y2);

	/* update the sign */
	*s = _mm_xor_ps (xmm1, sign_bit_sin);
	*c = _mm_xor_ps (xmm2, sign_bit_cos);
}

#undef COPY_XMM_TO_MM
#undef COPY_MM_TO_XMM

}  // namespace pommier
