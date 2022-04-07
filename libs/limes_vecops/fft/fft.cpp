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

#include "fft.h"
#include <cstddef>
#include <cmath>
#include <type_traits>
#include <string_view>
#include "limes_vecops.h"

#if LIMES_VECOPS_USE_FFTW
#	include <fftw3.h>

#	ifndef FFTW_SINGLE_ONLY
#		define FFTW_SINGLE_ONLY 0
#	endif

#	ifndef FFTW_DOUBLE_ONLY
#		define FFTW_DOUBLE_ONLY 0
#	endif

#	if (FFTW_SINGLE_ONLY && FFTW_DOUBLE_ONLY)
#		error FFTW_SINGLE_ONLY and FFTW_DOUBLE_ONLY cannot both be defined to 1!
#	endif
#endif

namespace limes::vecops
{

static inline int orderFromFFTSize (int size)
{
	for (int i = 0;; ++i)
		if ((size & (1 << i)) != 0)
			return i;

	// jassertfalse;
	return 0;
}

template <Scalar SampleType>
constexpr SampleType shiftAmount = SampleType (0.000001);


template <Scalar SampleType>
class FFT<SampleType>::FFTImpl
{
public:

	explicit FFTImpl (int size)
		: fft_size (size), m_order (orderFromFFTSize (size))
	{
	}

	virtual ~FFTImpl() = default;

	[[nodiscard]] int getSize() const noexcept
	{
		return fft_size;
	}

	virtual void forward (const SampleType* realIn, SampleType* realOut, SampleType* imagOut) = 0;

	virtual void forwardInterleaved (const SampleType* realIn, SampleType* complexOut) = 0;

	virtual void forwardPolar (const SampleType* realIn, SampleType* magOut, SampleType* phaseOut) = 0;

	virtual void forwardMagnitude (const SampleType* realIn, SampleType* magOut) = 0;

	virtual void inverse (const SampleType* realIn, const SampleType* imagIn, SampleType* realOut) = 0;

	virtual void inverseInterleaved (const SampleType* complexIn, SampleType* realOut) = 0;

	virtual void inversePolar (const SampleType* magIn, const SampleType* phaseIn, SampleType* realOut) = 0;

	virtual void inverseCepstral (const SampleType* magIn, SampleType* cepOut) = 0;

protected:

