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
#include <algorithm>		 // for min, max
#include <cmath>			 // for abs
#include <limits>			 // for numeric_limits
#include <limes_vecops.h>	 // for min, range
#include <limes_platform.h>	 // for LIMES_ASSERT
#include <limes_core.h>		 // for round, numberIsEven
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE


namespace dsp::psola
{

static constinit const auto numPeaksToTest = 15;

template <Sample SampleType>
void PeakFinder<SampleType>::prepare (int maxBlocksize)
{
	LIMES_ASSERT (maxBlocksize > 0);

	peakIndices.reserve (static_cast<std::vector<int>::size_type>(maxBlocksize));
	peakCandidates.reserve (numPeaksToTest);
	candidateDeltas.reserve (numPeaksToTest);

	for (auto* array : arrays)
		alg::fill (*array, 0);
}

template <Sample SampleType>
void PeakFinder<SampleType>::reset()
{
	for (auto* array : arrays)
		alg::fill (*array, 0);

	analysisFrameStart = 0;
	lastPeak		   = 0;
	peakBeforeLast	   = 0;
}

template <Sample SampleType>
void PeakFinder<SampleType>::releaseResources()
{
	for (auto* array : arrays)
	{
		array->clear();
		array->shrink_to_fit();
	}

	analysisFrameStart = 0;
}

template <Sample SampleType>
const std::vector<int>& PeakFinder<SampleType>::findPeaks (const SampleType* const inputSamples, int numSamples, SampleType period) noexcept
{
	LIMES_ASSERT (period > 0.f && numSamples > 0);

	peakIndices.clear();

	const auto grainSize  = math::round (period * 2.f);
	const auto halfPeriod = math::round (period * 0.5f);
	const auto intPeriod  = math::round (period);

	LIMES_ASSERT (numSamples >= grainSize);

	// marks the center of the analysis windows, which are 1 period long
	auto analysisIndex = [this, intPeriod, halfPeriod]
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
			const auto numPeaksFound = peakIndices.size();

			if (numPeaksFound == 1)
				return peakIndices[0] + intPeriod;

			return peakIndices[numPeaksFound - 2] + grainSize;
		}();

		if (analysisIndex == prevAnalysisIndex)
			break;

		LIMES_ASSERT (analysisIndex > prevAnalysisIndex);
	} while (analysisIndex - halfPeriod < numSamples);

	peakIndices.erase (std::unique (std::begin (peakIndices), std::end (peakIndices)), std::end (peakIndices));

	alg::sort (peakIndices);

	const auto num = peakIndices.size();

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

	peakCandidates.clear();

	for (auto i = 0; i < numPeaksToTest; ++i)
	{
		const auto nextPeak = getPeakCandidateInRange (inputSamples, frameStart, frameEnd, predictedPeak);

		if (nextPeak == -1) break;

		peakCandidates.push_back (nextPeak);
	}

	peakCandidates.erase (std::unique (std::begin (peakCandidates), std::end (peakCandidates)), std::end (peakCandidates));

	alg::sort (peakCandidates);

	LIMES_ASSERT (peakCandidates.size() > 0);

	switch (peakCandidates.size())
	{
		case 1 : return peakCandidates[0];

		case 2 : return choosePeakWithGreatestPower (inputSamples);

		default :
		{
			if (peakIndices.size() >= 2)
				return chooseIdealPeakCandidate (inputSamples,
												 peakIndices.back() + period,
												 peakIndices[peakIndices.size() - 2] + grainSize);

			if (peakBeforeLast < 0 && lastPeak < 0)
			{
				const auto deltaTarget1 = period + lastPeak;
				const auto deltaTarget2 = grainSize + peakBeforeLast;

				if (deltaTarget1 > 0 && deltaTarget2 > 0)
					return chooseIdealPeakCandidate (inputSamples, deltaTarget1, deltaTarget2);
			}

			return choosePeakWithGreatestPower (inputSamples);
		}
	}
}


template <Sample SampleType>
int PeakFinder<SampleType>::getPeakCandidateInRange (const SampleType* const inputSamples,
													 int startSample, int endSample, int predictedPeak) const noexcept
{
	const auto starting = [startSample, endSample, this]
	{
		for (auto i = startSample; i < endSample; ++i)
			if (! alg::contains (peakCandidates, i))
				return i;

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

	for (auto i = starting + 1; i < endSample; ++i)
	{
		LIMES_ASSERT (! alg::contains (peakCandidates, i));

		const auto currentSample = get_weighted_sample (i);

		if (currentSample > strongest)
		{
			strongest	 = currentSample;
			strongestIdx = i;
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
	LIMES_ASSERT (deltaTarget1 >= 0);
	LIMES_ASSERT (deltaTarget2 >= 0);

	const auto numCandidates = peakCandidates.size();

	// calculate delta values for each peak candidate
	// delta represents how far off this peak candidate is from the expected peak location
	// in a way it's a measure of the jitter that picking a peak candidate as this frame's peak would introduce to the overall alignment of the stream of grains based on the previous grains

	candidateDeltas.clear();

	alg::transform (peakCandidates, candidateDeltas, [deltaTarget1, deltaTarget2] (const auto candidate)
					{ return (std::abs (candidate - deltaTarget1) + std::abs (candidate - deltaTarget2)); });

	LIMES_ASSERT (candidateDeltas.size() == numCandidates);

	const auto deltaRange = vecops::range (candidateDeltas.data(), candidateDeltas.size());

	if (deltaRange == 0)  // prevent dividing by 0 in the next step...
		return peakCandidates[0];

	LIMES_ASSERT (deltaRange > 0);

	auto get_weighted_sample = [this, deltaRange, inputSamples] (auto sampleIndex, auto candidateIdx) -> SampleType
	{
		const auto delta = static_cast<SampleType> (candidateDeltas[static_cast<std::vector<int>::size_type>(candidateIdx)]);

		const auto deltaWeight = SampleType (1) - (delta / static_cast<SampleType> (deltaRange));

		return std::abs (inputSamples[sampleIndex]) * deltaWeight;	// NOLINT
	};

	auto chosenPeak	   = peakCandidates[0];
	auto strongestPeak = get_weighted_sample (chosenPeak, 0);

	for (auto i = static_cast<decltype(numCandidates)>(1); i < numCandidates; ++i)
	{
		const auto candidate = peakCandidates[i];

		const auto testingPeak = get_weighted_sample (candidate, i);

		if (testingPeak > strongestPeak)
		{
			strongestPeak = testingPeak;
			chosenPeak	  = candidate;
		}
	}

	return chosenPeak;
}

template class PeakFinder<float>;
template class PeakFinder<double>;

}  // namespace dsp::psola

LIMES_END_NAMESPACE
