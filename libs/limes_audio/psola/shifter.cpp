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

#include "shifter.h"
#include <limes_platform.h>
#include <limes_core.h>
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace dsp::psola
{

template <Sample SampleType>
Shifter<SampleType>::Shifter (Analyzer<SampleType>& analyzerToUse)
	: SampleStream<SampleType> ([this]
								{ return getNextSample(); }),
	  analyzer (analyzerToUse)
{
	analyzer.registerShifter (*this);
}

template <Sample SampleType>
Shifter<SampleType>::~Shifter()
{
	analyzer.deregisterShifter (*this);
}

template <Sample SampleType>
void Shifter<SampleType>::setPitchHz (int pitchHz) noexcept
{
	// Did you call Analyzer::setSamplerate() first?
	LIMES_ASSERT (analyzer.samplerate > 0 && pitchHz > 0);

	targetPeriod  = math::periodInSamples (analyzer.samplerate, static_cast<decltype (targetPeriod)> (pitchHz));
	targetPitchHz = pitchHz;

	if (const auto intTargetPeriod = math::round (targetPeriod);
		samplesToNextGrain > intTargetPeriod)
	{
		// ???
		samplesToNextGrain = intTargetPeriod;
	}

	pitchHzChanged (pitchHz);
}

template <Sample SampleType>
float Shifter<SampleType>::getPitchHz() const noexcept
{
	// Did you call Analyzer::setSamplerate() first?
	LIMES_ASSERT (analyzer.samplerate > 0);

	return math::freqFromPeriod (analyzer.samplerate, static_cast<float> (targetPeriod));
}

template <Sample SampleType>
void Shifter<SampleType>::newBlockStarting() noexcept
{
	placeInBlock = 0;
	onNewBlock();
}

template <Sample SampleType>
SampleType Shifter<SampleType>::getNextSample() noexcept
{
	// did you call setPitch() first?
	LIMES_ASSERT (targetPeriod > 0.f);

	if (samplesToNextGrain == 0)
	{
		getGrainToStart().startNewGrain (analyzer.getClosestGrain (placeInBlock));
		samplesToNextGrain = math::round (targetPeriod);
	}

	SampleType sample { 0 };

	for (auto& grain : grains)
		if (grain.isActive())
			sample += grain.getNextSample();

	--samplesToNextGrain;
	++placeInBlock;

	return sample;
}

template <Sample SampleType>
typename Shifter<SampleType>::Grain& Shifter<SampleType>::getGrainToStart() noexcept
{
	for (auto& grain : grains)
		if (! grain.isActive())
			return grain;

	LIMES_ASSERT_FALSE;

	return grains.emplace_back();
}

template <Sample SampleType>
void Shifter<SampleType>::samplerateChanged() noexcept
{
	if (targetPitchHz > 0)
		setPitchHz (targetPitchHz);
}

template <Sample SampleType>
void Shifter<SampleType>::latencyChanged (int newLatency)
{
	grains.resize (static_cast<typename std::vector<Grain>::size_type>(newLatency / 2));
}

template <Sample SampleType>
void Shifter<SampleType>::reset() noexcept
{
	for (auto& grain : grains)
		grain.clearGrain();

	samplesToNextGrain = 0;
	targetPeriod	   = 0.f;
	placeInBlock	   = 0;
}

template <Sample SampleType>
void Shifter<SampleType>::releaseResources()
{
	grains.clear();

	samplesToNextGrain = 0;
	targetPeriod	   = 0.f;
	placeInBlock	   = 0;
}

/*---------------------------------------------------------------------------------------------------------------------------*/

template <Sample SampleType>
Shifter<SampleType>::Grain::~Grain()
{
	if (analysisGrain != nullptr)
		analysisGrain->decRefCountWithoutDeleting();
}

template <Sample SampleType>
SampleType Shifter<SampleType>::Grain::getNextSample() noexcept
{
	LIMES_ASSERT (analysisGrain != nullptr);

	const auto sample = analysisGrain->getSample (sampleIdx++);

	if (sampleIdx >= analysisGrain->getSize())
		clearGrain();

	return sample;
}

template <Sample SampleType>
void Shifter<SampleType>::Grain::startNewGrain (AnalysisGrain& analysisGrainToUse) noexcept
{
	LIMES_ASSERT (analysisGrain == nullptr);
	LIMES_ASSERT (analysisGrainToUse.getSize() > 0);

	analysisGrain = &analysisGrainToUse;
	sampleIdx	  = 0;

	analysisGrainToUse.incRefCount();
}

template <Sample SampleType>
void Shifter<SampleType>::Grain::clearGrain() noexcept
{
	if (analysisGrain != nullptr)
	{
		analysisGrain->decRefCountWithoutDeleting();
		analysisGrain = nullptr;
	}

	sampleIdx = 0;
}

template <Sample SampleType>
bool Shifter<SampleType>::Grain::isActive() const noexcept
{
	return analysisGrain != nullptr;
}

template class Shifter<float>;
template class Shifter<double>;

}  // namespace dsp::psola

LIMES_END_NAMESPACE
