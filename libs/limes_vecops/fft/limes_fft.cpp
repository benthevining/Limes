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

#include "./limes_fft.h"
#include <limes_namespace.h>
#include <limes_core.h>
#include <stdexcept>
#include <sstream>
#include "../vecops/vecops_macros.h"

#if LIMES_VECOPS_USE_FFTW
#	include "./impl/fftw_fft.h"
#elif LIMES_VECOPS_USE_VDSP
#	include "./impl/vdsp_fft.h"
#elif LIMES_VECOPS_USE_IPP
#	include "./impl/ipp_fft.h"
#elif LIMES_VECOPS_USE_NE10
#	include "./impl/ne10_fft.h"
#elif LIMES_VECOPS_USE_PFFFT
#	include "./impl/pffft_fft.h"
#else
#	include "./impl/fallback_fft.h"
#endif

LIMES_BEGIN_NAMESPACE

namespace vecops
{

// clang-format off

#if LIMES_VECOPS_USE_FFTW

	template <Scalar SampleType>
	using ImplType = fft::FFTW_FFT<SampleType>;

#elif LIMES_VECOPS_USE_VDSP

	template <Scalar SampleType>
	using ImplType = fft::vDSP_FFT<SampleType>;

#elif LIMES_VECOPS_USE_IPP

	template <Scalar SampleType>
	using ImplType = fft::IPP_FFT<SampleType>;

#elif LIMES_VECOPS_USE_NE10

	template <Scalar SampleType>
	using ImplType = fft::NE10_FFT<SampleType>;

#elif LIMES_VECOPS_USE_PFFFT

	template <Scalar SampleType>
	using ImplType = fft::pffft_FFT<SampleType>;

#else

	template <Scalar SampleType>
	using ImplType = fft::FallbackFFT<SampleType>;

#endif

// clang-format on

static inline void check_fft_size (int newSize)
{
	if (! math::isPowerOf2 (newSize))
	{
		std::stringstream s;

		s << "FFT: only accepts power-of-2 sizes, but "
		  << newSize << " was passed!";

		throw std::runtime_error { s.str() };
	}
}

template <Scalar SampleType>
FFT<SampleType>::FFT (int size)
	: pimpl (std::make_unique<ImplType<SampleType>> (size))
{
	check_fft_size (size);
}

template <Scalar SampleType>
int FFT<SampleType>::getSize() const noexcept
{
	return pimpl->getSize();
}

template <Scalar SampleType>
void FFT<SampleType>::changeSize (int newFFTSize)
{
	if (pimpl->getSize() == newFFTSize)
		return;

	check_fft_size (newFFTSize);

	pimpl.reset (new ImplType<SampleType> (newFFTSize));
}

template <Scalar SampleType>
void FFT<SampleType>::forward (const SampleType* realIn, SampleType* realOut, SampleType* imagOut) noexcept
{
	pimpl->forward (realIn, realOut, imagOut);
}

template <Scalar SampleType>
void FFT<SampleType>::forwardInterleaved (const SampleType* realIn, SampleType* complexOut) noexcept
{
	pimpl->forwardInterleaved (realIn, complexOut);
}

template <Scalar SampleType>
void FFT<SampleType>::forwardPolar (const SampleType* realIn, SampleType* magOut, SampleType* phaseOut) noexcept
{
	pimpl->forwardPolar (realIn, magOut, phaseOut);
}

template <Scalar SampleType>
void FFT<SampleType>::forwardMagnitude (const SampleType* realIn, SampleType* magOut) noexcept
{
	pimpl->forwardMagnitude (realIn, magOut);
}

template <Scalar SampleType>
void FFT<SampleType>::inverse (const SampleType* realIn, const SampleType* imagIn, SampleType* realOut) noexcept
{
	pimpl->inverse (realIn, imagIn, realOut);
}

template <Scalar SampleType>
void FFT<SampleType>::inverseInterleaved (const SampleType* complexIn, SampleType* realOut) noexcept
{
	pimpl->inverseInterleaved (complexIn, realOut);
}

template <Scalar SampleType>
void FFT<SampleType>::inversePolar (const SampleType* magIn, const SampleType* phaseIn, SampleType* realOut) noexcept
{
	pimpl->inversePolar (magIn, phaseIn, realOut);
}

template <Scalar SampleType>
void FFT<SampleType>::inverseCepstral (const SampleType* magIn, SampleType* cepOut) noexcept
{
	pimpl->inverseCepstral (magIn, cepOut);
}

template <Scalar SampleType>
void FFT<SampleType>::reset()
{
	pimpl->reset();
}

template <Scalar SampleType>
double FFT<SampleType>::getFrequencyForBin (int binNumber, double samplerate) const noexcept
{
	LIMES_ASSERT (binNumber >= 0);

	const auto size = pimpl->getSize();

	LIMES_ASSERT (binNumber < size);

	return (static_cast<double> (binNumber) * samplerate) / static_cast<double> (size);
}

template <Scalar SampleType>
int FFT<SampleType>::getBinForFrequency (double frequency, double samplerate) const noexcept
{
	const auto size = pimpl->getSize();

	const auto result = (frequency * static_cast<double> (size)) / samplerate;

	const auto bin = math::round (result);

	LIMES_ASSERT (bin < size);

	return bin;
}

template class FFT<float>;
template class FFT<double>;

}  // namespace vecops

LIMES_END_NAMESPACE
