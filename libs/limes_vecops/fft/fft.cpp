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

#include <cmath>
#include <iostream>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include "fft.h"

namespace limes::vecops
{

static inline int orderFromFFTSize (int size)
{
	for (int i = 0;; ++i)
		if (size & (1 << i))
			return i;

	// jassertfalse;
	return 0;
}

template <Scalar SampleType>
class FFT<SampleType>::FFTImpl
{
public:

	FFTImpl (int size)
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

	const int fft_size;
	const int m_order;
};

/*---------------------------------------------------------------------------------------------------------------------------*/

#if LIMES_VECOPS_USE_VDSP

class vDSP_FloatFFT final : public FFT<float>::FFTImpl
{
public:

	vDSP_FloatFFT (int size)
		: FFT<float>::FFTImpl (size)
	{
		m_spec = vDSP_create_fftsetup (m_order, FFT_RADIX2);
		//!!! "If possible, tempBuffer->realp and tempBuffer->imagp should be 32-byte aligned for best performance."
		// m_buf.realp	   = allocate<float> (fft_size);
		// m_buf.imagp	   = allocate<float> (fft_size);
		// m_packed.realp = allocate<float> (fft_size / 2 + 1);
		// m_packed.imagp = allocate<float> (fft_size / 2 + 1);
		// m_spare		   = allocate<float> (fft_size + 2);
		// m_spare2	   = allocate<float> (fft_size + 2);
	}

	~vDSP_FloatFFT()
	{
		vDSP_destroy_fftsetup (m_spec);
		// deallocate (m_spare);
		// deallocate (m_spare2);
		// deallocate (m_buf.realp);
		// deallocate (m_buf.imagp);
		// deallocate (m_packed.realp);
		// deallocate (m_packed.imagp);
	}

private:

	void forward (const float* realIn, float* realOut, float* imagOut) final
	{
		packReal (realIn);
		vDSP_fft_zript (m_spec, &m_packed, 1, &m_buf, m_order, FFT_FORWARD);
		fdenyq();
		unpackComplex (realOut, imagOut);
	}

	void forwardInterleaved (const float* realIn, float* complexOut) final
	{
		packReal (realIn);
		vDSP_fft_zript (m_spec, &m_packed, 1, &m_buf, m_order, FFT_FORWARD);
		fdenyq();
		unpackComplex (complexOut);
	}

	void forwardPolar (const float* realIn, float* magOut, float* phaseOut) final
	{
		const int hs1 = fft_size / 2 + 1;
		packReal (realIn);
		vDSP_fft_zript (m_spec, &m_packed, 1, &m_buf, m_order, FFT_FORWARD);
		fdenyq();
		// vDSP forward FFTs are scaled 2x (for some reason)
		for (int i = 0; i < hs1; ++i) m_packed.realp[i] *= 0.5f;
		for (int i = 0; i < hs1; ++i) m_packed.imagp[i] *= 0.5f;
		// v_cartesian_to_polar(magOut, phaseOut,
		//                      m_packed.realp, m_packed.imagp, hs1);
	}

	void forwardMagnitude (const float* realIn, float* magOut) final
	{
		packReal (realIn);
		vDSP_fft_zript (m_spec, &m_packed, 1, &m_buf, m_order, FFT_FORWARD);
		fdenyq();
		const int hs1 = fft_size / 2 + 1;
		vDSP_zvmags (&m_packed, 1, m_spare, 1, hs1);
		vvsqrtf (m_spare2, m_spare, &hs1);
		// vDSP forward FFTs are scaled 2x (for some reason)
		constexpr float two = 2.f;
		vDSP_vsdiv (m_spare2, 1, &two, magOut, 1, hs1);
	}

	void inverse (const float* realIn, const float* imagIn, float* realOut) final
	{
		packComplex (realIn, imagIn);
		vDSP_fft_zript (m_spec, &m_packed, 1, &m_buf, m_order, FFT_INVERSE);
		unpackReal (realOut);
	}

