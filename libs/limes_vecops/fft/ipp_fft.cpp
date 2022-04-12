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

#include "ipp_fft.h"

namespace limes::vecops
{

template <Scalar SampleType>
LIMES_FORCE_INLINE void ipp_pack (const SampleType* re, const SampleType* im, int fft_size, SampleType* m_packed)
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
LIMES_FORCE_INLINE void ipp_unpack (SampleType* const re, SampleType* const im, int fft_size, const SampleType* const m_packed)
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

/*---------------------------------------------------------------------------------------------------------------------------*/

IPP_FloatFFT::IPP_FloatFFT (int size)
	: FFTImpl<float> (size)
{
	static_assert (FFT<float>::isUsingIPP());

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

IPP_FloatFFT::~IPP_FloatFFT()
{
	ippsFree (m_specbuf);
	ippsFree (m_buf);
	ippsFree (m_packed);
	ippsFree (m_spare);
}

void IPP_FloatFFT::forward (const float* realIn, float* realOut, float* imagOut)
{
	ippsFFTFwd_RToCCS_32f (realIn, m_packed, m_spec, m_buf);
	ipp_unpack (realOut, imagOut, fft_size, m_packed);
}

void IPP_FloatFFT::forwardInterleaved (const float* realIn, float* complexOut)
{
	ippsFFTFwd_RToCCS_32f (realIn, complexOut, m_spec, m_buf);
}

void IPP_FloatFFT::forwardPolar (const float* realIn, float* magOut, float* phaseOut)
{
	ippsFFTFwd_RToCCS_32f (realIn, m_packed, m_spec, m_buf);
	ipp_unpack (m_packed, m_spare, fft_size, m_packed);
	ippsCartToPolar_32f (m_packed, m_spare, magOut, phaseOut, fft_size / 2 + 1);
}

void IPP_FloatFFT::forwardMagnitude (const float* realIn, float* magOut)
{
	ippsFFTFwd_RToCCS_32f (realIn, m_packed, m_spec, m_buf);
	ipp_unpack (m_packed, m_spare, fft_size, m_packed);
	ippsMagnitude_32f (m_packed, m_spare, magOut, fft_size / 2 + 1);
}

void IPP_FloatFFT::inverse (const float* realIn, const float* imagIn, float* realOut)
{
	ipp_pack (realIn, imagIn, fft_size, m_packed);
	ippsFFTInv_CCSToR_32f (m_packed, realOut, m_spec, m_buf);
}

void IPP_FloatFFT::inverseInterleaved (const float* complexIn, float* realOut)
{
	ippsFFTInv_CCSToR_32f (complexIn, realOut, m_spec, m_buf);
}

void IPP_FloatFFT::inversePolar (const float* magIn, const float* phaseIn, float* realOut)
{
	ippsPolarToCart_32f (magIn, phaseIn, realOut, m_spare, fft_size / 2 + 1);
	ipp_pack (realOut, m_spare, fft_size, m_packed);
	ippsFFTInv_CCSToR_32f (m_packed, realOut, m_spec, m_buf);
}

void IPP_FloatFFT::inverseCepstral (const float* magIn, float* cepOut)
{
	const auto hs1 = fft_size / 2 + 1;

	ippsCopy_32f (magIn, m_spare, hs1);
	ippsAddC_32f_I (shiftAmount<float>, m_spare, hs1);
	ippsLn_32f_I (m_spare, hs1);
	ipp_pack<float> (m_spare, nullptr, fft_size, m_packed);
	ippsFFTInv_CCSToR_32f (m_packed, cepOut, m_spec, m_buf);
}

/*---------------------------------------------------------------------------------------------------------------------------*/

IPP_DoubleFFT::IPP_DoubleFFT (int size)
	: FFTImpl<double> (size)
{
	static_assert (FFT<double>::isUsingIPP());

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

IPP_DoubleFFT::~IPP_DoubleFFT()
{
	ippsFree (m_specbuf);
	ippsFree (m_buf);
	ippsFree (m_packed);
	ippsFree (m_spare);
}

void IPP_DoubleFFT::forward (const double* realIn, double* realOut, double* imagOut)
{
	ippsFFTFwd_RToCCS_64f (realIn, m_packed, m_spec, m_buf);
	ipp_unpack (realOut, imagOut, fft_size, m_packed);
}

void IPP_DoubleFFT::forwardInterleaved (const double* realIn, double* complexOut)
{
	ippsFFTFwd_RToCCS_64f (realIn, complexOut, m_spec, m_buf);
}

void IPP_DoubleFFT::forwardPolar (const double* realIn, double* magOut, double* phaseOut)
{
	ippsFFTFwd_RToCCS_64f (realIn, m_packed, m_spec, m_buf);
	ipp_unpack (m_packed, m_spare, fft_size, m_packed);
	ippsCartToPolar_64f (m_packed, m_spare, magOut, phaseOut, fft_size / 2 + 1);
}

void IPP_DoubleFFT::forwardMagnitude (const double* realIn, double* magOut)
{
	ippsFFTFwd_RToCCS_64f (realIn, m_packed, m_spec, m_buf);
	ipp_unpack (m_packed, m_spare, fft_size, m_packed);
	ippsMagnitude_64f (m_packed, m_spare, magOut, fft_size / 2 + 1);
}

void IPP_DoubleFFT::inverse (const double* realIn, const double* imagIn, double* realOut)
{
	ipp_pack (realIn, imagIn, fft_size, m_packed);
	ippsFFTInv_CCSToR_64f (m_packed, realOut, m_spec, m_buf);
}

void IPP_DoubleFFT::inverseInterleaved (const double* complexIn, double* realOut)
{
	ippsFFTInv_CCSToR_64f (complexIn, realOut, m_spec, m_buf);
}

void IPP_DoubleFFT::inversePolar (const double* magIn, const double* phaseIn, double* realOut)
{
	ippsPolarToCart_64f (magIn, phaseIn, realOut, m_spare, fft_size / 2 + 1);
	ipp_pack (realOut, m_spare, fft_size, m_packed);
	ippsFFTInv_CCSToR_64f (m_packed, realOut, m_spec, m_buf);
}

void IPP_DoubleFFT::inverseCepstral (const double* magIn, double* cepOut)
{
	const auto hs1 = fft_size / 2 + 1;

	ippsCopy_64f (magIn, m_spare, hs1);
	ippsAddC_64f_I (shiftAmount<double>, m_spare, hs1);
	ippsLn_64f_I (m_spare, hs1);
	ipp_pack<double> (m_spare, nullptr, fft_size, m_packed);
	ippsFFTInv_CCSToR_64f (m_packed, cepOut, m_spec, m_buf);
}

}  // namespace limes::vecops
