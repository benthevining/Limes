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

#include <limes_export.h>			// for LIMES_EXPORT
#include "../Pitch.h"				// for Pitch
#include "../PitchClass.h"			// for PitchClass
#include <limes_data_structures.h>	// for vector
#include <limes_namespace.h>

/** @defgroup music_scales Scales
	Utilities for working with musical scales.
	@ingroup music_harmony
 */

/** @dir libs/limes_music/harmony/scales
	This directory contains utilities for working with musical scales.
	@ingroup music_scales
 */

/** @file
	This file defines the Scale class.
	@ingroup music_scales
 */

LIMES_BEGIN_NAMESPACE

namespace music
{
class Interval;
}


/** This namespace contains utilities for working with musical scales.
	@ingroup music_scales
 */
namespace music::scales
{

/** A base class for any kind of musical %scale.

	This class is intended for scales that are symmetrical in any octave, are symmetrical whether ascending or descending, and contain only intervals with whole numbers of semitones (ie, no microtones).
	Scales can have any number of notes in an octave.

	@ingroup music_scales
 */
struct LIMES_EXPORT Scale
{
	/** Default constructor. */
	constexpr Scale() = default;

	/** Destructor. */
	virtual ~Scale() = default;

	/** Returns true if the other %scale has the same root %pitch class and %interval set as this one. */
	[[nodiscard]] bool operator== (const Scale& other) const;

	/** Returns true if the other scale's root %pitch class or %interval set are different than this one's. */
	[[nodiscard]] bool operator!= (const Scale& other) const;

	/** Subclasses must implement this to return an array of integers representing the intervals in this %scale as a series of semitone steps. */
	[[nodiscard]] virtual ds::vector<int> getIntervalsAsSemitones() const = 0;

	/** Returns an array of Interval objects that represent the intervals in this %scale. */
	[[nodiscard]] ds::vector<Interval> getIntervals() const;

	/** Returns true if this %scale contains the passed %pitch class. */
	[[nodiscard]] bool containsPitchClass (const PitchClass& pitchClass) const;

	/** Returns true if this %scale contains the passed %pitch. */
	[[nodiscard]] bool containsPitch (const Pitch& pitch) const;

	/** Returns true if this %scale contains the passed MIDI note number. */
	[[nodiscard]] bool containsPitch (int midiNoteNumber) const;

	/** Returns the %pitch classes present in one octave of this %scale. */
	[[nodiscard]] ds::vector<PitchClass> getPitchClasses() const;

	/** Returns an array of %pitch objects representing one octave of this %scale, at the given octave number.
		@param octaveNumber The MIDI octave number to produce pitches for.
		@see lowestNoteOfMidiOctave()
	 */
	[[nodiscard]] ds::vector<Pitch> getPitches (int octaveNumber) const;

	/** Returns an array of %pitch objects representing this %scale between a lowest and highest MIDI pitch. */
	[[nodiscard]] ds::vector<Pitch> getPitches (int lowestMidiNote, int highestMidiNote) const;

	/** Subclasses must implement this to return the %pitch class of the root (or tonic) note of the %scale. */
	[[nodiscard]] virtual PitchClass getPitchClassOfRoot() const noexcept = 0;

	/** Returns a %pitch object representing the root of this %scale in the specified MIDI octave number. */
	[[nodiscard]] Pitch getRoot (int octaveNumber) const noexcept;

	/** Subclasses must implement this to return the number of notes per octave in this %scale. */
	[[nodiscard]] virtual int notesPerOctave() const noexcept = 0;

	/** Returns the %pitch class of a given %scale degree.
		If the passed %scale degree is greater than the value returned by notesPerOctave(), it will be truncated using the modulus operator.
	 */
	[[nodiscard]] PitchClass getPitchClassOfScaleDegree (int scaleDegree) const noexcept;
};

}  // namespace music::scales

LIMES_END_NAMESPACE