	const int fft_size;	 // NOLINT
	const int m_order;	 // NOLINT
};

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark vDSP FFT

#if LIMES_VECOPS_USE_VDSP

template <Scalar SampleType>
inline void vDSP_nyq (SampleType* real, SampleType* imag, int fft_size)
{
	// for ifft input in packed form, pack the DC and Nyquist bins
	const auto hs = fft_size / 2;

	imag[0]	 = real[hs];
	real[hs] = SampleType (0);
	imag[hs] = SampleType (0);
}

template <Scalar SampleType>
inline void vDSP_denyq (SampleType* real, SampleType* imag, int fft_size)
{
	// for fft result in packed form, unpack the DC and Nyquist bins
	const auto hs = fft_size / 2;

	real[hs] = imag[0];
	imag[hs] = SampleType (0);
	imag[0]	 = SampleType (0);
}

template <Scalar SampleType>
inline void vDSP_packComplex (const SampleType* const re, const SampleType* const im,
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
inline void vDSP_unpackComplex (SampleType* const cplx,
								SampleType*		  packedReal,
								SampleType*		  packedImag,
								int				  fft_size)
{
	// vDSP forward FFTs are scaled 2x (for some reason)
	for (auto i = 0; i < fft_size / 2 + 1; ++i)
	{
		cplx[static_cast<ptrdiff_t> (i * 2)]	 = packedReal[i] * SampleType (0.5);
		cplx[static_cast<ptrdiff_t> (i * 2 + 1)] = packedImag[i] * SampleType (0.5);
	}
}


class vDSP_FloatFFT final : public FFT<float>::FFTImpl
{
public:

	explicit vDSP_FloatFFT (int size)
		: FFT<float>::FFTImpl (size)
	{
		//!!! "If possible, tempBuffer->realp and tempBuffer->imagp should be 32-byte aligned for best performance."
		// m_buf.realp	   = allocate<float> (fft_size);
		// m_buf.imagp	   = allocate<float> (fft_size);
		// m_packed.realp = allocate<float> (fft_size / 2 + 1);
		// m_packed.imagp = allocate<float> (fft_size / 2 + 1);
		// m_spare		   = allocate<float> (fft_size + 2);
		// m_spare2	   = allocate<float> (fft_size + 2);
	}

	~vDSP_FloatFFT() final
	{
		vDSP_destroy_fftsetup (m_spec);
		// deallocate (m_spare);
		// deallocate (m_spare2);
		// deallocate (m_buf.realp);
		// deallocate (m_buf.imagp);
		// deallocate (m_packed.realp);
		// deallocate (m_packed.imagp);
	}

	vDSP_FloatFFT (const vDSP_FloatFFT&) = delete;

	vDSP_FloatFFT& operator= (const vDSP_FloatFFT&) = delete;

	vDSP_FloatFFT (vDSP_FloatFFT&&) = delete;

	vDSP_FloatFFT& operator= (vDSP_FloatFFT&&) = delete;

private:

	void forward (const float* realIn, float* realOut, float* imagOut) final
	{
		packReal (realIn);
		vDSP_fft_zript (m_spec, &m_packed, 1, &m_buf, m_order, FFT_FORWARD);
		vDSP_denyq (m_packed.realp, m_packed.imagp, fft_size);
		unpackComplex (realOut, imagOut);
	}

	void forwardInterleaved (const float* realIn, float* complexOut) final
	{
		packReal (realIn);
		vDSP_fft_zript (m_spec, &m_packed, 1, &m_buf, m_order, FFT_FORWARD);
		vDSP_denyq (m_packed.realp, m_packed.imagp, fft_size);

		vDSP_unpackComplex (complexOut, m_packed.realp, m_packed.imagp, fft_size);
	}

	void forwardPolar (const float* realIn, float* magOut, float* phaseOut) final
	{
		packReal (realIn);
		vDSP_fft_zript (m_spec, &m_packed, 1, &m_buf, m_order, FFT_FORWARD);
		vDSP_denyq (m_packed.realp, m_packed.imagp, fft_size);

		// vDSP forward FFTs are scaled 2x (for some reason)

		const auto	   hs1	= fft_size / 2 + 1;
		constexpr auto half = 0.5f;

		vDSP_vsmul (m_packed.realp, vDSP_Stride (1), &half, m_packed.realp, vDSP_Stride (1), vDSP_Length (hs1));
		vDSP_vsmul (m_packed.imagp, vDSP_Stride (1), &half, m_packed.imagp, vDSP_Stride (1), vDSP_Length (hs1));

		// v_cartesian_to_polar(magOut, phaseOut,
		//                      m_packed.realp, m_packed.imagp, hs1);
	}

	void forwardMagnitude (const float* realIn, float* magOut) final
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

	void inverse (const float* realIn, const float* imagIn, float* realOut) final
	{
		vDSP_packComplex (realIn, imagIn, m_packed.realp, m_packed.imagp, fft_size);

		vDSP_fft_zript (m_spec, &m_packed, 1, &m_buf, m_order, FFT_INVERSE);
		unpackReal (realOut);
	}

	void inverseInterleaved (const float* complexIn, float* realOut) final
	{
		float* f[2] = { m_packed.realp, m_packed.imagp };  // NOLINT

		vecops::deinterleave (f, complexIn, 2, fft_size / 2 + 1);

		vDSP_nyq (m_packed.realp, m_packed.imagp, fft_size);
		vDSP_fft_zript (m_spec, &m_packed, 1, &m_buf, m_order, FFT_INVERSE);
		unpackReal (realOut);
	}

	void inversePolar (const float* magIn, const float* phaseIn, float* realOut) final
	{
		const auto hs1 = fft_size / 2 + 1;

		vvsincosf (m_packed.imagp, m_packed.realp, phaseIn, &hs1);

		vDSP_vsmul (m_packed.realp, vDSP_Stride (1), magIn, m_packed.realp, vDSP_Stride (1), vDSP_Length (hs1));
		vDSP_vsmul (m_packed.imagp, vDSP_Stride (1), magIn, m_packed.imagp, vDSP_Stride (1), vDSP_Length (hs1));

		vDSP_nyq (m_packed.realp, m_packed.imagp, fft_size);
		vDSP_fft_zript (m_spec, &m_packed, 1, &m_buf, m_order, FFT_INVERSE);
		unpackReal (realOut);
	}

	void inverseCepstral (const float* magIn, float* cepOut) final
	{
		const auto hs1 = fft_size / 2 + 1;

		vecops::copy (m_spare, magIn, hs1);

		vDSP_vsadd (m_spare, vDSP_Stride (1), &shiftAmount<float>, m_spare, vDSP_Stride (1), vDSP_Length (hs1));

		vvlogf (m_spare2, m_spare, &hs1);
		inverse (m_spare2, nullptr, cepOut);
	}

	void packReal (const float* const re)
	{
		vDSP_ctoz (reinterpret_cast<const DSPComplex* const> (re),
				   2, &m_packed, 1, fft_size / 2);
	}

	void unpackReal (float* const re)
	{
		vDSP_ztoc (&m_packed, 1, reinterpret_cast<DSPComplex* const> (re),
				   2, fft_size / 2);
	}

	void unpackComplex (float* const re, float* const im)
	{
		// vDSP forward FFTs are scaled 2x (for some reason)
		constexpr auto two = 2.f;
		vDSP_vsdiv (m_packed.realp, vDSP_Stride (1), &two, re, vDSP_Stride (1), vDSP_Length (fft_size / 2 + 1));
		vDSP_vsdiv (m_packed.imagp, vDSP_Stride (1), &two, im, vDSP_Stride (1), vDSP_Length (fft_size / 2 + 1));
	}

	FFTSetup		m_spec { vDSP_create_fftsetup (m_order, FFT_RADIX2) };
	float*			m_spare;
	float*			m_spare2;
	DSPSplitComplex m_buf, m_packed;
};


class vDSP_DoubleFFT final : public FFT<double>::FFTImpl
{
public:

	explicit vDSP_DoubleFFT (int size)
		: FFT<double>::FFTImpl (size)
	{
		//!!! "If possible, tempBuffer->realp and tempBuffer->imagp should be 32-byte aligned for best performance."
		// m_buf.realp	   = allocate<double> (fft_size);
		// m_buf.imagp	   = allocate<double> (fft_size);
		// m_packed.realp = allocate<double> (fft_size / 2 + 1);
		// m_packed.imagp = allocate<double> (fft_size / 2 + 1);
		// m_spare		   = allocate<double> (fft_size + 2);
		// m_spare2	   = allocate<double> (fft_size + 2);
	}

	~vDSP_DoubleFFT() final
	{
		vDSP_destroy_fftsetupD (m_spec);
		// deallocate (m_spare);
		// deallocate (m_spare2);
		// deallocate (m_buf.realp);
		// deallocate (m_buf.imagp);
		// deallocate (m_packed.realp);
		// deallocate (m_packed.imagp);
	}

	vDSP_DoubleFFT (const vDSP_DoubleFFT&) = delete;

	vDSP_DoubleFFT& operator= (const vDSP_DoubleFFT&) = delete;

	vDSP_DoubleFFT (vDSP_DoubleFFT&&) = delete;

	vDSP_DoubleFFT& operator= (vDSP_DoubleFFT&&) = delete;

private:

	void forward (const double* realIn, double* realOut, double* imagOut) final
	{
		packReal (realIn);
		vDSP_fft_zriptD (m_spec, &m_packed, vDSP_Stride (1), &m_buf, vDSP_Length (m_order), FFT_FORWARD);
		vDSP_denyq (m_packed.realp, m_packed.imagp, fft_size);
		unpackComplex (realOut, imagOut);
	}

	void forwardInterleaved (const double* realIn, double* complexOut) final
	{
		packReal (realIn);
		vDSP_fft_zriptD (m_spec, &m_packed, 1, &m_buf, m_order, FFT_FORWARD);
		vDSP_denyq (m_packed.realp, m_packed.imagp, fft_size);

		vDSP_unpackComplex (complexOut, m_packed.realp, m_packed.imagp, fft_size);
	}

	void forwardPolar (const double* realIn, double* magOut, double* phaseOut) final
	{
		packReal (realIn);
		vDSP_fft_zriptD (m_spec, &m_packed, 1, &m_buf, m_order, FFT_FORWARD);
		vDSP_denyq (m_packed.realp, m_packed.imagp, fft_size);

		// vDSP forward FFTs are scaled 2x (for some reason)

		const auto	   hs1	= fft_size / 2 + 1;
		constexpr auto half = 0.5;

		vDSP_vsmulD (m_packed.realp, vDSP_Stride (1), &half, m_packed.realp, vDSP_Stride (1), vDSP_Length (hs1));
		vDSP_vsmulD (m_packed.imagp, vDSP_Stride (1), &half, m_packed.imagp, vDSP_Stride (1), vDSP_Length (hs1));

		// v_cartesian_to_polar(magOut, phaseOut,
		//                      m_packed.realp, m_packed.imagp, hs1);
	}

	void forwardMagnitude (const double* realIn, double* magOut) final
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

	void inverse (const double* realIn, const double* imagIn, double* realOut) final
	{
		vDSP_packComplex (realIn, imagIn, m_packed.realp, m_packed.imagp, fft_size);
		vDSP_fft_zriptD (m_spec, &m_packed, 1, &m_buf, m_order, FFT_INVERSE);
		unpackReal (realOut);
	}

	void inverseInterleaved (const double* complexIn, double* realOut) final
	{
		double* f[2] = { m_packed.realp, m_packed.imagp };	// NOLINT

		vecops::deinterleave (f, complexIn, 2, fft_size / 2 + 1);

		vDSP_nyq (m_packed.realp, m_packed.imagp, fft_size);
		vDSP_fft_zriptD (m_spec, &m_packed, 1, &m_buf, m_order, FFT_INVERSE);
		unpackReal (realOut);
	}

	void inversePolar (const double* magIn, const double* phaseIn, double* realOut) final
	{
		const auto hs1 = fft_size / 2 + 1;

		vvsincos (m_packed.imagp, m_packed.realp, phaseIn, &hs1);

		vDSP_vsmulD (m_packed.realp, vDSP_Stride (1), magIn, m_packed.realp, vDSP_Stride (1), vDSP_Length (hs1));
		vDSP_vsmulD (m_packed.imagp, vDSP_Stride (1), magIn, m_packed.imagp, vDSP_Stride (1), vDSP_Length (hs1));

		vDSP_nyq (m_packed.realp, m_packed.imagp, fft_size);
		vDSP_fft_zriptD (m_spec, &m_packed, 1, &m_buf, m_order, FFT_INVERSE);
		unpackReal (realOut);
	}

	void inverseCepstral (const double* magIn, double* cepOut) final
	{
		const auto hs1 = fft_size / 2 + 1;

		vecops::copy (m_spare, magIn, hs1);

		vDSP_vsaddD (m_spare, vDSP_Stride (1), &shiftAmount<double>, m_spare, vDSP_Stride (1), vDSP_Length (hs1));

		vvlog (m_spare2, m_spare, &hs1);
		inverse (m_spare2, nullptr, cepOut);
	}

	void packReal (const double* const re)
	{
		vDSP_ctozD (reinterpret_cast<const DSPDoubleComplex* const> (re),
					2, &m_packed, 1, fft_size / 2);
	}

	void unpackReal (double* const re)
	{
		vDSP_ztocD (&m_packed, 1, reinterpret_cast<DSPDoubleComplex* const> (re),
					2, fft_size / 2);
	}

	void unpackComplex (double* const re, double* const im)
	{
		// vDSP forward FFTs are scaled 2x (for some reason)
		constexpr auto two = 2.;
		vDSP_vsdivD (m_packed.realp, vDSP_Stride (1), &two, re, vDSP_Stride (1), vDSP_Length (fft_size / 2 + 1));
		vDSP_vsdivD (m_packed.imagp, vDSP_Stride (1), &two, im, vDSP_Stride (1), vDSP_Length (fft_size / 2 + 1));
	}

	FFTSetupD			  m_spec { vDSP_create_fftsetupD (m_order, FFT_RADIX2) };
	double*				  m_spare;
	double*				  m_spare2;
	DSPDoubleSplitComplex m_buf, m_packed;
};

#endif /* LIMES_VECOPS_USE_VDSP */

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark IPP FFT

#if LIMES_VECOPS_USE_IPP

template <Scalar SampleType>
inline void ipp_pack (const SampleType* re, const SampleType* im, int fft_size, SampleType* m_packed)
{
	const auto hs = fft_size / 2;

	for (auto i = 0, index = 0; i <= hs; ++i)
	{
		m_packed[index++] = re[i];
		index++;
	}

	if (im == nullptr)
	{
		for (auto i = 0, index = 0; i <= hs; ++i)
		{
			index++;
			m_packed[index++] = 0.;
		}
	}
	else
	{
		for (auto i = 0, index = 0; i <= hs; ++i)
		{
			index++;
			m_packed[index++] = im[i];
		}
	}
}

template <Scalar SampleType>
inline void ipp_unpack (SampleType* re, SampleType* im, int fft_size, SampleType* m_packed)
{
	const auto hs = fft_size / 2;

	if (im != nullptr)
	{
		for (auto i = 0, index = 0; i <= hs; ++i)
		{
			index++;
			im[i] = m_packed[index++];
		}
	}

	for (auto i = 0, index = 0; i <= hs; ++i)
	{
		re[i] = m_packed[index++];
		index++;
	}
}


class IPP_FloatFFT final : public FFT<float>::FFTImpl
{
public:

	explicit IPP_FloatFFT (int size)
		: FFT<float>::FFTImpl (size)
	{
		int specSize, specBufferSize, bufferSize;

		ippsFFTGetSize_R_32f (m_order, IPP_FFT_NODIV_BY_ANY, ippAlgHintFast,
							  &specSize, &specBufferSize, &bufferSize);

		m_specbuf  = ippsMalloc_8u (specSize);
		Ipp8u* tmp = ippsMalloc_8u (specBufferSize);
		m_buf	   = ippsMalloc_8u (bufferSize);
		m_packed   = ippsMalloc_32f (fft_size + 2);
		m_spare	   = ippsMalloc_32f (fft_size / 2 + 1);

		ippsFFTInit_R_32f (&m_spec,
						   m_order, IPP_FFT_NODIV_BY_ANY, ippAlgHintFast,
						   m_specbuf, tmp);

		ippsFree (tmp);
	}

	~IPP_FloatFFT() final
	{
		ippsFree (m_specbuf);
		ippsFree (m_buf);
		ippsFree (m_packed);
		ippsFree (m_spare);
	}

	IPP_FloatFFT (const IPP_FloatFFT&) = delete;

	IPP_FloatFFT& operator= (const IPP_FloatFFT&) = delete;

	IPP_FloatFFT (IPP_FloatFFT&&) = delete;

	IPP_FloatFFT& operator= (IPP_FloatFFT&&) = delete;

private:

	void forward (const float* realIn, float* realOut, float* imagOut) final
	{
		ippsFFTFwd_RToCCS_32f (realIn, m_packed, m_spec, m_buf);
		ipp_unpack (realOut, imagOut, fft_size, m_packed);
	}

	void forwardInterleaved (const float* realIn, float* complexOut) final
	{
		ippsFFTFwd_RToCCS_32f (realIn, complexOut, m_spec, m_buf);
	}

	void forwardPolar (const float* realIn, float* magOut, float* phaseOut) final
	{
		ippsFFTFwd_RToCCS_32f (realIn, m_packed, m_spec, m_buf);
		ipp_unpack (m_packed, m_spare, fft_size, m_packed);
		ippsCartToPolar_32f (m_packed, m_spare, magOut, phaseOut, fft_size / 2 + 1);
	}

	void forwardMagnitude (const float* realIn, float* magOut) final
	{
		ippsFFTFwd_RToCCS_32f (realIn, m_packed, m_spec, m_buf);
		ipp_unpack (m_packed, m_spare, fft_size, m_packed);
		ippsMagnitude_32f (m_packed, m_spare, magOut, fft_size / 2 + 1);
	}

	void inverse (const float* realIn, const float* imagIn, float* realOut) final
	{
		ipp_pack (realIn, imagIn, fft_size, m_packed);
		ippsFFTInv_CCSToR_32f (m_packed, realOut, m_spec, m_buf);
	}

	void inverseInterleaved (const float* complexIn, float* realOut) final
	{
		ippsFFTInv_CCSToR_32f (complexIn, realOut, m_spec, m_buf);
	}

	void inversePolar (const float* magIn, const float* phaseIn, float* realOut) final
	{
		ippsPolarToCart_32f (magIn, phaseIn, realOut, m_spare, fft_size / 2 + 1);
		ipp_pack (realOut, m_spare, fft_size, m_packed);
		ippsFFTInv_CCSToR_32f (m_packed, realOut, m_spec, m_buf);
	}

	void inverseCepstral (const float* magIn, float* cepOut) final
	{
		const auto hs1 = fft_size / 2 + 1;

		ippsCopy_32f (magIn, m_spare, hs1);
		ippsAddC_32f_I (shiftAmount<float>, m_spare, hs1);
		ippsLn_32f_I (m_spare, hs1);
		ipp_pack<float> (m_spare, nullptr, fft_size, m_packed);
		ippsFFTInv_CCSToR_32f (m_packed, cepOut, m_spec, m_buf);
	}

	IppsFFTSpec_R_32f* m_spec;
	Ipp8u*			   m_specbuf;
	Ipp8u*			   m_buf;
	float*			   m_packed;
	float*			   m_spare;
};


class IPP_DoubleFFT final : public FFT<double>::FFTImpl
{
public:

	explicit IPP_DoubleFFT (int size)
		: FFT<double>::FFTImpl (size)
	{
		int specSize, specBufferSize, bufferSize;

		ippsFFTGetSize_R_64f (m_order, IPP_FFT_NODIV_BY_ANY, ippAlgHintFast,
							  &specSize, &specBufferSize, &bufferSize);

		m_specbuf  = ippsMalloc_8u (specSize);
		Ipp8u* tmp = ippsMalloc_8u (specBufferSize);
		m_buf	   = ippsMalloc_8u (bufferSize);
		m_packed   = ippsMalloc_64f (fft_size + 2);
		m_spare	   = ippsMalloc_64f (fft_size / 2 + 1);

		ippsFFTInit_R_64f (&m_spec,
						   m_order, IPP_FFT_NODIV_BY_ANY, ippAlgHintFast,
						   m_specbuf, tmp);

		ippsFree (tmp);
	}

	~IPP_DoubleFFT() final
	{
		ippsFree (m_specbuf);
		ippsFree (m_buf);
		ippsFree (m_packed);
		ippsFree (m_spare);
	}

	IPP_DoubleFFT (const IPP_DoubleFFT&) = delete;

	IPP_DoubleFFT& operator= (const IPP_DoubleFFT&) = delete;

	IPP_DoubleFFT (IPP_DoubleFFT&&) = delete;

	IPP_DoubleFFT& operator= (IPP_DoubleFFT&&) = delete;

private:

	void forward (const double* realIn, double* realOut, double* imagOut) final
	{
		ippsFFTFwd_RToCCS_64f (realIn, m_packed, m_spec, m_buf);
		ipp_unpack (realOut, imagOut, fft_size, m_packed);
	}

	void forwardInterleaved (const double* realIn, double* complexOut) final
	{
		ippsFFTFwd_RToCCS_64f (realIn, complexOut, m_spec, m_buf);
	}

	void forwardPolar (const double* realIn, double* magOut, double* phaseOut) final
	{
		ippsFFTFwd_RToCCS_64f (realIn, m_packed, m_spec, m_buf);
		ipp_unpack (m_packed, m_spare, fft_size, m_packed);
		ippsCartToPolar_64f (m_packed, m_spare, magOut, phaseOut, fft_size / 2 + 1);
	}

	void forwardMagnitude (const double* realIn, double* magOut) final
	{
		ippsFFTFwd_RToCCS_64f (realIn, m_packed, m_spec, m_buf);
		ipp_unpack (m_packed, m_spare, fft_size, m_packed);
		ippsMagnitude_64f (m_packed, m_spare, magOut, fft_size / 2 + 1);
	}

	void inverse (const double* realIn, const double* imagIn, double* realOut) final
	{
		ipp_pack (realIn, imagIn, fft_size, m_packed);
		ippsFFTInv_CCSToR_64f (m_packed, realOut, m_spec, m_buf);
	}

	void inverseInterleaved (const double* complexIn, double* realOut) final
	{
		ippsFFTInv_CCSToR_64f (complexIn, realOut, m_spec, m_buf);
	}

	void inversePolar (const double* magIn, const double* phaseIn, double* realOut) final
	{
		ippsPolarToCart_64f (magIn, phaseIn, realOut, m_spare, fft_size / 2 + 1);
		ipp_pack (realOut, m_spare, fft_size, m_packed);
		ippsFFTInv_CCSToR_64f (m_packed, realOut, m_spec, m_buf);
	}

	void inverseCepstral (const double* magIn, double* cepOut) final
	{
		const auto hs1 = fft_size / 2 + 1;

		ippsCopy_64f (magIn, m_spare, hs1);
		ippsAddC_64f_I (shiftAmount<double>, m_spare, hs1);
		ippsLn_64f_I (m_spare, hs1);
		ipp_pack<double> (m_spare, nullptr, fft_size, m_packed);
		ippsFFTInv_CCSToR_64f (m_packed, cepOut, m_spec, m_buf);
	}

	IppsFFTSpec_R_64f* m_spec;
	Ipp8u*			   m_specbuf;
	Ipp8u*			   m_buf;
	double*			   m_packed;
	double*			   m_spare;
};

#endif /* LIMES_VECOPS_USE_IPP */

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark FFTW FFT

#if LIMES_VECOPS_USE_FFTW

#	if FFTW_DOUBLE_ONLY
#		define fft_float_type		  double
#		define fftwf_complex		  fftw_complex
#		define fftwf_plan			  fftw_plan
#		define fftwf_plan_dft_r2c_1d fftw_plan_dft_r2c_1d
#		define fftwf_plan_dft_c2r_1d fftw_plan_dft_c2r_1d
#		define fftwf_destroy_plan	  fftw_destroy_plan
#		define fftwf_malloc		  fftw_malloc
#		define fftwf_free			  fftw_free
#		define fftwf_execute		  fftw_execute
#		define fftwf_cleanup		  fftw_cleanup
#		define atan2f				  atan2
#		define sqrtf				  sqrt
#		define cosf				  cos
#		define sinf				  sin
#	else
#		define fft_float_type float
#	endif /* FFTW_DOUBLE_ONLY */

#	if FFTW_SINGLE_ONLY
#		define fft_double_type		 float
#		define fftw_complex		 fftwf_complex
#		define fftw_plan			 fftwf_plan
#		define fftw_plan_dft_r2c_1d fftwf_plan_dft_r2c_1d
#		define fftw_plan_dft_c2r_1d fftwf_plan_dft_c2r_1d
#		define fftw_destroy_plan	 fftwf_destroy_plan
#		define fftw_malloc			 fftwf_malloc
#		define fftw_free			 fftwf_free
#		define fftw_execute		 fftwf_execute
#		define fftw_cleanup		 fftwf_cleanup
#		define atan2				 atan2f
#		define sqrt				 sqrtf
#		define cos					 cosf
#		define sin					 sinf
#	else
#		define fft_double_type double
#	endif /* FFTW_SINGLE_ONLY */


template <Scalar SampleType, typename ComplexType>
inline void fftw_pack (const SampleType* re, const SampleType* im,
					   ComplexType* m_packed, int fft_size)
{
	const auto hs = fft_size / 2;

	for (auto i = 0; i <= hs; ++i)
		m_packed[i][0] = re[i];

	if (im == nullptr)
		for (auto i = 0; i <= hs; ++i)
			m_packed[i][1] = SampleType (0);
	else
		for (auto i = 0; i <= hs; ++i)
			m_packed[i][1] = im[i];
}

template <Scalar SampleType, typename ComplexType>
inline void fftw_unpack (SampleType* re, SampleType* im,
						 ComplexType* m_packed, int fft_size)
{
	const auto hs = fft_size / 2;

	for (auto i = 0; i <= hs; ++i)
		re[i] = m_packed[i][0];

	if (im != nullptr)
		for (auto i = 0; i <= hs; ++i)
			im[i] = m_packed[i][1];
}


// this is the public interface for wisdom
namespace fftw
{
static std::string widom_file_dir;	// NOLINT
static std::mutex  wisdom_lock;
static bool		   useWisdom { true };

void setWisdomFileDir (std::string_view dirAbsPath)
{
	const std::lock_guard g { wisdom_lock };

	widom_file_dir = dirAbsPath;
}

std::string getWisdomFileDir()
{
	const std::lock_guard g { wisdom_lock };

	if (widom_file_dir.empty())
	{
		const auto* homeDir = std::getenv ("HOME");

		if (homeDir != nullptr)
			widom_file_dir = homeDir;
	}

	return widom_file_dir;
}

void enableWisdom (bool shouldUseWisdom)
{
	useWisdom = shouldUseWisdom;
}

bool isUsingWisdom()
{
	if (getWisdomFileDir().empty())
		return false;

	return useWisdom;
}
}  // namespace fftw

[[nodiscard]] inline FILE* fftw_get_wisdom_file (bool isDouble, bool save)
{
#	if FFTW_SINGLE_ONLY
	if (isDouble)
		return nullptr;
#	elif FFTW_DOUBLE_ONLY
	if (! isDouble)
		return nullptr;
#	endif

	if (! fftw::useWisdom)
		return nullptr;

	const auto fileDir = fftw::getWisdomFileDir();

	if (fileDir.empty())
		return nullptr;

	const auto typeChar = isDouble ? 'd' : 'f';

	char fn[256];
	std::snprintf (fn, sizeof (fn), "%s/%s.%c", fileDir.c_str(), ".fftw_wisdom", typeChar);

	return std::fopen (fn, save ? "wb" : "rb");
}

inline void fftw_load_wisdom (bool isDouble)
{
	if (auto* wisdomFile = fftw_get_wisdom_file (isDouble, false))
	{
		if (isDouble)
			fftw_import_wisdom_from_file (wisdomFile);
		else
			fftwf_import_wisdom_from_file (wisdomFile);

		std::fclose (wisdomFile);
	}
}

inline void fftw_save_wisdom (bool isDouble)
{
	if (auto* wisdomFile = fftw_get_wisdom_file (isDouble, true))
	{
		if (isDouble)
			fftw_export_wisdom_to_file (wisdomFile);
		else
			fftwf_export_wisdom_to_file (wisdomFile);

		std::fclose (wisdomFile);
	}
}


class FFTW_FloatFFT final : public FFT<float>::FFTImpl
{
public:

	explicit FFTW_FloatFFT (int size)
		: FFT<float>::FFTImpl (size), m_fbuf (reinterpret_cast<fft_float_type*> (fftw_malloc (fft_size * sizeof (fft_float_type)))),
		  m_fpacked (reinterpret_cast<fftwf_complex*> (fftw_malloc ((fft_size / 2 + 1) * sizeof (fftwf_complex)))),
		  m_fplanf (fftwf_plan_dft_r2c_1d (fft_size, m_fbuf, m_fpacked, FFTW_ESTIMATE)),
		  m_fplani (fftwf_plan_dft_c2r_1d (fft_size, m_fpacked, m_fbuf, FFTW_ESTIMATE))
	{
		if (m_extantf == 0)
			fftw_load_wisdom (false);

		++m_extantf;
	}

	~FFTW_FloatFFT() final
	{
		if (m_fplanf)
		{
			fftwf_destroy_plan (m_fplanf);
			fftwf_destroy_plan (m_fplani);
			fftwf_free (m_fbuf);
			fftwf_free (m_fpacked);
		}

		--m_extantf;

		if (m_extantf <= 0)
		{
			fftw_save_wisdom (false);
			fftwf_cleanup();
		}
	}

	FFTW_FloatFFT (const FFTW_FloatFFT&) = delete;

	FFTW_FloatFFT& operator= (const FFTW_FloatFFT&) = delete;

	FFTW_FloatFFT (FFTW_FloatFFT&&) = delete;

	FFTW_FloatFFT& operator= (FFTW_FloatFFT&&) = delete;

private:

	void forward (const float* realIn, float* realOut, float* imagOut) final
	{
#	if ! FFTW_DOUBLE_ONLY
		if (realIn != m_fbuf)
#	endif
			vecops::copy (m_fbuf, realIn, fft_size);

		fftwf_execute (m_fplanf);

		fftw_unpack (realOut, imagOut, m_fpacked, fft_size);
	}

	void forwardInterleaved (const float* realIn, float* complexOut) final
	{
#	if ! FFTW_DOUBLE_ONLY
		if (realIn != m_fbuf)
#	endif
			vecops::copy (m_fbuf, realIn, fft_size);

		fftwf_execute (m_fplanf);
		// v_convert(complexOut, (const fft_float_type *)m_fpacked, fft_size + 2);
	}

	void forwardPolar (const float* realIn, float* magOut, float* phaseOut) final
	{
#	if ! FFTW_DOUBLE_ONLY
		if (realIn != m_fbuf)
#	endif
			vecops::copy (m_fbuf, realIn, fft_size);

		fftwf_execute (m_fplanf);

		// v_cartesian_interleaved_to_polar
		//     (magOut, phaseOut, (const fft_float_type *)m_fpacked, fft_size/2+1);
	}

	void forwardMagnitude (const float* realIn, float* magOut) final
	{
#	if ! FFTW_DOUBLE_ONLY
		if (realIn != m_fbuf)
#	endif
			vecops::copy (m_fbuf, realIn, fft_size);

		fftwf_execute (m_fplanf);

		// v_cartesian_interleaved_to_magnitudes
		//     (magOut, (const fft_float_type *)m_fpacked, fft_size/2+1);
	}

	void inverse (const float* realIn, const float* imagIn, float* realOut) final
	{
		fftw_pack (realIn, imagIn, m_fpacked, fft_size);

		fftwf_execute (m_fplani);

#	if ! FFTW_DOUBLE_ONLY
		if (realOut != m_fbuf)
#	endif
			vecops::copy (realOut, m_fbuf, fft_size);
	}

	void inverseInterleaved (const float* complexIn, float* realOut) final
	{
		// v_convert ((fft_float_type*) m_fpacked, complexIn, fft_size + 2);
		fftwf_execute (m_fplani);

#	if ! FFTW_DOUBLE_ONLY
		if (realOut != m_fbuf)
#	endif
			vecops::copy (realOut, m_fbuf, fft_size);
	}

	void inversePolar (const float* magIn, const float* phaseIn, float* realOut) final
	{
		// v_polar_to_cartesian_interleaved
		//           ((fft_float_type *)m_fpacked, magIn, phaseIn, fft_size/2+1);

		fftwf_execute (m_fplani);

#	if ! FFTW_DOUBLE_ONLY
		if (realOut != m_fbuf)
#	endif
			vecops::copy (realOut, m_fbuf, fft_size);
	}

	void inverseCepstral (const float* magIn, float* cepOut) final
	{
		for (auto i = 0; i <= fft_size / 2; ++i)
		{
			m_fpacked[i][0] = std::logf (magIn[i] + shiftAmount<fft_float_type>);
			m_fpacked[i][1] = 0.f;
		}

		fftwf_execute (m_fplani);

#	if ! FFTW_DOUBLE_ONLY
		if (cepOut != m_fbuf)
#	endif
			vecops::copy (cepOut, m_fbuf, fft_size);
	}

	fftwf_plan m_fplanf;
	fftwf_plan m_fplani;

	fft_float_type* m_fbuf;

	fftwf_complex* m_fpacked;

	static int m_extantf;
};

int FFTW_FloatFFT::m_extantf = 0;


class FFTW_DoubleFFT final : public FFT<double>::FFTImpl
{
public:

	explicit FFTW_DoubleFFT (int size)
		: FFT<double>::FFTImpl (size),
		  m_dbuf (reinterpret_cast<fft_double_type*> (fftw_malloc (fft_size * sizeof (fft_double_type)))),
		  m_dpacked (reinterpret_cast<fftw_complex*> (fftw_malloc ((fft_size / 2 + 1) * sizeof (fftw_complex)))),
		  m_dplanf (fftw_plan_dft_r2c_1d (fft_size, m_dbuf, m_dpacked, FFTW_ESTIMATE)),
		  m_dplani (fftw_plan_dft_c2r_1d (fft_size, m_dpacked, m_dbuf, FFTW_ESTIMATE))
	{
		if (m_extantd == 0)
			fftw_load_wisdom (true);

		++m_extantd;
	}

	~FFTW_DoubleFFT() final
	{
		fftw_destroy_plan (m_dplanf);
		fftw_destroy_plan (m_dplani);
		fftw_free (m_dbuf);
		fftw_free (m_dpacked);

		--m_extantd;

		if (m_extantd <= 0)
		{
			fftw_save_wisdom (true);
			fftw_cleanup();
		}
	}

	FFTW_DoubleFFT (const FFTW_DoubleFFT&) = delete;

	FFTW_DoubleFFT& operator= (const FFTW_DoubleFFT&) = delete;

	FFTW_DoubleFFT (FFTW_DoubleFFT&&) = delete;

	FFTW_DoubleFFT& operator= (FFTW_DoubleFFT&&) = delete;

private:

	void forward (const double* realIn, double* realOut, double* imagOut) final
	{
#	if ! FFTW_SINGLE_ONLY
		if (realIn != m_dbuf)
#	endif
			vecops::copy (m_dbuf, realIn, fft_size);

		fftw_execute (m_dplanf);

		fftw_unpack (realOut, imagOut, m_dpacked, fft_size);
	}

	void forwardInterleaved (const double* realIn, double* complexOut) final
	{
#	if ! FFTW_SINGLE_ONLY
		if (realIn != m_dbuf)
#	endif
			vecops::copy (m_dbuf, realIn, fft_size);

		fftw_execute (m_dplanf);
		// v_convert(complexOut, (const fft_double_type *)m_dpacked, fft_size + 2);
	}

	void forwardPolar (const double* realIn, double* magOut, double* phaseOut) final
	{
#	if ! FFTW_SINGLE_ONLY
		if (realIn != m_dbuf)
#	endif
			vecops::copy (m_dbuf, realIn, fft_size);

		fftw_execute (m_dplanf);

		// v_cartesian_interleaved_to_polar
		//     (magOut, phaseOut, (const fft_double_type *)m_dpacked, fft_size/2+1);
	}

	void forwardMagnitude (const double* realIn, double* magOut) final
	{
#	if ! FFTW_SINGLE_ONLY
		if (realIn != m_dbuf)
#	endif
			vecops::copy (m_dbuf, realIn, fft_size);

		fftw_execute (m_dplanf);

		// v_cartesian_interleaved_to_magnitudes
		//     (magOut, (const fft_double_type *)m_dpacked, fft_size/2+1);
	}

	void inverse (const double* realIn, const double* imagIn, double* realOut) final
	{
		fftw_pack (realIn, imagIn, m_dpacked, fft_size);

		fftw_execute (m_dplani);

#	if ! FFTW_SINGLE_ONLY
		if (realOut != m_dbuf)
#	endif
			vecops::copy (realOut, m_dbuf, fft_size);
	}

	void inverseInterleaved (const double* complexIn, double* realOut) final
	{
		// v_convert ((fft_double_type*) m_dpacked, complexIn, fft_size + 2);
		fftw_execute (m_dplani);

#	if ! FFTW_SINGLE_ONLY
		if (realOut != m_dbuf)
#	endif
			vecops::copy (realOut, m_dbuf, fft_size);
	}

	void inversePolar (const double* magIn, const double* phaseIn, double* realOut) final
	{
		// v_polar_to_cartesian_interleaved
		//           ((fft_double_type *)m_dpacked, magIn, phaseIn, fft_size/2+1);

		fftw_execute (m_dplani);

#	if ! FFTW_SINGLE_ONLY
		if (realOut != m_dbuf)
#	endif
			vecops::copy (realOut, m_dbuf, fft_size);
	}

	void inverseCepstral (const double* magIn, double* cepOut) final
	{
		for (auto i = 0; i <= fft_size / 2; ++i)
		{
			m_dpacked[i][0] = std::log (magIn[i] + shiftAmount<fft_double_type>);
			m_dpacked[i][1] = 0.;
		}

		fftw_execute (m_dplani);

#	if ! FFTW_SINGLE_ONLY
		if (cepOut != m_dbuf)
#	endif
			vecops::copy (cepOut, m_dbuf, fft_size);
	}

	fftw_plan m_dplanf;
	fftw_plan m_dplani;

	fft_double_type* m_dbuf;

	fftw_complex* m_dpacked;

	static int m_extantd;
};

int FFTW_DoubleFFT::m_extantd = 0;


#	undef fft_float_type
#	undef fft_double_type

#	if FFTW_DOUBLE_ONLY
#		undef fftwf_complex
#		undef fftwf_plan
#		undef fftwf_plan_dft_r2c_1d
#		undef fftwf_plan_dft_c2r_1d
#		undef fftwf_destroy_plan
#		undef fftwf_malloc
#		undef fftwf_free
#		undef fftwf_execute
#		undef atan2f
#		undef sqrtf
#		undef cosf
#		undef sinf
#	endif /* FFTW_DOUBLE_ONLY */

#	if FFTW_SINGLE_ONLY
#		undef fftw_complex
#		undef fftw_plan
#		undef fftw_plan_dft_r2c_1d
#		undef fftw_plan_dft_c2r_1d
#		undef fftw_destroy_plan
#		undef fftw_malloc
#		undef fftw_free
#		undef fftw_execute
#		undef atan2
#		undef sqrt
#		undef cos
#		undef sin
#	endif /* FFTW_SINGLE_ONLY */


#endif /* LIMES_VECOPS_USE_FFTW */

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark Fallback FFT

template <Scalar SampleType>
class FallbackFFT final : public FFT<SampleType>::FFTImpl
{
public:

	explicit FallbackFFT (int size)
		: FFT<SampleType>::FFTImpl (size)
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

	~FallbackFFT() final
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

	FallbackFFT (const FallbackFFT&) = delete;

	FallbackFFT& operator= (const FallbackFFT&) = delete;

	FallbackFFT (FallbackFFT&&) = delete;

	FallbackFFT& operator= (FallbackFFT&&) = delete;

private:

	void forward (const SampleType* realIn, SampleType* realOut, SampleType* imagOut) final
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

	void forwardInterleaved (const SampleType* realIn, SampleType* complexOut) final
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

	void forwardPolar (const SampleType* realIn, SampleType* magOut, SampleType* phaseOut) final
	{
		transformF (realIn, m_c, m_d);
		// v_cartesian_to_polar (magOut, phaseOut, m_c, m_d, m_half + 1);
	}

	void forwardMagnitude (const SampleType* realIn, SampleType* magOut) final
	{
		transformF (realIn, m_c, m_d);
		// v_cartesian_to_magnitudes (magOut, m_c, m_d, m_half + 1);
	}

	void inverse (const SampleType* realIn, const SampleType* imagIn, SampleType* realOut) final
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

	void inverseInterleaved (const SampleType* complexIn, SampleType* realOut) final
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

	void inversePolar (const SampleType* magIn, const SampleType* phaseIn, SampleType* realOut) final
	{
		// v_polar_to_cartesian (m_a, m_b, magIn, phaseIn, m_half + 1);
		transformI (m_a, m_b, realOut);
	}

	void inverseCepstral (const SampleType* magIn, SampleType* cepOut) final
	{
		for (auto i = 0; i <= m_half; ++i)
		{
			m_a[i] = std::log (magIn[i] + shiftAmount<double>);
			m_b[i] = 0.;
		}

		transformI (m_a, m_b, cepOut);
	}

	// Uses m_a and m_b internally; does not touch m_c or m_d
	void transformF (const SampleType* ri,
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
	void transformI (const double* ri, const double* ii,
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

	void transformComplex (const double* ri, const double* ii,
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
					constexpr double pi = 3.14157;

					const auto phase = 2. * pi / double (blockSize);

					vals.sm1 = ifactor * std::sin (phase);
					vals.sm2 = ifactor * std::sin (2. * phase);
					vals.cm1 = std::cos (phase);
					vals.cm2 = std::cos (2. * phase);
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

	void makeTables()
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

		constexpr auto pi = 3.14157;

		// sin and cos tables for complex fft
		for (auto i = 2, ix = 0; i <= m_maxTabledBlock; i <<= 1)
		{
			const auto phase = 2. * pi / double (i);
			m_sincos[ix++]	 = std::sin (phase);
			m_sincos[ix++]	 = std::sin (2. * phase);
			m_sincos[ix++]	 = std::cos (phase);
			m_sincos[ix++]	 = std::cos (2. * phase);
		}

		// sin and cos tables for real-complex transform
		for (auto i = 0, ix = 0; i < m_half / 2; ++i)
		{
			const auto phase = pi * (double (i + 1) / double (m_half) + 0.5);
			m_sincos_r[ix++] = std::sin (phase);
			m_sincos_r[ix++] = std::cos (phase);
		}
	}

	static constexpr auto m_blockTableSize = 16;
	static constexpr auto m_maxTabledBlock = 1 << m_blockTableSize;

	const int m_half { this->fft_size / 2 };

	int*	m_table;
	double* m_sincos;
	double* m_sincos_r;

	double* m_vr;
	double* m_vi;
	double* m_a;
	double* m_b;
	double* m_c;
	double* m_d;
	double* m_a_and_b[2] { m_a, m_b };
	double* m_c_and_d[2] { m_c, m_d };
};

/*---------------------------------------------------------------------------------------------------------------------------*/

template <Scalar SampleType>
FFT<SampleType>::FFT (int size)
{
#if LIMES_VECOPS_USE_FFTW
	if constexpr (std::is_same_v<SampleType, float>)
		pimpl = std::make_unique<FFTW_FloatFFT> (size);
	else
		pimpl = std::make_unique<FFTW_DoubleFFT> (size);
#elif LIMES_VECOPS_USE_VDSP
	if constexpr (std::is_same_v<SampleType, float>)
		pimpl = std::make_unique<vDSP_FloatFFT> (size);
	else
		pimpl = std::make_unique<vDSP_DoubleFFT> (size);
#elif LIMES_VECOPS_USE_IPP
	if constexpr (std::is_same_v<SampleType, float>)
		pimpl = std::make_unique<IPP_FloatFFT> (size);
	else
		pimpl = std::make_unique<IPP_DoubleFFT> (size);
#else
	pimpl = std::make_unique<FallbackFFT<SampleType>> (size);
#endif
}

template <Scalar SampleType>
int FFT<SampleType>::getSize() const noexcept
{
	return pimpl->getSize();
}

template <Scalar SampleType>
void FFT<SampleType>::forward (const SampleType* realIn, SampleType* realOut, SampleType* imagOut)
{
	pimpl->forward (realIn, realOut, imagOut);
}

template <Scalar SampleType>
void FFT<SampleType>::forwardInterleaved (const SampleType* realIn, SampleType* complexOut)
{
	pimpl->forwardInterleaved (realIn, complexOut);
}

template <Scalar SampleType>
void FFT<SampleType>::forwardPolar (const SampleType* realIn, SampleType* magOut, SampleType* phaseOut)
{
	pimpl->forwardPolar (realIn, magOut, phaseOut);
}

template <Scalar SampleType>
void FFT<SampleType>::forwardMagnitude (const SampleType* realIn, SampleType* magOut)
{
	pimpl->forwardMagnitude (realIn, magOut);
}

template <Scalar SampleType>
void FFT<SampleType>::inverse (const SampleType* realIn, const SampleType* imagIn, SampleType* realOut)
{
	pimpl->inverse (realIn, imagIn, realOut);
}

template <Scalar SampleType>
void FFT<SampleType>::inverseInterleaved (const SampleType* complexIn, SampleType* realOut)
{
	pimpl->inverseInterleaved (complexIn, realOut);
}

template <Scalar SampleType>
void FFT<SampleType>::inversePolar (const SampleType* magIn, const SampleType* phaseIn, SampleType* realOut)
{
	pimpl->inversePolar (magIn, phaseIn, realOut);
}

template <Scalar SampleType>
void FFT<SampleType>::inverseCepstral (const SampleType* magIn, SampleType* cepOut)
{
	pimpl->inverseCepstral (magIn, cepOut);
}

template class FFT<float>;
template class FFT<double>;

}  // namespace limes::vecops
