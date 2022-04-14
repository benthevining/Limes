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

#include "limes_fft.h"
#include <type_traits>	 // for is_same_v
#include "fft_common.h"	 // for FFTImpl


#if LIMES_VECOPS_USE_FFTW
#	include "fftw_fft.h"
#elif LIMES_VECOPS_USE_VDSP
#	include "vdsp_fft.h"
#elif LIMES_VECOPS_USE_IPP
#	include "ipp_fft.h"
#else
#	include "fallback_fft.h"
#endif

namespace limes::vecops
{

template <Scalar SampleType>
FFT<SampleType>::FFT (int size)
{
#if LIMES_VECOPS_USE_FFTW
	if constexpr (std::is_same_v<SampleType, float>)
		pimpl = std::make_unique<FFTW_FloatFFT> (size);
	else
		pimpl = std::make_unique<FFTW_DoubleFFT> (size);
#elif LIMES_VECOPS_USE_VDSP
	pimpl = std::make_unique<vDSP_FFT<SampleType>> (size);
#elif LIMES_VECOPS_USE_IPP
	pimpl = std::make_unique<IPP_FFT<SampleType>> (size);
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
