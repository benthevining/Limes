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

	void forward (const float* realIn, float* realOut, float* imagOut)
	{
		packReal (realIn);
		vDSP_fft_zript (m_spec, &m_packed, 1, &m_buf, m_order, FFT_FORWARD);
		fdenyq();
		unpackComplex (realOut, imagOut);
	}

	void forwardInterleaved (const float* realIn, float* complexOut)
	{
		packReal (realIn);
		vDSP_fft_zript (m_spec, &m_packed, 1, &m_buf, m_order, FFT_FORWARD);
		fdenyq();
		unpackComplex (complexOut);
	}

	void forwardPolar (const float* realIn, float* magOut, float* phaseOut)
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

	void forwardMagnitude (const float* realIn, float* magOut)
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

	void inverse (const float* realIn, const float* imagIn, float* realOut)
	{
		packComplex (realIn, imagIn);
		vDSP_fft_zript (m_spec, &m_packed, 1, &m_buf, m_order, FFT_INVERSE);
		unpackReal (realOut);
	}

	void inverseInterleaved (const float* complexIn, float* realOut)
	{
		float* f[2] = { m_packed.realp, m_packed.imagp };
		// v_deinterleave (f, complexIn, 2, fft_size / 2 + 1);
		fnyq();
		vDSP_fft_zript (m_spec, &m_packed, 1, &m_buf, m_order, FFT_INVERSE);
		unpackReal (realOut);
	}

	void inversePolar (const float* magIn, const float* phaseIn, float* realOut)
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

	void inverseCepstral (const float* magIn, float* cepOut)
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

	void forward (const double* realIn, double* realOut, double* imagOut)
	{
		packReal (realIn);
		vDSP_fft_zriptD (m_spec, &m_packed, vDSP_Stride (1), &m_buf, vDSP_Length (m_order), FFT_FORWARD);
		ddenyq();
		unpackComplex (realOut, imagOut);
	}

	void forwardInterleaved (const double* realIn, double* complexOut)
	{
		packReal (realIn);
		vDSP_fft_zriptD (m_spec, &m_packed, 1, &m_buf, m_order, FFT_FORWARD);
		ddenyq();
		unpackComplex (complexOut);
	}

	void forwardPolar (const double* realIn, double* magOut, double* phaseOut)
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

	void forwardMagnitude (const double* realIn, double* magOut)
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

	void inverse (const double* realIn, const double* imagIn, double* realOut)
	{
		packComplex (realIn, imagIn);
		vDSP_fft_zriptD (m_spec, &m_packed, 1, &m_buf, m_order, FFT_INVERSE);
		unpackReal (realOut);
	}

	void inverseInterleaved (const double* complexIn, double* realOut)
	{
		double* f[2] = { m_packed.realp, m_packed.imagp };
		// v_deinterleave (f, complexIn, 2, fft_size / 2 + 1);
		dnyq();
		vDSP_fft_zriptD (m_spec, &m_packed, 1, &m_buf, m_order, FFT_INVERSE);
		unpackReal (realOut);
	}

	void inversePolar (const double* magIn, const double* phaseIn, double* realOut)
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

	void inverseCepstral (const double* magIn, double* cepOut)
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

#endif

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

	void forward (const float* realIn, float* realOut, float* imagOut)
	{
		ippsFFTFwd_RToCCS_32f (realIn, m_packed, m_spec, m_buf);
		unpackFloat (realOut, imagOut);
	}

	void forwardInterleaved (const float* realIn, float* complexOut)
	{
		ippsFFTFwd_RToCCS_32f (realIn, complexOut, m_spec, m_buf);
	}

	void forwardPolar (const float* realIn, float* magOut, float* phaseOut)
	{
		ippsFFTFwd_RToCCS_32f (realIn, m_packed, m_spec, m_buf);
		unpackFloat (m_packed, m_spare);
		ippsCartToPolar_32f (m_packed, m_spare, magOut, phaseOut, fft_size / 2 + 1);
	}

	void forwardMagnitude (const float* realIn, float* magOut)
	{
		ippsFFTFwd_RToCCS_32f (realIn, m_packed, m_spec, m_buf);
		unpackFloat (m_packed, m_spare);
		ippsMagnitude_32f (m_packed, m_spare, magOut, fft_size / 2 + 1);
	}

	void inverse (const float* realIn, const float* imagIn, float* realOut)
	{
		packFloat (realIn, imagIn);
		ippsFFTInv_CCSToR_32f (m_packed, realOut, m_spec, m_buf);
	}

	void inverseInterleaved (const float* complexIn, float* realOut)
	{
		ippsFFTInv_CCSToR_32f (complexIn, realOut, m_spec, m_buf);
	}

	void inversePolar (const float* magIn, const float* phaseIn, float* realOut)
	{
		ippsPolarToCart_32f (magIn, phaseIn, realOut, m_spare, fft_size / 2 + 1);
		packFloat (realOut, m_spare);  // to m_packed
		ippsFFTInv_CCSToR_32f (m_packed, realOut, m_spec, m_buf);
	}

	void inverseCepstral (const float* magIn, float* cepOut)
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

	void forward (const double* realIn, double* realOut, double* imagOut)
	{
		ippsFFTFwd_RToCCS_64f (realIn, m_packed, m_spec, m_buf);
		unpackDouble (realOut, imagOut);
	}

	void forwardInterleaved (const double* realIn, double* complexOut)
	{
		ippsFFTFwd_RToCCS_64f (realIn, complexOut, m_spec, m_buf);
	}

	void forwardPolar (const double* realIn, double* magOut, double* phaseOut)
	{
		ippsFFTFwd_RToCCS_64f (realIn, m_packed, m_spec, m_buf);
		unpackDouble (m_packed, m_spare);
		ippsCartToPolar_64f (m_packed, m_spare, magOut, phaseOut, fft_size / 2 + 1);
	}

	void forwardMagnitude (const double* realIn, double* magOut)
	{
		ippsFFTFwd_RToCCS_64f (realIn, m_packed, m_spec, m_buf);
		unpackDouble (m_packed, m_spare);
		ippsMagnitude_64f (m_packed, m_spare, magOut, fft_size / 2 + 1);
	}

	void inverse (const double* realIn, const double* imagIn, double* realOut)
	{
		packDouble (realIn, imagIn);
		ippsFFTInv_CCSToR_64f (m_packed, realOut, m_spec, m_buf);
	}

	void inverseInterleaved (const double* complexIn, double* realOut)
	{
		ippsFFTInv_CCSToR_64f (complexIn, realOut, m_spec, m_buf);
	}

	void inversePolar (const double* magIn, const double* phaseIn, double* realOut)
	{
		ippsPolarToCart_64f (magIn, phaseIn, realOut, m_spare, fft_size / 2 + 1);
		packDouble (realOut, m_spare);	// to m_packed
		ippsFFTInv_CCSToR_64f (m_packed, realOut, m_spec, m_buf);
	}

	void inverseCepstral (const double* magIn, double* cepOut)
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

#endif

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