	void inverseInterleaved (const float* complexIn, float* realOut) final
	{
		float* f[2] = { m_packed.realp, m_packed.imagp };
		// v_deinterleave (f, complexIn, 2, fft_size / 2 + 1);
		fnyq();
		vDSP_fft_zript (m_spec, &m_packed, 1, &m_buf, m_order, FFT_INVERSE);
		unpackReal (realOut);
	}

	void inversePolar (const float* magIn, const float* phaseIn, float* realOut) final
	{
		const int hs1 = fft_size / 2 + 1;
		vvsincosf (m_packed.imagp, m_packed.realp, phaseIn, &hs1);
		float* const rp = m_packed.realp;
		float* const ip = m_packed.imagp;
		for (int i = 0; i < hs1; ++i) rp[i] *= magIn[i];
		for (int i = 0; i < hs1; ++i) ip[i] *= magIn[i];
		fnyq();
		vDSP_fft_zript (m_spec, &m_packed, 1, &m_buf, m_order, FFT_INVERSE);
		unpackReal (realOut);
	}

	void inverseCepstral (const float* magIn, float* cepOut) final
	{
		const int hs1 = fft_size / 2 + 1;
		// v_copy (m_spare, magIn, hs1);
		for (int i = 0; i < hs1; ++i) m_spare[i] += 0.000001f;
		vvlogf (m_spare2, m_spare, &hs1);
		inverse (m_spare2, 0, cepOut);
	}

	void packReal (const float* const re)
	{
		vDSP_ctoz (reinterpret_cast<const DSPComplex* const> (re), 2, &m_packed, 1, fft_size / 2);
	}

	void packComplex (const float* const re, const float* const im)
	{
		// if (re)
		// 	v_copy (m_packed.realp, re, fft_size / 2 + 1);
		// else
		// 	v_zero (m_packed.realp, fft_size / 2 + 1);

		// if (im)
		// 	v_copy (m_packed.imagp, im, fft_size / 2 + 1);
		// else
		// 	v_zero (m_packed.imagp, fft_size / 2 + 1);

		fnyq();
	}

	void unpackReal (float* const re)
	{
		vDSP_ztoc (&m_packed, 1, reinterpret_cast<DSPComplex* const> (re), 2, fft_size / 2);
	}

	void unpackComplex (float* const re, float* const im)
	{
		// vDSP forward FFTs are scaled 2x (for some reason)
		constexpr float two = 2.f;
		vDSP_vsdiv (m_packed.realp, 1, &two, re, 1, fft_size / 2 + 1);
		vDSP_vsdiv (m_packed.imagp, 1, &two, im, 1, fft_size / 2 + 1);
	}

	void unpackComplex (float* const cplx)
	{
		// vDSP forward FFTs are scaled 2x (for some reason)
		const int hs1 = fft_size / 2 + 1;
		for (int i = 0; i < hs1; ++i)
		{
			cplx[i * 2]		= m_packed.realp[i] * 0.5f;
			cplx[i * 2 + 1] = m_packed.imagp[i] * 0.5f;
		}
	}

	void fdenyq()
	{
		// for fft result in packed form, unpack the DC and Nyquist bins
		const int hs	   = fft_size / 2;
		m_packed.realp[hs] = m_packed.imagp[0];
		m_packed.imagp[hs] = 0.f;
		m_packed.imagp[0]  = 0.f;
	}

	void fnyq()
	{
		// for ifft input in packed form, pack the DC and Nyquist bins
		const int hs	   = fft_size / 2;
		m_packed.imagp[0]  = m_packed.realp[hs];
		m_packed.realp[hs] = 0.f;
		m_packed.imagp[hs] = 0.f;
	}

	FFTSetup		m_spec;
	float*			m_spare;
	float*			m_spare2;
	DSPSplitComplex m_buf, m_packed;
};


class vDSP_DoubleFFT final : public FFT<double>::FFTImpl
{
public:

