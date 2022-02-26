/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

#pragma once

#include "Scale.h"
#include "KeySignature.h"

namespace lemons::music::scales
{

/** Represents a modal scale.
 */
class Mode final : public Scale
{
public:

	/** Represents which kind of mode this scale is. */
	enum class Type
	{
		Ionian,
		Dorian,
		Phrygian,
		Lydian,
		Mixolydian,
		Aeolian,
		Locrian
	};

	/** Creates a modal scale object with the given type and root. */
	constexpr explicit Mode (Type typeToUse, const PitchClass& pitchClassOfRoot) noexcept
		: type (typeToUse), rootPitchClass (pitchClassOfRoot)
	{
	}

	/** Creates a modal scale of the given type, starting on the given MIDI note. */
	constexpr explicit Mode (Type typeToUse, int rootNote) noexcept
		: type (typeToUse), rootPitchClass (rootNote)
	{
	}

	/** Copy constructor. */
	constexpr Mode (const Mode& other) noexcept
		: type (other.type), rootPitchClass (other.rootPitchClass)
	{
	}

	/** Assignment operator. */
	constexpr Mode& operator= (const Mode& other) noexcept
	{
		type		   = other.type;
		rootPitchClass = other.rootPitchClass;
		return *this;
	}

	/** Creates a modal scale object from a string description of one.
		@see getStringDescription()
	 */
	//	[[nodiscard]] static Mode fromStringDescription (const String& description);

	/** Returns true if the other modal scale has the same type and root as this one. */
	[[nodiscard]] constexpr bool operator== (const Mode& other) const noexcept
	{
		return type == other.type && rootPitchClass == other.rootPitchClass;
	}

	/** Returns true if the other modal scale does not have the same type and root as this one. */
	[[nodiscard]] constexpr bool operator!= (const Mode& other) const noexcept
	{
		return ! (*this == other);
	}

	/** Returns the set of intervals that make up this mode. */
	[[nodiscard]] vector<int> getIntervalsAsSemitones() const final;

	/** Returns the pitch class of the root of this mode. */
	[[nodiscard]] PitchClass getPitchClassOfRoot() const noexcept final;

	/** Returns a string description of this mode, for example, "E lydian". */
	//	[[nodiscard]] String getStringDescription() const final;

	/** Returns 8. */
	[[nodiscard]] int notesPerOctave() const noexcept final;

	/** Returns the number of sharps that make up the key signature for this mode. */
	[[nodiscard]] int getNumSharps() const noexcept;

	/** Returns the number of flats that make up the key signature for this mode. */
	[[nodiscard]] int getNumFlats() const noexcept;

	/** Returns an equivalent key signature for this mode. The returned key signature contains all the same pitch classes as this mode. */
	[[nodiscard]] KeySignature getKeySignature() const noexcept;

private:

	Type type { Type::Ionian };

	PitchClass rootPitchClass { 0 };
};

}  // namespace lemons::music::scales
