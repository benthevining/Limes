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

#include "neon_mathfun.h"

#include <array>

namespace pommier
{

v4sf log_ps (v4sf x)
{
	static constexpr auto c_inv_mant_mask = ~0x7f800000u;
	static constexpr auto c_cephes_SQRTHF = 0.707106781186547524;
	static constexpr auto c_cephes_log_p0 = 7.0376836292E-2;
	static constexpr auto c_cephes_log_p1 = -1.1514610310E-1;
	static constexpr auto c_cephes_log_p2 = 1.1676998740E-1;
	static constexpr auto c_cephes_log_p3 = -1.2420140846E-1;
	static constexpr auto c_cephes_log_p4 = +1.4249322787E-1;
	static constexpr auto c_cephes_log_p5 = -1.6668057665E-1;
	static constexpr auto c_cephes_log_p6 = +2.0000714765E-1;
	static constexpr auto c_cephes_log_p7 = -2.4999993993E-1;
	static constexpr auto c_cephes_log_p8 = +3.3333331174E-1;
	static constexpr auto c_cephes_log_q1 = -2.12194440e-4;
	static constexpr auto c_cephes_log_q2 = 0.693359375;

	const v4sf one = vdupq_n_f32 (1);

	x = vmaxq_f32 (x, vdupq_n_f32 (0)); /* force flush to zero on denormal values */

	v4si ux = vreinterpretq_s32_f32 (x);

	v4si emm0 = vshrq_n_s32 (ux, 23);

	/* keep only the fractional part */
	ux = vandq_s32 (ux, vdupq_n_s32 (c_inv_mant_mask));
	ux = vorrq_s32 (ux, vreinterpretq_s32_f32 (vdupq_n_f32 (0.5f)));
	x  = vreinterpretq_f32_s32 (ux);

	emm0   = vsubq_s32 (emm0, vdupq_n_s32 (0x7f));
	v4sf e = vcvtq_f32_s32 (emm0);

	e = vaddq_f32 (e, one);

	/* part2:
	   if( x < SQRTHF ) {
		 e -= 1;
		 x = x + x - 1.0;
	   } else { x = x - 1.0; }
	*/
	const v4su mask = vcltq_f32 (x, vdupq_n_f32 (c_cephes_SQRTHF));
	v4sf	   tmp	= vreinterpretq_f32_u32 (vandq_u32 (vreinterpretq_u32_f32 (x), mask));
	x				= vsubq_f32 (x, one);
	e				= vsubq_f32 (e, vreinterpretq_f32_u32 (vandq_u32 (vreinterpretq_u32_f32 (one), mask)));
	x				= vaddq_f32 (x, tmp);

	v4sf y = vdupq_n_f32 (c_cephes_log_p0);
	y	   = vmulq_f32 (y, x);
	y	   = vaddq_f32 (y, vdupq_n_f32 (c_cephes_log_p1));
	y	   = vmulq_f32 (y, x);
	y	   = vaddq_f32 (y, vdupq_n_f32 (c_cephes_log_p2));
	y	   = vmulq_f32 (y, x);
	y	   = vaddq_f32 (y, vdupq_n_f32 (c_cephes_log_p3));
	y	   = vmulq_f32 (y, x);
	y	   = vaddq_f32 (y, vdupq_n_f32 (c_cephes_log_p4));
	y	   = vmulq_f32 (y, x);
	y	   = vaddq_f32 (y, vdupq_n_f32 (c_cephes_log_p5));
	y	   = vmulq_f32 (y, x);
	y	   = vaddq_f32 (y, vdupq_n_f32 (c_cephes_log_p6));
	y	   = vmulq_f32 (y, x);
	y	   = vaddq_f32 (y, vdupq_n_f32 (c_cephes_log_p7));
	y	   = vmulq_f32 (y, x);
	y	   = vaddq_f32 (y, vdupq_n_f32 (c_cephes_log_p8));
	y	   = vmulq_f32 (y, x);

	const v4sf z = vmulq_f32 (x, x);

	y = vmulq_f32 (y, z);

	tmp = vmulq_f32 (e, vdupq_n_f32 (c_cephes_log_q1));
	y	= vaddq_f32 (y, tmp);

	tmp = vmulq_f32 (z, vdupq_n_f32 (0.5f));
	y	= vsubq_f32 (y, tmp);

	tmp = vmulq_f32 (e, vdupq_n_f32 (c_cephes_log_q2));
	x	= vaddq_f32 (x, y);
	x	= vaddq_f32 (x, tmp);

	const v4su invalid_mask = vcleq_f32 (x, vdupq_n_f32 (0));

	return vreinterpretq_f32_u32 (vorrq_u32 (vreinterpretq_u32_f32 (x), invalid_mask));	 // negative arg will be NAN
}

v4sf exp_ps (v4sf x)
{
	static constexpr auto c_exp_hi		  = 88.3762626647949f;
	static constexpr auto c_exp_lo		  = -88.3762626647949f;
	static constexpr auto c_cephes_LOG2EF = 1.44269504088896341;
	static constexpr auto c_cephes_exp_C1 = 0.693359375;
	static constexpr auto c_cephes_exp_C2 = -2.12194440e-4;
	static constexpr auto c_cephes_exp_p0 = 1.9875691500E-4;
	static constexpr auto c_cephes_exp_p1 = 1.3981999507E-3;
	static constexpr auto c_cephes_exp_p2 = 8.3334519073E-3;
	static constexpr auto c_cephes_exp_p3 = 4.1665795894E-2;
	static constexpr auto c_cephes_exp_p4 = 1.6666665459E-1;
	static constexpr auto c_cephes_exp_p5 = 5.0000001201E-1;

	static constexpr std::array<float32_t, 6> cephes_exp_p { c_cephes_exp_p0, c_cephes_exp_p1, c_cephes_exp_p2, c_cephes_exp_p3, c_cephes_exp_p4, c_cephes_exp_p5 };

	v4sf tmp, fx;

	const v4sf one = vdupq_n_f32 (1);

	x = vminq_f32 (x, vdupq_n_f32 (c_exp_hi));
	x = vmaxq_f32 (x, vdupq_n_f32 (c_exp_lo));

	/* express exp(x) as exp(g + n*log(2)) */
	fx = vmlaq_f32 (vdupq_n_f32 (0.5f), x, vdupq_n_f32 (c_cephes_LOG2EF));

	/* perform a floorf */
	tmp = vcvtq_f32_s32 (vcvtq_s32_f32 (fx));

	/* if greater, subtract 1 */
	v4su mask = vcgtq_f32 (tmp, fx);
	mask	  = vandq_u32 (mask, vreinterpretq_u32_f32 (one));

	fx = vsubq_f32 (tmp, vreinterpretq_f32_u32 (mask));

	tmp	   = vmulq_f32 (fx, vdupq_n_f32 (c_cephes_exp_C1));
	v4sf z = vmulq_f32 (fx, vdupq_n_f32 (c_cephes_exp_C2));
	x	   = vsubq_f32 (x, tmp);
	x	   = vsubq_f32 (x, z);

	v4sf y = vld1q_dup_f32 (cephes_exp_p[0]);

	y = vmulq_f32 (y, x);
	z = vmulq_f32 (x, x);

	const v4sf c1 = vld1q_dup_f32 (cephes_exp_p[1]);

	y = vaddq_f32 (y, c1);
	y = vmulq_f32 (y, x);

	const v4sf c2 = vld1q_dup_f32 (cephes_exp_p[2]);

	y = vaddq_f32 (y, c2);
	y = vmulq_f32 (y, x);

	const v4sf c3 = vld1q_dup_f32 (cephes_exp_p[3]);

	y = vaddq_f32 (y, c3);
	y = vmulq_f32 (y, x);

	const v4sf c4 = vld1q_dup_f32 (cephes_exp_p[4]);

	y = vaddq_f32 (y, c4);
	y = vmulq_f32 (y, x);

	const v4sf c5 = vld1q_dup_f32 (cephes_exp_p[5]);

	y = vaddq_f32 (y, c5);

	y = vmulq_f32 (y, z);
	y = vaddq_f32 (y, x);
	y = vaddq_f32 (y, one);

	/* build 2^n */
	int32x4_t mm;
	mm = vcvtq_s32_f32 (fx);
	mm = vaddq_s32 (mm, vdupq_n_s32 (0x7f));
	mm = vshlq_n_s32 (mm, 23);

	const v4sf pow2n = vreinterpretq_f32_s32 (mm);

	return vmulq_f32 (y, pow2n);
}

void sincos_ps (v4sf x, v4sf* ysin, v4sf* ycos)
{
	static constexpr auto c_minus_cephes_DP1 = -0.78515625;
	static constexpr auto c_minus_cephes_DP2 = -2.4187564849853515625e-4;
	static constexpr auto c_minus_cephes_DP3 = -3.77489497744594108e-8;
	static constexpr auto c_sincof_p0		 = -1.9515295891E-4;
	static constexpr auto c_sincof_p1		 = 8.3321608736E-3;
	static constexpr auto c_sincof_p2		 = -1.6666654611E-1;
	static constexpr auto c_coscof_p0		 = 2.443315711809948E-005;
	static constexpr auto c_coscof_p1		 = -1.388731625493765E-003;
	static constexpr auto c_coscof_p2		 = 4.166664568298827E-002;
	static constexpr auto c_cephes_FOPI		 = 1.27323954473516;  // 4 / M_PI

	v4sf xmm1, xmm2, xmm3, y;

	v4su emm2;

	v4su sign_mask_sin, sign_mask_cos;
	sign_mask_sin = vcltq_f32 (x, vdupq_n_f32 (0));
	x			  = vabsq_f32 (x);

	/* scale by 4/Pi */
	y = vmulq_f32 (x, vdupq_n_f32 (c_cephes_FOPI));

	/* store the integer part of y in mm0 */
	emm2 = vcvtq_u32_f32 (y);
	/* j=(j+1) & (~1) (see the cephes sources) */
	emm2 = vaddq_u32 (emm2, vdupq_n_u32 (1));
	emm2 = vandq_u32 (emm2, vdupq_n_u32 (~1));
	y	 = vcvtq_f32_u32 (emm2);

	/* The magic pass: "Extended precision modular arithmetic"
	   x = ((x - y * DP1) - y * DP2) - y * DP3; */
	xmm1 = vmulq_n_f32 (y, c_minus_cephes_DP1);
	xmm2 = vmulq_n_f32 (y, c_minus_cephes_DP2);
	xmm3 = vmulq_n_f32 (y, c_minus_cephes_DP3);
	x	 = vaddq_f32 (x, xmm1);
	x	 = vaddq_f32 (x, xmm2);
	x	 = vaddq_f32 (x, xmm3);

	sign_mask_sin = veorq_u32 (sign_mask_sin, vtstq_u32 (emm2, vdupq_n_u32 (4)));
	sign_mask_cos = vtstq_u32 (vsubq_u32 (emm2, vdupq_n_u32 (2)), vdupq_n_u32 (4));

	/* Evaluate the first polynom  (0 <= x <= Pi/4) in y1,
	   and the second polynom      (Pi/4 <= x <= 0) in y2 */
	const v4sf z = vmulq_f32 (x, x);

	v4sf y1, y2;

	y1 = vmulq_n_f32 (z, c_coscof_p0);
	y2 = vmulq_n_f32 (z, c_sincof_p0);
	y1 = vaddq_f32 (y1, vdupq_n_f32 (c_coscof_p1));
	y2 = vaddq_f32 (y2, vdupq_n_f32 (c_sincof_p1));
	y1 = vmulq_f32 (y1, z);
	y2 = vmulq_f32 (y2, z);
	y1 = vaddq_f32 (y1, vdupq_n_f32 (c_coscof_p2));
	y2 = vaddq_f32 (y2, vdupq_n_f32 (c_sincof_p2));
	y1 = vmulq_f32 (y1, z);
	y2 = vmulq_f32 (y2, z);
	y1 = vmulq_f32 (y1, z);
	y2 = vmulq_f32 (y2, x);
	y1 = vsubq_f32 (y1, vmulq_f32 (z, vdupq_n_f32 (0.5f)));
	y2 = vaddq_f32 (y2, x);
	y1 = vaddq_f32 (y1, vdupq_n_f32 (1));

	/* get the polynom selection mask
	   there is one polynom for 0 <= x <= Pi/4
	   and another one for Pi/4<x<=Pi/2

	   Both branches will be computed.
	*/
	const v4su poly_mask = vtstq_u32 (emm2, vdupq_n_u32 (2));

	/* select the correct result from the two polynoms */
	v4sf ys = vbslq_f32 (poly_mask, y1, y2);
	v4sf yc = vbslq_f32 (poly_mask, y2, y1);

	*ysin = vbslq_f32 (sign_mask_sin, vnegq_f32 (ys), ys);
	*ycos = vbslq_f32 (sign_mask_cos, yc, vnegq_f32 (yc));
}

v4sf sin_ps (v4sf x)
{
	v4sf ysin, ycos;
	sincos_ps (x, &ysin, &ycos);
	return ysin;
}

v4sf cos_ps (v4sf x)
{
	v4sf ysin, ycos;
	sincos_ps (x, &ysin, &ycos);
	return ycos;
}

}  // namespace pommier