	vDSP_DoubleFFT (int size)
		: FFT<double>::FFTImpl (size)
	{
		m_spec = vDSP_create_fftsetupD (m_order, FFT_RADIX2);
		//!!! "If possible, tempBuffer->realp and tempBuffer->imagp should be 32-byte aligned for best performance."
		// m_buf.realp	   = allocate<double> (fft_size);
		// m_buf.imagp	   = allocate<double> (fft_size);
		// m_packed.realp = allocate<double> (fft_size / 2 + 1);
		// m_packed.imagp = allocate<double> (fft_size / 2 + 1);
		// m_spare		   = allocate<double> (fft_size + 2);
		// m_spare2	   = allocate<double> (fft_size + 2);
	}

	~vDSP_DoubleFFT()
	{
		vDSP_destroy_fftsetupD (m_spec);
		// deallocate (m_spare);
		// deallocate (m_spare2);
		// deallocate (m_buf.realp);
		// deallocate (m_buf.imagp);
		// deallocate (m_packed.realp);
		// deallocate (m_packed.imagp);
	}

private:

	void forward (const double* realIn, double* realOut, double* imagOut) final
	{
		packReal (realIn);
		vDSP_fft_zriptD (m_spec, &m_packed, vDSP_Stride (1), &m_buf, vDSP_Length (m_order), FFT_FORWARD);
		ddenyq();
		unpackComplex (realOut, imagOut);
	}

	void forwardInterleaved (const double* realIn, double* complexOut) final
	{
		packReal (realIn);
		vDSP_fft_zriptD (m_spec, &m_packed, 1, &m_buf, m_order, FFT_FORWARD);
		ddenyq();
		unpackComplex (complexOut);
	}

	void forwardPolar (const double* realIn, double* magOut, double* phaseOut) final
	{
		const int hs1 = fft_size / 2 + 1;
		packReal (realIn);
		vDSP_fft_zriptD (m_spec, &m_packed, 1, &m_buf, m_order, FFT_FORWARD);
		ddenyq();
		// vDSP forward FFTs are scaled 2x (for some reason)
		for (int i = 0; i < hs1; ++i) m_packed.realp[i] *= 0.5;
		for (int i = 0; i < hs1; ++i) m_packed.imagp[i] *= 0.5;
		// v_cartesian_to_polar(magOut, phaseOut,
		//                      m_packed.realp, m_packed.imagp, hs1);
	}

	void forwardMagnitude (const double* realIn, double* magOut) final
	{
		packReal (realIn);
		vDSP_fft_zriptD (m_spec, &m_packed, 1, &m_buf, m_order, FFT_FORWARD);
		ddenyq();
		const int hs1 = fft_size / 2 + 1;
		vDSP_zvmagsD (&m_packed, 1, m_spare, 1, hs1);
		vvsqrt (m_spare2, m_spare, &hs1);
		// vDSP forward FFTs are scaled 2x (for some reason)
		constexpr double two = 2.;
		vDSP_vsdivD (m_spare2, 1, &two, magOut, 1, hs1);
	}

	void inverse (const double* realIn, const double* imagIn, double* realOut) final
	{
		packComplex (realIn, imagIn);
		vDSP_fft_zriptD (m_spec, &m_packed, 1, &m_buf, m_order, FFT_INVERSE);
		unpackReal (realOut);
	}

	void inverseInterleaved (const double* complexIn, double* realOut) final
	{
		double* f[2] = { m_packed.realp, m_packed.imagp };
		// v_deinterleave (f, complexIn, 2, fft_size / 2 + 1);
		dnyq();
		vDSP_fft_zriptD (m_spec, &m_packed, 1, &m_buf, m_order, FFT_INVERSE);
		unpackReal (realOut);
	}

