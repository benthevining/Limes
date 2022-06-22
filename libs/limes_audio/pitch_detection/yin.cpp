
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
#include <algorithm>		 // for min, max
#include <limes_platform.h>	 // for LIMES_ASSERT
#include <limes_core.h>		 // for periodInSamples, freqFromPeriod, round
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace dsp::pitch
{

namespace yin
{

// since we're calculating minima, we need an arbitrarily large number
// to represent any value we know isn't a valid period candidate
template <Sample SampleType>
static constinit const auto INVALID_TAU = SampleType (1000);


template <Sample SampleType>
static LIMES_FORCE_INLINE void calculate_cmndf (const SampleType* inputAudio, int numSamples,
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

				return 0;
			}();

			// the min number of consecutive local maxima that must have been found before this tau
			// in order to trigger skipping calculation of the SDF for this tau
			static constinit const auto minMaximaAreaLen = 5;

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


template <Sample SampleType>
static LIMES_FORCE_INLINE int select_final_taus (std::vector<int>& finalTaus,
												 const SampleType* cmndfData,
												 int minPeriod, int maxPeriod) noexcept
{
	alg::fill (finalTaus, 0);

	auto num_final_taus = 0;
	auto min_tau_value	= INVALID_TAU<SampleType>;

	const auto maxNumFinalTaus = std::min (static_cast<int> (finalTaus.size()), (maxPeriod - minPeriod) / 2);

	for (auto i = 0; i < maxNumFinalTaus; ++i)
	{
		const auto added = [&finalTaus, &min_tau_value, minPeriod, maxPeriod, cmndfData, i]
		{
			for (auto tau = minPeriod; tau <= maxPeriod; ++tau)
			{
				const auto yinIdx = tau - minPeriod;

				if (alg::contains (finalTaus, yinIdx))
					continue;

				const auto this_tau_value = cmndfData[yinIdx];

				if (this_tau_value < min_tau_value)
				{
					min_tau_value											= this_tau_value;
					finalTaus[static_cast<std::vector<int>::size_type> (i)] = yinIdx;
					return true;
				}
			}

			return false;
		}();

		if (added)
			++num_final_taus;
		else
			break;
	}

	return num_final_taus;
}


template <Sample SampleType>
static LIMES_FORCE_INLINE int pick_period_estimate (const int* finalTaus, int numFinalTaus,
													int				  maxYinIdx,
													SampleType		  confidenceThresh,
													const SampleType* cmndfData, const SampleType* sdfData) noexcept
{
	for (auto i = 0; i < numFinalTaus; ++i)
	{
		const auto tau = finalTaus[i];

		if (tau == 0)
			continue;

		if (cmndfData[tau] >= confidenceThresh)
			continue;

		auto t = tau;

		// find the local minimum in the SDF function
		for (;
			 t + 1 <= maxYinIdx && sdfData[t + 1] < sdfData[t];
			 ++t)
			;

		return t;
	}

	return 0;
}


template <Sample SampleType>
static LIMES_FORCE_INLINE SampleType parabolic_interpolation (int periodEstimate, int maxPeriod,
															  const SampleType* sdfData, const SampleType* cmndfData) noexcept
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
		if (sdfData[periodEstimate] > sdfData[x2])
			return static_cast<SampleType> (x2);

		return static_cast<SampleType> (periodEstimate);
	}

	if (x2 == periodEstimate)
	{
		if (sdfData[periodEstimate] > sdfData[x0])
			return static_cast<SampleType> (x0);

		return static_cast<SampleType> (periodEstimate);
	}

	const auto s0 = cmndfData[x0];
	const auto s2 = cmndfData[x2];

	const auto period = static_cast<SampleType> (periodEstimate)
					  + (s2 - s0)
							/ (SampleType (2)
							   * (SampleType (2) * cmndfData[periodEstimate] - s2 - s0));

	return static_cast<SampleType> (period);
}

}  // namespace yin


