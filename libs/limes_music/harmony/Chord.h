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

class LIMES_EXPORT Chord final
{
public:

	explicit Chord (const std::initializer_list<int>& midiNotes);

	explicit Chord (const std::initializer_list<Pitch>& midiNotes);

	LIMES_DEFAULT_MOVABLE (Chord);
	LIMES_DEFAULT_COPYABLE (Chord);

	[[nodiscard]] int getNumPitches() const;

	[[nodiscard]] Pitch getLowestPitch() const;

	[[nodiscard]] Pitch getHighestPitch() const;

	[[nodiscard]] bool contains (const PitchClass& pitchClass) const noexcept;

	[[nodiscard]] bool contains (const Pitch& pitch) const noexcept;

	[[nodiscard]] bool contains (int midiNote) const noexcept;

	[[nodiscard]] bool fitsInScale (const scales::Scale& scale) const noexcept;

	[[nodiscard]] ds::vector<Interval> getIntervals() const;

	[[nodiscard]] ds::vector<PitchClass> getPitchClasses() const;

	[[nodiscard]] int getNumUniquePitchClasses() const;

	[[nodiscard]] ds::vector<Pitch> getPitches() const;

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