	void inversePolar (const double* magIn, const double* phaseIn, double* realOut) final
	{
		const int hs1 = fft_size / 2 + 1;
		vvsincos (m_packed.imagp, m_packed.realp, phaseIn, &hs1);
		double* const rp = m_packed.realp;
		double* const ip = m_packed.imagp;
		for (int i = 0; i < hs1; ++i) rp[i] *= magIn[i];
		for (int i = 0; i < hs1; ++i) ip[i] *= magIn[i];
		dnyq();
		vDSP_fft_zriptD (m_spec, &m_packed, 1, &m_buf, m_order, FFT_INVERSE);
		unpackReal (realOut);
	}

	void inverseCepstral (const double* magIn, double* cepOut) final
	{
		const int hs1 = fft_size / 2 + 1;
		// v_copy (m_spare, magIn, hs1);
		for (int i = 0; i < hs1; ++i) m_spare[i] += 0.000001;
		vvlog (m_spare2, m_spare, &hs1);
		inverse (m_spare2, 0, cepOut);
	}

	void packReal (const double* const re)
	{
		vDSP_ctozD (reinterpret_cast<const DSPDoubleComplex* const> (re), 2, &m_packed, 1, fft_size / 2);
	}

	void packComplex (const double* const re, const double* const im)
	{
		// if (re)
		// 	v_copy (m_packed.realp, re, fft_size / 2 + 1);
		// else
		// 	v_zero (m_packed.realp, fft_size / 2 + 1);

		// if (im)
		// 	v_copy (m_packed.imagp, im, fft_size / 2 + 1);
		// else
		// 	v_zero (m_packed.imagp, fft_size / 2 + 1);

		dnyq();
	}

	void unpackReal (double* const re)
	{
		vDSP_ztocD (&m_packed, 1, reinterpret_cast<DSPDoubleComplex* const> (re), 2, fft_size / 2);
	}

	void unpackComplex (double* const re, double* const im)
	{
		// vDSP forward FFTs are scaled 2x (for some reason)
		constexpr double two = 2.;
		vDSP_vsdivD (m_packed.realp, 1, &two, re, 1, fft_size / 2 + 1);
		vDSP_vsdivD (m_packed.imagp, 1, &two, im, 1, fft_size / 2 + 1);
	}

	void unpackComplex (double* const cplx)
	{
		// vDSP forward FFTs are scaled 2x (for some reason)
		const int hs1 = fft_size / 2 + 1;
		for (int i = 0; i < hs1; ++i)
		{
			cplx[i * 2]		= m_packed.realp[i] * 0.5f;
			cplx[i * 2 + 1] = m_packed.imagp[i] * 0.5f;
		}
	}

	void ddenyq()
	{
		// for fft result in packed form, unpack the DC and Nyquist bins
		const int hs	   = fft_size / 2;
		m_packed.realp[hs] = m_packed.imagp[0];
		m_packed.imagp[hs] = 0.;
		m_packed.imagp[0]  = 0.;
	}

	void dnyq()
	{
		// for ifft input in packed form, pack the DC and Nyquist bins
		const int hs	   = fft_size / 2;
		m_packed.imagp[0]  = m_packed.realp[hs];
		m_packed.realp[hs] = 0.;
		m_packed.imagp[hs] = 0.;
	}

	FFTSetupD			  m_spec;
	double*				  m_spare;
	double*				  m_spare2;
	DSPDoubleSplitComplex m_buf, m_packed;
};

#endif /* LIMES_VECOPS_USE_VDSP */

/*---------------------------------------------------------------------------------------------------------------------------*/

#if LIMES_VECOPS_USE_IPP

class IPP_FloatFFT final : public FFT<float>::FFTImpl
{
public:

	IPP_FloatFFT (int size)
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

	~IPP_FloatFFT()
	{
		ippsFree (m_specbuf);
		ippsFree (m_buf);
		ippsFree (m_packed);
		ippsFree (m_spare);
	}

private:

