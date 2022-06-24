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

#include "./zeroCrossing.h"

LIMES_BEGIN_NAMESPACE

namespace dsp::pitch
{

template <Sample SampleType>
typename ZeroCrossings<SampleType>::Result ZeroCrossings<SampleType>::detectPeriod (const SampleType* const inputAudio, int numSamples) noexcept
{
	int numZeroCrossings = 0;

	// find number of zero crossings in the signal
	{
		bool positive = inputAudio[0] > SampleType (0);

		for (auto i = 1; i < numSamples; ++i)
		{
			if (inputAudio[i] == SampleType (0))
				continue;

			const auto nowPositive = inputAudio[i] > SampleType (0);

			if (nowPositive != positive)
			{
				positive = nowPositive;
				++numZeroCrossings;
			}
		}
	}

	if (numZeroCrossings == 0)
	{
		data.reset();
		return data;
	}

	data.freqHz = (static_cast<SampleType> (numZeroCrossings) / SampleType (2))
				/ (static_cast<SampleType> (numSamples) / static_cast<SampleType> (samplerate));

	// TODO: data.confidence

	return data;
}

template <Sample SampleType>
typename ZeroCrossings<SampleType>::Result ZeroCrossings<SampleType>::getLastDetectedPeriod() const noexcept
{
	return data;
}

template <Sample SampleType>
int ZeroCrossings<SampleType>::getLatencySamples() const noexcept
{
	return 0;
}

template <Sample SampleType>
int ZeroCrossings<SampleType>::setSamplerate (double newSamplerate)
{
	samplerate = newSamplerate;

	return 0;
}

template <Sample SampleType>
void ZeroCrossings<SampleType>::reset() noexcept
{
	data.reset();
}

template <Sample SampleType>
void ZeroCrossings<SampleType>::releaseResources()
{
	data.reset();
}

template class ZeroCrossings<float>;
template class ZeroCrossings<double>;

}  // namespace dsp::pitch

LIMES_END_NAMESPACE
