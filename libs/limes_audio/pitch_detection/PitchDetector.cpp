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

#include "PitchDetector.h"
#include <limes_namespace.h>
#include <algorithm>
#include <limes_core.h>

LIMES_BEGIN_NAMESPACE

namespace dsp::pitch
{

template <Sample SampleType>
typename PitchDetector<SampleType>::Result PitchDetector<SampleType>::detectPeriod (const SampleType* const inputAudio, int numSamples) noexcept
{
	LIMES_ASSERT (numSamples >= getLatencySamples());

	// calculate pitch using each algorithm
	// TO DO: stop early if a result is sufficiently confident?
	for (size_t i = 0; i < num_algos; ++i)
		algoResults[i] = algos[i]->detectPeriod (inputAudio, numSamples);

	// find result with greatest confidence
	chosenResult = nullptr;

	for (const auto& result : algoResults)
		if (chosenResult == nullptr || result.confidence > chosenResult->confidence)
			chosenResult = &result;

	return *chosenResult;
}

template <Sample SampleType>
typename PitchDetector<SampleType>::Result PitchDetector<SampleType>::getLastDetectedPeriod() const noexcept
{
	if (chosenResult == nullptr)
		return Result {};

	return *chosenResult;
}

template <Sample SampleType>
int PitchDetector<SampleType>::getLatencySamples() const noexcept
{
	auto latency = 0;

	for (const auto* alg : algos)
		latency = std::max (latency, alg->getLatencySamples());

	return latency;
}

template <Sample SampleType>
int PitchDetector<SampleType>::setSamplerate (double newSamplerate)
{
	for (auto* alg : algos)
		alg->setSamplerate (newSamplerate);

	return getLatencySamples();
}

template <Sample SampleType>
void PitchDetector<SampleType>::reset() noexcept
{
	for (auto* alg : algos)
		alg->reset();

	chosenResult = nullptr;
}

template <Sample SampleType>
void PitchDetector<SampleType>::releaseResources()
{
	for (auto* alg : algos)
		alg->releaseResources();

	chosenResult = nullptr;
}

template class PitchDetector<float>;
template class PitchDetector<double>;

}  // namespace dsp::pitch

LIMES_END_NAMESPACE
