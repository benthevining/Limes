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

namespace limes::vecops
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

vDSP_FloatFFT::vDSP_FloatFFT (int size)
	: FFTImpl<float> (size)
{
	static_assert (FFT<float>::isUsingVDSP());

	//!!! "If possible, tempBuffer->realp and tempBuffer->imagp should be 32-byte aligned for best performance."
	// m_buf.realp	   = allocate<float> (fft_size);
	// m_buf.imagp	   = allocate<float> (fft_size);
	// m_packed.realp = allocate<float> (fft_size / 2 + 1);
	// m_packed.imagp = allocate<float> (fft_size / 2 + 1);
	// m_spare		   = allocate<float> (fft_size + 2);
	// m_spare2	   = allocate<float> (fft_size + 2);
}

vDSP_FloatFFT::~vDSP_FloatFFT()
{
	vDSP_destroy_fftsetup (m_spec);
	// deallocate (m_spare);
	// deallocate (m_spare2);
	// deallocate (m_buf.realp);
	// deallocate (m_buf.imagp);
	// deallocate (m_packed.realp);
	// deallocate (m_packed.imagp);
}

void vDSP_FloatFFT::forward (const float* realIn, float* realOut, float* imagOut)
{
	packReal (realIn);
	vDSP_fft_zript (m_spec, &m_packed, 1, &m_buf, m_order, FFT_FORWARD);
	vDSP_denyq (m_packed.realp, m_packed.imagp, fft_size);
	unpackComplex (realOut, imagOut);
}

void vDSP_FloatFFT::forwardInterleaved (const float* realIn, float* complexOut)
{
	packReal (realIn);
	vDSP_fft_zript (m_spec, &m_packed, 1, &m_buf, m_order, FFT_FORWARD);
	vDSP_denyq (m_packed.realp, m_packed.imagp, fft_size);

	vDSP_unpackComplex (complexOut, m_packed.realp, m_packed.imagp, fft_size);
}

void vDSP_FloatFFT::forwardPolar (const float* realIn, float* magOut, float* phaseOut)
{
	packReal (realIn);
	vDSP_fft_zript (m_spec, &m_packed, 1, &m_buf, m_order, FFT_FORWARD);
	vDSP_denyq (m_packed.realp, m_packed.imagp, fft_size);

	// vDSP forward FFTs are scaled 2x (for some reason)

	const auto	   hs1	= fft_size / 2 + 1;
	constexpr auto half = 0.5f;

	vDSP_vsmul (m_packed.realp, vDSP_Stride (1), &half, m_packed.realp, vDSP_Stride (1), vDSP_Length (hs1));
	vDSP_vsmul (m_packed.imagp, vDSP_Stride (1), &half, m_packed.imagp, vDSP_Stride (1), vDSP_Length (hs1));

	vecops::cartesianToPolar (magOut, phaseOut, m_packed.realp, m_packed.imagp, hs1);
}

void vDSP_FloatFFT::forwardMagnitude (const float* realIn, float* magOut)
{
	packReal (realIn);
	vDSP_fft_zript (m_spec, &m_packed, 1, &m_buf, m_order, FFT_FORWARD);
	vDSP_denyq (m_packed.realp, m_packed.imagp, fft_size);

	const auto hs1 = fft_size / 2 + 1;

	vDSP_zvmags (&m_packed, 1, m_spare, 1, hs1);
	vvsqrtf (m_spare2, m_spare, &hs1);

	// vDSP forward FFTs are scaled 2x (for some reason)
	constexpr auto two = 2.f;
	vDSP_vsdiv (m_spare2, 1, &two, magOut, 1, hs1);
}

void vDSP_FloatFFT::inverse (const float* realIn, const float* imagIn, float* realOut)
{
	vDSP_packComplex (realIn, imagIn, m_packed.realp, m_packed.imagp, fft_size);

	vDSP_fft_zript (m_spec, &m_packed, 1, &m_buf, m_order, FFT_INVERSE);
	unpackReal (realOut);
}

void vDSP_FloatFFT::inverseInterleaved (const float* complexIn, float* realOut)
{
	float* f[2] = { m_packed.realp, m_packed.imagp };  // NOLINT

	vecops::deinterleave (f, complexIn, 2, fft_size / 2 + 1);

	vDSP_nyq (m_packed.realp, m_packed.imagp, fft_size);
	vDSP_fft_zript (m_spec, &m_packed, 1, &m_buf, m_order, FFT_INVERSE);
	unpackReal (realOut);
}

