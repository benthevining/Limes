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

#include <array>
#include <limes_export.h>
#include <limes_namespace.h>
#include <limes_core.h>

/** @file
	This file defines the PitchClass class.
	@ingroup music_harmony
 */

LIMES_BEGIN_NAMESPACE

namespace music
{

/** A struct that represents a %pitch class.
	This class is agnostic to the octave the %pitch class appears in; and also to enharmonic spellings -- thus, C-sharp and D-flat are considered the same %pitch class.
	A %pitch class is represented as an integer between 0 and 11, with 0 being C, 1 being C-sharp/D-flat, and 11 being B natural.

	Here are all possible %pitch classes:

	Integer value | Note name
	------------- | -----------------
	0             | C
	1             | C-sharp / D-flat
	2             | D
	3             | D-sharp / E-flat
	4             | E
	5             | F
	6             | F-sharp / G-flat
	7             | G
	8             | G-sharp / A-flat
	9             | A
	10            | A-sharp / B-flat
	11            | B

	@ingroup music_harmony
	@see Pitch
 */
struct LIMES_EXPORT PitchClass final
{
	/** Creates a %pitch class from an integer, where 0 is C, 1 is C#/Db, and 11 is B.
	 */
	constexpr explicit PitchClass (int midiPitch) noexcept
		: pitchClass (midiPitch % 12)
	{
	}

	LIMES_CONSTEXPR_MOVABLE (PitchClass);
	LIMES_CONSTEXPR_COPYABLE (PitchClass);

	/** Returns true if the two %pitch classes are equal. */
	[[nodiscard]] constexpr bool operator== (const PitchClass& other) const noexcept
	{
		return pitchClass == other.pitchClass;
	}

	/** Returns true if the two %pitch classes are not equal. */
	[[nodiscard]] constexpr bool operator!= (const PitchClass& other) const noexcept
	{
		return pitchClass != other.pitchClass;
	}

	/** Returns true if this %pitch class represents a black key on a standard keyboard. */
	[[nodiscard]] constexpr bool isBlackKey() const noexcept
	{
		return pitchClass == 1 || pitchClass == 3 || pitchClass == 6 || pitchClass == 8 || pitchClass == 10;
	}

	/** Returns true if this %pitch class represents a white key on a standard keyboard. */
	[[nodiscard]] constexpr bool isWhiteKey() const noexcept
	{
		return ! isBlackKey();
	}

	/** Returns this %pitch class as an integer between 0 and 11, where 0 is C and 11 is B. */
	constexpr operator int() const noexcept
	{
		return pitchClass;
	}

	/** Returns this %pitch class as an integer between 0 and 11, where 0 is C and 11 is B. */
	[[nodiscard]] constexpr int getAsInt() const noexcept
	{
		return pitchClass;
	}

	/** Returns an iterable list of all the %pitch classes, starting from 0 and ending with 11. */
	[[nodiscard]] static constexpr std::array<PitchClass, 12> getAll()
	{
		using PC = PitchClass;
		return { PC { 0 }, PC { 1 }, PC { 2 }, PC { 3 }, PC { 4 }, PC { 5 }, PC { 6 }, PC { 7 }, PC { 8 }, PC { 9 }, PC { 10 }, PC { 11 } };
	}

private:

	int pitchClass { 0 };
};

}  // namespace music

LIMES_END_NAMESPACE
