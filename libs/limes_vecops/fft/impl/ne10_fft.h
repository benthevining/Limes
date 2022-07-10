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

// IWYU pragma: private
// IWYU pragma: friend limes_fft.cpp

#pragma once

#include "../fft_common.h"
#include <limes_export.h>
#include <limes_namespace.h>
#include <limes_core.h>
#include <memory>
#include <NE10.h>

/** @file
	This file defines the NE10 FFT implementation.
	@ingroup fft
	@see limes_vecops.h
 */

LIMES_BEGIN_NAMESPACE

namespace vecops::fft
{

/// @cond internals

/** This class implements an FFT using the NE10 library.
	@ingroup fft
	@see FFT
 */
template <Scalar SampleType>
class LIMES_NO_EXPORT NE10_FFT final : public FFTImpl<SampleType>
{
public:
	explicit NE10_FFT (int size);

	LIMES_NON_COPYABLE (NE10_FFT)
	LIMES_DEFAULT_MOVABLE (NE10_FFT)

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

	// inStorage -> outStorage
	void perform_forward() noexcept;

	// inStorage -> outStorage
	void perform_inverse() noexcept;

	const int m_half { this->fft_size / 2 };

	using State = ne10_fft_state_float32_t;

	std::unique_ptr<State, decltype ([] (State* p)
									 { ne10_fft_destroy_c2c_float32 (p); })>
		plan;

	memory::array_pointer<ne10_fft_cpx_float32_t> inStorage, outStorage memory::array_pointer<SampleType> realStorage, imagStorage;
};

/// @endcond

}  // namespace vecops::fft

LIMES_END_NAMESPACE
