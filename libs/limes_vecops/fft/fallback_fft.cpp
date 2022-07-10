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

#include "./fallback_fft.h"
#include <limes_platform.h>
#include <limes_namespace.h>
#include <limes_core.h>
#include "./limes_fft.h"
#include "../limes_vecops.h"
#include <array>

#if LIMES_VECOPS_USE_MIPP
#	include "../impl/mipp.h"
#	include <utility>
#endif

LIMES_BEGIN_NAMESPACE

namespace vecops::fft
{

static_assert (isUsingFallback());

static constexpr auto FFTblockTableSize = 16;
static constexpr auto FFTmaxTabledBlock = 1 << FFTblockTableSize;
static constexpr auto FFTalignment		= 32UL;

template <Scalar SampleType>
FallbackFFT<SampleType>::FallbackFFT (int size)
	: FFTImpl<SampleType> (size),
	  m_table (static_cast<std::size_t> (m_half), FFTalignment, 0),
	  m_sincos (FFTblockTableSize * 4, FFTalignment, SampleType (0)),
	  m_sincos_r (static_cast<std::size_t> (m_half), FFTalignment, SampleType (0)),
	  m_vr (static_cast<std::size_t> (m_half), FFTalignment, SampleType (0)),
	  m_vi (static_cast<std::size_t> (m_half), FFTalignment, SampleType (0)),
	  m_a (static_cast<std::size_t> (m_half) + 1, FFTalignment, SampleType (0)),
	  m_b (static_cast<std::size_t> (m_half) + 1, FFTalignment, SampleType (0)),
	  m_c (static_cast<std::size_t> (m_half) + 1, FFTalignment, SampleType (0)),
	  m_d (static_cast<std::size_t> (m_half) + 1, FFTalignment, SampleType (0))
{
	{
		const auto bits = [halfSize = m_half]
		{
			for (auto i = 0;; ++i)
				if ((halfSize & (1 << i)) != 0)
					return i;

			LIMES_ASSERT_FALSE;
			return 0;
		}();

		for (auto i = 0; i < m_half; ++i)
		{
			auto m = i;
			auto k = 0;

			for (auto j = 0; j < bits; ++j)
			{
				k = (k << 1) | (m & 1);
				m >>= 1;
			}

			m_table[static_cast<std::size_t> (i)] = k;
		}
	}

	// sin and cos tables for complex fft
	for (std::size_t i = 2, ix = 0;
		 i <= FFTmaxTabledBlock;
		 i <<= 1)
	{
		const auto phase	   = math::constants::two_pi<SampleType> / static_cast<SampleType> (i);
		const auto doublePhase = phase * SampleType (2.);

		m_sincos[ix++] = std::sin (phase);
		m_sincos[ix++] = std::sin (doublePhase);
		m_sincos[ix++] = std::cos (phase);
		m_sincos[ix++] = std::cos (doublePhase);
	}

	const auto halfPlusHalf = static_cast<SampleType> (m_half) + SampleType (0.5);

	// sin and cos tables for real-complex transform
	for (std::size_t i = 1, ix = 0;
		 i <= static_cast<std::size_t> (m_half / 2);
		 ++i)
	{
		const auto phase = math::constants::pi<SampleType> * (static_cast<SampleType> (i) / halfPlusHalf);

		m_sincos_r[ix++] = std::sin (phase);
		m_sincos_r[ix++] = std::cos (phase);
	}
}

template <Scalar SampleType>
void FallbackFFT<SampleType>::forward (const SampleType* realIn, SampleType* realOut, SampleType* imagOut) noexcept
{
	transformF (realIn, realOut, imagOut);
}

template <Scalar SampleType>
void FallbackFFT<SampleType>::forwardInterleaved (const SampleType* realIn, SampleType* complexOut) noexcept
{
	transformF (realIn, m_c, m_d);

	vecops::interleave (complexOut, m_c_and_d, 2, m_half + 1);
}

template <Scalar SampleType>
void FallbackFFT<SampleType>::forwardPolar (const SampleType* realIn, SampleType* magOut, SampleType* phaseOut) noexcept
{
	transformF (realIn, m_c, m_d);

	vecops::cartesianToPolar (magOut, phaseOut, m_c.get(), m_d.get(), m_half + 1);
}

template <Scalar SampleType>
void FallbackFFT<SampleType>::forwardMagnitude (const SampleType* realIn, SampleType* magOut) noexcept
{
	transformF (realIn, m_c, m_d);

	vecops::cartesianToMagnitudes (magOut, m_c.get(), m_d.get(), m_half + 1);
}

template <Scalar SampleType>
void FallbackFFT<SampleType>::inverse (const SampleType* realIn, const SampleType* imagIn, SampleType* realOut) noexcept
{
	transformI (realIn, imagIn, realOut);
}

template <Scalar SampleType>
void FallbackFFT<SampleType>::inverseInterleaved (const SampleType* complexIn, SampleType* realOut) noexcept
{
	vecops::deinterleave (m_a_and_b, complexIn, 2, m_half + 1);

	transformI (m_a, m_b, realOut);
}

template <Scalar SampleType>
void FallbackFFT<SampleType>::inversePolar (const SampleType* magIn, const SampleType* phaseIn, SampleType* realOut) noexcept
{
	vecops::polarToCartesian (m_a.get(), m_b.get(), magIn, phaseIn, m_half + 1);

	transformI (m_a, m_b, realOut);
}

template <Scalar SampleType>
void FallbackFFT<SampleType>::inverseCepstral (const SampleType* magIn, SampleType* cepOut) noexcept
{
	auto scalar_op = [this, magIn] (auto i)
	{
		m_a[static_cast<std::size_t> (i)] = std::log (magIn[i] + this->shiftAmount);
		m_b[static_cast<std::size_t> (i)] = SampleType (0.);
	};

#if LIMES_VECOPS_USE_MIPP

	mipp::Reg<SampleType> a;

	const auto shiftRegister = mipp::set1<SampleType> (this->shiftAmount);

	// NB. clang doesn't like auto here for some reason
	const mipp::Reg<SampleType> zeroRegister = mipp::set1<SampleType> (SampleType (0));

	auto vector_op = [this, &a, &shiftRegister, &zeroRegister, magIn] (auto i)
	{
		a.load (&magIn[i]);
		a += shiftRegister;
		a = mipp::log (a);
		a.store (&m_a[static_cast<std::size_t> (i)]);

		zeroRegister.store (&m_b[static_cast<std::size_t> (i)]);
	};

	detail::perform<SampleType> (
		m_half,
		std::move (vector_op),
		std::move (scalar_op));

#else
	for (std::size_t i = 0; i <= static_cast<std::size_t> (m_half); ++i)
		scalar_op (i);
#endif

	transformI (m_a, m_b, cepOut);
}

// Uses m_a and m_b internally; does not touch m_c or m_d
template <Scalar SampleType>
LIMES_FORCE_INLINE void FallbackFFT<SampleType>::transformF (const SampleType* ri,
															 SampleType* ro, SampleType* io) noexcept
{
	const auto half_size = static_cast<std::size_t> (m_half);

	{
		SampleType* a_and_b[2] = { m_a + half_size, m_b + half_size };

		vecops::deinterleave (a_and_b, ri + half_size, 2, static_cast<std::size_t> (this->fft_size) - half_size);
	}

	transformComplex (m_a, m_b, m_vr, m_vi, false);

	ro[0]	   = m_vr[0] + m_vi[0];
	ro[m_half] = m_vr[0] - m_vi[0];
	io[0] = io[m_half] = SampleType (0.);

	for (std::size_t i = 0, ix = 0;
		 i < half_size / 2;
		 ++i)
	{
		// this can be MIPP-ified if I can figure out how to
		// deinterleave the sin & cos values and then put the
		// sines into 1 register and the cosines into 1 register...
		const auto s = -m_sincos_r[ix++];
		const auto c = m_sincos_r[ix++];

		const auto k	= i + 1;
		const auto r0	= m_vr[k];
		const auto i0	= m_vi[k];
		const auto r1	= m_vr[half_size - k];
		const auto i1	= -m_vi[half_size - k];
		const auto tw_r = (r0 - r1) * c - (i0 - i1) * s;
		const auto tw_i = (r0 - r1) * s + (i0 - i1) * c;

		ro[k]			  = (r0 + r1 + tw_r) * SampleType (0.5);
		ro[half_size - k] = (r0 + r1 - tw_r) * SampleType (0.5);
		io[k]			  = (i0 + i1 + tw_i) * SampleType (0.5);
		io[half_size - k] = (tw_i - i0 - i1) * SampleType (0.5);
	}
}

// Uses m_c and m_d internally; does not touch m_a or m_b
template <Scalar SampleType>
LIMES_FORCE_INLINE void FallbackFFT<SampleType>::transformI (const SampleType* ri, const SampleType* ii,
															 SampleType* ro) noexcept
{
	m_vr[0] = ri[0] + ri[m_half];
	m_vi[0] = ri[0] - ri[m_half];

	const auto half_size = static_cast<std::size_t> (m_half);

	for (std::size_t i = 0, ix = 0;
		 i < half_size / 2;
		 ++i)
	{
		// this can be MIPP-ified if I can figure out how to
		// deinterleave the sin & cos values and then put the
		// sines into 1 register and the cosines into 1 register...
		const auto s = m_sincos_r[ix++];
		const auto c = m_sincos_r[ix++];

		const auto k	= i + 1;
		const auto r0	= ri[k];
		const auto r1	= ri[half_size - k];
		const auto i0	= ii[k];
		const auto i1	= -ii[half_size - k];
		const auto tw_r = (r0 - r1) * c - (i0 - i1) * s;
		const auto tw_i = (r0 - r1) * s + (i0 - i1) * c;

		m_vr[k]				= (r0 + r1 + tw_r);
		m_vr[half_size - k] = (r0 + r1 - tw_r);
		m_vi[k]				= (i0 + i1 + tw_i);
		m_vi[half_size - k] = (tw_i - i0 - i1);
	}

	transformComplex (m_vr, m_vi, m_c, m_d, true);

	{
		const SampleType* c_and_d[2] = { m_c + half_size, m_d + half_size };

		vecops::interleave (ro + half_size, c_and_d, 2,
							static_cast<std::size_t> (this->fft_size) - half_size);
	}
}

template <Scalar SampleType>
LIMES_FORCE_INLINE void FallbackFFT<SampleType>::transformComplex (const SampleType* ri, const SampleType* ii,
																   SampleType* ro, SampleType* io,
																   bool inverse) noexcept
{
	// Following Don Cross's 1998 implementation, described by its author as public domain.

	for (std::size_t i = 0; i < static_cast<std::size_t> (m_half); ++i)
	{
		const auto j = static_cast<std::size_t> (m_table[i]);

		ro[j] = ri[i];
		io[j] = ii[i];
	}

	const auto ifactor = (inverse ? SampleType (-1.) : SampleType (1.));

	for (auto blockSize = 2, blockEnd = 1;
		 blockSize <= m_half;
		 blockSize <<= 1, blockEnd = blockSize)
	{
		const auto values = [blockSize, ifactor, this]() -> std::array<SampleType, 4>
		{
			std::array<SampleType, 4> vals;

			if (blockSize <= FFTmaxTabledBlock)
			{
				std::size_t ix = 0;

				vals[0] = ifactor * m_sincos[ix++];
				vals[1] = ifactor * m_sincos[ix++];
				vals[2] = m_sincos[ix++];
				vals[3] = m_sincos[ix++];
			}
			else
			{
				const auto phase	   = SampleType (2.) * math::constants::pi<SampleType> / SampleType (blockSize);
				const auto doublePhase = SampleType (2.) * phase;

				vals[0] = ifactor * std::sin (phase);
				vals[1] = ifactor * std::sin (doublePhase);
				vals[2] = std::cos (phase);
				vals[3] = std::cos (doublePhase);
			}

			return vals;
		}();

		const auto w = SampleType (2.) * values[2];
		SampleType ar[3], ai[3];  // NOLINT

		for (auto i = 0; i < m_half; i += blockSize)
		{
			ar[2] = values[3];
			ar[1] = values[2];
			ai[2] = values[1];
			ai[1] = values[0];

			for (auto m = 0, j = i;
				 m < blockEnd;
				 ++m, ++j)
			{
				ar[0] = w * ar[1] - ar[2];
				ar[2] = ar[1];
				ar[1] = ar[0];
				ai[0] = w * ai[1] - ai[2];
				ai[2] = ai[1];
				ai[1] = ai[0];

				const auto k  = j + blockEnd;
				const auto tr = ar[0] * ro[k] - ai[0] * io[k];
				const auto ti = ar[0] * io[k] + ai[0] * ro[k];

				ro[k] = ro[j] - tr;
				io[k] = io[j] - ti;

				ro[j] += tr;
				io[j] += ti;
			}
		}
	}
}

template <Scalar SampleType>
void FallbackFFT<SampleType>::reset()
{
	vecops::clear (m_vr.get(), m_half);
	vecops::clear (m_vi.get(), m_half);

	const auto plusOne = m_half + 1;

	vecops::clear (m_a.get(), plusOne);
	vecops::clear (m_b.get(), plusOne);
	vecops::clear (m_c.get(), plusOne);
	vecops::clear (m_d.get(), plusOne);
}

template class FallbackFFT<float>;
template class FallbackFFT<double>;

}  // namespace vecops::fft

LIMES_END_NAMESPACE
