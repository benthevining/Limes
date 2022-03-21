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

#include "Pitch.h"
#include <limes_core.h>

namespace limes::music
{

// Pitch::Pitch (const String& pitchString) noexcept
//	: midiPitch (static_cast<double> (stringToPitch (pitchString)))
//{
// }

bool Pitch::approximatelyEqual (const Pitch& other) const noexcept
{
	return math::round (midiPitch) == math::round (other.midiPitch);
}

double Pitch::getFreqHz() const noexcept
{
	return math::midiToFreq (midiPitch);
}

int Pitch::getRoundedFreqHz() const noexcept
{
	return math::round (getFreqHz());
}

int Pitch::getRoundedMidiPitch() const noexcept
{
	return math::round (midiPitch);
}

PitchClass Pitch::getPitchClass() const noexcept
{
	return PitchClass { math::round (midiPitch) };
}

int Pitch::getOctaveNumber() const noexcept
{
	return octaveNumberOfMidiNote (math::round (midiPitch));
}

bool Pitch::isBlackKey() const noexcept
{
	return getPitchClass().isBlackKey();
}

bool Pitch::isWhiteKey() const noexcept
{
	return ! isBlackKey();
}

// String Pitch::toString (bool asSharps) const noexcept
//{
//	return pitchToString (juce::roundToInt (midiPitch), asSharps);
// }

bool Pitch::isMicrotone() const noexcept
{
	return std::floor (midiPitch) != midiPitch;
}

}  // namespace limes::music
