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

#include <limes_export.h>
#include <limes_vecops.h>
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace vecops
{

LIMES_NO_EXPORT [[nodiscard]] static inline int orderFromFFTSize (int size)
{
	for (int i = 0;; ++i)
		if ((size & (1 << i)) != 0)
			return i;

	// jassertfalse;
	return 0;
}

template <Scalar SampleType>
LIMES_NO_EXPORT constexpr SampleType shiftAmount = SampleType (0.000001);


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

	virtual void forward (const SampleType* realIn, SampleType* realOut, SampleType* imagOut) = 0;

	virtual void forwardInterleaved (const SampleType* realIn, SampleType* complexOut) = 0;

	virtual void forwardPolar (const SampleType* realIn, SampleType* magOut, SampleType* phaseOut) = 0;

	virtual void forwardMagnitude (const SampleType* realIn, SampleType* magOut) = 0;

	virtual void inverse (const SampleType* realIn, const SampleType* imagIn, SampleType* realOut) = 0;

	virtual void inverseInterleaved (const SampleType* complexIn, SampleType* realOut) = 0;

	virtual void inversePolar (const SampleType* magIn, const SampleType* phaseIn, SampleType* realOut) = 0;

	virtual void inverseCepstral (const SampleType* magIn, SampleType* cepOut) = 0;

protected:

	const int fft_size;	 // NOLINT
	const int m_order;	 // NOLINT
};

}  // namespace vecops

LIMES_END_NAMESPACE
