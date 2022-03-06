
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

namespace lemons::dsp::psola
{

template <Sample SampleType>
PitchDetector<SampleType>::PitchDetector (int minFreqHz, float confidenceThreshold)
	: minHz (minFreqHz), confidenceThresh (static_cast<SampleType> (confidenceThreshold))
{
	LIMES_ASSERT (minHz > 0);
}

template <Sample SampleType>
float PitchDetector<SampleType>::detectPitch (const SampleType* const inputAudio, int numSamples)
{
	const auto period = detectPeriod (inputAudio, numSamples);

	if (period > 0.f)
		return math::freqFromPeriod (samplerate, period);

	return 0.f;
}

template <Sample SampleType>
float PitchDetector<SampleType>::detectPeriod (const SampleType* const inputAudio, int numSamples)
{
	LIMES_ASSERT (samplerate > 0.);					   // pitch detector hasn't been prepared before calling this function!
	LIMES_ASSERT (numSamples >= getLatencySamples());  // not enough samples in this frame to do analysis

	updatePeriodBounds();

	LIMES_ASSERT (maxPeriod <= math::round (numSamples * 0.5f));
	LIMES_ASSERT (yinDataStorage.capacity() >= (maxPeriod - minPeriod + 1));

	yinDataStorage.zero();

	{
		SampleType runningSum = 0;

		for (auto tau = minPeriod; tau <= maxPeriod; ++tau)
		{
			const auto yinIdx = tau - minPeriod;

			// difference function
			for (auto i = 0; i + tau < numSamples; ++i)
			{
				const auto delta = inputAudio[i] - inputAudio[i + tau];
				yinDataStorage[yinIdx] += (delta * delta);
			}

			// cumulative mean normalized difference

			runningSum += yinDataStorage[yinIdx];

			if (runningSum == SampleType (0))  // possible with silent input
				continue;

			LIMES_ASSERT (runningSum > SampleType (0));

			yinDataStorage[yinIdx] *= (static_cast<SampleType> (tau) / runningSum);
		}
	}

	// NB. at this point, periodEstimate is still offset by minPeriod
	const auto periodEstimate = absoluteThreshold();

	LIMES_ASSERT (periodEstimate >= 0 && periodEstimate <= (maxPeriod - minPeriod));

	if (periodEstimate > 0)
	{
		periodLastFrame = parabolicInterpolation (periodEstimate) + static_cast<float> (minPeriod);

		return periodLastFrame;
	}

	periodLastFrame = 0.f;
	return 0.f;
}

template <Sample SampleType>
void PitchDetector<SampleType>::updatePeriodBounds()
{
	LIMES_ASSERT (minHz > 0);

	const auto periodUpperBound = math::periodInSamples (samplerate, minHz);
	const auto periodLowerBound = math::periodInSamples (samplerate, 10000);

	if (periodLastFrame > 0.f)	// Pitch should not halve or double between consecutive pitched frames
	{
		const auto freqLastFrame = math::freqFromPeriod (samplerate, periodLastFrame);

		maxPeriod = std::min (periodUpperBound, math::periodInSamples (samplerate, freqLastFrame * 0.5f));
		minPeriod = std::max (periodLowerBound, math::periodInSamples (samplerate, freqLastFrame * 2.f));
	}
	else
	{
		maxPeriod = periodUpperBound;
		minPeriod = periodLowerBound;
	}

	LIMES_ASSERT (maxPeriod > minPeriod);
	LIMES_ASSERT (minPeriod > 0);
}

template <Sample SampleType>
int PitchDetector<SampleType>::absoluteThreshold() const
{
	const auto maxYinIdx = maxPeriod - minPeriod;

	const auto tau = [this, maxYinIdx, thresh = confidenceThresh]
	{
		for (auto t = 0; t <= maxYinIdx; ++t)
		{
			if (yinDataStorage[t] < thresh)
			{
				while (t + 1 <= maxYinIdx && yinDataStorage[t + 1] < yinDataStorage[t])
				{
					++t;
				}

				return t;
			}
		}

		return maxYinIdx + 1;
	}();

	if (tau > maxYinIdx || yinDataStorage[tau] >= confidenceThresh)
		return 0;

	return tau;
}

template <Sample SampleType>
float PitchDetector<SampleType>::parabolicInterpolation (int periodEstimate) const
{
	LIMES_ASSERT (periodEstimate > 0);

	const auto x0 = [periodEstimate]
	{
		if (periodEstimate < 1)
			return periodEstimate;

		return periodEstimate - 1;
	}();

	const auto x2 = [periodEstimate, max = maxPeriod]
	{
		if (const auto plusOne = periodEstimate + 1;
			plusOne < max)
			return plusOne;

		return periodEstimate;
	}();

	if (x0 == periodEstimate)
	{
		if (yinDataStorage[periodEstimate] > yinDataStorage[x2])
			return static_cast<float> (x2);

		return static_cast<float> (periodEstimate);
	}

	if (x2 == periodEstimate)
	{
		if (yinDataStorage[periodEstimate] > yinDataStorage[x0])
			return static_cast<float> (x0);

		return static_cast<float> (periodEstimate);
	}

	const auto s0 = yinDataStorage[x0];
	const auto s2 = yinDataStorage[x2];

	const auto period = static_cast<SampleType> (periodEstimate) + (s2 - s0) / (SampleType (2) * (SampleType (2) * yinDataStorage[periodEstimate] - s2 - s0));

	return static_cast<float> (period);
}

template <Sample SampleType>
void PitchDetector<SampleType>::setConfidenceThresh (float newThresh) noexcept
{
	LIMES_ASSERT (newThresh >= 0.f && newThresh <= 1.f);
	confidenceThresh = static_cast<SampleType> (newThresh);
}

template <Sample SampleType>
int PitchDetector<SampleType>::setSamplerate (double newSamplerate)
{
	LIMES_ASSERT (newSamplerate > 0.);

	samplerate = newSamplerate;

	const auto latency = getLatencySamples();

	yinDataStorage.reserveAndZero ((latency / 2) + 1);

	return latency;
}

template <Sample SampleType>
int PitchDetector<SampleType>::setMinHz (int newMinHz)
{
	LIMES_ASSERT (newMinHz > 0);

	minHz = newMinHz;

	if (samplerate > 0.)
		return setSamplerate (samplerate);

	return 512;
}

template <Sample SampleType>
int PitchDetector<SampleType>::getMinHz() const noexcept
{
	return minHz;
}

template <Sample SampleType>
int PitchDetector<SampleType>::getLatencySamples() const noexcept
{
	if (samplerate == 0)
		return 512;

	return math::periodInSamples (samplerate, minHz) * 2;
}

template <Sample SampleType>
void PitchDetector<SampleType>::reset()
{
	periodLastFrame = 0.f;
	minPeriod		= 0;
	maxPeriod		= 0;
}

template <Sample SampleType>
void PitchDetector<SampleType>::releaseResources()
{
	reset();

	samplerate = 0.;

	yinDataStorage.clearAndFree();
}

template <Sample SampleType>
void PitchDetector<SampleType>::getCurrentLegalPeriodRange (int& min, int& max) const
{
	// Did you call one of the pitch detection functions first?
	LIMES_ASSERT (maxPeriod > minPeriod);

	min = minPeriod;
	max = maxPeriod;
}

template class PitchDetector<float>;
template class PitchDetector<double>;

}  // namespace lemons::dsp::psola
