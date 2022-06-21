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
#include <algorithm>			 // for all_of
#include "PitchClass.h"			 // for PitchClass
#include "intervals/Interval.h"	 // for Interval
#include "scales/Scale.h"		 // for Scale
#include <limes_core.h>			 // for transform
#include "Pitch.h"				 // for Pitch
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace music
{

Chord::Chord (const std::initializer_list<int>& midiNotes)
{
	alg::transform (midiNotes, pitches, [] (const auto midiNote)
					{ return Pitch { midiNote }; });

	pitches.erase (std::unique (std::begin (pitches), std::end (pitches)), std::end (pitches));

	alg::sort (pitches);
}

Chord::Chord (const std::initializer_list<Pitch>& midiNotes)
	: pitches (midiNotes)
{
	pitches.erase (std::unique (std::begin (pitches), std::end (pitches)), std::end (pitches));

	alg::sort (pitches);
}

int Chord::getNumPitches() const
{
	return static_cast<int> (pitches.size());
}

Pitch Chord::getLowestPitch() const
{
	return alg::min_value (pitches);
}

Pitch Chord::getHighestPitch() const
{
	return alg::max_value (pitches);
}

bool Chord::contains (const PitchClass& pitchClass) const noexcept
{
	return alg::contains_if (pitches, [&pitchClass] (const Pitch& p)
							 { return p.getPitchClass() == pitchClass; });
}

bool Chord::contains (const Pitch& pitch) const noexcept
{
	return alg::contains (pitches, pitch);
}

bool Chord::contains (int midiNote) const noexcept
{
	return alg::contains_if (pitches, [note = midiNote] (const Pitch& p)
							 { return p.getRoundedMidiPitch() == note; });
}

bool Chord::fitsInScale (const scales::Scale& scale) const noexcept
{
	return std::all_of (pitches.begin(), pitches.end(),
						[&scale] (const Pitch& p)
						{ return scale.containsPitch (p); });
}

std::vector<Interval> Chord::getIntervals() const
{
	std::vector<Interval> intervals;

	for (auto i = 1UL; i < pitches.size(); ++i)
		intervals.push_back (Interval::fromPitches (pitches[i - 1], pitches[i]));

	return intervals;
}

std::vector<PitchClass> Chord::getPitchClasses() const
{
	auto pitchClasses = alg::createFromTransform<std::vector<PitchClass>> (pitches,
																		   [] (const auto& pitch)
																		   { return pitch.getPitchClass(); });

	pitchClasses.erase (std::unique (std::begin (pitchClasses), std::end (pitchClasses)), std::end (pitchClasses));

	alg::sort (pitchClasses);

	return pitchClasses;
}

int Chord::getNumUniquePitchClasses() const
{
	return static_cast<int> (getPitchClasses().size());
}

std::vector<Pitch> Chord::getPitches() const
{
	return pitches;
}

Chord Chord::applyInterval (const Interval& interval, bool above)
{
	Chord newChord { *this };

	for (auto i = 0UL; i < newChord.pitches.size(); ++i)
		newChord.pitches[i] = interval.applyToPitch (newChord.pitches[i], above);

	return newChord;
}

}  // namespace music

LIMES_END_NAMESPACE
