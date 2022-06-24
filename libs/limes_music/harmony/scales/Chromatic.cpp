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

#include "./Chromatic.h"
#include "../PitchUtils.h"	// for semitonesInOctave
#include "../PitchClass.h"	// for PitchClass
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace music::scales
{

bool Chromatic::operator== (const Chromatic& other) const
{
	return startingPitchClass == other.startingPitchClass;
}

bool Chromatic::operator!= (const Chromatic& other) const
{
	return startingPitchClass != other.startingPitchClass;
}

std::vector<int> Chromatic::getIntervalsAsSemitones() const
{
	std::vector<int> intervals;

	intervals.reserve (semitonesInOctave);

	for (auto i = 0; i < semitonesInOctave; ++i)
		intervals.emplace_back (1);

	return intervals;
}

PitchClass Chromatic::getPitchClassOfRoot() const noexcept
{
	return startingPitchClass;
}

// String Chromatic::getStringDescription() const
//{
//	return startingPitchClass.getAsString() + " " + TRANS ("chromatic");
// }

int Chromatic::notesPerOctave() const noexcept
{
	return semitonesInOctave;
}

// Chromatic Chromatic::fromStringDescription (const String& string)
//{
//	const PitchClass pc { string.upToFirstOccurrenceOf (TRANS ("chromatic"), false, true).trim() };
//
//	return Chromatic { pc };
// }

}  // namespace music::scales

LIMES_END_NAMESPACE
