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

/** @file
	This file defines the fallback FFT implementation.
	@ingroup fft
	@see limes_fft.h
 */

LIMES_BEGIN_NAMESPACE

namespace vecops::fft
{

/// @cond internals

/** This class implements an FFT in regular C++ code.
	This class's code may end up calling some MIPP or Pommier functions, or it may not, depending on what's available in your environment.
	@ingroup fft
 */
template <Scalar SampleType>
class LIMES_NO_EXPORT FallbackFFT final : public FFTImpl<SampleType>
{
public:
	explicit FallbackFFT (int size);

	LIMES_NON_COPYABLE (FallbackFFT)
	LIMES_DEFAULT_MOVABLE (FallbackFFT)

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

	// Uses m_a and m_b internally; does not touch m_c or m_d
	LIMES_FORCE_INLINE void transformF (const SampleType* ri,
										SampleType* ro, SampleType* io) noexcept;

	// Uses m_c and m_d internally; does not touch m_a or m_b
	LIMES_FORCE_INLINE void transformI (const SampleType* ri, const SampleType* ii,
										SampleType* ro) noexcept;

	LIMES_FORCE_INLINE void transformComplex (const SampleType* ri, const SampleType* ii,
											  SampleType* ro, SampleType* io,
											  bool inverse) noexcept;

	const int m_half { this->fft_size / 2 };

	memory::aligned_pointer<int> m_table;

	memory::aligned_pointer<SampleType> m_sincos, m_sincos_r, m_vr, m_vi, m_a, m_b, m_c, m_d;

	SampleType* m_a_and_b[2] { m_a, m_b };
	SampleType* m_c_and_d[2] { m_c, m_d };
};

/// @endcond

}  // namespace vecops::fft

LIMES_END_NAMESPACE
