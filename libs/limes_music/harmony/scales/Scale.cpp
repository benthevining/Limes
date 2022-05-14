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

#include "Scale.h"
#include "../PitchUtils.h"			// for lowestNoteOfMidiOctave
#include "../intervals/Interval.h"	// for Interval
#include "../Pitch.h"				// for Pitch
#include "../PitchClass.h"			// for PitchClass
#include <limes_data_structures.h>	// for vector, scalar_vector, basic_vector
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace music::scales
{

bool Scale::operator== (const Scale& other) const
{
	return getPitchClassOfRoot() == other.getPitchClassOfRoot() && getIntervalsAsSemitones() == other.getIntervalsAsSemitones();
}

bool Scale::operator!= (const Scale& other) const
{
	return ! (*this == other);
}

ds::vector<Interval> Scale::getIntervals() const
{
	return getIntervalsAsSemitones().transformElementsTo<Interval> ([] (const auto interval)
																	{ return Interval::fromNumSemitones (interval); });
}

bool Scale::containsPitch (const Pitch& pitch) const
{
	return containsPitchClass (pitch.getPitchClass());
}

bool Scale::containsPitch (int midiNoteNumber) const
{
	const PitchClass pc { midiNoteNumber };
	return containsPitchClass (pc);
}

bool Scale::containsPitchClass (const PitchClass& pitchClass) const
{
	const auto root = getPitchClassOfRoot().getAsInt();

	return getIntervalsAsSemitones().contains_if ([pitchClass, root] (int interval)
												  { return pitchClass == PitchClass { root + interval }; });
}

ds::vector<PitchClass> Scale::getPitchClasses() const
{
	ds::vector<PitchClass> pitchClasses;

	for (auto i = 0; i <= notesPerOctave(); ++i)
	{
		const PitchClass pc { i };

		if (containsPitchClass (pc))
			pitchClasses.emplace_back (pc);
	}

	return pitchClasses;
}

ds::vector<Pitch> Scale::getPitches (int octaveNumber) const
{
	const auto startingNote = [this, octaveNumber]
	{
		auto starting = lowestNoteOfMidiOctave (octaveNumber);

		while (! containsPitch (starting))
			++starting;

		return starting;
	}();

	return getIntervalsAsSemitones().transformElementsTo<Pitch> ([startingNote] (const auto interval)
																 { return Pitch { startingNote + interval }; });
}

ds::vector<Pitch> Scale::getPitches (int lowestMidiNote, int highestMidiNote) const
{
	const auto startingNote = [this, &lowestMidiNote]
	{
		while (! containsPitch (lowestMidiNote))
			++lowestMidiNote;

		return lowestMidiNote;
	}();

	ds::vector<Pitch> pitches;

	const auto intervals = getIntervalsAsSemitones();

	int idx { 0 };
	int octaveOffset { 0 };

	auto lastNote = startingNote;

	while (lastNote <= highestMidiNote)
	{
		lastNote = startingNote + octaveOffset + intervals[idx++];

		pitches.emplace_back (Pitch { lastNote });

		if (idx >= static_cast<int> (intervals.numObjects()))
		{
			idx = 0;

			constexpr auto notesPerOctave = 8;
			octaveOffset += notesPerOctave;
		}
	}

	return pitches;
}

Pitch Scale::getRoot (int octaveNumber) const noexcept
{
	const auto octaveStart = lowestNoteOfMidiOctave (octaveNumber);

	return Pitch { octaveStart + getPitchClassOfRoot() };
}

// String Scale::getRootAsString() const noexcept
//{
//	return getPitchClassOfRoot().getAsString();
// }

PitchClass Scale::getPitchClassOfScaleDegree (int scaleDegree) const noexcept
{
	return PitchClass { scaleDegree % notesPerOctave() };
}

// String Scale::getScaleDegreeAsString (int scaleDegree) const noexcept
//{
//	return getPitchClassOfScaleDegree (scaleDegree).getAsString();
// }

}  // namespace music::scales

LIMES_END_NAMESPACE
