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

#include "Scale.h"
#include <limes_export.h>

namespace limes::music::scales
{

/** Represents an octatonic scale.
	Octatonic scales are scales that alternate between whole and half steps; thus there are two variations -- one starting with a whole step, and one starting with a half step.
 */
class LIMES_EXPORT Octatonic final : public Scale
{
public:

	/** Creates an octatonic scale object.
		@param pitchClassToStartOn The pitch class of the root of this scale.
		@param startWithSemitone True if this scale should start with a half step; otherwise, it will start with a whole step.
	 */
	constexpr explicit Octatonic (const PitchClass& pitchClassToStartOn, bool startWithSemitone) noexcept
		: startWithHalfStep (startWithSemitone), pitchClassOfRoot (pitchClassToStartOn)
	{
	}

	/** Creates an octatonic scale object.
		@param noteToStartOn The MIDI note to start on.
		@param startWithSemitone True if this scale should start with a half step; otherwise, it will start with a whole step.
	 */
	constexpr explicit Octatonic (int noteToStartOn, bool startWithSemitone) noexcept
		: startWithHalfStep (startWithSemitone), pitchClassOfRoot (noteToStartOn)
	{
	}

	/** Copy constructor. */
	constexpr Octatonic (const Octatonic& other) noexcept
		: startWithHalfStep (other.startWithHalfStep), pitchClassOfRoot (other.pitchClassOfRoot)
	{
	}

	/** Assignment operator. */
	constexpr Octatonic& operator= (const Octatonic& other) noexcept
	{
		startWithHalfStep = other.startWithHalfStep;
		pitchClassOfRoot  = other.pitchClassOfRoot;
		return *this;
	}

	/** Creates an octatonic scale object from a string description of one.
		@see getStringDescription()
	 */
	//	[[nodiscard]] static Octatonic fromStringDescription (const String& string);

	/** Returns true if the other octatonic scale is equal to this one. */
	[[nodiscard]] bool operator== (const Octatonic& other) const;

	/** Returns true if the other octatonic scale is not equal to this one. */
	[[nodiscard]] bool operator!= (const Octatonic& other) const;

	/** Returns a sequence of alternating 2's and 1's.
	 */
	[[nodiscard]] vector<int> getIntervalsAsSemitones() const final;

	/** Returns the pitch class of the root of this scale. */
	[[nodiscard]] PitchClass getPitchClassOfRoot() const noexcept final;

	/** Returns true if this octatonic scale starts with a half step. */
	[[nodiscard]] constexpr bool startsWithHalfStep() const noexcept
	{
		return startWithHalfStep;
	}

	/** Returns a string description of this scale, for example, "D whole/half octatonic".
		@see fromStringDescription()
	 */
	//	[[nodiscard]] String getStringDescription() const final;

	/** Returns 8. */
	[[nodiscard]] int notesPerOctave() const noexcept final;

private:

	bool startWithHalfStep { true };

	PitchClass pitchClassOfRoot { 0 };
};

}  // namespace limes::music::scales
