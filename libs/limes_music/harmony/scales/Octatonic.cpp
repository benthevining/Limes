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

#include "Octatonic.h"
#include "../PitchClass.h"			// for PitchClass
#include <limes_data_structures.h>	// for vector
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace music::scales
{

bool Octatonic::operator== (const Octatonic& other) const
{
	return startWithHalfStep == other.startWithHalfStep && pitchClassOfRoot == other.pitchClassOfRoot;
}

bool Octatonic::operator!= (const Octatonic& other) const
{
	return ! (*this == other);
}

vector<int> Octatonic::getIntervalsAsSemitones() const
{
	vector<int> intervals;

	auto semitone = startWithHalfStep;

	for (auto i = 0; i < 8; ++i)
	{
		if (semitone)
			intervals.emplace_back (1);
		else
			intervals.emplace_back (2);

		semitone = ! semitone;
	}

	return intervals;
}

PitchClass Octatonic::getPitchClassOfRoot() const noexcept
{
	return pitchClassOfRoot;
}

// String Octatonic::getStringDescription() const
//{
//	const auto kindString = [halfWhole = startWithHalfStep]
//	{
//		if (halfWhole)
//			return TRANS ("half/whole");
//
//		return TRANS ("whole/half");
//	}();
//
//	return pitchClassOfRoot.getAsString() + " " + kindString + " " + TRANS ("octatonic");
// }

int Octatonic::notesPerOctave() const noexcept
{
	return 8;
}

// Octatonic Octatonic::fromStringDescription (const String& string)
//{
//	const auto halfWhole = string.containsIgnoreCase (TRANS ("half/whole"));
//
//	const PitchClass pc { string.upToFirstOccurrenceOf (" ", false, false).trim() };
//
//	return Octatonic { pc, halfWhole };
// }

}  // namespace music::scales

LIMES_END_NAMESPACE
