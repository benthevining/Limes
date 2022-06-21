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

#include "analyzer.h"
#include "shifter.h"
#include <limes_platform.h>
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace dsp::psola
{

template <Sample SampleType>
Analyzer<SampleType>::Analyzer (int minFreqHz)
{
	setMinInputFreq (minFreqHz);
}

template <Sample SampleType>
void Analyzer<SampleType>::registerShifter (Shifter<SampleType>& shifter)
{
	LIMES_ASSERT (! alg::contains (shifters, &shifter));

	shifters.push_back (&shifter);
}

template <Sample SampleType>
void Analyzer<SampleType>::deregisterShifter (Shifter<SampleType>& shifter)
{
	std::remove (shifters.begin(), shifters.end(), &shifter);
}

template <Sample SampleType>
int Analyzer<SampleType>::getLastInputPitch() const noexcept
{
	if (currentPeriod > 0.f && samplerate > 0.)
		return math::round (math::freqFromPeriod (samplerate, currentPeriod));

	return 0;
}

template <Sample SampleType>
void Analyzer<SampleType>::analyzeInput (const Buffer& input) noexcept
{
	analyzeInput (input.getReadPointer (0), input.getNumSamples());
}

template <Sample SampleType>
void Analyzer<SampleType>::analyzeInput (const SampleType* const inputAudio, int numSamples) noexcept
{
	LIMES_ASSERT (samplerate > 0.);
	LIMES_ASSERT (numSamples >= getLatencySamples());

	for (auto& grain : grains)
		grain.newBlockStarting (lastBlocksize);

	for (auto* shifter : shifters)
		shifter->newBlockStarting();

	const auto* const prevFrameSamples = prevFrame.getReadPointer (0);

	if (incompleteGrainsFromLastFrame.size() > 0)
	{
		LIMES_ASSERT (lastFrameGrainSize > 0 && lastBlocksize > 0);

		makeWindow (lastFrameGrainSize);

		for (const auto grainStartInLastFrame : incompleteGrainsFromLastFrame)
		{
			const auto samplesFromLastFrame = lastBlocksize - grainStartInLastFrame;
			LIMES_ASSERT (samplesFromLastFrame > 0);

			getGrainToStoreIn().storeNewGrain (prevFrameSamples, grainStartInLastFrame, samplesFromLastFrame,
											   inputAudio, lastFrameGrainSize - samplesFromLastFrame,
											   window, lastFrameGrainSize, -grainStartInLastFrame);
		}

		incompleteGrainsFromLastFrame.clear();
	}

	currentPeriod = [this, inputAudio, numSamples]() -> SampleType
	{
		const auto detectedPeriod = pitchDetector.detectPeriod (inputAudio, numSamples);

		if (detectedPeriod.isPitched())
			return detectedPeriod.getPeriod (this->samplerate);

		int min, max;
		pitchDetector.getYin().getCurrentLegalPeriodRange (min, max);

		return static_cast<SampleType> (random.next (min, max));
	}();

	LIMES_ASSERT (currentPeriod > 0.f && currentPeriod <= numSamples / 2);

	const auto grainSize = math::round (currentPeriod * 2.f);

	for (const auto peak : peakFinder.findPeaks (inputAudio, numSamples, currentPeriod))
	{
		const auto start = math::round (static_cast<float> (peak) - currentPeriod);

		if (start < 0)
		{
			const auto samplesFromPrevFrame = grainSize + start;
			const auto samplesFromThisFrame = grainSize - samplesFromPrevFrame;

			LIMES_ASSERT (samplesFromPrevFrame > 0 && samplesFromThisFrame > 0);

			if (lastBlocksize == 0)	 // no samples from prev frame to complete this grain, so fill in the beginning of the grain with zeroes
			{
				if (samplesFromThisFrame < 3)  // not enough samples to window...
					continue;

				makeWindow (samplesFromThisFrame);

				getGrainToStoreIn().storeNewGrainWithZeroesAtStart (samplesFromPrevFrame,
																	inputAudio, samplesFromThisFrame,
																	window, grainSize, start);

				continue;
			}

			makeWindow (grainSize);

			getGrainToStoreIn().storeNewGrain (prevFrameSamples, lastBlocksize - samplesFromPrevFrame, samplesFromPrevFrame,
											   inputAudio, samplesFromThisFrame,
											   window, grainSize, start);

			continue;
		}

		const auto end = math::round (static_cast<float> (peak) + currentPeriod);

		LIMES_ASSERT (end - start == grainSize);

		if (end >= numSamples)
		{
			incompleteGrainsFromLastFrame.push_back (start);
			continue;
		}

		makeWindow (grainSize);

		getGrainToStoreIn().storeNewGrain (inputAudio, start, window, grainSize);
	}

	prevFrame.copyFrom (inputAudio, static_cast<std::size_t> (numSamples), 0);

	lastBlocksize	   = numSamples;
	lastFrameGrainSize = grainSize;
}

template <Sample SampleType>
typename Analyzer<SampleType>::Grain& Analyzer<SampleType>::getGrainToStoreIn() noexcept
{
	for (auto& grain : grains)
		if (grain.getRefCount() == 0)
			return grain;

	LIMES_ASSERT_FALSE;

	return grains.emplace_back();
}

template <Sample SampleType>
void Analyzer<SampleType>::makeWindow (int size) noexcept
{
	LIMES_ASSERT (size > 2);

	vecops::window::generateHanning (window.getWritePointer (0), size);
}

template <Sample SampleType>
typename Analyzer<SampleType>::Grain& Analyzer<SampleType>::getClosestGrain (int placeInBlock) noexcept
{
	LIMES_ASSERT (grains.size() > 0);
	LIMES_ASSERT (placeInBlock >= 0);

	struct GainDistanceData final
	{
		void test (Grain& newGrain, int newDistance) noexcept
		{
			if (newDistance < distance)
			{
				distance = newDistance;
				grain	 = &newGrain;
			}
		}

		Grain* grain { nullptr };
		int	   distance { std::numeric_limits<int>::max() };
	};

	GainDistanceData before, after;

	for (auto& grain : grains)
	{
		if (grain.getSize() == 0)
			continue;

		const auto origStart = grain.getOrigStart();

		if (origStart == placeInBlock) return grain;

		const auto currentDist = std::abs (origStart - placeInBlock);

		LIMES_ASSERT (currentDist > 0);

		if (origStart < placeInBlock)
			before.test (grain, currentDist);
		else
			after.test (grain, currentDist);
	}

	if (before.grain != nullptr)
	{
		if (after.grain != nullptr)
		{
			if (before.distance <= after.distance)
				return *before.grain;

			return *after.grain;
		}

		return *before.grain;
	}

	LIMES_ASSERT (after.grain != nullptr);

	return *after.grain;  // cppcheck-suppress returnReference
}

template <Sample SampleType>
int Analyzer<SampleType>::getLatencySamples() const noexcept
{
	return pitchDetector.getLatencySamples();
}

template <Sample SampleType>
int Analyzer<SampleType>::setSamplerate (double newSamplerate)
{
	LIMES_ASSERT (newSamplerate > 0.);

	samplerate = newSamplerate;

	pitchDetector.setSamplerate (newSamplerate);

	for (auto* shifter : shifters)
		shifter->samplerateChanged();

	return latencyChanged();
}

template <Sample SampleType>
int Analyzer<SampleType>::setMinInputFreq (int minFreqHz)
{
	LIMES_ASSERT (minFreqHz > 0);

	pitchDetector.getYin().setMinHz (minFreqHz);

	return latencyChanged();
}

template <Sample SampleType>
int Analyzer<SampleType>::latencyChanged()
{
	const auto latency = pitchDetector.getLatencySamples();

	peakFinder.prepare (latency);

	window.resize (latency);
	prevFrame.resize (latency);
	incompleteGrainsFromLastFrame.reserve (static_cast<std::vector<int>::size_type> (latency / 2));

	grains.resize (static_cast<typename std::vector<Grain>::size_type> (latency / 2));

	for (auto& grain : grains)
		grain.reserveSize (latency);

	for (auto* shifter : shifters)
		shifter->latencyChanged (latency);

	return latency;
}

template <Sample SampleType>
void Analyzer<SampleType>::reset() noexcept
{
	peakFinder.reset();

	for (auto& grain : grains)
		grain.clearGrain();

	for (auto* shifter : shifters)
		shifter->reset();
}

template <Sample SampleType>
void Analyzer<SampleType>::releaseResources()
{
	for (auto* shifter : shifters)
		shifter->releaseResources();

	peakFinder.releaseResources();

	samplerate		   = 0.;
	lastBlocksize	   = 0;
	lastFrameGrainSize = 0;
	currentPeriod	   = 0.f;

	prevFrame.deallocate();
	window.deallocate();
	incompleteGrainsFromLastFrame.clear();
	incompleteGrainsFromLastFrame.shrink_to_fit();

	grains.clear();
}

/*---------------------------------------------------------------------------------------------------------------------------*/

template <Sample SampleType>
SampleType Analyzer<SampleType>::Grain::getSample (int index) const noexcept
{
	LIMES_ASSERT (index >= 0 && index < getSize());
	LIMES_ASSERT (samples.getNumSamples() >= static_cast<int> (grainSize));

	return samples.getSample (0, static_cast<std::size_t> (index));
}

template <Sample SampleType>
void Analyzer<SampleType>::Grain::storeNewGrain (const SampleType* const origSamples, int startIndex,
												 const Buffer& windowSamples, int numSamples) noexcept
{
	storeNewGrain (origSamples, startIndex, numSamples, nullptr, 0, windowSamples, numSamples, startIndex);
}

template <Sample SampleType>
void Analyzer<SampleType>::Grain::storeNewGrain (const SampleType* const origSamples1, int startIndex1, int blocksize1,
												 const SampleType* const origSamples2, int blocksize2,
												 const Buffer& windowSamples, int totalNumSamples,
												 int grainStartIdx) noexcept
{
	LIMES_ASSERT (getRefCount() == 0);
	LIMES_ASSERT (totalNumSamples == blocksize1 + blocksize2);
	LIMES_ASSERT (samples.getNumSamples() >= totalNumSamples);
	LIMES_ASSERT (windowSamples.getNumSamples() == totalNumSamples);
	LIMES_ASSERT (blocksize1 > 0);
	LIMES_ASSERT (startIndex1 >= 0);

	origStartIndex = static_cast<std::size_t> (grainStartIdx);
	grainSize	   = static_cast<std::size_t> (totalNumSamples);

	samples.clear();

	auto* const destSamples = samples.getWritePointer (0);

	vecops::copy (destSamples, origSamples1 + startIndex1, blocksize1);

	if (blocksize2 > 0)
		vecops::copy (destSamples + startIndex1, origSamples2, blocksize2);

	vecops::multiply (destSamples, totalNumSamples, windowSamples.getReadPointer (0));
}

template <Sample SampleType>
void Analyzer<SampleType>::Grain::storeNewGrainWithZeroesAtStart (int					  numZeroes,
																  const SampleType* const origSamples, int numSamples,
																  const Buffer& windowSamples, int totalNumSamples, int grainStartIdx) noexcept
{
	LIMES_ASSERT (getRefCount() == 0);
	LIMES_ASSERT (numZeroes > 0 && numSamples > 0);
	LIMES_ASSERT (totalNumSamples == numZeroes + numSamples);
	LIMES_ASSERT (samples.getNumSamples() >= totalNumSamples);
	LIMES_ASSERT (windowSamples.getNumSamples() == numSamples);

	origStartIndex = static_cast<std::size_t> (grainStartIdx);
	grainSize	   = static_cast<std::size_t> (totalNumSamples);

	samples.clear();

	auto* const destSamples = samples.getWritePointer (0,
													   static_cast<std::size_t> (numZeroes));

	vecops::copy (destSamples, origSamples, numSamples);

	vecops::multiply (destSamples, numSamples, windowSamples.getReadPointer (0));
}

template <Sample SampleType>
void Analyzer<SampleType>::Grain::newBlockStarting (int last_blocksize) noexcept
{
	if (getRefCount() > 0)
	{
		origStartIndex -= static_cast<std::size_t> (last_blocksize);
	}
	else
	{
		grainSize	   = 0;
		origStartIndex = 0;
	}
}

template <Sample SampleType>
int Analyzer<SampleType>::Grain::getSize() const noexcept
{
	return static_cast<int> (grainSize);
}

template <Sample SampleType>
int Analyzer<SampleType>::Grain::getOrigStart() const noexcept
{
	return static_cast<int> (origStartIndex);
}

template <Sample SampleType>
void Analyzer<SampleType>::Grain::reserveSize (int numSamples)
{
	samples.resize (numSamples);
}

template <Sample SampleType>
void Analyzer<SampleType>::Grain::clearGrain()
{
	samples.clear();
	origStartIndex = 0;
	grainSize	   = 0;
}

template class Analyzer<float>;
template class Analyzer<double>;

}  // namespace dsp::psola

LIMES_END_NAMESPACE
