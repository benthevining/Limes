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

#include "vdsp_fft.h"
#include <limes_platform.h>
#include <limes_namespace.h>
#include <limes_core.h>

LIMES_BEGIN_NAMESPACE

namespace vecops
{

template <Scalar SampleType>
LIMES_FORCE_INLINE void vDSP_nyq (SampleType* real, SampleType* imag, int fft_size)
{
	// for ifft input in packed form, pack the DC and Nyquist bins
	const auto hs = fft_size / 2;

	imag[0]	 = real[hs];
	real[hs] = SampleType (0);
	imag[hs] = SampleType (0);
}

template <Scalar SampleType>
LIMES_FORCE_INLINE void vDSP_denyq (SampleType* real, SampleType* imag, int fft_size)
{
	// for fft result in packed form, unpack the DC and Nyquist bins
	const auto hs = fft_size / 2;

	real[hs] = imag[0];
	imag[hs] = SampleType (0);
	imag[0]	 = SampleType (0);
}

template <Scalar SampleType>
LIMES_FORCE_INLINE void vDSP_packComplex (const SampleType* const re, const SampleType* const im,
										  SampleType* realPacked, SampleType* imagPacked, int fft_size)
{
	const auto num = fft_size / 2 + 1;

	if (re == nullptr)
		vecops::clear (realPacked, num);
	else
		vecops::copy (realPacked, re, num);

	if (im == nullptr)
		vecops::clear (imagPacked, num);
	else
		vecops::copy (imagPacked, im, num);

	vDSP_nyq (realPacked, imagPacked, fft_size);
}

template <Scalar SampleType>
LIMES_FORCE_INLINE void vDSP_unpackComplex (SampleType* const		cplx,
											const SampleType* const packedReal,
											const SampleType* const packedImag,
											int						fft_size)
{
	// vDSP forward FFTs are scaled 2x (for some reason)
	for (auto i = 0; i < fft_size / 2 + 1; ++i)
	{
		cplx[static_cast<ptrdiff_t> (i * 2)]	 = packedReal[i] * SampleType (0.5);
		cplx[static_cast<ptrdiff_t> (i * 2 + 1)] = packedImag[i] * SampleType (0.5);
	}
}

/*---------------------------------------------------------------------------------------------------------------------------*/

static constexpr std::size_t vDSPalignment = 32UL;

template <Scalar SampleType>
vDSP_FFT<SampleType>::vDSP_FFT (int size)
	: FFTImpl<SampleType> (size),
	  m_spare (static_cast<std::size_t> (this->fft_size) + 2, vDSPalignment, SampleType (0)),
	  m_spare2 (static_cast<std::size_t> (this->fft_size) + 2, vDSPalignment, SampleType (0))
{
	static_assert (FFT<SampleType>::isUsingVDSP());

	if constexpr (std::is_same_v<SampleType, float>)
		m_spec = vDSP_create_fftsetup (vDSP_Length (this->m_order), FFT_RADIX2);
	else
		m_spec = vDSP_create_fftsetupD (vDSP_Length (this->m_order), FFT_RADIX2);

	m_buf.realp = allocate_aligned<SampleType> (static_cast<std::size_t> (this->fft_size), vDSPalignment, SampleType (0));
	m_buf.imagp = allocate_aligned<SampleType> (static_cast<std::size_t> (this->fft_size), vDSPalignment, SampleType (0));

	m_packed.realp = allocate_aligned<SampleType> (static_cast<std::size_t> (this->fft_size) / 2 + 1, vDSPalignment, SampleType (0));
	m_packed.imagp = allocate_aligned<SampleType> (static_cast<std::size_t> (this->fft_size) / 2 + 1, vDSPalignment, SampleType (0));
}

template <Scalar SampleType>
vDSP_FFT<SampleType>::~vDSP_FFT()
{
	if constexpr (std::is_same_v<SampleType, float>)
		vDSP_destroy_fftsetup (m_spec);
	else
		vDSP_destroy_fftsetupD (m_spec);

	deallocate_aligned (m_buf.realp);
	deallocate_aligned (m_buf.imagp);
	deallocate_aligned (m_packed.realp);
	deallocate_aligned (m_packed.imagp);
}

template <Scalar SampleType>
void vDSP_FFT<SampleType>::forward (const SampleType* realIn, SampleType* realOut, SampleType* imagOut)
{
	packReal (realIn);

	if constexpr (std::is_same_v<SampleType, float>)
		vDSP_fft_zript (m_spec, &m_packed, vDSP_Stride (1), &m_buf, vDSP_Length (this->m_order), FFT_FORWARD);
	else
		vDSP_fft_zriptD (m_spec, &m_packed, vDSP_Stride (1), &m_buf, vDSP_Length (this->m_order), FFT_FORWARD);

	vDSP_denyq (m_packed.realp, m_packed.imagp, this->fft_size);

	unpackComplex (realOut, imagOut);
}

template <Scalar SampleType>
void vDSP_FFT<SampleType>::forwardInterleaved (const SampleType* realIn, SampleType* complexOut)
{
	packReal (realIn);

	if constexpr (std::is_same_v<SampleType, float>)
		vDSP_fft_zript (m_spec, &m_packed, vDSP_Stride (1), &m_buf, vDSP_Length (this->m_order), FFT_FORWARD);
	else
		vDSP_fft_zriptD (m_spec, &m_packed, vDSP_Stride (1), &m_buf, vDSP_Length (this->m_order), FFT_FORWARD);

	vDSP_denyq (m_packed.realp, m_packed.imagp, this->fft_size);

	vDSP_unpackComplex (complexOut, m_packed.realp, m_packed.imagp, this->fft_size);
}

template <Scalar SampleType>
void vDSP_FFT<SampleType>::forwardPolar (const SampleType* realIn, SampleType* magOut, SampleType* phaseOut)
{
	packReal (realIn);

	if constexpr (std::is_same_v<SampleType, float>)
		vDSP_fft_zript (m_spec, &m_packed, vDSP_Stride (1), &m_buf, vDSP_Length (this->m_order), FFT_FORWARD);
	else
		vDSP_fft_zriptD (m_spec, &m_packed, vDSP_Stride (1), &m_buf, vDSP_Length (this->m_order), FFT_FORWARD);

	vDSP_denyq (m_packed.realp, m_packed.imagp, this->fft_size);

	// vDSP forward FFTs are scaled 2x (for some reason)

	const auto	   hs1	= this->fft_size / 2 + 1;
	constexpr auto half = SampleType (0.5);

	if constexpr (std::is_same_v<SampleType, float>)
	{
		vDSP_vsmul (m_packed.realp, vDSP_Stride (1), &half, m_packed.realp, vDSP_Stride (1), vDSP_Length (hs1));
		vDSP_vsmul (m_packed.imagp, vDSP_Stride (1), &half, m_packed.imagp, vDSP_Stride (1), vDSP_Length (hs1));
	}
	else
	{
		vDSP_vsmulD (m_packed.realp, vDSP_Stride (1), &half, m_packed.realp, vDSP_Stride (1), vDSP_Length (hs1));
		vDSP_vsmulD (m_packed.imagp, vDSP_Stride (1), &half, m_packed.imagp, vDSP_Stride (1), vDSP_Length (hs1));
	}

	vecops::cartesianToPolar (magOut, phaseOut, m_packed.realp, m_packed.imagp, hs1);
}

template <Scalar SampleType>
void vDSP_FFT<SampleType>::forwardMagnitude (const SampleType* realIn, SampleType* magOut)
{
	packReal (realIn);

	if constexpr (std::is_same_v<SampleType, float>)
		vDSP_fft_zript (m_spec, &m_packed, vDSP_Stride (1), &m_buf, vDSP_Length (this->m_order), FFT_FORWARD);
	else
		vDSP_fft_zriptD (m_spec, &m_packed, vDSP_Stride (1), &m_buf, vDSP_Length (this->m_order), FFT_FORWARD);

	vDSP_denyq (m_packed.realp, m_packed.imagp, this->fft_size);

	const auto hs1 = this->fft_size / 2 + 1;

	constexpr auto two = SampleType (2.);

	// vDSP forward FFTs are scaled 2x (for some reason)
	if constexpr (std::is_same_v<SampleType, float>)
	{
		vDSP_zvmags (&m_packed, vDSP_Stride (1), m_spare, vDSP_Stride (1), static_cast<vDSP_Length> (hs1));
		vvsqrtf (m_spare2, m_spare, &hs1);
		vDSP_vsdiv (m_spare2, vDSP_Stride (1), &two, magOut, vDSP_Stride (1), static_cast<vDSP_Length> (hs1));
	}
	else
	{
		vDSP_zvmagsD (&m_packed, vDSP_Stride (1), m_spare, vDSP_Stride (1), static_cast<vDSP_Length> (hs1));
		vvsqrt (m_spare2, m_spare, &hs1);
		vDSP_vsdivD (m_spare2, vDSP_Stride (1), &two, magOut, vDSP_Stride (1), static_cast<vDSP_Length> (hs1));
	}
}

template <Scalar SampleType>
void vDSP_FFT<SampleType>::inverse (const SampleType* realIn, const SampleType* imagIn, SampleType* realOut)
{
	vDSP_packComplex (realIn, imagIn, m_packed.realp, m_packed.imagp, this->fft_size);

	if constexpr (std::is_same_v<SampleType, float>)
		vDSP_fft_zript (m_spec, &m_packed, vDSP_Stride (1), &m_buf, vDSP_Length (this->m_order), FFT_INVERSE);
	else
		vDSP_fft_zriptD (m_spec, &m_packed, vDSP_Stride (1), &m_buf, vDSP_Length (this->m_order), FFT_INVERSE);

	unpackReal (realOut);
}

template <Scalar SampleType>
void vDSP_FFT<SampleType>::inverseInterleaved (const SampleType* complexIn, SampleType* realOut)
{
	SampleType* f[2] = { m_packed.realp, m_packed.imagp };	// NOLINT

	vecops::deinterleave (f, complexIn, 2, this->fft_size / 2 + 1);

	vDSP_nyq (m_packed.realp, m_packed.imagp, this->fft_size);

	if constexpr (std::is_same_v<SampleType, float>)
		vDSP_fft_zript (m_spec, &m_packed, vDSP_Stride (1), &m_buf, vDSP_Length (this->m_order), FFT_INVERSE);
	else
		vDSP_fft_zriptD (m_spec, &m_packed, vDSP_Stride (1), &m_buf, vDSP_Length (this->m_order), FFT_INVERSE);

	unpackReal (realOut);
}

template <Scalar SampleType>
void vDSP_FFT<SampleType>::inversePolar (const SampleType* magIn, const SampleType* phaseIn, SampleType* realOut)
{
	const auto hs1 = this->fft_size / 2 + 1;

	if constexpr (std::is_same_v<SampleType, float>)
	{
		vvsincosf (m_packed.imagp, m_packed.realp, phaseIn, &hs1);

		vDSP_vsmul (m_packed.realp, vDSP_Stride (1), magIn, m_packed.realp, vDSP_Stride (1), vDSP_Length (hs1));
		vDSP_vsmul (m_packed.imagp, vDSP_Stride (1), magIn, m_packed.imagp, vDSP_Stride (1), vDSP_Length (hs1));
	}
	else
	{
		vvsincos (m_packed.imagp, m_packed.realp, phaseIn, &hs1);

		vDSP_vsmulD (m_packed.realp, vDSP_Stride (1), magIn, m_packed.realp, vDSP_Stride (1), vDSP_Length (hs1));
		vDSP_vsmulD (m_packed.imagp, vDSP_Stride (1), magIn, m_packed.imagp, vDSP_Stride (1), vDSP_Length (hs1));
	}

	vDSP_nyq (m_packed.realp, m_packed.imagp, this->fft_size);

	if constexpr (std::is_same_v<SampleType, float>)
		vDSP_fft_zript (m_spec, &m_packed, vDSP_Stride (1), &m_buf, vDSP_Length (this->m_order), FFT_INVERSE);
	else
		vDSP_fft_zriptD (m_spec, &m_packed, vDSP_Stride (1), &m_buf, vDSP_Length (this->m_order), FFT_INVERSE);

	unpackReal (realOut);
}

template <Scalar SampleType>
void vDSP_FFT<SampleType>::inverseCepstral (const SampleType* magIn, SampleType* cepOut)
{
	const auto hs1 = this->fft_size / 2 + 1;

	vecops::copy (m_spare.get(), magIn, hs1);

	if constexpr (std::is_same_v<SampleType, float>)
	{
		vDSP_vsadd (m_spare, vDSP_Stride (1), &shiftAmount<SampleType>, m_spare, vDSP_Stride (1), vDSP_Length (hs1));
		vvlogf (m_spare2, m_spare, &hs1);
	}
	else
	{
		vDSP_vsaddD (m_spare, vDSP_Stride (1), &shiftAmount<double>, m_spare, vDSP_Stride (1), vDSP_Length (hs1));
		vvlog (m_spare2, m_spare, &hs1);
	}

	inverse (m_spare2, nullptr, cepOut);
}

template <Scalar SampleType>
LIMES_FORCE_INLINE void vDSP_FFT<SampleType>::packReal (const SampleType* const re)
{
	if constexpr (std::is_same_v<SampleType, float>)
	{
		vDSP_ctoz (reinterpret_cast<const DSPComplexType* const> (re),
				   vDSP_Stride (2), &m_packed, vDSP_Stride (1), vDSP_Length (this->fft_size) / 2);
	}
	else
	{
		vDSP_ctozD (reinterpret_cast<const DSPDoubleComplex* const> (re),
					vDSP_Stride (2), &m_packed, vDSP_Stride (1), vDSP_Length (this->fft_size) / 2);
	}
}

template <Scalar SampleType>
LIMES_FORCE_INLINE void vDSP_FFT<SampleType>::unpackReal (SampleType* const re)
{
	if constexpr (std::is_same_v<SampleType, float>)
	{
		vDSP_ztoc (&m_packed, vDSP_Stride (1), reinterpret_cast<DSPComplexType* const> (re),
				   vDSP_Stride (2), vDSP_Length (this->fft_size) / 2);
	}
	else
	{
		vDSP_ztocD (&m_packed, vDSP_Stride (1), reinterpret_cast<DSPDoubleComplex* const> (re),
					vDSP_Stride (2), vDSP_Length (this->fft_size) / 2);
	}
}

template <Scalar SampleType>
LIMES_FORCE_INLINE void vDSP_FFT<SampleType>::unpackComplex (SampleType* const re, SampleType* const im)
{
	// vDSP forward FFTs are scaled 2x (for some reason)
	constexpr auto two = SampleType (2.);

	if constexpr (std::is_same_v<SampleType, float>)
	{
		vDSP_vsdiv (m_packed.realp, vDSP_Stride (1), &two, re, vDSP_Stride (1), vDSP_Length (this->fft_size / 2 + 1));
		vDSP_vsdiv (m_packed.imagp, vDSP_Stride (1), &two, im, vDSP_Stride (1), vDSP_Length (this->fft_size / 2 + 1));
	}
	else
	{
		vDSP_vsdivD (m_packed.realp, vDSP_Stride (1), &two, re, vDSP_Stride (1), vDSP_Length (this->fft_size / 2 + 1));
		vDSP_vsdivD (m_packed.imagp, vDSP_Stride (1), &two, im, vDSP_Stride (1), vDSP_Length (this->fft_size / 2 + 1));
	}
}

template class vDSP_FFT<float>;
template class vDSP_FFT<double>;

}  // namespace vecops

LIMES_END_NAMESPACE
