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

#include "WholeTone.h"

namespace lemons::music::scales
{

bool WholeTone::operator== (const WholeTone& other) const
{
	return startingPitchClass == other.startingPitchClass;
}

bool WholeTone::operator!= (const WholeTone& other) const
{
	return startingPitchClass != other.startingPitchClass;
}

vector<int> WholeTone::getIntervalsAsSemitones() const
{
	vector<int> intervals;

	intervals.reserve (6);

	for (auto i = 0; i < 6; ++i)
		intervals.emplace_back (2);

	return intervals;
}

PitchClass WholeTone::getPitchClassOfRoot() const noexcept
{
	return startingPitchClass;
}

// String WholeTone::getStringDescription() const
//{
//	return startingPitchClass.getAsString() + " " + TRANS ("whole tone");
// }

int WholeTone::notesPerOctave() const noexcept
{
	return 6;
}

// WholeTone WholeTone::fromStringDescription (const String& string)
//{
//	const PitchClass pc { string.upToFirstOccurrenceOf (TRANS ("whole tone"), false, true).trim() };
//
//	return WholeTone { pc };
// }

}  // namespace lemons::music::scales
