/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

#include "oscillator.h"
#include <limes_core.h>

namespace lemons::dsp
{

template <typename SampleType>
void Oscillator<SampleType>::Phase::resetPhase() noexcept
{
	phase = 0;
}

template <typename SampleType>
void Oscillator<SampleType>::Phase::setFrequency (SampleType frequency, SampleType sampleRate)
{
	LIMES_ASSERT (sampleRate > 0 && frequency > 0);
	increment = frequency / sampleRate;
}

template <typename SampleType>
SampleType Oscillator<SampleType>::Phase::next (SampleType wrapLimit) noexcept
{
	const auto p = phase;

	phase += increment;

	while (phase >= wrapLimit)
		phase -= wrapLimit;

	return p;
}

template <typename SampleType>
SampleType Oscillator<SampleType>::Phase::getIncrement() const noexcept
{
	return increment;
}

template struct Oscillator<float>;
template struct Oscillator<double>;

}  // namespace lemons::dsp
