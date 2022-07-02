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

#include <limes_export.h>
#include <limes_namespace.h>

/** @dir libs/limes_midi/utilities
	This directory contains general MIDI utilities.
	@ingroup limes_midi
 */

/** @file
	This file defines the PitchbendMapper class.
	@ingroup limes_midi
 */

LIMES_BEGIN_NAMESPACE

/** This namespace contains utilities for working with MIDI.
	@ingroup limes_midi
 */
namespace midi
{

/** This class maps MIDI pitchbend values and note numbers to fractional MIDI note numbers.
	You can set separate ranges up and down in semitones, then for each MIDI note that comes in, you can call \c getAdjustedMidiPitch() to get the original note's mapped pitch, based on the last received pitchbend value and the pitchbend range.
	@ingroup limes_midi
 */
class LIMES_EXPORT PitchbendMapper final
{
public:
	/** Sets the range up and down in semitones.
		If the pitchbend value is 0, each incoming note will be mapped to \c note-stDown , and if the pitchbend value is 127, each note will be mapped to \c note+stUp .
	 */
	void setRange (float newStUp, float newStDown) noexcept;

	/** Returns the pitchbend range up in semitones. */
	[[nodiscard]] float getRangeUp() const noexcept;

	/** Returns the pitchbend range down in semitones. */
	[[nodiscard]] float getRangeDown() const noexcept;

	/** Returns the last pitchbend value that was set via \c newPitchbendReceived() . */
	[[nodiscard]] int getLastReceivedPitchbend() const noexcept;

	/** Each incoming pitchbend value from the pitch wheel should be sent to this function.
		The pitchbend values should be between 0 and 127, with 0 being bent down the lowest, 127 being bent up the highest, and 64 being no bend.
	 */
	void newPitchbendReceived (int newPitchbend) noexcept;

	/** Resets the state of the internal pitchbend to 64 (neutral). */
	void resetPitchbend() noexcept;

	/** Returns an adjusted MIDI pitch for an incoming MIDI pitch.
		The input pitch is adjusted based on the current pitchbend value, and the pitchbend range that's been set.
	 */
	[[nodiscard]] float getAdjustedMidiPitch (float inputMidiPitch) const;

private:
	float rangeUp { 2.f }, rangeDown { 2.f };

	int lastReceivedPitchbend { 64 };
};

}  // namespace midi

LIMES_END_NAMESPACE
