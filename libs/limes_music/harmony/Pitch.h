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

#pragma once

#include "PitchClass.h"
#include "PitchUtils.h"
#include <limes_export.h>
#include <limes_namespace.h>
#include <limes_core.h>

LIMES_BEGIN_NAMESPACE

namespace music
{

/** This class is a simple wrapper around a MIDI pitch, with some convenient functions for representing it certain ways.
 */
class LIMES_EXPORT Pitch final
{
public:

	/** Creates a pitch object from a MIDI pitch, which can be passed to the constructor as any scalar arithmetic type. */
	template <typename T>
	constexpr explicit Pitch (T midiNote) noexcept
		: midiPitch (static_cast<double> (midiNote))
	{
	}

	/** Creates a pitch object from a pitch class and a MIDI octave number. */
	constexpr explicit Pitch (const PitchClass& pitchClass, int midiOctave) noexcept
		: midiPitch (static_cast<double> (lowestNoteOfMidiOctave (midiOctave) + pitchClass.getAsInt()))
	{
	}

	LIMES_CONSTEXPR_MOVABLE (Pitch);
	LIMES_CONSTEXPR_COPYABLE (Pitch);

	/** Creates a pitch object from a string representation of a pitch, for example "C4".
		@see stringToPitch()
	 */
	//	explicit Pitch (const String& pitchString) noexcept;

	/** Returns true if the two pitch objects are exactly equal. */
	[[nodiscard]] constexpr bool operator== (const Pitch& other) const noexcept
	{
		return midiPitch == other.midiPitch;
	}

	/** Returns true if the two pitch objects are not exactly equal. */
	[[nodiscard]] constexpr bool operator!= (const Pitch& other) const noexcept
	{
		return midiPitch != other.midiPitch;
	}

	/** Returns true if the two pitch objects round to the same nearest MIDI note. */
	[[nodiscard]] bool approximatelyEqual (const Pitch& other) const noexcept;

	/** Returns true if this pitch is higher than the other one. */
	[[nodiscard]] constexpr bool operator> (const Pitch& other) const noexcept
	{
		return midiPitch > other.midiPitch;
	}

	/** Returns true if this pitch is lower than the other one. */
	[[nodiscard]] constexpr bool operator< (const Pitch& other) const noexcept
	{
		return midiPitch < other.midiPitch;
	}

	/** Returns this pitch as a frequency in Hz.
		@see math::midiToFreq()
	 */
	[[nodiscard]] double getFreqHz() const noexcept;

	/** Returns this pitch as a frequency in Hz, rounded to the nearest integer.
	 */
	[[nodiscard]] int getRoundedFreqHz() const noexcept;

	/** Returns the MIDI pitch that this pitch object represents. */
	[[nodiscard]] constexpr double getMidiPitch() const noexcept
	{
		return midiPitch;
	}

	/** Returns this pitch object's MIDI pitch, rounded to the nearest integer MIDI note. */
	[[nodiscard]] int getRoundedMidiPitch() const noexcept;

	/** Returns this pitch object's pitch class as an integer from 0 to 11, with 0 being C and 11 being B.
		@see makeValidPitchClass()
	 */
	[[nodiscard]] PitchClass getPitchClass() const noexcept;

	/** Returns this pitch object's MIDI octave number.
		@see octaveNumberOfMidiNote()
	 */
	[[nodiscard]] int getOctaveNumber() const noexcept;

	/** Returns true if this pitch's nearest integer MIDI note is a black key on a standard keyboard. */
	[[nodiscard]] bool isBlackKey() const noexcept;

	/** Returns true if this pitch's nearest integer MIDI note is a white key on a standard keyboard. */
	[[nodiscard]] bool isWhiteKey() const noexcept;

	/** Returns a string representation of this pitch object.
		@param asSharps When true, the string representation will use sharp symbols for any needed accidentals; otherwise, flats will be used.
		@see pitchToString()
	 */
	//	[[nodiscard]] String toString (bool asSharps = true) const noexcept;

	/** Returns true if this pitch does not correspond exactly with any of the keys on a piano tuned to equal temperament.
	 */
	[[nodiscard]] bool isMicrotone() const noexcept;

private:

	double midiPitch { 60. };
};

}  // namespace music

LIMES_END_NAMESPACE