	void forward (const float* realIn, float* realOut, float* imagOut) final
	{
		ippsFFTFwd_RToCCS_32f (realIn, m_packed, m_spec, m_buf);
		unpackFloat (realOut, imagOut);
	}

	void forwardInterleaved (const float* realIn, float* complexOut) final
	{
		ippsFFTFwd_RToCCS_32f (realIn, complexOut, m_spec, m_buf);
	}

	void forwardPolar (const float* realIn, float* magOut, float* phaseOut) final
	{
		ippsFFTFwd_RToCCS_32f (realIn, m_packed, m_spec, m_buf);
		unpackFloat (m_packed, m_spare);
		ippsCartToPolar_32f (m_packed, m_spare, magOut, phaseOut, fft_size / 2 + 1);
	}

	void forwardMagnitude (const float* realIn, float* magOut) final
	{
		ippsFFTFwd_RToCCS_32f (realIn, m_packed, m_spec, m_buf);
		unpackFloat (m_packed, m_spare);
		ippsMagnitude_32f (m_packed, m_spare, magOut, fft_size / 2 + 1);
	}

	void inverse (const float* realIn, const float* imagIn, float* realOut) final
	{
		packFloat (realIn, imagIn);
		ippsFFTInv_CCSToR_32f (m_packed, realOut, m_spec, m_buf);
	}

	void inverseInterleaved (const float* complexIn, float* realOut) final
	{
		ippsFFTInv_CCSToR_32f (complexIn, realOut, m_spec, m_buf);
	}

	void inversePolar (const float* magIn, const float* phaseIn, float* realOut) final
	{
		ippsPolarToCart_32f (magIn, phaseIn, realOut, m_spare, fft_size / 2 + 1);
		packFloat (realOut, m_spare);  // to m_packed
		ippsFFTInv_CCSToR_32f (m_packed, realOut, m_spec, m_buf);
	}

	void inverseCepstral (const float* magIn, float* cepOut) final
	{
		const int hs1 = fft_size / 2 + 1;
		ippsCopy_32f (magIn, m_spare, hs1);
		ippsAddC_32f_I (0.000001f, m_spare, hs1);
		ippsLn_32f_I (m_spare, hs1);
		packFloat (m_spare, nullptr);
		ippsFFTInv_CCSToR_32f (m_packed, cepOut, m_spec, m_buf);
	}

	void packFloat (const float* re, const float* im)
	{
		int		  index = 0;
		const int hs	= fft_size / 2;
		for (int i = 0; i <= hs; ++i)
		{
			m_packed[index++] = re[i];
			index++;
		}
		index = 0;
		if (im)
		{
			for (int i = 0; i <= hs; ++i)
			{
				index++;
				m_packed[index++] = im[i];
			}
		}
		else {
			for (int i = 0; i <= hs; ++i)
			{
				index++;
				m_packed[index++] = 0.f;
			}
		}
	}

