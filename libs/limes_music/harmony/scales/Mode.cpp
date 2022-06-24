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

#include "./Mode.h"
#include "./KeySignature.h"	 // for KeySignature, KeySigna...
#include <limes_platform.h>	 // for LIMES_ASSERT_FALSE
#include "../PitchClass.h"	 // for PitchClass
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace music::scales
{

std::vector<int> Mode::getIntervalsAsSemitones() const
{
	switch (type)
	{
		case (Type::Ionian) : return { 2, 2, 1, 2, 2, 2, 1 };
		case (Type::Dorian) : return { 2, 1, 2, 2, 2, 1, 2 };
		case (Type::Phrygian) : return { 1, 2, 2, 2, 1, 2, 2 };
		case (Type::Lydian) : return { 2, 2, 2, 1, 2, 2, 1 };
		case (Type::Mixolydian) : return { 2, 2, 1, 2, 2, 1, 2 };
		case (Type::Aeolian) : return { 2, 1, 2, 2, 1, 2, 2 };
		case (Type::Locrian) : return { 1, 2, 2, 1, 2, 2, 2 };
	}

	LIMES_ASSERT_FALSE;
	return {};
}

PitchClass Mode::getPitchClassOfRoot() const noexcept
{
	return rootPitchClass;
}

// String Mode::getStringDescription() const
//{
//	const auto typeString = [t = type]
//	{
//		switch (t)
//		{
//			case (Type::Ionian) : return TRANS ("ionian");
//			case (Type::Dorian) : return TRANS ("dorian");
//			case (Type::Phrygian) : return TRANS ("phrygian");
//			case (Type::Lydian) : return TRANS ("lydian");
//			case (Type::Mixolydian) : return TRANS ("mixolydian");
//			case (Type::Aeolian) : return TRANS ("aeolian");
//			case (Type::Locrian) : return TRANS ("locrian");
//		}
//
//		jassertfalse;
//		return String {};
//	}();
//
//	return rootPitchClass.getAsString() + " " + typeString;
// }

// Mode Mode::fromStringDescription (const String& description)
//{
//	const auto type = [typeString = description.fromFirstOccurrenceOf (" ", false, false).trim()]
//	{
//		if (typeString.equalsIgnoreCase (TRANS ("ionian")))
//			return Type::Ionian;
//
//		if (typeString.equalsIgnoreCase (TRANS ("dorian")))
//			return Type::Dorian;
//
//		if (typeString.equalsIgnoreCase (TRANS ("phrygian")))
//			return Type::Phrygian;
//
//		if (typeString.equalsIgnoreCase (TRANS ("lydian")))
//			return Type::Lydian;
//
//		if (typeString.equalsIgnoreCase (TRANS ("mixolydian")))
//			return Type::Mixolydian;
//
//		if (typeString.equalsIgnoreCase (TRANS ("aeolian")))
//			return Type::Aeolian;
//
//		if (typeString.equalsIgnoreCase (TRANS ("locrian")))
//			return Type::Locrian;
//
//		jassertfalse;
//		return Type::Ionian;
//	}();
//
//	const PitchClass pc { description.upToFirstOccurrenceOf (" ", false, false).trim() };
//
//	return Mode { type, pc };
// }

int Mode::notesPerOctave() const noexcept
{
	return 8;
}

int Mode::getNumSharps() const noexcept
{
	const auto addedSharps = [t = type]
	{
		if (t == Type::Lydian)
			return 1;

		return 0;
	}();

	const auto parallelMajor = KeySignature { KeySignature::Type::Major, rootPitchClass };

	return parallelMajor.getNumSharps() + addedSharps;
}

int Mode::getNumFlats() const noexcept
{
	const auto addedFlats = [t = type]
	{
		switch (t)
		{
			case (Type::Ionian) : return 0;
			case (Type::Dorian) : return 2;
			case (Type::Phrygian) : return 4;
			case (Type::Lydian) : return 0;
			case (Type::Mixolydian) : return 1;
			case (Type::Aeolian) : return 3;
			case (Type::Locrian) : return 5;
		}

		LIMES_ASSERT_FALSE;
		return 0;
	}();

	const auto parallelMajor = KeySignature { KeySignature::Type::Major, rootPitchClass };

	return parallelMajor.getNumFlats() + addedFlats;
}

KeySignature Mode::getKeySignature() const noexcept
{
	if (type == Type::Aeolian)
		return KeySignature { KeySignature::Type::NaturalMinor, rootPitchClass.getAsInt() };

	if (const auto flats = getNumFlats(); flats > 0)
		return KeySignature { flats, false, KeySignature::Type::Major };

	return KeySignature { getNumSharps(), true, KeySignature::Type::Major };
}

}  // namespace music::scales

LIMES_END_NAMESPACE
