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

#include "PeakFinder.h"
#include <algorithm>				// for min, max
#include <cmath>					// for abs
#include <limits>					// for numeric_limits
#include <limes_vecops.h>			// for min, range
#include <limes_platform.h>			// for LIMES_ASSERT
#include <limes_core.h>				// for round, numberIsEven
#include <limes_data_structures.h>	// for vector, basic_vector, scalar_vector
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE


namespace dsp::psola
{

template <Sample SampleType>
void PeakFinder<SampleType>::prepare (int maxBlocksize)
{
	LIMES_ASSERT (maxBlocksize > 0);

	peakIndices.reserveAndZero (maxBlocksize);
	peakSearchingOrder.reserveAndZero (maxBlocksize);
	peakCandidates.reserveAndZero (numPeaksToTest);
	candidateDeltas.reserveAndZero (numPeaksToTest);
	finalHandful.reserveAndZero (defaultFinalHandfulSize);
	finalHandfulDeltas.reserveAndZero (defaultFinalHandfulSize);

	for (auto* array : arrays)
		array->zero();
}

template <Sample SampleType>
void PeakFinder<SampleType>::reset()
{
	for (auto* array : arrays)
		array->zero();

	analysisFrameStart = 0;
	lastPeak		   = 0;
	peakBeforeLast	   = 0;
}

template <Sample SampleType>
void PeakFinder<SampleType>::releaseResources()
{
	for (auto* array : arrays)
		array->clearAndFree();

	analysisFrameStart = 0;
}

template <Sample SampleType>
const ds::scalar_vector<int>& PeakFinder<SampleType>::findPeaks (const SampleType* const inputSamples, int numSamples, float period) noexcept
{
	LIMES_ASSERT (period > 0.f && numSamples > 0);

	peakIndices.clear();

	const auto grainSize  = math::round (period * 2.f);
	const auto halfPeriod = math::round (period * 0.5f);
	const auto intPeriod  = math::round (period);

	LIMES_ASSERT (numSamples >= grainSize);

	// marks the center of the analysis windows, which are 1 period long
	auto analysisIndex = [lastPeak, peakBeforeLast, intPeriod, halfPeriod]
	{
		if (peakBeforeLast < 0)
			return peakBeforeLast + (2 * intPeriod);

		if (lastPeak < 0)
			return lastPeak + intPeriod;

		return analysisFrameStart + halfPeriod;
	}();

	int lastFrameRealEnd { 0 };

	do
	{
		const auto frameStart = analysisIndex - halfPeriod;

		lastFrameRealEnd = analysisIndex + halfPeriod;

		const auto frameEnd = std::min (numSamples, lastFrameRealEnd);

		LIMES_ASSERT (frameStart >= 0 && frameEnd <= numSamples && frameEnd > frameStart);

		peakIndices.push_back (findNextPeak (frameStart, frameEnd,
											 std::min (analysisIndex, frameEnd),
											 inputSamples, intPeriod, grainSize));

		const auto prevAnalysisIndex = analysisIndex;

		analysisIndex = [this, intPeriod, grainSize]
		{
			const auto numPeaksFound = peakIndices.numObjects();

			if (numPeaksFound == 1)
				return peakIndices[0] + intPeriod;

			return peakIndices[numPeaksFound - 2] + grainSize;
		}();

		if (analysisIndex == prevAnalysisIndex)
			break;

		LIMES_ASSERT (analysisIndex > prevAnalysisIndex);
	} while (analysisIndex - halfPeriod < numSamples);

	peakIndices.removeDuplicates();
	peakIndices.sort();

	const auto num = peakIndices.numObjects();

	LIMES_ASSERT (num > 0);

	lastPeak = peakIndices[num - 1] - numSamples;

	if (num > 1)
		peakBeforeLast = peakIndices[num - 2] - numSamples;
	else
		peakBeforeLast = 0;

	analysisFrameStart = std::max (0, lastFrameRealEnd - numSamples);

	return peakIndices;
}

template <Sample SampleType>
int PeakFinder<SampleType>::findNextPeak (int frameStart, int frameEnd, int predictedPeak,
										  const SampleType* const inputSamples, int period, int grainSize) noexcept
{
	LIMES_ASSERT (predictedPeak >= frameStart && predictedPeak <= frameEnd);

	sortSampleIndicesForPeakSearching (frameStart, frameEnd, predictedPeak);

	peakCandidates.clear();

	for (auto i = 0; i < numPeaksToTest; ++i)
	{
		const auto nextPeak = getPeakCandidateInRange (inputSamples, frameStart, frameEnd, predictedPeak);

		if (nextPeak == -1) break;

		peakCandidates.push_back (nextPeak);
	}

	peakCandidates.removeDuplicates();
	peakCandidates.sort();

	LIMES_ASSERT (peakCandidates.isNotEmpty());

	switch (peakCandidates.numObjects())
	{
		case 1 : return peakCandidates[0];

		case 2 : return choosePeakWithGreatestPower (inputSamples);

		default :
		{
			// TO DO
			// save the last frame's last peak
			if (peakIndices.numObjects() <= 1)
				return choosePeakWithGreatestPower (inputSamples);

			return chooseIdealPeakCandidate (inputSamples,
											 peakIndices.last() + period,
											 peakIndices[peakIndices.numObjects() - 2] + grainSize);
		}
	}
}


template <Sample SampleType>
int PeakFinder<SampleType>::getPeakCandidateInRange (const SampleType* const inputSamples,
													 int startSample, int endSample, int predictedPeak) const noexcept
{
	LIMES_ASSERT (peakSearchingOrder.isNotEmpty());

	const auto starting = [this]
	{
		for (const auto p : peakSearchingOrder)
			if (! peakCandidates.contains (p))
				return p;

		return -1;
	}();

	if (starting == -1) return -1;

	LIMES_ASSERT (starting >= startSample && starting <= endSample);

	const auto numSamples = endSample - startSample;

	auto get_weighted_sample = [predictedPeak, numSamples, inputSamples] (int index) -> SampleType
	{
		const auto distance = static_cast<SampleType> (std::abs (predictedPeak - index));
		const auto weight	= SampleType (1) - (distance / static_cast<SampleType> (numSamples));

		return std::abs (inputSamples[index]) * weight;	 // NOLINT
	};

	auto strongest	  = get_weighted_sample (starting);
	auto strongestIdx = starting;

	for (const auto sampleNum : peakSearchingOrder)
	{
		LIMES_ASSERT (sampleNum >= startSample && sampleNum <= endSample);

		if (sampleNum == starting || peakCandidates.contains (sampleNum)) continue;

		const auto currentSample = get_weighted_sample (sampleNum);

		if (currentSample > strongest)
		{
			strongest	 = currentSample;
			strongestIdx = sampleNum;
		}
	}

	return strongestIdx;
}

template <Sample SampleType>
int PeakFinder<SampleType>::choosePeakWithGreatestPower (const SampleType* const inputSamples) const noexcept
{
	auto strongestPeakIndex = peakCandidates[0];
	auto strongestPeak		= std::abs (inputSamples[strongestPeakIndex]);	// NOLINT

	for (const auto candidate : peakCandidates)
	{
		const auto current = std::abs (inputSamples[candidate]);  // NOLINT

		if (current > strongestPeak)
		{
			strongestPeak	   = current;
			strongestPeakIndex = candidate;
		}
	}

	return strongestPeakIndex;
}


template <Sample SampleType>
int PeakFinder<SampleType>::chooseIdealPeakCandidate (const SampleType* const inputSamples, int deltaTarget1, int deltaTarget2) noexcept
{
	finalHandful.clear();
	finalHandfulDeltas.clear();

	// 1. calculate delta values for each peak candidate
	// delta represents how far off this peak candidate is from the expected peak location
	// in a way it's a measure of the jitter that picking a peak candidate as this frame's peak would introduce to the overall alignment of the stream of grains based on the previous grains

	candidateDeltas.clearAndZero (peakCandidates.numObjects());

	candidateDeltas.transform (peakCandidates, [deltaTarget1, deltaTarget2] (const auto candidate)
							   { return (std::abs (candidate - deltaTarget1) + std::abs (candidate - deltaTarget2)); });

	// 2. whittle our remaining candidates down to the final candidates with the minimum delta values

	const auto finalHandfulSize = std::min (defaultFinalHandfulSize, candidateDeltas.numObjects());

	for (auto i = 0; i < finalHandfulSize; ++i)
	{
		int index, deltaValue;	// NOLINT

		vecops::min (candidateDeltas.data(), candidateDeltas.numObjects(), deltaValue, index);

		finalHandfulDeltas.push_back (deltaValue);
		finalHandful.push_back (peakCandidates[index]);

		// make sure this value won't be chosen again, w/o deleting it from the candidateDeltas array
		candidateDeltas[index] = std::numeric_limits<int>::max();
	}

	LIMES_ASSERT (finalHandful.numObjects() == finalHandfulSize && finalHandfulDeltas.numObjects() == finalHandfulSize);

	// 3. choose the strongest overall peak from these final candidates, with peaks weighted by their delta values

	const auto deltaRange = vecops::range (finalHandfulDeltas.data(), finalHandfulDeltas.numObjects());

	if (deltaRange == 0)  // prevent dividing by 0 in the next step...
		return finalHandful[0];

	LIMES_ASSERT (deltaRange > 0);

	auto get_weighted_sample = [this, deltaRange, inputSamples] (int sampleIndex, int finalHandfulIdx) -> SampleType
	{
		const auto delta = static_cast<SampleType> (finalHandfulDeltas[finalHandfulIdx]);

		const auto deltaWeight = SampleType (1) - (delta / static_cast<SampleType> (deltaRange));

		return std::abs (inputSamples[sampleIndex]) * deltaWeight;	// NOLINT
	};

	auto chosenPeak	   = finalHandful[0];
	auto strongestPeak = get_weighted_sample (chosenPeak, 0);

	for (auto i = 1; i < finalHandfulSize; ++i)
	{
		const auto candidate = finalHandful[i];

		if (candidate == chosenPeak) continue;

		const auto testingPeak = get_weighted_sample (candidate, i);

		if (testingPeak > strongestPeak)
		{
			strongestPeak = testingPeak;
			chosenPeak	  = candidate;
		}
	}

	return chosenPeak;
}

template <Sample SampleType>
void PeakFinder<SampleType>::sortSampleIndicesForPeakSearching (int startSample, int endSample, int predictedPeak) noexcept
{
	LIMES_ASSERT (predictedPeak >= startSample && predictedPeak <= endSample);
	LIMES_ASSERT (endSample > startSample);

	const auto searchingSize = endSample - startSample;

	LIMES_ASSERT (peakSearchingOrder.capacity() >= searchingSize);

	peakSearchingOrder.clearAndZero (searchingSize);

	peakSearchingOrder[0] = predictedPeak;

	for (auto p = 1, m = -1, n = 1; n < searchingSize; ++n)
	{
		const auto pos = predictedPeak + p;
		const auto neg = predictedPeak + m;

		if (math::numberIsEven (n))
		{
			if (neg >= startSample)
			{
				peakSearchingOrder[n] = neg;
				--m;
			}
			else
			{
				LIMES_ASSERT (pos <= endSample);
				peakSearchingOrder[n] = pos;
				++p;
			}
		}
		else
		{
			if (pos <= endSample)
			{
				peakSearchingOrder[n] = pos;
				++p;
			}
			else
			{
				LIMES_ASSERT (neg >= startSample);
				peakSearchingOrder[n] = neg;
				--m;
			}
		}
	}

	LIMES_ASSERT (peakSearchingOrder.numObjects() == searchingSize);
}

template class PeakFinder<float>;
template class PeakFinder<double>;

}  // namespace dsp::psola

LIMES_END_NAMESPACE
