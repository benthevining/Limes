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

#include "Chord.h"

namespace limes::music
{

Chord::Chord (const std::initializer_list<int>& midiNotes)
{
	alg::transform (midiNotes, pitches, [] (const auto midiNote)
					{ return Pitch { midiNote }; });

	pitches.removeDuplicates();
	pitches.sort();
}

Chord::Chord (const std::initializer_list<Pitch>& midiNotes)
	: pitches (midiNotes)
{
	pitches.removeDuplicates();
	pitches.sort();
}

int Chord::getNumPitches() const
{
	return pitches.numObjects();
}

Pitch Chord::getLowestPitch() const
{
	return pitches.max();
}

Pitch Chord::getHighestPitch() const
{
	return pitches.min();
}

bool Chord::contains (const PitchClass& pitchClass) const noexcept
{
	return pitches.contains_if ([&pitchClass] (const Pitch& p)
								{ return p.getPitchClass() == pitchClass; });
}

bool Chord::contains (const Pitch& pitch) const noexcept
{
	return pitches.contains (pitch);
}

bool Chord::contains (int midiNote) const noexcept
{
	return pitches.contains_if ([note = midiNote] (const Pitch& p)
								{ return p.getRoundedMidiPitch() == note; });
}

bool Chord::fitsInScale (const scales::Scale& scale) const noexcept
{
	return std::all_of (pitches.begin(), pitches.end(),
						[&scale] (const Pitch& p)
						{ return scale.containsPitch (p); });
}

vector<Interval> Chord::getIntervals() const
{
	vector<Interval> intervals;

	for (auto i = 1; i < static_cast<int> (pitches.numObjects()); ++i)
		intervals.push_back (Interval::fromPitches (pitches[i - 1], pitches[i]));

	return intervals;
}

vector<PitchClass> Chord::getPitchClasses() const
{
	auto pitchClasses = pitches.transformElementsTo<PitchClass> ([] (const auto& pitch)
																 { return pitch.getPitchClass(); });

	pitchClasses.removeDuplicates();

	return pitchClasses;
}

int Chord::getNumUniquePitchClasses() const
{
	return getPitchClasses().numObjects();
}

vector<Pitch> Chord::getPitches() const
{
	return pitches;
}

Chord Chord::applyInterval (const Interval& interval, bool above)
{
	Chord newChord { *this };

	for (auto i = 0; i < static_cast<int> (newChord.pitches.numObjects()); ++i)
		newChord.pitches[i] = interval.applyToPitch (newChord.pitches[i], above);

	return newChord;
}

}  // namespace limes::music
