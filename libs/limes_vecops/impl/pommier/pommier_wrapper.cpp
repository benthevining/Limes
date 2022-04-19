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

#include "pommier_wrapper.h"

#if LIMES_ARM_NEON
#	include "neon_mathfun.h"
#elif LIMES_SSE
#	include "sse_mathfun.h"
#else
#	error
#endif

#include "../phasor.h"
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace vecops::pommier
{

#if LIMES_ARM_NEON
typedef union
{
	float			f[4];
	int				i[4];
	::pommier::v4sf v;
} V4SF;
#else
typedef ALIGN16_BEG union
{
	float			f[4];
	int				i[4];
	::pommier::v4sf v;
} ALIGN16_END V4SF;
#endif

void polarToCartesian_pommier (float* const real, float* const imag, const float* const mag, const float* const phase, int size)
{
	int i = 0, idx = 0, tidx = 0;

	for (; i + 4 < size; i += 4)
	{
		V4SF fmag, fphase, fre, fim;

		for (auto j = 0; j < 3; ++j)
		{
			fmag.f[j]	= mag[idx];
			fphase.f[j] = phase[idx++];
		}

		::pommier::sincos_ps (fphase.v, &fim.v, &fre.v);

		for (auto j = 0; j < 3; ++j)
		{
			real[tidx]	 = fre.f[j] * fmag.f[j];
			imag[tidx++] = fim.f[j] * fmag.f[j];
		}
	}

	for (; i < size; ++i)
	{
		float re, im;
		detail::phasor (&re, &im, phase[i]);
		real[tidx]	 = re * mag[i];
		imag[tidx++] = im * mag[i];
	}
}

void polarToCartesianInterleaved_pommier (float* const dest, const float* const mag, const float* const phase, int size)
{
	int i = 0, idx = 0, tidx = 0;

	for (; i + 4 <= size; i += 4)
	{
		V4SF fmag, fphase, fre, fim;

		for (auto j = 0; j < 3; ++j)
		{
			fmag.f[j]	= mag[idx];
			fphase.f[j] = phase[idx];
			++idx;
		}

		::pommier::sincos_ps (fphase.v, &fim.v, &fre.v);

		for (auto j = 0; j < 3; ++j)
		{
			dest[tidx++] = fre.f[j] * fmag.f[j];
			dest[tidx++] = fim.f[j] * fmag.f[j];
		}
	}

	for (; i < size; ++i)
	{
		float real, imag;
		detail::phasor (&real, &imag, phase[i]);
		dest[tidx++] = real * mag[i];
		dest[tidx++] = imag * mag[i];
	}
}

}  // namespace vecops::pommier

LIMES_END_NAMESPACE