	void unpackFloat (float* re, float* im)	 // re may be equal to m_fpacked
	{
		int		  index = 0;
		const int hs	= fft_size / 2;
		if (im)
		{
			for (int i = 0; i <= hs; ++i)
			{
				index++;
				im[i] = m_packed[index++];
			}
		}
		index = 0;
		for (int i = 0; i <= hs; ++i)
		{
			re[i] = m_packed[index++];
			index++;
		}
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

	IPP_DoubleFFT (int size)
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

	~IPP_DoubleFFT()
	{
		ippsFree (m_specbuf);
		ippsFree (m_buf);
		ippsFree (m_packed);
		ippsFree (m_spare);
	}

private:

	void forward (const double* realIn, double* realOut, double* imagOut) final
	{
		ippsFFTFwd_RToCCS_64f (realIn, m_packed, m_spec, m_buf);
		unpackDouble (realOut, imagOut);
	}

	void forwardInterleaved (const double* realIn, double* complexOut) final
	{
		ippsFFTFwd_RToCCS_64f (realIn, complexOut, m_spec, m_buf);
	}

	void forwardPolar (const double* realIn, double* magOut, double* phaseOut) final
	{
		ippsFFTFwd_RToCCS_64f (realIn, m_packed, m_spec, m_buf);
		unpackDouble (m_packed, m_spare);
		ippsCartToPolar_64f (m_packed, m_spare, magOut, phaseOut, fft_size / 2 + 1);
	}

	void forwardMagnitude (const double* realIn, double* magOut) final
	{
		ippsFFTFwd_RToCCS_64f (realIn, m_packed, m_spec, m_buf);
		unpackDouble (m_packed, m_spare);
		ippsMagnitude_64f (m_packed, m_spare, magOut, fft_size / 2 + 1);
	}

	void inverse (const double* realIn, const double* imagIn, double* realOut) final
	{
		packDouble (realIn, imagIn);
		ippsFFTInv_CCSToR_64f (m_packed, realOut, m_spec, m_buf);
	}

	void inverseInterleaved (const double* complexIn, double* realOut) final
	{
		ippsFFTInv_CCSToR_64f (complexIn, realOut, m_spec, m_buf);
	}

	void inversePolar (const double* magIn, const double* phaseIn, double* realOut) final
	{
		ippsPolarToCart_64f (magIn, phaseIn, realOut, m_spare, fft_size / 2 + 1);
		packDouble (realOut, m_spare);	// to m_packed
		ippsFFTInv_CCSToR_64f (m_packed, realOut, m_spec, m_buf);
	}

	void inverseCepstral (const double* magIn, double* cepOut) final
	{
		const int hs1 = fft_size / 2 + 1;
		ippsCopy_64f (magIn, m_spare, hs1);
		ippsAddC_64f_I (0.000001, m_spare, hs1);
		ippsLn_64f_I (m_spare, hs1);
		packDouble (m_spare, nullptr);
		ippsFFTInv_CCSToR_64f (m_packed, cepOut, m_spec, m_buf);
	}

	void packDouble (const double* re, const double* im)
	{
		int		  index = 0;
		const int hs	= fft_size / 2;
		for (int i = 0; i <= hs; ++i)
		{
			m_packed[index++] = re[i];
			index++;
		}
		index = 0;
		if (im)
		{
			for (int i = 0; i <= hs; ++i)
			{
				index++;
				m_packed[index++] = im[i];
			}
		}
		else {
			for (int i = 0; i <= hs; ++i)
			{
				index++;
				m_packed[index++] = 0.;
			}
		}
	}

	void unpackDouble (double* re, double* im)	// re may be equal to m_dpacked
	{
		int		  index = 0;
		const int hs	= fft_size / 2;
		if (im)
		{
			for (int i = 0; i <= hs; ++i)
			{
				index++;
				im[i] = m_packed[index++];
			}
		}
		index = 0;
		for (int i = 0; i <= hs; ++i)
		{
			re[i] = m_packed[index++];
			index++;
		}
	}

	IppsFFTSpec_R_64f* m_spec;
	Ipp8u*			   m_specbuf;
	Ipp8u*			   m_buf;
	double*			   m_packed;
	double*			   m_spare;
};

#endif /* LIMES_VECOPS_USE_IPP */

/*---------------------------------------------------------------------------------------------------------------------------*/

template <Scalar SampleType>
class FallbackFFT final : public FFT<SampleType>::FFTImpl
{
public:

	FallbackFFT (int size)
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
		m_a_and_b[0] = m_a;
		m_a_and_b[1] = m_b;
		m_c_and_d[0] = m_c;
		m_c_and_d[1] = m_d;
		makeTables();
	}

	~FallbackFFT()
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

private:

