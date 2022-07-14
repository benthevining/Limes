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

#pragma once

#include "../fft_common.h"
#include <limes_export.h>
#include <limes_namespace.h>
#include <limes_core.h>
#include <memory>
#include <pffft.h>

/** @file
	This file defines the pffft_FFT class.
	@ingroup fft
	@see limes_fft.h
 */

LIMES_BEGIN_NAMESPACE

namespace vecops::fft
{

/// @cond internals

/** This class implements an FFT using the pffft library.
	@ingroup fft
 */
template <Scalar SampleType>
class LIMES_NO_EXPORT pffft_FFT final : public FFTImpl<SampleType>
{
public:
	explicit pffft_FFT (int size);

	LIMES_NON_COPYABLE (pffft_FFT)
	LIMES_DEFAULT_MOVABLE (pffft_FFT)

private:
	void forward (const SampleType* realIn, SampleType* realOut, SampleType* imagOut) noexcept final;

	void forwardInterleaved (const SampleType* realIn, SampleType* complexOut) noexcept final;

	void forwardPolar (const SampleType* realIn, SampleType* magOut, SampleType* phaseOut) noexcept final;

	void forwardMagnitude (const SampleType* realIn, SampleType* magOut) noexcept final;

	void inverse (const SampleType* realIn, const SampleType* imagIn, SampleType* realOut) noexcept final;

	void inverseInterleaved (const SampleType* complexIn, SampleType* realOut) noexcept final;

	void inversePolar (const SampleType* magIn, const SampleType* phaseIn, SampleType* realOut) noexcept final;

	void inverseCepstral (const SampleType* magIn, SampleType* cepOut) noexcept final;

	void reset() final;

	LIMES_FORCE_INLINE void perform (const float* input, float* output, bool isForward) noexcept;

	const int m_half { this->fft_size / 2 };

	std::unique_ptr<PFFFT_Setup, decltype ([] (PFFFT_Setup* s)
										   { pffft_destroy_setup (s); })>
		impl;

	memory::aligned_pointer<float> workbuf;

	memory::array_pointer<float> inStorage, outStorage;

	memory::array_pointer<SampleType> interleavedStorage, realStorage, imagStorage;
};

/// @endcond

}  // namespace vecops::fft

LIMES_END_NAMESPACE
