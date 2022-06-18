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
#include "Scale.h"			// for Scale
#include "../PitchClass.h"	// for PitchClass
#include <limes_namespace.h>
#include <limes_core.h>

/** @file
	This file defines the Chromatic %scale class.
	@ingroup music_scales
 */

LIMES_BEGIN_NAMESPACE

namespace music::scales
{

/** A class representing a %chromatic %scale.
	%Chromatic scales only differ from one another by their starting %pitch.
	@ingroup music_scales
 */
class LIMES_EXPORT Chromatic final : public Scale
{
public:

	/** @name Constructors */
	///@{

	/** Creates a %chromatic %scale that starts on the specified MIDI %pitch. */
	constexpr explicit Chromatic (int noteToStartOn) noexcept
		: startingPitchClass (noteToStartOn)
	{
	}

	/** Creates a %chromatic %scale that starts on the specified %pitch class. */
	constexpr explicit Chromatic (const PitchClass& pitchClass) noexcept
		: startingPitchClass (pitchClass)
	{
	}

	///@}

	LIMES_DEFAULT_COPYABLE (Chromatic)
	LIMES_DEFAULT_MOVABLE (Chromatic)

	/** Returns true if the other %chromatic %scale starts on the same %pitch class as this one. */
	[[nodiscard]] bool operator== (const Chromatic& other) const;

	/** Returns true if the other %chromatic %scale does not start on the same %pitch class as this one. */
	[[nodiscard]] bool operator!= (const Chromatic& other) const;

	/** Returns an array of 12 1's, since every %interval in a %chromatic %scale is a half step. */
	[[nodiscard]] std::vector<int> getIntervalsAsSemitones() const final;

	/** Returns the %pitch class that this %chromatic %scale starts on. */
	[[nodiscard]] PitchClass getPitchClassOfRoot() const noexcept final;

	/** Returns 12. */
	[[nodiscard]] int notesPerOctave() const noexcept final;

private:

	PitchClass startingPitchClass { 0 };
};

}  // namespace music::scales

LIMES_END_NAMESPACE
