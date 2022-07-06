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

#include "./ipp_fft.h"
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace vecops
{

template <Scalar SampleType>
IPP_FFT<SampleType>::IPP_FFT (int size)
	: FFTImpl<SampleType> (size)
{
	static_assert (fft::isUsingIPP());

	int specSize, specBufferSize, bufferSize;  // NOLINT

	if constexpr (std::is_same_v<SampleType, float>)
	{
		ippsFFTGetSize_R_32f (this->m_order, IPP_FFT_NODIV_BY_ANY, ippAlgHintFast,
							  &specSize, &specBufferSize, &bufferSize);

		m_packed = ippsMalloc_32f (this->fft_size + 2);
		m_spare	 = ippsMalloc_32f (this->fft_size / 2 + 1);

		Ipp8u* tmp = ippsMalloc_8u (specBufferSize);

		ippsFFTInit_R_32f (&m_spec,
						   this->m_order, IPP_FFT_NODIV_BY_ANY, ippAlgHintFast,
						   m_specbuf, tmp);

		ippsFree (tmp);
	}
	else
	{
		ippsFFTGetSize_R_64f (this->m_order, IPP_FFT_NODIV_BY_ANY, ippAlgHintFast,
							  &specSize, &specBufferSize, &bufferSize);

		m_packed = ippsMalloc_64f (this->fft_size + 2);
		m_spare	 = ippsMalloc_64f (this->fft_size / 2 + 1);

		Ipp8u* tmp = ippsMalloc_8u (specBufferSize);

		ippsFFTInit_R_64f (&m_spec,
						   this->m_order, IPP_FFT_NODIV_BY_ANY, ippAlgHintFast,
						   m_specbuf, tmp);

		ippsFree (tmp);
	}

	m_specbuf = ippsMalloc_8u (specSize);
	m_buf	  = ippsMalloc_8u (bufferSize);
}

template <Scalar SampleType>
IPP_FFT<SampleType>::~IPP_FFT()
{
	ippsFree (m_specbuf);
	ippsFree (m_buf);
	ippsFree (m_packed);
	ippsFree (m_spare);
}

template <Scalar SampleType>
void IPP_FFT<SampleType>::forward (const SampleType* realIn, SampleType* realOut, SampleType* imagOut) noexcept
{
	if constexpr (std::is_same_v<SampleType, float>)
		ippsFFTFwd_RToCCS_32f (realIn, m_packed, m_spec, m_buf);
	else
		ippsFFTFwd_RToCCS_64f (realIn, m_packed, m_spec, m_buf);

	ipp_unpack (realOut, imagOut);
}

template <Scalar SampleType>
void IPP_FFT<SampleType>::forwardInterleaved (const SampleType* realIn, SampleType* complexOut) noexcept
{
	if constexpr (std::is_same_v<SampleType, float>)
		ippsFFTFwd_RToCCS_32f (realIn, complexOut, m_spec, m_buf);
	else
		ippsFFTFwd_RToCCS_64f (realIn, complexOut, m_spec, m_buf);
}

template <Scalar SampleType>
void IPP_FFT<SampleType>::forwardPolar (const SampleType* realIn, SampleType* magOut, SampleType* phaseOut) noexcept
{
	if constexpr (std::is_same_v<SampleType, float>)
		ippsFFTFwd_RToCCS_32f (realIn, m_packed, m_spec, m_buf);
	else
		ippsFFTFwd_RToCCS_64f (realIn, m_packed, m_spec, m_buf);

	ipp_unpack (m_packed, m_spare);

	if constexpr (std::is_same_v<SampleType, float>)
		ippsCartToPolar_32f (m_packed, m_spare, magOut, phaseOut, this->fft_size / 2 + 1);
	else
		ippsCartToPolar_64f (m_packed, m_spare, magOut, phaseOut, this->fft_size / 2 + 1);
}

template <Scalar SampleType>
void IPP_FFT<SampleType>::forwardMagnitude (const SampleType* realIn, SampleType* magOut) noexcept
{
	if constexpr (std::is_same_v<SampleType, float>)
		ippsFFTFwd_RToCCS_32f (realIn, m_packed, m_spec, m_buf);
	else
		ippsFFTFwd_RToCCS_64f (realIn, m_packed, m_spec, m_buf);

	ipp_unpack (m_packed, m_spare);

	if constexpr (std::is_same_v<SampleType, float>)
		ippsMagnitude_32f (m_packed, m_spare, magOut, this->fft_size / 2 + 1);
	else
		ippsMagnitude_64f (m_packed, m_spare, magOut, this->fft_size / 2 + 1);
}

template <Scalar SampleType>
void IPP_FFT<SampleType>::inverse (const SampleType* realIn, const SampleType* imagIn, SampleType* realOut) noexcept
{
	ipp_pack (realIn, imagIn);

	if constexpr (std::is_same_v<SampleType, float>)
		ippsFFTInv_CCSToR_32f (m_packed, realOut, m_spec, m_buf);
	else
		ippsFFTInv_CCSToR_64f (m_packed, realOut, m_spec, m_buf);
}

template <Scalar SampleType>
void IPP_FFT<SampleType>::inverseInterleaved (const SampleType* complexIn, SampleType* realOut) noexcept
{
	if constexpr (std::is_same_v<SampleType, float>)
		ippsFFTInv_CCSToR_32f (complexIn, realOut, m_spec, m_buf);
	else
		ippsFFTInv_CCSToR_64f (complexIn, realOut, m_spec, m_buf);
}

template <Scalar SampleType>
void IPP_FFT<SampleType>::inversePolar (const SampleType* magIn, const SampleType* phaseIn, SampleType* realOut) noexcept
{
	if constexpr (std::is_same_v<SampleType, float>)
		ippsPolarToCart_32f (magIn, phaseIn, realOut, m_spare, this->fft_size / 2 + 1);
	else
		ippsPolarToCart_64f (magIn, phaseIn, realOut, m_spare, this->fft_size / 2 + 1);

	ipp_pack (realOut, m_spare);

	if constexpr (std::is_same_v<SampleType, float>)
		ippsFFTInv_CCSToR_32f (m_packed, realOut, m_spec, m_buf);
	else
		ippsFFTInv_CCSToR_64f (m_packed, realOut, m_spec, m_buf);
}

template <Scalar SampleType>
void IPP_FFT<SampleType>::inverseCepstral (const SampleType* magIn, SampleType* cepOut) noexcept
{
	const auto hs1 = this->fft_size / 2 + 1;

	if constexpr (std::is_same_v<SampleType, float>)
	{
		ippsCopy_32f (magIn, m_spare, hs1);
		ippsAddC_32f_I (this->shiftAmount, m_spare, hs1);
		ippsLn_32f_I (m_spare, hs1);
	}
	else
	{
		ippsCopy_64f (magIn, m_spare, hs1);
		ippsAddC_64f_I (this->shiftAmount, m_spare, hs1);
		ippsLn_64f_I (m_spare, hs1);
	}

	ipp_pack (m_spare, nullptr);

	if constexpr (std::is_same_v<SampleType, float>)
		ippsFFTInv_CCSToR_32f (m_packed, cepOut, m_spec, m_buf);
	else
		ippsFFTInv_CCSToR_64f (m_packed, cepOut, m_spec, m_buf);
}

template <Scalar SampleType>
void IPP_FFT<SampleType>::reset()
{
	vecops::clear (m_packed, this->fft_size + 2);
	vecops::clear (m_spare, this->fft_size / 2 + 1);
}

template <Scalar SampleType>
LIMES_FORCE_INLINE void IPP_FFT<SampleType>::ipp_pack (const SampleType* re, const SampleType* im) noexcept
{
	const auto hs = this->fft_size / 2;

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
LIMES_FORCE_INLINE void IPP_FFT<SampleType>::ipp_unpack (SampleType* const re, SampleType* const im) const noexcept
{
	const auto hs = this->fft_size / 2;

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

}  // namespace vecops

LIMES_END_NAMESPACE
