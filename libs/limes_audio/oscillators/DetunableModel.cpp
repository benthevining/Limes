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

#include "DetunableModel.h"
#include <limes_platform.h>
#include <limes_core.h>
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace dsp::osc
{

DetunableModel::DetunableModel (std::size_t initialNumVoices)  // cppcheck-suppress uninitMemberVar
	: numVoices (initialNumVoices)
{
	frequencies.reserve (static_cast<std::vector<float>::size_type>(numVoices));
}

void DetunableModel::setFrequency (float frequency)
{
	LIMES_ASSERT (numVoices > 0);

	const auto spreadSemitones = static_cast<float> (totalSpreadCents) * 0.01f;
	const auto increment	   = spreadSemitones / static_cast<float> (numVoices);
	const auto centerPitch	   = math::freqToMidi (frequency);

	auto lowBound = centerPitch - (spreadSemitones * 0.5f);

	for (auto i = 0UL; i < numVoices; ++i)
	{
		LIMES_ASSERT (lowBound > 0.);
		frequencies[i] = static_cast<float> (math::midiToFreq (lowBound));
		lowBound += increment;
	}
}

float DetunableModel::getCenterFrequency() const noexcept
{
	return lastFrequency;
}

void DetunableModel::setDetuneAmount (int totalPitchSpreadInCents)
{
	LIMES_ASSERT (totalPitchSpreadInCents >= 0);
	totalSpreadCents = totalPitchSpreadInCents;
	setFrequency (lastFrequency);
}

int DetunableModel::getPitchSpreadCents() const noexcept
{
	return totalSpreadCents;
}

void DetunableModel::changeNumVoices (std::size_t newNumVoices)
{
	numVoices = newNumVoices;
	frequencies.reserve (newNumVoices);
	setFrequency (lastFrequency);
}

int DetunableModel::getNumVoices() const noexcept
{
	return static_cast<int>(numVoices);
}

float DetunableModel::getFrequency (std::size_t voiceNumber) const
{
	LIMES_ASSERT (voiceNumber >= 0 && voiceNumber < numVoices);
	return frequencies[voiceNumber];
}

void DetunableModel::applyFrequencies (std::function<void (float)>&& func) const
{
	for (auto i = 0UL; i < numVoices; ++i)
		func (frequencies[i]);
}

}  // namespace dsp::osc

LIMES_END_NAMESPACE