void vDSP_FloatFFT::inversePolar (const float* magIn, const float* phaseIn, float* realOut)
{
	const auto hs1 = fft_size / 2 + 1;

	vvsincosf (m_packed.imagp, m_packed.realp, phaseIn, &hs1);

	vDSP_vsmul (m_packed.realp, vDSP_Stride (1), magIn, m_packed.realp, vDSP_Stride (1), vDSP_Length (hs1));
	vDSP_vsmul (m_packed.imagp, vDSP_Stride (1), magIn, m_packed.imagp, vDSP_Stride (1), vDSP_Length (hs1));

	vDSP_nyq (m_packed.realp, m_packed.imagp, fft_size);
	vDSP_fft_zript (m_spec, &m_packed, 1, &m_buf, m_order, FFT_INVERSE);
	unpackReal (realOut);
}

void vDSP_FloatFFT::inverseCepstral (const float* magIn, float* cepOut)
{
	const auto hs1 = fft_size / 2 + 1;

	vecops::copy (m_spare, magIn, hs1);

	vDSP_vsadd (m_spare, vDSP_Stride (1), &shiftAmount<float>, m_spare, vDSP_Stride (1), vDSP_Length (hs1));

	vvlogf (m_spare2, m_spare, &hs1);
	inverse (m_spare2, nullptr, cepOut);
}

LIMES_FORCE_INLINE void vDSP_FloatFFT::packReal (const float* const re)
{
	vDSP_ctoz (reinterpret_cast<const DSPComplex* const> (re),
			   2, &m_packed, 1, fft_size / 2);
}

LIMES_FORCE_INLINE void vDSP_FloatFFT::unpackReal (float* const re)
{
	vDSP_ztoc (&m_packed, 1, reinterpret_cast<DSPComplex* const> (re),
			   2, fft_size / 2);
}

LIMES_FORCE_INLINE void vDSP_FloatFFT::unpackComplex (float* const re, float* const im)
{
	// vDSP forward FFTs are scaled 2x (for some reason)
	constexpr auto two = 2.f;
	vDSP_vsdiv (m_packed.realp, vDSP_Stride (1), &two, re, vDSP_Stride (1), vDSP_Length (fft_size / 2 + 1));
	vDSP_vsdiv (m_packed.imagp, vDSP_Stride (1), &two, im, vDSP_Stride (1), vDSP_Length (fft_size / 2 + 1));
}

/*---------------------------------------------------------------------------------------------------------------------------*/

vDSP_DoubleFFT::vDSP_DoubleFFT (int size)
	: FFTImpl<double> (size)
{
	static_assert (FFT<double>::isUsingVDSP());

	//!!! "If possible, tempBuffer->realp and tempBuffer->imagp should be 32-byte aligned for best performance."
	// m_buf.realp	   = allocate<double> (fft_size);
	// m_buf.imagp	   = allocate<double> (fft_size);
	// m_packed.realp = allocate<double> (fft_size / 2 + 1);
	// m_packed.imagp = allocate<double> (fft_size / 2 + 1);
	// m_spare		   = allocate<double> (fft_size + 2);
	// m_spare2	   = allocate<double> (fft_size + 2);
}

vDSP_DoubleFFT::~vDSP_DoubleFFT()
{
	vDSP_destroy_fftsetupD (m_spec);
	// deallocate (m_spare);
	// deallocate (m_spare2);
	// deallocate (m_buf.realp);
	// deallocate (m_buf.imagp);
	// deallocate (m_packed.realp);
	// deallocate (m_packed.imagp);
}

void vDSP_DoubleFFT::forward (const double* realIn, double* realOut, double* imagOut)
{
	packReal (realIn);
	vDSP_fft_zriptD (m_spec, &m_packed, vDSP_Stride (1), &m_buf, vDSP_Length (m_order), FFT_FORWARD);
	vDSP_denyq (m_packed.realp, m_packed.imagp, fft_size);
	unpackComplex (realOut, imagOut);
}

void vDSP_DoubleFFT::forwardInterleaved (const double* realIn, double* complexOut)
{
	packReal (realIn);
	vDSP_fft_zriptD (m_spec, &m_packed, 1, &m_buf, m_order, FFT_FORWARD);
	vDSP_denyq (m_packed.realp, m_packed.imagp, fft_size);

	vDSP_unpackComplex (complexOut, m_packed.realp, m_packed.imagp, fft_size);
}

