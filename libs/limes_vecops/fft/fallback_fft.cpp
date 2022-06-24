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

LIMES_BEGIN_NAMESPACE

namespace vecops
{

static constexpr std::size_t FFTalignment = 32UL;

template <Scalar SampleType>
FallbackFFT<SampleType>::FallbackFFT (int size)
	: FFTImpl<SampleType> (size),
	  m_table (static_cast<std::size_t> (m_half), FFTalignment, 0),
	  m_sincos (m_blockTableSize * 4, FFTalignment, SampleType (0)),
	  m_sincos_r (static_cast<std::size_t> (m_half), FFTalignment, SampleType (0)),
	  m_vr (static_cast<std::size_t> (m_half), FFTalignment, SampleType (0)),
	  m_vi (static_cast<std::size_t> (m_half), FFTalignment, SampleType (0)),
	  m_a (static_cast<std::size_t> (m_half) + 1, FFTalignment, SampleType (0)),
	  m_b (static_cast<std::size_t> (m_half) + 1, FFTalignment, SampleType (0)),
	  m_c (static_cast<std::size_t> (m_half) + 1, FFTalignment, SampleType (0)),
	  m_d (static_cast<std::size_t> (m_half) + 1, FFTalignment, SampleType (0))
{
	// static_assert (fft::isUsingFallback());

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

	// sin and cos tables for complex fft
	for (std::size_t i = 2, ix = 0; i <= m_maxTabledBlock; i <<= 1)
	{
		const auto phase	   = SampleType (2.) * math::constants::pi<SampleType> / static_cast<SampleType> (i);
		const auto doublePhase = phase * SampleType (2.);

		m_sincos[ix++] = std::sin (phase);
		m_sincos[ix++] = std::sin (doublePhase);
		m_sincos[ix++] = std::cos (phase);
		m_sincos[ix++] = std::cos (doublePhase);
	}

	// sin and cos tables for real-complex transform
	for (std::size_t i = 0, ix = 0; i < static_cast<std::size_t> (m_half / 2); ++i)
	{
		const auto phase = math::constants::pi<SampleType> * (static_cast<SampleType> (i + 1) / static_cast<SampleType> (m_half) + SampleType (0.5));
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
	for (std::size_t i = 0; i <= static_cast<std::size_t> (m_half); ++i)
	{
		m_a[i] = std::log (magIn[i] + shiftAmount<SampleType>);
		m_b[i] = SampleType (0.);
	}

	transformI (m_a, m_b, cepOut);
}

// Uses m_a and m_b internally; does not touch m_c or m_d
template <Scalar SampleType>
LIMES_FORCE_INLINE void FallbackFFT<SampleType>::transformF (const SampleType* ri,
															 SampleType* ro, SampleType* io) noexcept
{
	const auto half_size = static_cast<std::size_t> (m_half);

	for (std::size_t i = half_size;
		 i < static_cast<std::size_t> (this->fft_size);
		 ++i)
	{
		m_a[i] = ri[i];
		m_b[i] = ri[i + 1];
	}

	transformComplex (m_a, m_b, m_vr, m_vi, false);

	ro[0]	   = m_vr[0] + m_vi[0];
	ro[m_half] = m_vr[0] - m_vi[0];
	io[0] = io[m_half] = SampleType (0.);

	for (std::size_t i = 0, ix = 0; i < half_size / 2; ++i)
	{
		const auto s	= -m_sincos_r[ix++];
		const auto c	= m_sincos_r[ix++];
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

	for (std::size_t i = 0, ix = 0; i < half_size / 2; ++i)
	{
		const auto s	= m_sincos_r[ix++];
		const auto c	= m_sincos_r[ix++];
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

	for (std::size_t i = half_size;
		 i < static_cast<std::size_t> (this->fft_size);
		 ++i)
	{
		ro[i]	  = m_c[i];
		ro[i + 1] = m_d[i];
	}
}

template <Scalar SampleType>
LIMES_FORCE_INLINE void FallbackFFT<SampleType>::transformComplex (const SampleType* ri, const SampleType* ii,
																   SampleType* ro, SampleType* io,
																   bool inverse) noexcept
{
	// Following Don Cross's 1998 implementation, described by its author as public domain.

	const auto half_size = static_cast<std::size_t> (m_half);

	for (std::size_t i = 0; i < half_size; ++i)
	{
		const auto j = static_cast<std::size_t> (m_table[i]);

		ro[j] = ri[i];
		io[j] = ii[i];
	}

	const auto ifactor = (inverse ? SampleType (-1.) : SampleType (1.));

	for (auto blockSize = 2, blockEnd = 1;
		 blockSize <= m_half;
		 blockSize <<= 1)
	{
		const auto values = [blockSize, ifactor, this]
		{
			struct four_values final
			{
				SampleType sm1, sm2, cm1, cm2;
			};

			four_values vals {};

			if (blockSize <= m_maxTabledBlock)
			{
				std::size_t ix = 0;

				vals.sm1 = ifactor * m_sincos[ix++];
				vals.sm2 = ifactor * m_sincos[ix++];
				vals.cm1 = m_sincos[ix++];
				vals.cm2 = m_sincos[ix++];
			}
			else
			{
				const auto phase	   = SampleType (2.) * math::constants::pi<SampleType> / SampleType (blockSize);
				const auto doublePhase = SampleType (2.) * phase;

				vals.sm1 = ifactor * std::sin (phase);
				vals.sm2 = ifactor * std::sin (doublePhase);
				vals.cm1 = std::cos (phase);
				vals.cm2 = std::cos (doublePhase);
			}

			return vals;
		}();

		const auto w = SampleType (2.) * values.cm1;
		SampleType ar[3], ai[3];  // NOLINT

		for (auto i = 0; i < m_half; i += blockSize)
		{
			ar[2] = values.cm2;
			ar[1] = values.cm1;
			ai[2] = values.sm2;
			ai[1] = values.sm1;

			for (auto m = 0, j = i; m < blockEnd; ++m)
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

				++j;
			}
		}

		blockEnd = blockSize;
	}
}

template class FallbackFFT<float>;
template class FallbackFFT<double>;

}  // namespace vecops

LIMES_END_NAMESPACE
