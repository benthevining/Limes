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

#include <limes_export.h>  // for LIMES_EXPORT
#include <vector>		   // for vector
#include "Interval.h"	   // for Interval, Interval::Quality
#include <limes_namespace.h>
#include <limes_core.h>

/** @file
	This file defines the CompoundInterval class.
	@ingroup music_intervals
 */

LIMES_BEGIN_NAMESPACE

namespace music
{
class Pitch;
}


namespace music
{

/** A class for representing intervals larger than an octave.

	Internally, the %interval is represented as a number of whole octaves and an Interval object.
	This class is capable of representing intervals smaller than an octave, but if you know your %interval is going to be smaller than an octave, prefer to use the Interval class directly.

	@ingroup music_intervals
	@see Interval, MicrotonalInterval
 */
class LIMES_EXPORT CompoundInterval final : public Interval
{
public:

	/** @name Constructors */
	///@{
	/** Creates a compound %interval with the given number of octaves and the given simple %interval component.
		If the simple %interval is a perfect or augmented octave, the number of octaves will be incremented and the simple %interval component will be reduced to a perfect or augmented unison.
	 */
	explicit CompoundInterval (int octaves, const Interval& simpleInterval) noexcept;

	/** Creates a compound %interval from a given kind and quality, eg, major ninth, etc. */
	constexpr explicit CompoundInterval (int kindToUse, Interval::Quality qualityToUse) noexcept;

	/** Constructs a compound %interval with the given number of semitones. */
	explicit CompoundInterval (int numSemitones) noexcept;
	///@}

	/** @name Creation functions */
	///@{
	/** Returns a compound %interval object representing the distance between two MIDI notes. */
	[[nodiscard]] static CompoundInterval fromPitches (int midiPitch1, int midiPitch2) noexcept;

	/** Returns a compound %interval object representing the distance between two pitch objects. */
	[[nodiscard]] static CompoundInterval fromPitches (const Pitch& pitch1, const Pitch& pitch2) noexcept;
	///@}

	LIMES_CONSTEXPR_MOVABLE (CompoundInterval)
	LIMES_CONSTEXPR_COPYABLE (CompoundInterval)

	/** Returns true if the two compound intervals are exactly equal; that is, have the same number of octaves, and their simple %interval components are exactly equal in kind and quality. */
	[[nodiscard]] constexpr bool operator== (const CompoundInterval& other) const noexcept;

	/** Returns true if the two intervals are not exactly equal. */
	[[nodiscard]] constexpr bool operator!= (const CompoundInterval& other) const noexcept;

	/** Returns true if this %interval is semantically larger than the other one, taking enharmonic spellings into account. */
	[[nodiscard]] constexpr bool operator> (const CompoundInterval& other) const noexcept;

	/** Returns true if this %interval is semantically smaller than the other one, taking enharmonic spellings into account. */
	[[nodiscard]] constexpr bool operator< (const CompoundInterval& other) const noexcept;

	/** Returns the number of whole octaves contained in this compound %interval. */
	[[nodiscard]] constexpr int getNumOctaves() const noexcept;

	/** Returns the simple %interval component of this compound %interval.
		The %interval object returned by this function will always be a diminished octave or smaller, and may be a perfect unison.
		@see hasSameSimpleInterval()
	 */
	[[nodiscard]] Interval getSimpleInterval() const noexcept;

	/** Returns the number of semitones this compound %interval represents. */
	[[nodiscard]] int getNumSemitones() const noexcept final;

	/** Returns true if the two compound intervals have the same sub-octave component, regardless of how many octaves either compound %interval spans.
		For example, a major tenth and a major seventeenth would return true, because they are both compound major thirds.
		@see getSimpleInterval()
	 */
	[[nodiscard]] constexpr bool hasSameSimpleInterval (const CompoundInterval& other) const noexcept;

	/** Returns the quality of this compound %interval.
		The quality of compound intervals is determined by the quality of their simple %interval components; this is identical to calling:
		@code
		compoundInterval.getSimpleInterval().getQuality();
		@endcode
	 */
	[[nodiscard]] Interval::Quality getQuality() const noexcept final;

	/** Returns the kind of this compound %interval as an integer, eg, 9, 10, etc. */
	[[nodiscard]] int getKind() const noexcept final;

	/** Returns an array of possible cents values this %interval can represent in just intonation, taking enharmonic spelling into account. */
	[[nodiscard]] std::vector<double> getCents_JustIntonation() const final;

private:

	Interval interval;

	int numOctaves { 0 };
};


}  // namespace music

LIMES_END_NAMESPACE

#include "CompoundInterval_impl.h"	// IWYU pragma: export