void vDSP_DoubleFFT::forwardPolar (const double* realIn, double* magOut, double* phaseOut)
{
	packReal (realIn);
	vDSP_fft_zriptD (m_spec, &m_packed, 1, &m_buf, m_order, FFT_FORWARD);
	vDSP_denyq (m_packed.realp, m_packed.imagp, fft_size);

	// vDSP forward FFTs are scaled 2x (for some reason)

	const auto	   hs1	= fft_size / 2 + 1;
	constexpr auto half = 0.5;

	vDSP_vsmulD (m_packed.realp, vDSP_Stride (1), &half, m_packed.realp, vDSP_Stride (1), vDSP_Length (hs1));
	vDSP_vsmulD (m_packed.imagp, vDSP_Stride (1), &half, m_packed.imagp, vDSP_Stride (1), vDSP_Length (hs1));

	vecops::cartesianToPolar (magOut, phaseOut, m_packed.realp, m_packed.imagp, hs1);
}

void vDSP_DoubleFFT::forwardMagnitude (const double* realIn, double* magOut)
{
	packReal (realIn);
	vDSP_fft_zriptD (m_spec, &m_packed, 1, &m_buf, m_order, FFT_FORWARD);
	vDSP_denyq (m_packed.realp, m_packed.imagp, fft_size);

	const auto hs1 = fft_size / 2 + 1;

	vDSP_zvmagsD (&m_packed, 1, m_spare, 1, hs1);
	vvsqrt (m_spare2, m_spare, &hs1);

	// vDSP forward FFTs are scaled 2x (for some reason)
	constexpr auto two = 2.;
	vDSP_vsdivD (m_spare2, 1, &two, magOut, 1, hs1);
}

void vDSP_DoubleFFT::inverse (const double* realIn, const double* imagIn, double* realOut)
{
	vDSP_packComplex (realIn, imagIn, m_packed.realp, m_packed.imagp, fft_size);
	vDSP_fft_zriptD (m_spec, &m_packed, 1, &m_buf, m_order, FFT_INVERSE);
	unpackReal (realOut);
}

void vDSP_DoubleFFT::inverseInterleaved (const double* complexIn, double* realOut)
{
	double* f[2] = { m_packed.realp, m_packed.imagp };	// NOLINT

	vecops::deinterleave (f, complexIn, 2, fft_size / 2 + 1);

	vDSP_nyq (m_packed.realp, m_packed.imagp, fft_size);
	vDSP_fft_zriptD (m_spec, &m_packed, 1, &m_buf, m_order, FFT_INVERSE);
	unpackReal (realOut);
}

void vDSP_DoubleFFT::inversePolar (const double* magIn, const double* phaseIn, double* realOut)
{
	const auto hs1 = fft_size / 2 + 1;

	vvsincos (m_packed.imagp, m_packed.realp, phaseIn, &hs1);

	vDSP_vsmulD (m_packed.realp, vDSP_Stride (1), magIn, m_packed.realp, vDSP_Stride (1), vDSP_Length (hs1));
	vDSP_vsmulD (m_packed.imagp, vDSP_Stride (1), magIn, m_packed.imagp, vDSP_Stride (1), vDSP_Length (hs1));

	vDSP_nyq (m_packed.realp, m_packed.imagp, fft_size);
	vDSP_fft_zriptD (m_spec, &m_packed, 1, &m_buf, m_order, FFT_INVERSE);
	unpackReal (realOut);
}

void vDSP_DoubleFFT::inverseCepstral (const double* magIn, double* cepOut)
{
	const auto hs1 = fft_size / 2 + 1;

	vecops::copy (m_spare, magIn, hs1);

	vDSP_vsaddD (m_spare, vDSP_Stride (1), &shiftAmount<double>, m_spare, vDSP_Stride (1), vDSP_Length (hs1));

	vvlog (m_spare2, m_spare, &hs1);
	inverse (m_spare2, nullptr, cepOut);
}

LIMES_FORCE_INLINE void vDSP_DoubleFFT::packReal (const double* const re)
{
	vDSP_ctozD (reinterpret_cast<const DSPDoubleComplex* const> (re),
				2, &m_packed, 1, fft_size / 2);
}

LIMES_FORCE_INLINE void vDSP_DoubleFFT::unpackReal (double* const re)
{
	vDSP_ztocD (&m_packed, 1, reinterpret_cast<DSPDoubleComplex* const> (re),
				2, fft_size / 2);
}

LIMES_FORCE_INLINE void vDSP_DoubleFFT::unpackComplex (double* const re, double* const im)
{
	// vDSP forward FFTs are scaled 2x (for some reason)
	constexpr auto two = 2.;
	vDSP_vsdivD (m_packed.realp, vDSP_Stride (1), &two, re, vDSP_Stride (1), vDSP_Length (fft_size / 2 + 1));
	vDSP_vsdivD (m_packed.imagp, vDSP_Stride (1), &two, im, vDSP_Stride (1), vDSP_Length (fft_size / 2 + 1));
}

}  // namespace limes::vecops
