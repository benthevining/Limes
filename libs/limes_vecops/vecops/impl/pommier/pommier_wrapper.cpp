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

#include "./pommier_wrapper.h"
#include <limes_platform.h>

#if LIMES_ARM_NEON
#	include "./neon_mathfun.h"
#elif LIMES_SSE
#	include "./sse_mathfun.h"
#else
#	error
#endif

#include "../phasor.h"
#include <limes_namespace.h>
#include <limes_core.h>
#include <utility>
#include <cmath>
#include "../fallback_impl.h"

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

template <typename VecorizedOp, typename ScalarOp>
LIMES_NO_EXPORT LIMES_FORCE_INLINE static void perform (int size, VecorizedOp&& vectorOp, ScalarOp&& scalarOp) noexcept
{
	int i = 0;

	for (; i + 4 < size; i += 4)
		vectorOp (i);

	for (; i < size; ++i)
		scalarOp (i);
}

void polarToCartesian_pommier (float* const real, float* const imag, const float* const mag, const float* const phase, int size) noexcept
{
	int idx = 0, tidx = 0;

	const auto vecOp = [&idx, &tidx, mag, phase, real, imag] (int)	// NOLINT
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
	};

	const auto scalarOp = [&tidx, phase, real, imag, mag] (int i)
	{
		float re, im;
		detail::phasor (&re, &im, phase[i]);
		real[tidx]	 = re * mag[i];
		imag[tidx++] = im * mag[i];
	};

	perform (size, std::move (vecOp), std::move (scalarOp));
}

void polarToCartesianInterleaved_pommier (float* const dest, const float* const mag, const float* const phase, int size) noexcept
{
	int idx = 0, tidx = 0;

	const auto vecOp = [&idx, &tidx, mag, phase, dest] (int)  // NOLINT
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
	};

	const auto scalarOp = [&tidx, phase, dest, mag] (int i)
	{
		float real, imag;
		detail::phasor (&real, &imag, phase[i]);
		dest[tidx++] = real * mag[i];
		dest[tidx++] = imag * mag[i];
	};

	perform (size, std::move (vecOp), std::move (scalarOp));
}

void sine (float* const data, int size) noexcept
{
	const auto vecOp = [data] (int i)
	{
		V4SF reg;

		for (auto j = 0; j < 4; ++j)
			reg.f[j] = data[i + j];

		const auto result = ::pommier::sin_ps (reg.v);

		for (auto j = 0; j < 4; ++j)
			data[i + j] = result[j];
	};

	const auto scalarOp = [data] (int i)
	{
		data[i] = vecops::fb::detail::fast_sine (data[i]);
	};

	perform (size, std::move (vecOp), std::move (scalarOp));
}

void sineAndCopy (float* const dest, const float* const data, int size) noexcept
{
	const auto vecOp = [dest, data] (int i)
	{
		V4SF reg;

		for (auto j = 0; j < 4; ++j)
			reg.f[j] = data[i + j];

		const auto result = ::pommier::sin_ps (reg.v);

		for (auto j = 0; j < 4; ++j)
			dest[i + j] = result[j];
	};

	const auto scalarOp = [dest, data] (int i)
	{
		dest[i] = vecops::fb::detail::fast_sine (data[i]);
	};

	perform (size, std::move (vecOp), std::move (scalarOp));
}

void cos (float* const data, int size) noexcept
{
	const auto vecOp = [data] (int i)
	{
		V4SF reg;

		for (auto j = 0; j < 4; ++j)
			reg.f[j] = data[i + j];

		const auto result = ::pommier::cos_ps (reg.v);

		for (auto j = 0; j < 4; ++j)
			data[i + j] = result[j];
	};

	const auto scalarOp = [data] (int i)
	{
		data[i] = vecops::fb::detail::fast_cosine (data[i]);
	};

	perform (size, std::move (vecOp), std::move (scalarOp));
}

