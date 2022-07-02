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

#include <limes_export.h>	// for LIMES_EXPORT
#include "./Scale.h"		// for Scale
#include "../PitchClass.h"	// for PitchClass
#include <limes_namespace.h>
#include <limes_core.h>

/** @file
	This file defines the WholeTone %scale class.
	@ingroup music_scales
 */

LIMES_BEGIN_NAMESPACE

namespace music::scales
{

/** A class that represents a whole tone %scale.
	Whole tone scale objects only differ from one another by their starting %pitch.
	@ingroup music_scales
 */
class LIMES_EXPORT WholeTone final : public Scale
{
public:
	/** @name Constructors */
	///@{

	/** Creates a whole tone %scale object with the given %pitch class as its root. */
	constexpr explicit WholeTone (const PitchClass& pitchClassOfRoot) noexcept
		: startingPitchClass (pitchClassOfRoot)
	{
	}

	/** Creates a whole tone %scale starting on the given MIDI note. */
	constexpr explicit WholeTone (int rootNote) noexcept
		: startingPitchClass (rootNote)
	{
	}

	///@}

	LIMES_DEFAULT_COPYABLE (WholeTone)
	LIMES_DEFAULT_MOVABLE (WholeTone)

	/** Returns true if the other whole tone %scale has the same root as this one. */
	[[nodiscard]] bool operator== (const WholeTone& other) const;

	/** Returns true if the other whole tone %scale does not have the same root as this one. */
	[[nodiscard]] bool operator!= (const WholeTone& other) const;

	/** Returns an array of 6 2's, since every %interval in this %scale is a whole step. */
	[[nodiscard]] std::vector<int> getIntervalsAsSemitones() const final;

	/** Returns the %pitch class of the root of this %scale. */
	[[nodiscard]] PitchClass getPitchClassOfRoot() const noexcept final;

	/** Returns 6. */
	[[nodiscard]] int notesPerOctave() const noexcept final;

private:
	PitchClass startingPitchClass { 0 };
};

}  // namespace music::scales

LIMES_END_NAMESPACE
