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

#include "fallback_fft.h"

namespace limes::vecops
{

template <Scalar SampleType>
FallbackFFT<SampleType>::FallbackFFT (int size)
	: FFTImpl<SampleType> (size)
{
	// m_table = allocate_and_zero<int>(m_half);
	//       m_sincos = allocate_and_zero<double>(m_blockTableSize * 4);
	//       m_sincos_r = allocate_and_zero<double>(m_half);
	//       m_vr = allocate_and_zero<double>(m_half);
	//       m_vi = allocate_and_zero<double>(m_half);
	//       m_a = allocate_and_zero<double>(m_half + 1);
	//       m_b = allocate_and_zero<double>(m_half + 1);
	//       m_c = allocate_and_zero<double>(m_half + 1);
	//       m_d = allocate_and_zero<double>(m_half + 1);

	makeTables();
}

template <Scalar SampleType>
FallbackFFT<SampleType>::~FallbackFFT()
{
	// deallocate(m_table);
	//       deallocate(m_sincos);
	//       deallocate(m_sincos_r);
	//       deallocate(m_vr);
	//       deallocate(m_vi);
	//       deallocate(m_a);
	//       deallocate(m_b);
	//       deallocate(m_c);
	//       deallocate(m_d);
}

template <Scalar SampleType>
void FallbackFFT<SampleType>::forward (const SampleType* realIn, SampleType* realOut, SampleType* imagOut)
{
	if constexpr (std::is_same_v<SampleType, float>)
	{
		transformF (realIn, m_c, m_d);
		// v_convert (realOut, m_c, m_half + 1);
		// v_convert (imagOut, m_d, m_half + 1);
	}
	else
	{
		transformF (realIn, realOut, imagOut);
	}
}

template <Scalar SampleType>
void FallbackFFT<SampleType>::forwardInterleaved (const SampleType* realIn, SampleType* complexOut)
{
	transformF (realIn, m_c, m_d);

	if constexpr (std::is_same_v<SampleType, double>)
	{
		vecops::interleave (complexOut, m_c_and_d, 2, m_half + 1);
	}
	else
	{
		for (auto i = 0; i <= m_half; ++i) complexOut[static_cast<ptrdiff_t> (i * 2)] = m_c[i];
		for (auto i = 0; i <= m_half; ++i) complexOut[static_cast<ptrdiff_t> (i * 2 + 1)] = m_d[i];
	}
}

template <Scalar SampleType>
void FallbackFFT<SampleType>::forwardPolar (const SampleType* realIn, SampleType* magOut, SampleType* phaseOut)
{
	transformF (realIn, m_c, m_d);

	if constexpr (std::is_same_v<SampleType, double>)
	{
		vecops::cartesianToPolar (magOut, phaseOut, m_c, m_d, m_half + 1);
	}
	else
	{
		vecops::cartesianToPolar (m_a, m_b, m_c, m_d, m_half + 1);
		// convert m_a -> magOut, m_b -> phaseOut
	}
}

template <Scalar SampleType>
void FallbackFFT<SampleType>::forwardMagnitude (const SampleType* realIn, SampleType* magOut)
{
	transformF (realIn, m_c, m_d);

	if constexpr (std::is_same_v<SampleType, double>)
	{
		vecops::cartesianToMagnitudes (magOut, m_c, m_d, m_half + 1);
	}
	else
	{
		vecops::cartesianToMagnitudes (m_a, m_c, m_d, m_half + 1);
		// convert m_a -> magOut
	}
}

template <Scalar SampleType>
void FallbackFFT<SampleType>::inverse (const SampleType* realIn, const SampleType* imagIn, SampleType* realOut)
{
	if constexpr (std::is_same_v<SampleType, float>)
	{
		// v_convert (m_a, realIn, m_half + 1);
		// v_convert (m_b, imagIn, m_half + 1);
		transformI (m_a, m_b, realOut);
	}
	else
	{
		transformI (realIn, imagIn, realOut);
	}
}

template <Scalar SampleType>
void FallbackFFT<SampleType>::inverseInterleaved (const SampleType* complexIn, SampleType* realOut)
{
	if constexpr (std::is_same_v<SampleType, double>)
	{
		vecops::deinterleave (m_a_and_b, complexIn, 2, m_half + 1);
	}
	else
	{
		for (auto i = 0; i <= m_half; ++i) m_a[i] = complexIn[static_cast<ptrdiff_t> (i * 2)];
		for (auto i = 0; i <= m_half; ++i) m_b[i] = complexIn[static_cast<ptrdiff_t> (i * 2 + 1)];
	}

	transformI (m_a, m_b, realOut);
}

template <Scalar SampleType>
void FallbackFFT<SampleType>::inversePolar (const SampleType* magIn, const SampleType* phaseIn, SampleType* realOut)
{
	if constexpr (std::is_same_v<SampleType, double>)
	{
		vecops::polarToCartesian (m_a, m_b, magIn, phaseIn, m_half + 1);
	}
	else
	{
		// convert magIn -> m_c, phaseIn -> m_d
		vecops::polarToCartesian (m_a, m_b, m_c, m_d, m_half + 1);
	}

	transformI (m_a, m_b, realOut);
}

template <Scalar SampleType>
void FallbackFFT<SampleType>::inverseCepstral (const SampleType* magIn, SampleType* cepOut)
{
	for (auto i = 0; i <= m_half; ++i)
	{
		m_a[i] = std::log (magIn[i] + shiftAmount<double>);
		m_b[i] = 0.;
	}

	transformI (m_a, m_b, cepOut);
}

// Uses m_a and m_b internally; does not touch m_c or m_d
template <Scalar SampleType>
LIMES_FORCE_INLINE void FallbackFFT<SampleType>::transformF (const SampleType* ri,
															 double* ro, double* io)
{
	for (auto i = 0; i < m_half; ++i)
	{
		m_a[i] = ri[static_cast<ptrdiff_t> (i * 2)];
		m_b[i] = ri[static_cast<ptrdiff_t> (i * 2 + 1)];
	}

	transformComplex (m_a, m_b, m_vr, m_vi, false);

	ro[0]	   = m_vr[0] + m_vi[0];
	ro[m_half] = m_vr[0] - m_vi[0];
	io[0] = io[m_half] = 0.0;

	for (auto i = 0, ix = 0; i < m_half / 2; ++i)
	{
		const auto s	= -m_sincos_r[ix++];
		const auto c	= m_sincos_r[ix++];
		const auto k	= i + 1;
		const auto r0	= m_vr[k];
		const auto i0	= m_vi[k];
		const auto r1	= m_vr[m_half - k];
		const auto i1	= -m_vi[m_half - k];
		const auto tw_r = (r0 - r1) * c - (i0 - i1) * s;
		const auto tw_i = (r0 - r1) * s + (i0 - i1) * c;

		ro[k]		   = (r0 + r1 + tw_r) * 0.5;
		ro[m_half - k] = (r0 + r1 - tw_r) * 0.5;
		io[k]		   = (i0 + i1 + tw_i) * 0.5;
		io[m_half - k] = (tw_i - i0 - i1) * 0.5;
	}
}

// Uses m_c and m_d internally; does not touch m_a or m_b
template <Scalar SampleType>
LIMES_FORCE_INLINE void FallbackFFT<SampleType>::transformI (const double* ri, const double* ii,
															 SampleType* ro)
{
	m_vr[0] = ri[0] + ri[m_half];
	m_vi[0] = ri[0] - ri[m_half];

	for (auto i = 0, ix = 0; i < m_half / 2; ++i)
	{
		const auto s	= m_sincos_r[ix++];
		const auto c	= m_sincos_r[ix++];
		const auto k	= i + 1;
		const auto r0	= ri[k];
		const auto r1	= ri[m_half - k];
		const auto i0	= ii[k];
		const auto i1	= -ii[m_half - k];
		const auto tw_r = (r0 - r1) * c - (i0 - i1) * s;
		const auto tw_i = (r0 - r1) * s + (i0 - i1) * c;

		m_vr[k]			 = (r0 + r1 + tw_r);
		m_vr[m_half - k] = (r0 + r1 - tw_r);
		m_vi[k]			 = (i0 + i1 + tw_i);
		m_vi[m_half - k] = (tw_i - i0 - i1);
	}

	transformComplex (m_vr, m_vi, m_c, m_d, true);

	for (auto i = 0; i < m_half; ++i)
	{
		ro[static_cast<ptrdiff_t> (i * 2)]	   = m_c[i];
		ro[static_cast<ptrdiff_t> (i * 2 + 1)] = m_d[i];
	}
}

template <Scalar SampleType>
LIMES_FORCE_INLINE void FallbackFFT<SampleType>::transformComplex (const double* ri, const double* ii,
																   double* ro, double* io,
																   bool inverse)
{
	// Following Don Cross's 1998 implementation, described by its author as public domain.

	for (auto i = 0; i < m_half; ++i)
	{
		const auto j = m_table[i];
		ro[j]		 = ri[i];
		io[j]		 = ii[i];
	}

	const auto ifactor = (inverse ? -1. : 1.);

	for (auto blockSize = 2, blockEnd = 1;
		 blockSize <= m_half;
		 blockSize <<= 1)
	{
		const auto values = [blockSize, ifactor, this]
		{
			struct four_values final
			{
				double sm1, sm2, cm1, cm2;
			};

			four_values vals;

			if (blockSize <= m_maxTabledBlock)
			{
				auto ix = 0;

				vals.sm1 = ifactor * m_sincos[ix++];
				vals.sm2 = ifactor * m_sincos[ix++];
				vals.cm1 = m_sincos[ix++];
				vals.cm2 = m_sincos[ix++];
			}
			else
			{
				const auto phase	   = 2. * math::constants::pi<double> / double (blockSize);
				const auto doublePhase = 2. * phase;

				vals.sm1 = ifactor * std::sin (phase);
				vals.sm2 = ifactor * std::sin (doublePhase);
				vals.cm1 = std::cos (phase);
				vals.cm2 = std::cos (doublePhase);
			}

			return vals;
		}();

		const auto w = 2. * values.cm1;
		double	   ar[3], ai[3];  // NOLINT

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

template <Scalar SampleType>
inline void FallbackFFT<SampleType>::makeTables()
{
	const auto bits = [halfSize = m_half]
	{
		for (auto i = 0;; ++i)
			if ((halfSize & (1 << i)) != 0)
				return i;

		// jassertfalse;
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

		m_table[i] = k;
	}

	// sin and cos tables for complex fft
	for (auto i = 2, ix = 0; i <= m_maxTabledBlock; i <<= 1)
	{
		const auto phase = 2. * math::constants::pi<double> / double (i);
		m_sincos[ix++]	 = std::sin (phase);
		m_sincos[ix++]	 = std::sin (2. * phase);
		m_sincos[ix++]	 = std::cos (phase);
		m_sincos[ix++]	 = std::cos (2. * phase);
	}

	// sin and cos tables for real-complex transform
	for (auto i = 0, ix = 0; i < m_half / 2; ++i)
	{
		const auto phase = math::constants::pi<double> * (double (i + 1) / double (m_half) + 0.5);
		m_sincos_r[ix++] = std::sin (phase);
		m_sincos_r[ix++] = std::cos (phase);
	}
}

template class FallbackFFT<float>;
template class FallbackFFT<double>;

}  // namespace limes::vecops
