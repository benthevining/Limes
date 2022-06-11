
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
#include <algorithm>				// for min, max
#include <limes_platform.h>			// for LIMES_ASSERT
#include <limes_core.h>				// for periodInSamples, freqFromPeriod, round
#include <limes_data_structures.h>	// for vector, basic_vector
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace dsp::pitch
{

// since we're calculating minima, we need an arbitrarily large number
// to represent any value we know isn't a valid period candidate
template <Sample SampleType>
static constinit const auto INVALID_TAU = SampleType (1000);

template <Sample SampleType>
LIMES_FORCE_INLINE void calculate_cmndf (const SampleType* inputAudio, int numSamples,
										 int minPeriod, int maxPeriod, SampleType* sdfOut, SampleType* cmndfOut,
										 SampleType confidenceThresh) noexcept
{
	// don't want to skip testing too many tau values in a row!
	const auto maxTausToSkipTesting = std::min (numSamples / 4, 20);

	SampleType runningSum = 0;

	// tau is the period candidate we're currently testing
	for (auto tau = minPeriod; tau <= maxPeriod; ++tau)
	{
		const auto yinIdx = tau - minPeriod;  // offset by minPeriod
											  // that's the lowest tau value we're testing
											  // so put it at index 0 in the buffer

		// called once SDF is calculated for this tau value
		// performs the cumulative mean normalized difference function
		// and writes the result to cmndfOut
		auto finalize_value = [&runningSum, yinIdx, tau, sdfOut, cmndfOut]
		{
			const auto sdfResult = sdfOut[yinIdx];

			runningSum += sdfResult;

			if (runningSum == SampleType (0))  // possible with silent input
				cmndfOut[yinIdx] = sdfResult;
			else
				cmndfOut[yinIdx] = sdfResult * (static_cast<SampleType> (tau) / runningSum);
		};

		// check if we're in an area of local maxima
		// if none of the previous tau values have been good period candidates, maybe this one can be skipped
		if (yinIdx >= 10)  // always do the first 10 samples of the window
		{
			// find the number of consecutive maxima values found right before this one
			const auto localMaximaLength = [yinIdx, confidenceThresh, maxTausToSkipTesting, cmndfOut]() -> int
			{
				for (auto i = yinIdx - 1, len = 0;
					 i >= 0 && len <= maxTausToSkipTesting;
					 --i, ++len)
				{
					// this one is NOT a maxima!
					if (cmndfOut[i] < confidenceThresh)
						return len;
				}

				return len;
			}();

			// the min number of consecutive local maxima that must have been found before this tau
			// in order to trigger skipping calculation of the SDF for this tau
			constinit const auto minMaximaAreaLen = 5;

			if (localMaximaLength >= minMaximaAreaLen		  // check if we're in an area of lots of consecutive maxima
				&& localMaximaLength < maxTausToSkipTesting)  // BUT don't want to skip testing too many tau values in a row!
			{
				sdfOut[yinIdx] = INVALID_TAU<SampleType>;

				finalize_value();

				continue;
			}
		}

		// autocorrelation via SDF function
		for (auto i = 0; i + tau < numSamples; ++i)
		{
			const auto delta = inputAudio[i] - inputAudio[i + tau];

			sdfOut[yinIdx] += (delta * delta);
		}

		finalize_value();
	}
}

/*----------------------------------------------------------------------------------------------------------------------------------------------*/

template <Sample SampleType>
Yin<SampleType>::Yin (int minFreqHz, float confidenceThreshold)
	: minHz (minFreqHz), confidenceThresh (static_cast<SampleType> (confidenceThreshold))
{
	LIMES_ASSERT (minHz > 0);
}

template <Sample SampleType>
typename Yin<SampleType>::Result Yin<SampleType>::detectPeriod (const SampleType* const inputAudio, int numSamples) noexcept
{
	// TODO
	// pre-process -- apply a hanning window to the incoming signal?
	// or to the calculated YIN values?
	// filter the input signal at the min & max frequencies?

	LIMES_ASSERT (samplerate > 0.);					   // pitch detector hasn't been prepared before calling this function!
	LIMES_ASSERT (numSamples >= getLatencySamples());  // not enough samples in this frame to do analysis

	// update the range of period candidates we're going to test for this frame
	updatePeriodBounds();

	LIMES_ASSERT (maxPeriod <= math::round (numSamples * 0.5f));
	LIMES_ASSERT (yinDataStorage.capacity() >= (maxPeriod - minPeriod + 1));
	LIMES_ASSERT (sdfOut.capacity() >= (maxPeriod - minPeriod + 1));

	yinDataStorage.zero();
	finalTaus.zero();

	// calculate autocorrelation using SDF function
	// SDF results are written to sdfOut
	// CMNDF results are written to yinDataStorage
	calculate_cmndf (inputAudio, numSamples,
					 minPeriod, maxPeriod,
					 sdfOut.data(),
					 yinDataStorage.data(),
					 confidenceThresh);

	const auto numFinalTaus = std::min (finalTaus.numObjects(),
										(maxPeriod - minPeriod) / 2);

	// post-process the set of tested taus
	// keep only the taus with the minima in the CMNDF output
	// TODO - weight based on distance to previously chosen period?
	{
		bool any_final_taus = false;

		auto min_tau_value = INVALID_TAU;

		for (auto i = 0; i < numFinalTaus; ++i)
		{
			auto added = [this]
			{
				for (auto tau = minPeriod; tau <= maxPeriod; ++tau)
				{
					const auto yinIdx = tau - minPeriod;

					if (finalTaus.contains (yinIdx))
						continue;

					const auto this_tau_value = yinDataStorage[yinIdx];

					if (this_tau_value < min_tau_value)
					{
						min_tau_value = this_tau_value;
						finalTaus[i]  = yinIdx;
						return true;
					}
				}

				return false;
			}();

			if (added)
				any_final_taus = true;
			else
				break;
		}

		if (! any_final_taus)
		{
			data.reset();
			return data;
		}
	}

	const auto maxYinIdx = maxPeriod - minPeriod;

	// absolute threshold
	// examining only our final tau candidates
	const auto periodEstimate = [this]
	{
		for (auto i = 0; i < numFinalTaus; ++i)
		{
			const auto tau = finalTaus[i];

			if (tau == 0)
				continue;

			if (yinDataStorage[tau] >= confidenceThresh)
				continue;

			auto t = tau;

			// find the local minimum in the SDF function
			for (;
				 t + 1 <= maxYinIdx && sdfOut[t + 1] < sdfOut[t];
				 ++t)
				;

			return t;
		}

		return 0;
	}();

	// NB. at this point, periodEstimate is still offset by minPeriod

	LIMES_ASSERT (periodEstimate >= 0 && periodEstimate <= maxYinIdx);

	if (periodEstimate == 0)
	{
		data.reset();
		return data;
	}

	data.setPeriod (parabolicInterpolation (periodEstimate) + static_cast<SampleType> (minPeriod),	// add minPeriod bc of the initial offset
					samplerate);

	data.confidence = yinDataStorage[periodEstimate];

	return data;
}

template <Sample SampleType>
typename Yin<SampleType>::Result Yin<SampleType>::getLastDetectedPeriod() const noexcept
{
	return data;
}

template <Sample SampleType>
void Yin<SampleType>::updatePeriodBounds() noexcept
{
	LIMES_ASSERT (minHz > 0);

	const auto periodUpperBound = math::periodInSamples (samplerate, minHz);
	const auto periodLowerBound = math::max (math::periodInSamples (samplerate, 10000), 4);

	if (data.isPitched())
	{
		// Pitch should not halve or double between consecutive pitched frames
		maxPeriod = math::min (periodUpperBound, math::periodInSamples (samplerate, data.freqHz * SampleType (0.5)));
		minPeriod = math::max (periodLowerBound, math::periodInSamples (samplerate, data.freqHz * SampleType (2)));
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
SampleType Yin<SampleType>::parabolicInterpolation (int periodEstimate) const noexcept
{
	LIMES_ASSERT (periodEstimate > 0);

	const auto x0 = periodEstimate == 0 ? 0 : periodEstimate - 1;

	const auto x2 = [periodEstimate, max = maxPeriod]
	{
		const auto plusOne = periodEstimate + 1;

		if (plusOne < max)
			return plusOne;

		return periodEstimate;
	}();

	if (x0 == periodEstimate)
	{
		if (sdfOut[periodEstimate] > sdfOut[x2])
			return static_cast<SampleType> (x2);

		return static_cast<SampleType> (periodEstimate);
	}

	if (x2 == periodEstimate)
	{
		if (sdfOut[periodEstimate] > sdfOut[x0])
			return static_cast<SampleType> (x0);

		return static_cast<SampleType> (periodEstimate);
	}

	const auto s0 = yinDataStorage[x0];
	const auto s2 = yinDataStorage[x2];

	const auto period = static_cast<SampleType> (periodEstimate)
					  + (s2 - s0)
							/ (SampleType (2)
							   * (SampleType (2) * yinDataStorage[periodEstimate] - s2 - s0));

	return static_cast<SampleType> (period);
}

template <Sample SampleType>
void Yin<SampleType>::setConfidenceThresh (float newThresh) noexcept
{
	LIMES_ASSERT (newThresh >= 0.f && newThresh <= 1.f);
	confidenceThresh = static_cast<SampleType> (newThresh);
}

template <Sample SampleType>
int Yin<SampleType>::setSamplerate (double newSamplerate)
{
	LIMES_ASSERT (newSamplerate > 0.);

	samplerate = newSamplerate;

	const auto latency = getLatencySamples();

	const auto bufferLen = (latency / 2) + 1;

	yinDataStorage.reserveAndZero (bufferLen);
	sdfOut.reserveAndZero (bufferLen);

	finalTaus.reserveAndZero (std::max (latency / 4, 20));

	return latency;
}

template <Sample SampleType>
int Yin<SampleType>::setMinHz (int newMinHz)
{
	LIMES_ASSERT (newMinHz > 0);

	minHz = newMinHz;

	if (samplerate > 0.)
		return setSamplerate (samplerate);

	return 512;
}

template <Sample SampleType>
int Yin<SampleType>::getMinHz() const noexcept
{
	return minHz;
}

template <Sample SampleType>
int Yin<SampleType>::getLatencySamples() const noexcept
{
	if (samplerate == 0)
		return 512;

	return math::periodInSamples (samplerate, minHz) * 2;
}

template <Sample SampleType>
void Yin<SampleType>::reset() noexcept
{
	data.reset();
	yinDataStorage.zero();
	sdfOut.zero();
	finalTaus.zero();
	minPeriod = 0;
	maxPeriod = 0;
}

template <Sample SampleType>
void Yin<SampleType>::releaseResources()
{
	reset();

	samplerate = 0.;

	yinDataStorage.clearAndFree();
	sdfOut.clearAndFree();
	finalTaus.clearAndFree();
}

template <Sample SampleType>
void Yin<SampleType>::getCurrentLegalPeriodRange (int& min, int& max) const noexcept
{
	// Did you call one of the pitch detection functions first?
	LIMES_ASSERT (maxPeriod > minPeriod);

	min = minPeriod;
	max = maxPeriod;
}

template class Yin<float>;
template class Yin<double>;

}  // namespace dsp::pitch

LIMES_END_NAMESPACE
