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

#include "KeySignature.h"
#include <limes_platform.h>			// for LIMES_ASSERT_FALSE
#include "../PitchClass.h"			// for PitchClass
#include <limes_data_structures.h>	// for vector


namespace limes::music::scales
{

// KeySignature KeySignature::fromStringDescription (const String& description)
//{
//	const auto type = [&description]
//	{
//		if (! description.containsIgnoreCase (TRANS ("minor")))
//			return Type::Major;
//
//		if (description.containsIgnoreCase (TRANS ("harmonic")))
//			return Type::HarmonicMinor;
//
//		return Type::NaturalMinor;
//	}();
//
//	const auto root = stringToPitchClass (description.upToFirstOccurrenceOf (" ", false, false).trim());
//
//	return KeySignature { type, root };
// }

// String KeySignature::getRootAsString() const noexcept
//{
//	return getPitchClassOfRoot().getAsString (! isFlat);
// }
//
// String KeySignature::getStringDescription() const
//{
//	const auto res = getRootAsString();
//
//	if (type == Type::Major)
//		return res + " " + TRANS ("major");
//
//	if (type == Type::HarmonicMinor)
//		return res + " " + TRANS ("harmonic minor");
//
//	return res + " " + TRANS ("minor");
// }

// String KeySignature::getScaleDegreeAsString (int scaleDegree) const noexcept
//{
//	return getPitchClassOfScaleDegree (scaleDegree).getAsString (! isFlat);
// }


vector<int> KeySignature::getIntervalsAsSemitones() const
{
	switch (type)
	{
		case (Type::Major) : return { 2, 2, 1, 2, 2, 2, 1 };
		case (Type::NaturalMinor) : return { 2, 1, 2, 2, 1, 2, 2 };
		case (Type::HarmonicMinor) : return { 2, 1, 2, 2, 1, 3, 1 };
	}

	LIMES_ASSERT_FALSE;
	return {};
}

PitchClass KeySignature::getPitchClassOfRoot() const noexcept
{
	struct MajorMinorPair final
	{
		int majorRoot, minorRoot;
	};

	const auto pair = [num = numAccidentals, flat = isFlat]() -> MajorMinorPair
	{
		if (flat)
		{
			switch (num)
			{
				case (0) : return { 0, 9 };
				case (1) : return { 5, 2 };
				case (2) : return { 10, 7 };
				case (3) : return { 3, 0 };
				case (4) : return { 8, 5 };
				case (5) : return { 1, 10 };
				case (6) : return { 6, 3 };
				case (7) : return { 11, 8 };
			}

			LIMES_ASSERT_FALSE;
			return { 0, 9 };
		}

		switch (num)
		{
			case (0) : return { 0, 9 };
			case (1) : return { 7, 4 };
			case (2) : return { 2, 11 };
			case (3) : return { 9, 6 };
			case (4) : return { 4, 1 };
			case (5) : return { 11, 8 };
			case (6) : return { 6, 3 };
			case (7) : return { 1, 10 };
		}

		LIMES_ASSERT_FALSE;
		return { 0, 9 };
	}();

	const auto pitchClassInt = [&pair, t = type]
	{
		if (t == Type::Major)
			return pair.majorRoot;

		return pair.minorRoot;
	}();

	return PitchClass { pitchClassInt };
}

int KeySignature::notesPerOctave() const noexcept
{
	return 8;
}

bool KeySignature::hasEnharmonicKey() const noexcept
{
	return rootHasEnharmonicKey (getPitchClassOfRoot());
}

KeySignature KeySignature::getEnharmonicKey() const noexcept
{
	const auto root = getPitchClassOfRoot();

	if (! rootHasEnharmonicKey (root))
		return { *this };

	return KeySignature { type, ! isFlat, root };
}

bool KeySignature::isEnharmonicKeyOf (const KeySignature& other) const noexcept
{
	return getPitchClassOfRoot() == other.getPitchClassOfRoot() && type == other.type && isFlat != other.isFlat;
}

KeySignature KeySignature::getRelativeKey() const noexcept
{
	const auto otherType = [t = type]
	{
		if (t == Type::Major)
			return Type::NaturalMinor;

		return Type::Major;
	}();

	return KeySignature { numAccidentals, isFlat, otherType };
}

KeySignature KeySignature::getParallelKey() const noexcept
{
	const auto otherType = [t = type]
	{
		if (t == Type::Major)
			return Type::NaturalMinor;

		return Type::Major;
	}();

	return KeySignature { otherType, isFlat, getPitchClassOfRoot() };
}

bool KeySignature::isParallelKeyOf (const KeySignature& other) const noexcept
{
	return hasOppositeTonality (other) && isFlat == other.isFlat && getPitchClassOfRoot() == other.getPitchClassOfRoot();
}

KeySignature KeySignature::getDominantKey() const noexcept
{
	const PitchClass dominant { getPitchClassOfRoot() + 7 };

	return KeySignature { Type::Major, dominant };
}

bool KeySignature::isDominantKeyOf (const KeySignature& other) const noexcept
{
	if (type != Type::Major)
		return false;

	const PitchClass dominant { other.getPitchClassOfRoot() + 7 };

	return getPitchClassOfRoot() == dominant;
}

}  // namespace limes::music::scales
