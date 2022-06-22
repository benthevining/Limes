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
#include <cmath>					// for sin, fmod
#include <limes_platform.h>			// for LIMES_ASSERT
#include <limes_core.h>				// for two_pi
#include "../streams/oscillator.h"	// for Oscillator<>::Phase
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace dsp::osc
{

template <Sample SampleType>
[[nodiscard]] static constexpr LIMES_FORCE_INLINE SampleType blep (SampleType phase, SampleType increment) noexcept
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

#pragma mark Sine

LIMES_DISABLE_ALL_COMPILER_WARNINGS

template <Sample SampleType>
Sine<SampleType>::Sine()
	: Oscillator<SampleType> ([this]() noexcept (noexcept (std::sin (phase.next (math::constants::two_pi<SampleType>))))
							  { return std::sin (phase.next (math::constants::two_pi<SampleType>)); })
{
	phase.resetPhase();
}

LIMES_REENABLE_ALL_COMPILER_WARNINGS

template <Sample SampleType>
void Sine<SampleType>::resetPhase()
{
	phase.resetPhase();
}

template <Sample SampleType>
void Sine<SampleType>::setFrequency (SampleType frequency, SampleType sampleRate)
{
	phase.setFrequency (math::constants::two_pi<SampleType> * frequency, sampleRate);

	freq = frequency;
}

template <Sample SampleType>
SampleType Sine<SampleType>::getFrequency() const noexcept
{
	return freq;
}

template struct Sine<float>;
template struct Sine<double>;

/*--------------------------------------------------------------------------------------------*/

#pragma mark Saw

template <Sample SampleType>
Saw<SampleType>::Saw()
	: Oscillator<SampleType> ([this]() noexcept
							  {
	const auto p = phase.next (1);
	return SampleType (2) * p - SampleType (1) - blep (p, phase.getIncrement()); })
{
	phase.resetPhase();
}

template <Sample SampleType>
void Saw<SampleType>::resetPhase()
{
	phase.resetPhase();
}

template <Sample SampleType>
void Saw<SampleType>::setFrequency (SampleType frequency, SampleType sampleRate)
{
	phase.setFrequency (frequency, sampleRate);
	freq = frequency;
}

template <Sample SampleType>
SampleType Saw<SampleType>::getFrequency() const noexcept
{
	return freq;
}

template struct Saw<float>;
template struct Saw<double>;

/*--------------------------------------------------------------------------------------------*/

#pragma mark Square

template <Sample SampleType>
Square<SampleType>::Square()
	: Oscillator<SampleType> ([this]() noexcept
							  {
	const auto inc = phase.getIncrement();
	const auto p   = phase.next (1);

	const auto base = p < SampleType (0.5) ? SampleType (-1) : SampleType (1);

	return base - blep (p, inc) + blep (std::fmod (p + SampleType (0.5), SampleType (1)), inc); })
{
	phase.resetPhase();
}

template <Sample SampleType>
void Square<SampleType>::resetPhase()
{
	phase.resetPhase();
}

template <Sample SampleType>
void Square<SampleType>::setFrequency (SampleType frequency, SampleType sampleRate)
{
	phase.setFrequency (frequency, sampleRate);
	freq = frequency;
}

template <Sample SampleType>
SampleType Square<SampleType>::getFrequency() const noexcept
{
	return freq;
}

template struct Square<float>;
template struct Square<double>;

/*--------------------------------------------------------------------------------------------*/

#pragma mark Triangle

template <Sample SampleType>
Triangle<SampleType>::Triangle()
	: Oscillator<SampleType> ([this]() noexcept
							  {
	sum += SampleType (4) * square.phase.getIncrement() * square.getSample();
	return sum; })
{
	resetPhase();
}

template <Sample SampleType>
void Triangle<SampleType>::resetPhase()
{
	square.resetPhase();
	sum = static_cast<SampleType> (1);
}

template <Sample SampleType>
void Triangle<SampleType>::setFrequency (SampleType frequency, SampleType sampleRate)
{
	square.setFrequency (frequency, sampleRate);
	freq = frequency;
}

template <Sample SampleType>
SampleType Triangle<SampleType>::getFrequency() const noexcept
{
	return freq;
}

template struct Triangle<float>;
template struct Triangle<double>;

}  // namespace dsp::osc

LIMES_END_NAMESPACE
