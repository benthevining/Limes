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

#include "./pffft_fft.h"
#include <limes_namespace.h>
#include <type_traits>
#include <cstring>
#include "../../vecops/vecops.h"

LIMES_BEGIN_NAMESPACE

namespace vecops::fft
{

static_assert (isUsingpffft());

template <Scalar SampleType>
pffft_FFT<SampleType>::pffft_FFT (int size)
	: FFTImpl<SampleType> (size),
	  impl (pffft_new_setup (size, PFFFT_REAL)),
	  workbuf (size, 16),
	  interleavedStorage (static_cast<std::size_t> (size * 2)),
	  realStorage (static_cast<std::size_t> (this->fft_size)), imagStorage (static_cast<std::size_t> (this->fft_size))
{
	if (impl.get() == nullptr)
		throw std::runtime_error { "pffft FFT - failed to initialize!" };

	if constexpr (std::is_same_v<SampleType, double>)
	{
		inStorage.reallocate (size);
		outStorage.reallocate (size);
	}
}

template <Scalar SampleType>
void pffft_FFT<SampleType>::forward (const SampleType* realIn, SampleType* realOut, SampleType* imagOut) noexcept
{
	forwardInterleaved (realIn, interleavedStorage.get());

	SampleType* chans[2] = { realOut, imagOut };

	vecops::deinterleave (chans, interleavedStorage.get(), 2, interleavedStorage.getSize());
}

template <Scalar SampleType>
void pffft_FFT<SampleType>::forwardInterleaved (const SampleType* realIn, SampleType* complexOut) noexcept
{
	if constexpr (std::is_same_v<SampleType, float>)
		perform (realIn, complexOut, true);
	else
	{
		for (auto i = 0; i < size; ++i)
			inStorage[i] = static_cast<float> (realIn[i]);

		perform (inStorage.get(), outStorage.get(), true);

		for (auto i = 0; i < size; ++i)
			complexOut[i] = static_cast<SampleType> (outStorage[i]);
	}
}

template <Scalar SampleType>
void pffft_FFT<SampleType>::forwardPolar (const SampleType* realIn, SampleType* magOut, SampleType* phaseOut) noexcept
{
	forward (realIn, realStorage.get(), imagStorage.get());

	vecops::cartesianToPolar (magOut, phaseOut, realStorage.get(), imagStorage.get(), m_half + 1);
}

template <Scalar SampleType>
void pffft_FFT<SampleType>::forwardMagnitude (const SampleType* realIn, SampleType* magOut) noexcept
{
	forward (realIn, realStorage.get(), imagStorage.get());

	vecops::cartesianToMagnitudes (magOut, realStorage.get(), imagStorage.get(), m_half + 1);
}

template <Scalar SampleType>
void pffft_FFT<SampleType>::inverse (const SampleType* realIn, const SampleType* imagIn, SampleType* realOut) noexcept
{
	const SampleType* chans[2] = { realIn, imagIn };

	vecops::interleave (interleavedStorage.get(), chans, 2, interleavedStorage.getSize());

	inverseInterleaved (interleavedStorage.get(), realOut);
}

template <Scalar SampleType>
void pffft_FFT<SampleType>::inverseInterleaved (const SampleType* complexIn, SampleType* realOut) noexcept
{
	if constexpr (std::is_same_v<SampleType, float>)
		perform (complexIn, realOut, false);
	else
	{
		for (auto i = 0; i < size; ++i)
			inStorage[i] = static_cast<float> (realIn[i]);

		perform (inStorage.get(), outStorage.get(), false);

		for (auto i = 0; i < size; ++i)
			complexOut[i] = static_cast<SampleType> (outStorage[i]);
	}
}

template <Scalar SampleType>
void pffft_FFT<SampleType>::inversePolar (const SampleType* magIn, const SampleType* phaseIn, SampleType* realOut) noexcept
{
	vecops::polarToCartesian (realStorage.get(), imagStorage.get(), magIn, phaseIn, m_half + 1);

	inverse (realStorage.get(), imagStorage.get(), realOut);
}

template <Scalar SampleType>
void pffft_FFT<SampleType>::inverseCepstral (const SampleType* magIn, SampleType* cepOut) noexcept
{
	for (std::size_t i = 0; i <= static_cast<std::size_t> (m_half); ++i)
	{
		realStorage[static_cast<std::size_t> (i)] = std::log (magIn[i] + this->shiftAmount);
		imagStorage[static_cast<std::size_t> (i)] = SampleType (0.);
	}

	inverse (realStorage.get(), imagStorage.get(), cepOut);
}

template <Scalar SampleType>
LIMES_FORCE_INLINE void pffft_FFT<SampleType>::perform (const float* input, float* output, bool isForward) noexcept
{
	const auto direction = isForward ? PFFFT_FORWARD : PFFFT_BACKWARD;

	pffft_transform_ordered (impl.get(), input, output, workbuf.get(), direction);
}

template <Scalar SampleType>
void pffft_FFT<SampleType>::reset()
{
	std::memset (workbuf.get(), 0, static_cast<std::size_t> (this->fft_size));

	inStorage.clear();
	outStorage.clear();
	interleavedStorage.clear();
	realStorage.clear();
	imagStorage.clear();
}

template class pffft_FFT<float>;
template class pffft_FFT<double>;

}  // namespace vecops::fft

LIMES_END_NAMESPACE
