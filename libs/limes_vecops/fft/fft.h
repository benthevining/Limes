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

#include <memory>  // for unique_ptr
#include "../limes_vecops.h"

namespace limes::vecops
{

template <Scalar SampleType>
class FFT final
{
public:

	class FFTImpl;

	explicit FFT (int size);

	[[nodiscard]] int getSize() const noexcept;

	void forward (const SampleType* realIn, SampleType* realOut, SampleType* imagOut);

	void forwardInterleaved (const SampleType* realIn, SampleType* complexOut);

	void forwardPolar (const SampleType* realIn, SampleType* magOut, SampleType* phaseOut);

	void forwardMagnitude (const SampleType* realIn, SampleType* magOut);

	void inverse (const SampleType* realIn, const SampleType* imagIn, SampleType* realOut);

	void inverseInterleaved (const SampleType* complexIn, SampleType* realOut);

	void inversePolar (const SampleType* magIn, const SampleType* phaseIn, SampleType* realOut);

	void inverseCepstral (const SampleType* magIn, SampleType* cepOut);

private:

	std::unique_ptr<FFTImpl> pimpl;
};

}  // namespace limes::vecops
