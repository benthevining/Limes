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

#include "fft_common.h"
#include <limes_export.h>
#include <type_traits>
#include <limes_namespace.h>
#include <limes_core.h>

/** @file
	This file defines the vDSP FFT implementation.
	@ingroup fft
 */

LIMES_BEGIN_NAMESPACE

namespace vecops
{

/// @cond

template <Scalar SampleType>
class LIMES_NO_EXPORT vDSP_FFT final : public FFTImpl<SampleType>
{
public:

	explicit vDSP_FFT (int size);

	~vDSP_FFT() final;

	LIMES_NON_COPYABLE (vDSP_FFT);
	LIMES_DEFAULT_MOVABLE (vDSP_FFT);

private:

	void forward (const SampleType* realIn, SampleType* realOut, SampleType* imagOut) final;

	void forwardInterleaved (const SampleType* realIn, SampleType* complexOut) final;

	void forwardPolar (const SampleType* realIn, SampleType* magOut, SampleType* phaseOut) final;

	void forwardMagnitude (const SampleType* realIn, SampleType* magOut) final;

	void inverse (const SampleType* realIn, const SampleType* imagIn, SampleType* realOut) final;

	void inverseInterleaved (const SampleType* complexIn, SampleType* realOut) final;

	void inversePolar (const SampleType* magIn, const SampleType* phaseIn, SampleType* realOut) final;

	void inverseCepstral (const SampleType* magIn, SampleType* cepOut) final;

	LIMES_FORCE_INLINE void packReal (const SampleType* const re);

	LIMES_FORCE_INLINE void unpackReal (SampleType* const re);

	LIMES_FORCE_INLINE void unpackComplex (SampleType* const re, SampleType* const im);

	LIMES_FORCE_INLINE void vDSP_nyq (SampleType* real, SampleType* imag) const;

	LIMES_FORCE_INLINE void vDSP_denyq (SampleType* real, SampleType* imag) const;

	using FFTSetupType = std::conditional_t<std::is_same_v<SampleType, float>,
											FFTSetup,
											FFTSetupD>;

	using DSPSplitComplexType = std::conditional_t<std::is_same_v<SampleType, float>,
												   DSPSplitComplex,
												   DSPDoubleSplitComplex>;

	using DSPComplexType = std::conditional_t<std::is_same_v<SampleType, float>,
											  DSPComplex,
											  DSPDoubleComplex>;

	FFTSetupType m_spec;

	memory::aligned_pointer<SampleType> m_spare, m_spare2;

	DSPSplitComplexType m_buf, m_packed;
};

/// @endcond

}  // namespace vecops

LIMES_END_NAMESPACE
