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
// IWYU pragma: friend *_fft.h

#pragma once

#include <limes_export.h>
#include <limes_namespace.h>
#include "../limes_vecops.h"
#include <limes_core.h>

/** @file
	This file contains implementation details common to multiple FFT implementations.
	@ingroup fft
 */

LIMES_BEGIN_NAMESPACE

namespace vecops
{

/// @cond internals

/** This class defines the interface for any implementation of the FFT class backend.
	@see FFT
	@ingroup fft
 */
template <Scalar SampleType>
class LIMES_NO_EXPORT FFTImpl
{
public:
	explicit FFTImpl (int size)
		: fft_size (size), m_order (orderFromFFTSize (size))
	{
	}

	virtual ~FFTImpl() = default;

	[[nodiscard]] int getSize() const noexcept
	{
		return fft_size;
	}

	virtual void forward (const SampleType* realIn, SampleType* realOut, SampleType* imagOut) noexcept = 0;

	virtual void forwardInterleaved (const SampleType* realIn, SampleType* complexOut) noexcept = 0;

	virtual void forwardPolar (const SampleType* realIn, SampleType* magOut, SampleType* phaseOut) noexcept = 0;

	virtual void forwardMagnitude (const SampleType* realIn, SampleType* magOut) noexcept = 0;

	virtual void inverse (const SampleType* realIn, const SampleType* imagIn, SampleType* realOut) noexcept = 0;

	virtual void inverseInterleaved (const SampleType* complexIn, SampleType* realOut) noexcept = 0;

	virtual void inversePolar (const SampleType* magIn, const SampleType* phaseIn, SampleType* realOut) noexcept = 0;

	virtual void inverseCepstral (const SampleType* magIn, SampleType* cepOut) noexcept = 0;

	virtual void reset() = 0;

protected:
	const int fft_size;	 // NOLINT
	const int m_order;	 // NOLINT

	static constexpr SampleType shiftAmount = SampleType (0.000001);  // NOLINT

private:
	[[nodiscard]] static LIMES_FORCE_INLINE int orderFromFFTSize (int size) noexcept
	{
		for (auto i = 0;; ++i)
			if ((size & (1 << i)) != 0)
				return i;

		LIMES_ASSERT_FALSE;
		return 0;
	}
};

/// @endcond

}  // namespace vecops

LIMES_END_NAMESPACE