	void forward (const SampleType* realIn, SampleType* realOut, SampleType* imagOut) final
	{
		transformF (realIn, realOut, imagOut);

		if constexpr (std::is_same_v<SampleType, float>)
		{
			// v_convert (realOut, m_c, m_half + 1);
			// v_convert (imagOut, m_d, m_half + 1);
		}
	}

	void forwardInterleaved (const SampleType* realIn, SampleType* complexOut) final
	{
		transformF (realIn, m_c, m_d);

		if constexpr (std::is_same_v<SampleType, double>)
		{
			// v_interleave (complexOut, m_c_and_d, 2, m_half + 1);
		}
		else
		{
			for (int i = 0; i <= m_half; ++i) complexOut[i * 2] = m_c[i];
			for (int i = 0; i <= m_half; ++i) complexOut[i * 2 + 1] = m_d[i];
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
		}

		transformI (realIn, imagIn, realOut);
	}

	void inverseInterleaved (const SampleType* complexIn, SampleType* realOut) final
	{
		if constexpr (std::is_same_v<SampleType, double>)
		{
			// v_deinterleave (m_a_and_b, complexIn, 2, m_half + 1);
		}
		else
		{
			for (int i = 0; i <= m_half; ++i) m_a[i] = complexIn[i * 2];
			for (int i = 0; i <= m_half; ++i) m_b[i] = complexIn[i * 2 + 1];
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
		for (int i = 0; i <= m_half; ++i)
		{
			SampleType real = log (magIn[i] + SampleType (0.000001));
			m_a[i]			= real;
			m_b[i]			= 0.0;
		}

		transformI (m_a, m_b, cepOut);
	}

	void makeTables()
	{
		int i, j, k, m;

		int n = m_half;

		const auto bits = [h = m_half]
		{
			for (i = 0;; ++i)
				if (h & (1 << i))
					return i;

			// jassertfalse;
			return 0;
		}();

		for (i = 0; i < n; ++i)
		{
			m = i;
			for (j = k = 0; j < bits; ++j)
			{
				k = (k << 1) | (m & 1);
				m >>= 1;
			}
			m_table[i] = k;
		}

		// sin and cos tables for complex fft
		int ix = 0;
		for (i = 2; i <= m_maxTabledBlock; i <<= 1)
		{
			double phase   = 2.0 * M_PI / double (i);
			m_sincos[ix++] = sin (phase);
			m_sincos[ix++] = sin (2.0 * phase);
			m_sincos[ix++] = cos (phase);
			m_sincos[ix++] = cos (2.0 * phase);
		}

		// sin and cos tables for real-complex transform
		ix = 0;
		for (i = 0; i < n / 2; ++i)
		{
			double phase	 = M_PI * (double (i + 1) / double (m_half) + 0.5);
			m_sincos_r[ix++] = sin (phase);
			m_sincos_r[ix++] = cos (phase);
		}
	}

	// Uses m_a and m_b internally; does not touch m_c or m_d
	void transformF (const SampleType* ri,
					 double* ro, double* io)
	{
		int halfhalf = m_half / 2;
		for (int i = 0; i < m_half; ++i)
		{
			m_a[i] = ri[i * 2];
			m_b[i] = ri[i * 2 + 1];
		}

		transformComplex (m_a, m_b, m_vr, m_vi, false);
		ro[0]	   = m_vr[0] + m_vi[0];
		ro[m_half] = m_vr[0] - m_vi[0];
		io[0] = io[m_half] = 0.0;
		int ix			   = 0;
		for (int i = 0; i < halfhalf; ++i)
		{
			double s	   = -m_sincos_r[ix++];
			double c	   = m_sincos_r[ix++];
			int	   k	   = i + 1;
			double r0	   = m_vr[k];
			double i0	   = m_vi[k];
			double r1	   = m_vr[m_half - k];
			double i1	   = -m_vi[m_half - k];
			double tw_r	   = (r0 - r1) * c - (i0 - i1) * s;
			double tw_i	   = (r0 - r1) * s + (i0 - i1) * c;
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
		int halfhalf = m_half / 2;
		m_vr[0]		 = ri[0] + ri[m_half];
		m_vi[0]		 = ri[0] - ri[m_half];
		int ix		 = 0;
		for (int i = 0; i < halfhalf; ++i)
		{
			double s		 = m_sincos_r[ix++];
			double c		 = m_sincos_r[ix++];
			int	   k		 = i + 1;
			double r0		 = ri[k];
			double r1		 = ri[m_half - k];
			double i0		 = ii[k];
			double i1		 = -ii[m_half - k];
			double tw_r		 = (r0 - r1) * c - (i0 - i1) * s;
			double tw_i		 = (r0 - r1) * s + (i0 - i1) * c;
			m_vr[k]			 = (r0 + r1 + tw_r);
			m_vr[m_half - k] = (r0 + r1 - tw_r);
			m_vi[k]			 = (i0 + i1 + tw_i);
			m_vi[m_half - k] = (tw_i - i0 - i1);
		}
		transformComplex (m_vr, m_vi, m_c, m_d, true);
		for (int i = 0; i < m_half; ++i)
		{
			ro[i * 2]	  = m_c[i];
			ro[i * 2 + 1] = m_d[i];
		}
	}

	void transformComplex (const double* ri, const double* ii,
						   double* ro, double* io,
						   bool inverse)
	{
		// Following Don Cross's 1998 implementation, described by its author as public domain.

		// Because we are at heart a real-complex fft only, and we know that:
		const int n = m_half;

		for (int i = 0; i < n; ++i)
		{
			int j = m_table[i];
			ro[j] = ri[i];
			io[j] = ii[i];
		}

		int	   ix		= 0;
		int	   blockEnd = 1;
		double ifactor	= (inverse ? -1.0 : 1.0);

		for (int blockSize = 2; blockSize <= n; blockSize <<= 1)
		{
			double sm1, sm2, cm1, cm2;

			if (blockSize <= m_maxTabledBlock)
			{
				sm1 = ifactor * m_sincos[ix++];
				sm2 = ifactor * m_sincos[ix++];
				cm1 = m_sincos[ix++];
				cm2 = m_sincos[ix++];
			}
			else {
				double phase = 2.0 * M_PI / double (blockSize);
				sm1			 = ifactor * sin (phase);
				sm2			 = ifactor * sin (2.0 * phase);
				cm1			 = cos (phase);
				cm2			 = cos (2.0 * phase);
			}

			double w = 2 * cm1;
			double ar[3], ai[3];

			for (int i = 0; i < n; i += blockSize)
			{
				ar[2] = cm2;
				ar[1] = cm1;

				ai[2] = sm2;
				ai[1] = sm1;

				int j = i;

				for (int m = 0; m < blockEnd; ++m)
				{
					ar[0] = w * ar[1] - ar[2];
					ar[2] = ar[1];
					ar[1] = ar[0];

					ai[0] = w * ai[1] - ai[2];
					ai[2] = ai[1];
					ai[1] = ai[0];

					int	   k  = j + blockEnd;
					double tr = ar[0] * ro[k] - ai[0] * io[k];
					double ti = ar[0] * io[k] + ai[0] * ro[k];

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

	const int m_half { fft_size / 2 };
	const int m_blockTableSize { 16 };
	const int m_maxTabledBlock { 1 << m_blockTableSize };
	int*	  m_table;
	double*	  m_sincos;
	double*	  m_sincos_r;
	double*	  m_vr;
	double*	  m_vi;
	double*	  m_a;
	double*	  m_b;
	double*	  m_c;
	double*	  m_d;
	double*	  m_a_and_b[2];
	double*	  m_c_and_d[2];
};

/*---------------------------------------------------------------------------------------------------------------------------*/

template <Scalar SampleType>
FFT<SampleType>::FFT (int size)
{
#if LIMES_VECOPS_USE_VDSP
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
