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

#include "basic_oscillators.h"
#include <limes_core.h>
#include <limes_vecops.h>
#include <cmath>

namespace lemons::dsp::osc
{

template <typename SampleType>
[[nodiscard]] constexpr SampleType blep (SampleType phase, SampleType increment) noexcept
{
	LIMES_ASSERT (increment > 0);

	if (phase < increment)
	{
		const auto p = phase / increment;
		return (SampleType (2) - p) * p - SampleType (1);
	}

	if (phase > (SampleType (1) - increment))
	{
		const auto p = (phase - SampleType (1)) / increment;
		return (p + SampleType (2)) * p + SampleType (1);
	}

	return SampleType (0);
}

/*--------------------------------------------------------------------------------------------*/

template <typename SampleType>
Sine<SampleType>::Sine()
	: Oscillator<SampleType> ([this]
							  { return std::sin (phase.next (constants::two_pi<SampleType>)); })
{
	phase.resetPhase();
}

template <typename SampleType>
void Sine<SampleType>::resetPhase()
{
	phase.resetPhase();
}

template <typename SampleType>
void Sine<SampleType>::setFrequency (SampleType frequency, SampleType sampleRate)
{
	phase.setFrequency (constants::two_pi<SampleType> * frequency, sampleRate);

	freq = frequency;
}

template <typename SampleType>
SampleType Sine<SampleType>::getFrequency() const noexcept
{
	return freq;
}

template struct Sine<float>;
template struct Sine<double>;

/*--------------------------------------------------------------------------------------------*/

template <typename SampleType>
Saw<SampleType>::Saw()
	: Oscillator<SampleType> ([this]
							  {
	const auto p = phase.next (1);
	return SampleType (2) * p - SampleType (1) - blep (p, phase.getIncrement()); })
{
	phase.resetPhase();
}

template <typename SampleType>
void Saw<SampleType>::resetPhase()
{
	phase.resetPhase();
}

template <typename SampleType>
void Saw<SampleType>::setFrequency (SampleType frequency, SampleType sampleRate)
{
	phase.setFrequency (frequency, sampleRate);
	freq = frequency;
}

template <typename SampleType>
SampleType Saw<SampleType>::getFrequency() const noexcept
{
	return freq;
}

template struct Saw<float>;
template struct Saw<double>;

/*--------------------------------------------------------------------------------------------*/

template <typename SampleType>
Square<SampleType>::Square()
	: Oscillator<SampleType> ([this]
							  {
	const auto inc = phase.getIncrement();
	const auto p   = phase.next (1);

	const auto base = p < SampleType (0.5) ? SampleType (-1) : SampleType (1);

	return base - blep (p, inc) + blep (std::fmod (p + SampleType (0.5), SampleType (1)), inc); })
{
	phase.resetPhase();
}

template <typename SampleType>
void Square<SampleType>::resetPhase()
{
	phase.resetPhase();
}

template <typename SampleType>
void Square<SampleType>::setFrequency (SampleType frequency, SampleType sampleRate)
{
	phase.setFrequency (frequency, sampleRate);
	freq = frequency;
}

template <typename SampleType>
SampleType Square<SampleType>::getFrequency() const noexcept
{
	return freq;
}

template struct Square<float>;
template struct Square<double>;

/*--------------------------------------------------------------------------------------------*/

template <typename SampleType>
Triangle<SampleType>::Triangle()
	: Oscillator<SampleType> ([this]
							  {
	sum += SampleType (4) * square.phase.getIncrement() * square.getSample();
	return sum; })
{
	resetPhase();
}

template <typename SampleType>
void Triangle<SampleType>::resetPhase()
{
	square.resetPhase();
	sum = static_cast<SampleType> (1);
}

template <typename SampleType>
void Triangle<SampleType>::setFrequency (SampleType frequency, SampleType sampleRate)
{
	square.setFrequency (frequency, sampleRate);
	freq = frequency;
}

template <typename SampleType>
SampleType Triangle<SampleType>::getFrequency() const noexcept
{
	return freq;
}

template struct Triangle<float>;
template struct Triangle<double>;

}  // namespace lemons::dsp::osc
