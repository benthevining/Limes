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

#include "./ne10_fft.h"
#include <limes_namespace.h>
#include <limes_export.h>
#include <NE10.h>
#include <memory>
#include <stdexcept>
#include "../../vecops/vecops.h"

LIMES_BEGIN_NAMESPACE

namespace vecops::fft
{

template <Scalar SampleType>
NE10_FFT<SampleType>::NE10_FFT (int size)
	: plan (ne10_fft_alloc_c2c_float32 (size)),
	  inStorage (static_cast<std::size_t> (this->fft_size)), outStorage (static_cast<std::size_t> (this->fft_size)),
	  realStorage (static_cast<std::size_t> (this->fft_size)), imagStorage (static_cast<std::size_t> (this->fft_size))
{
	if (plan.get() == nullptr)
		throw std::runtime_error { "NE10 FFT - failed to initialize!" };
}

template <Scalar SampleType>
void NE10_FFT<SampleType>::forward (const SampleType* realIn, SampleType* realOut, SampleType* imagOut) noexcept
{
	for (auto i = 0; i < this->fft_size; ++i)
	{
		inStorage[i].r = static_cast<float> (realIn[i]);
		inStorage[i].i = 0.f;
	}

	perform_forward();

	for (auto i = 0; i < this->fft_size; ++i)
	{
		realOut[i] = static_cast<SampleType> (outStorage[i].r);
		imagOut[i] = static_cast<SampleType> (outStorage[i].i);
	}
}

template <Scalar SampleType>
void NE10_FFT<SampleType>::forwardInterleaved (const SampleType* realIn, SampleType* complexOut) noexcept
{
	for (auto i = 0; i < this->fft_size; ++i)
	{
		inStorage[i].r = static_cast<float> (realIn[i]);
		inStorage[i].i = 0.f;
	}

	perform_forward();

	for (auto i = 0, j = 0; i < this->fft_size; ++i)
	{
		complexOut[j++] = static_cast<SampleType> (outStorage[i].r);
		complexOut[j++] = static_cast<SampleType> (outStorage[i].i);
	}
}

template <Scalar SampleType>
void NE10_FFT<SampleType>::forwardPolar (const SampleType* realIn, SampleType* magOut, SampleType* phaseOut) noexcept
{
	forward (realIn, realStorage.get(), imagStorage.get());

	vecops::cartesianToPolar (magOut, phaseOut, realStorage.get(), imagStorage.get(), m_half + 1);
}

template <Scalar SampleType>
void NE10_FFT<SampleType>::forwardMagnitude (const SampleType* realIn, SampleType* magOut) noexcept
{
	forward (realIn, realStorage.get(), imagStorage.get());

	vecops::cartesianToMagnitudes (magOut, realStorage.get(), imagStorage.get(), m_half + 1);
}

template <Scalar SampleType>
void NE10_FFT<SampleType>::inverse (const SampleType* realIn, const SampleType* imagIn, SampleType* realOut) noexcept
{
	for (auto i = 0; i < this->fft_size; ++i)
	{
		inStorage[i].r = static_cast<float> (realIn[i]);
		inStorage[i].i = static_cast<float> (imagIn[i]);
	}

	perform_inverse();

	for (auto i = 0; i < this->fft_size; ++i)
	{
		realOut[i] = static_cast<SampleType> (outStorage[i].r);
	}
}

template <Scalar SampleType>
void NE10_FFT<SampleType>::inverseInterleaved (const SampleType* complexIn, SampleType* realOut) noexcept
{
	for (auto i = 0, j = 0; i < this->fft_size; ++i)
	{
		inStorage[i].r = static_cast<float> (complexIn[j++]);
		inStorage[i].i = static_cast<float> (complexIn[j++]);
	}

	perform_inverse();

	for (auto i = 0; i < this->fft_size; ++i)
	{
		realOut[i] = static_cast<SampleType> (outStorage[i].r);
	}
}

template <Scalar SampleType>
void NE10_FFT<SampleType>::inversePolar (const SampleType* magIn, const SampleType* phaseIn, SampleType* realOut) noexcept
{
	vecops::polarToCartesian (realStorage.get(), imagStorage.get(), magIn, phaseIn, m_half + 1);

	inverse (realStorage.get(), imagStorage.get(), realOut);
}

template <Scalar SampleType>
void NE10_FFT<SampleType>::inverseCepstral (const SampleType* magIn, SampleType* cepOut) noexcept
{
	for (std::size_t i = 0; i <= static_cast<std::size_t> (m_half); ++i)
	{
		realStorage[static_cast<std::size_t> (i)] = std::log (magIn[i] + this->shiftAmount);
		imagStorage[static_cast<std::size_t> (i)] = SampleType (0.);
	}

	inverse (realStorage.get(), imagStorage.get(), cepOut);
}

template <Scalar SampleType>
void NE10_FFT<SampleType>::perform_forward() noexcept
{
	ne10_fft_c2c_1d_float32 (outStorage.get(), inStorage.get(), plan.get(), std::int32_t (0));
}

template <Scalar SampleType>
void NE10_FFT<SampleType>::perform_inverse() noexcept
{
	ne10_fft_c2c_1d_float32 (outStorage.get(), inStorage.get(), plan.get(), std::int32_t (1));
}

template <Scalar SampleType>
void NE10_FFT<SampleType>::reset()
{
	inStorage.clear();
	outStorage.clear();
}

template class NE10_FFT<float>;
template class NE10_FFT<double>;

}  // namespace vecops::fft

LIMES_END_NAMESPACE
