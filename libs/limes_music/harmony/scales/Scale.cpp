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

#include "./Scale.h"
#include "../PitchUtils.h"			// for lowestNoteOfMidiOctave
#include "../intervals/Interval.h"	// for Interval
#include "../Pitch.h"				// for Pitch
#include "../PitchClass.h"			// for PitchClass
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

std::vector<Interval> Scale::getIntervals() const
{
	return alg::createFromTransform<std::vector<Interval>> (getIntervalsAsSemitones(),
															[] (const auto interval)
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

	return alg::contains_if (getIntervalsAsSemitones(), [pitchClass, root] (int interval)
							 { return pitchClass == PitchClass { root + interval }; });
}

std::vector<PitchClass> Scale::getPitchClasses() const
{
	std::vector<PitchClass> pitchClasses;

	for (auto i = 0; i <= notesPerOctave(); ++i)
	{
		const PitchClass pc { i };

		if (containsPitchClass (pc))
			pitchClasses.emplace_back (pc);
	}

	return pitchClasses;
}

std::vector<Pitch> Scale::getPitches (int octaveNumber) const
{
	const auto startingNote = [this, octaveNumber]
	{
		auto starting = lowestNoteOfMidiOctave (octaveNumber);

		while (! containsPitch (starting))
			++starting;

		return starting;
	}();

	return alg::createFromTransform<std::vector<Pitch>> (getIntervalsAsSemitones(),
														 [startingNote] (const auto interval)
														 { return Pitch { startingNote + interval }; });
}

std::vector<Pitch> Scale::getPitches (int lowestMidiNote, int highestMidiNote) const
{
	const auto startingNote = [this, &lowestMidiNote]
	{
		while (! containsPitch (lowestMidiNote))
			++lowestMidiNote;

		return lowestMidiNote;
	}();

	std::vector<Pitch> pitches;

	const auto intervals = getIntervalsAsSemitones();

	int octaveOffset { 0 };

	auto lastNote = startingNote;

	auto idx = 0UL;

	while (lastNote <= highestMidiNote)
	{
		lastNote = startingNote + octaveOffset + intervals[idx++];

		pitches.emplace_back (Pitch { lastNote });

		if (idx >= intervals.size())
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