/*------------------------------------------------------------------------------------------------------------------------------------------*/


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
	// apply a hanning window to the incoming signal?
	// or to the calculated YIN values?

	LIMES_ASSERT (samplerate > 0.);					   // pitch detector hasn't been prepared before calling this function!
	LIMES_ASSERT (numSamples >= getLatencySamples());  // not enough samples in this frame to do analysis

	// update the range of period candidates we're going to test for this frame
	updatePeriodBounds();

	LIMES_ASSERT (maxPeriod <= math::round (numSamples * 0.5f));
	LIMES_ASSERT (cmndfData.getNumSamples() >= (maxPeriod - minPeriod + 1));
	LIMES_ASSERT (sdfData.getNumSamples() >= (maxPeriod - minPeriod + 1));

	// filter the incoming signal at the maximum & minimum detectable frequency for this frame
	{
		lowPass.coefs.makeLowPass (samplerate,
								   static_cast<SampleType> (math::freqFromPeriod (samplerate, minPeriod)));

		hiPass.coefs.makeHighPass (samplerate, static_cast<SampleType> (math::freqFromPeriod (samplerate, maxPeriod)));

		inputStorage.copyFrom (inputAudio,
							   static_cast<std::size_t> (numSamples),
							   0);

		lowPass.process (inputStorage);
		hiPass.process (inputStorage);
	}

	sdfData.clear();

	// calculate autocorrelation using SDF function
	// SDF results are written to sdfData
	// CMNDF results are written to cmndfData
	yin::calculate_cmndf (inputStorage.getReadPointer (0), numSamples,
						  minPeriod, maxPeriod,
						  sdfData.getWritePointer (0),
						  cmndfData.getWritePointer (0),
						  confidenceThresh);

	// post-process the set of tested taus
	// keep only the taus with the minima in the CMNDF output
	// TODO - weight based on distance to previously chosen period?
	const auto num_final_taus = yin::select_final_taus (finalTaus,
														cmndfData.getReadPointer (0),
														minPeriod, maxPeriod);

	if (num_final_taus == 0)
	{
		data.reset();
		return data;
	}

	const auto maxYinIdx = maxPeriod - minPeriod;

	// absolute threshold
	// examining only our final tau candidates
	const auto periodEstimate = yin::pick_period_estimate (finalTaus.data(), num_final_taus, maxYinIdx,
														   confidenceThresh, cmndfData.getReadPointer (0), sdfData.getReadPointer (0));

	// NB. at this point, periodEstimate is still offset by minPeriod

	LIMES_ASSERT (periodEstimate >= 0 && periodEstimate <= maxYinIdx);

	if (periodEstimate == 0)
	{
		data.reset();
		return data;
	}

	data.setPeriod (yin::parabolic_interpolation (periodEstimate, maxPeriod,
												  sdfData.getReadPointer (0), cmndfData.getReadPointer (0))
						+ static_cast<SampleType> (minPeriod),	// add minPeriod bc of the initial offset
					samplerate);

	// TO DO: need to invert this
	// so that lower vals = less confidence
	data.confidence = cmndfData.getSample (0,
										   static_cast<std::size_t> (periodEstimate));

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

	inputStorage.resize (latency);

	const auto bufferLen = (latency / 2) + 1;

	cmndfData.resize (bufferLen);
	sdfData.resize (bufferLen);

	finalTaus.reserve (static_cast<std::vector<int>::size_type> (std::max (latency / 4, 20)));

	lowPass.prepare();
	hiPass.prepare();

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

	cmndfData.clear();
	sdfData.clear();
	inputStorage.clear();

	alg::fill (finalTaus, 0);

	lowPass.reset();
	hiPass.reset();

	minPeriod = 0;
	maxPeriod = 0;
}

template <Sample SampleType>
void Yin<SampleType>::releaseResources()
{
	minPeriod  = 0;
	maxPeriod  = 0;
	samplerate = 0.;

	data.reset();

	cmndfData.deallocate();
	sdfData.deallocate();
	inputStorage.deallocate();

	finalTaus.clear();
	finalTaus.shrink_to_fit();
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
