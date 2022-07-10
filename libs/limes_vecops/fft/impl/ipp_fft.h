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
#include <ipps.h>

/** @file
	This file defines the IPP FFT implementation.
	@ingroup fft
	@see limes_vecops.h
 */

LIMES_BEGIN_NAMESPACE

namespace vecops::fft
{

/// @cond internals

/** This class implements an FFT using the Intel IPP library.
	@ingroup fft
	@see FFT
 */
template <Scalar SampleType>
class LIMES_NO_EXPORT IPP_FFT final : public FFTImpl<SampleType>
{
public:
	explicit IPP_FFT (int size);

	~IPP_FFT() final;

	LIMES_NON_COPYABLE (IPP_FFT)
	LIMES_DEFAULT_MOVABLE (IPP_FFT)

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

	LIMES_FORCE_INLINE void ipp_pack (const SampleType* re, const SampleType* im) noexcept;

	LIMES_FORCE_INLINE void ipp_unpack (SampleType* const re, SampleType* const im) const noexcept;

	using FFTSpecType = std::conditional_t<std::is_same_v<SampleType, float>,
										   IppsFFTSpec_R_32f,
										   IppsFFTSpec_R_64f>;

	FFTSpecType* m_spec;
	Ipp8u*		 m_specbuf;
	Ipp8u*		 m_buf;
	SampleType*	 m_packed;
	SampleType*	 m_spare;
};

/// @endcond

}  // namespace vecops::fft

LIMES_END_NAMESPACE