void cosAndCopy (float* const dest, const float* const data, int size) noexcept
{
	const auto vecOp = [dest, data] (int i)
	{
		V4SF reg;

		for (auto j = 0; j < 4; ++j)
			reg.f[j] = data[i + j];

		const auto result = ::pommier::cos_ps (reg.v);

		for (auto j = 0; j < 4; ++j)
			dest[i + j] = result[j];
	};

	const auto scalarOp = [dest, data] (int i)
	{
		dest[i] = vecops::fb::detail::fast_cosine (data[i]);
	};

	perform (size, std::move (vecOp), std::move (scalarOp));
}

void sinCos (const float* const data, int size, float* const sinesOut, float* const cosinesOut) noexcept
{
	const auto vecOp = [data, sinesOut, cosinesOut] (int i)
	{
		V4SF reg, sin, cos;

		for (auto j = 0; j < 4; ++j)
			reg.f[j] = data[i + j];

		::pommier::sincos_ps (reg.v, &sin.v, &cos.v);

		for (auto j = 0; j < 4; ++j)
		{
			const auto idx = i + j;

			sinesOut[idx]	= sin.f[j];
			cosinesOut[idx] = cos.f[j];
		}
	};

	const auto scalarOp = [data, sinesOut, cosinesOut] (int i)
	{
		const auto thisData = data[i];

		sinesOut[i]	  = vecops::fb::detail::fast_sine (data[i]);
		cosinesOut[i] = vecops::fb::detail::fast_cosine (data[i]);
	};

	perform (size, std::move (vecOp), std::move (scalarOp));
}

void natLog (float* const data, int size) noexcept
{
	const auto vecOp = [data] (int i)
	{
		V4SF reg;

		for (auto j = 0; j < 4; ++j)
			reg.f[j] = data[i + j];

		const auto result = ::pommier::log_ps (reg.v);

		for (auto j = 0; j < 4; ++j)
			data[i + j] = result[j];
	};

	const auto scalarOp = [data] (int i)
	{
		data[i] = std::log (data[i]);
	};

	perform (size, std::move (vecOp), std::move (scalarOp));
}

void natLogAndCopy (float* const dest, const float* const data, int size) noexcept
{
	const auto vecOp = [data, dest] (int i)
	{
		V4SF reg;

		for (auto j = 0; j < 4; ++j)
			reg.f[j] = data[i + j];

		const auto result = ::pommier::log_ps (reg.v);

		for (auto j = 0; j < 4; ++j)
			dest[i + j] = result[j];
	};

	const auto scalarOp = [data, dest] (int i)
	{
		dest[i] = std::log (data[i]);
	};

	perform (size, std::move (vecOp), std::move (scalarOp));
}

void exp (float* const data, int size) noexcept
{
	const auto vecOp = [data] (int i)
	{
		V4SF reg;

		for (auto j = 0; j < 4; ++j)
			reg.f[j] = data[i + j];

		const auto result = ::pommier::exp_ps (reg.v);

		for (auto j = 0; j < 4; ++j)
			data[i + j] = result[j];
	};

	const auto scalarOp = [data] (int i)
	{
		data[i] = vecops::fb::exp::detail::fast_exp (data[i]);
	};

	perform (size, std::move (vecOp), std::move (scalarOp));
}

void expAndCopy (float* const dest, const float* const data, int size) noexcept
{
	const auto vecOp = [data, dest] (int i)
	{
		V4SF reg;

		for (auto j = 0; j < 4; ++j)
			reg.f[j] = data[i + j];

		const auto result = ::pommier::exp_ps (reg.v);

		for (auto j = 0; j < 4; ++j)
			dest[i + j] = result[j];
	};

	const auto scalarOp = [data, dest] (int i)
	{
		dest[i] = vecops::fb::exp::detail::fast_exp (data[i]);
	};

	perform (size, std::move (vecOp), std::move (scalarOp));
}

}  // namespace vecops::pommier

LIMES_END_NAMESPACE
