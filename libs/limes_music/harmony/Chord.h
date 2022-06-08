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
#include <initializer_list>			// for initializer_list
#include "Pitch.h"					// for Pitch
#include <limes_data_structures.h>	// for vector
#include <limes_namespace.h>
#include <limes_core.h>

/** @file
	This file defines the Chord class.
	@ingroup music_harmony
 */

LIMES_BEGIN_NAMESPACE

namespace music
{
class Interval;
struct PitchClass;

namespace scales
{
struct Scale;
}
}  // namespace music


namespace music
{

/** This class represents a musical %chord.
	A %chord is simply represented as a collection of Pitch objects.
	@ingroup music_harmony
 */
class LIMES_EXPORT Chord final
{
public:

	/** @name Constructors */
	///@{
	/** Creates a %chord from a list of pitches.
		Duplicate pitches will only be present once in the constructed %chord.
	 */
	explicit Chord (const std::initializer_list<int>& midiNotes);
	explicit Chord (const std::initializer_list<Pitch>& midiNotes);
	///@}

	LIMES_DEFAULT_MOVABLE (Chord)
	LIMES_DEFAULT_COPYABLE (Chord)

	/** Returns the number of unique pitches in this %chord. */
	[[nodiscard]] int getNumPitches() const;

	/** Returns the lowest %pitch in the %chord. */
	[[nodiscard]] Pitch getLowestPitch() const;

	/** Returns the highest %pitch in the %chord. */
	[[nodiscard]] Pitch getHighestPitch() const;

	/** @name Contains queries */
	///@{
	/** Returns true if this %chord contains the given %pitch or pitch class. */
	[[nodiscard]] bool contains (const PitchClass& pitchClass) const noexcept;
	[[nodiscard]] bool contains (const Pitch& pitch) const noexcept;
	[[nodiscard]] bool contains (int midiNote) const noexcept;
	///@}

	/** Returns true if all of this chord's pitches are present in the given %scale without using accidentals. */
	[[nodiscard]] bool fitsInScale (const scales::Scale& scale) const noexcept;

	/** Returns a list of intervals between each consecutive pair of notes in this %chord. */
	[[nodiscard]] ds::vector<Interval> getIntervals() const;

	/** Returns a list of %pitch classes present in this %chord. */
	[[nodiscard]] ds::vector<PitchClass> getPitchClasses() const;

	/** Returns the number of unique %pitch classes present in this %chord.
		The same %pitch class present in multiple octaves will only be tallied once by this function.
	 */
	[[nodiscard]] int getNumUniquePitchClasses() const;

	/** Returns a copy of the list of pitches present in this %chord. */
	[[nodiscard]] ds::vector<Pitch> getPitches() const;

	/** Returns a new %chord with each %pitch shifted by a given interval.
		@param interval The interval to shift each %pitch by
		@param above If true, each %pitch will be shifted up. If false, each %pitch will be shifted down.
	 */
	[[nodiscard]] Chord applyInterval (const Interval& interval, bool above);

	// get inversion

	// isMajor, isMinor, isDiminished, isDomSeventh ...

	// get root note

	// static/free functions to create chord of given quality with given root note

private:

	ds::vector<Pitch> pitches;
};

}  // namespace music

LIMES_END_NAMESPACE
