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

#include <limes_core.h>
#include "./PitchDetectionAlgorithm.h"
#include "./PitchData.h"

LIMES_BEGIN_NAMESPACE

namespace dsp::pitch
{

template <Sample SampleType>
class LIMES_EXPORT ZeroCrossings final : public PitchDetectionAlgorithm<SampleType>
{
public:

	/** Convenience typedef for the datatype returned by the algorithm. */
	using Result = PitchData<SampleType>;

	[[nodiscard]] Result detectPeriod (const SampleType* const inputAudio, int numSamples) noexcept final;

	[[nodiscard]] Result getLastDetectedPeriod() const noexcept final;

	[[nodiscard]] int getLatencySamples() const noexcept final;

	int setSamplerate (double newSamplerate) final;

	void reset() noexcept final;

	void releaseResources() final;

private:

	Result data;

	double samplerate { 0 };
};

}  // namespace dsp::pitch

LIMES_END_NAMESPACE
