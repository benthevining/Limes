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
#include "../Pitch.h"	   // for Pitch
#include <limes_core.h>	   // for Fraction
#include <limes_namespace.h>

/** @defgroup music_intervals Intervals
	Utilities for working with musical pitch intervals.
	@ingroup music_harmony
 */

/** @dir libs/limes_music/harmony/intervals
	This directory contains utilities for working with musical intervals.
	@ingroup music_intervals
 */

/** @file
	This file defines the Interval class.
	@ingroup music_intervals
 */

LIMES_BEGIN_NAMESPACE

namespace music
{

/** A class that represents a musical interval between two pitches.
	This class is meant for intervals that are an octave or smaller, and that can be expressed as a whole number of semitones.
	(Technically, the largest interval representable by this class is an augmented octave.)
	To represent intervals larger than an octave, use the CompoundInterval class.
	@ingroup music_intervals
	@see CompoundInterval, MicrotonalInterval
 */
class LIMES_EXPORT Interval
{
public:

	/** Destructor. */
	virtual ~Interval() = default;

	LIMES_CONSTEXPR_MOVABLE (Interval);
	LIMES_CONSTEXPR_COPYABLE (Interval);

	/** Describes the quality of the interval.
		Note that not all possible quality values are valid for every possible interval type; a "major fifth" or a "perfect third" do not exist.
		@see isValidQualityForKind(), getQuality()
	 */
	enum class Quality
	{
		Diminished,
		Minor,
		Major,
		Augmented,
		Perfect
	};

	/** @name Constructors */
	///@{

	/** Creates an interval representing a perfect unison. */
	constexpr Interval() noexcept;

	/** Creates an interval with a specified kind and quality.
		If the kind or quality parameters are invalid, an assertion will be thrown.
		@param kindToUse The kind of interval to create -- 0 for unison, 2 for second, 3 for third, etc. Note that 1 is not a valid value, and values greater than 8 will be reduced to their equivalent intervals that are within one octave (ie, using the modulus operator).
		@param qualityToUse The quality of interval to create. Note that not all possible quality values are valid for every possible interval type; a "major fifth" or a "perfect third" do not exist.
		@see isValidQualityForKind()
	 */
	constexpr explicit Interval (int kindToUse, Quality qualityToUse) noexcept;

	///@}

	/** @name Creation functions */
	///@{

	/** Creates an interval from a number of semitones.
		This uses reasonable defaults, always preferring major, minor, or perfect intervals when available instead of augmented or diminished.
		@see fromPitches
	 */
	[[nodiscard]] static Interval fromNumSemitones (int semitones) noexcept;

	/** Creates an interval from the distance between two pitch objects.
		This uses reasonable defaults, always preferring major, minor, or perfect intervals when available instead of augmented or diminished.
		@see fromNumSemitones
	 */
	[[nodiscard]] static Interval fromPitches (const Pitch& pitch1, const Pitch& pitch2) noexcept;

	/** Creates an interval from the distance between two MIDI pitches.
		This uses reasonable defaults, always preferring major, minor, or perfect intervals when available instead of augmented or diminished.
		@see fromNumSemitones
	 */
	[[nodiscard]] static Interval fromPitches (int midiPitch1, int midiPitch2) noexcept;

	///@}

	/** @name Equality comparisons */
	///@{

	/** Returns true if the two intervals are exactly equal -- that is, have the same kind and quality.
		Note that this will return false for intervals that represent the same number of semitones but are spelled differently.
		@see isEnharmonicTo()
	 */
	[[nodiscard]] constexpr bool operator== (const Interval& other) const noexcept;

	/** Returns true if the two intervals are not exactly equal.
		Note that this will return true for intervals that represent the same number of semitones but are spelled differently.
		@see isEnharmonicTo()
	 */
	[[nodiscard]] constexpr bool operator!= (const Interval& other) const noexcept;

	/** Returns true if the two intervals represent the same number of semitones, regardless of their enharmonic spelling.
		For example, a diminished fifth is enharmonic to an augmented fourth.
	 */
	[[nodiscard]] bool isEnharmonicTo (const Interval& other) const noexcept;

	///@}

	/** Returns true if this interval is semantically larger than the other one, taking enharmonic spellings into account.
		For example, this will tell you that a diminished fifth is "larger" than an augmented fourth.
	 */
	[[nodiscard]] bool operator> (const Interval& other) const noexcept;

	/** Returns true if this interval is semantically smaller than the other one, taking enharmonic spellings into account.
		For example, this will tell you that an augmented fourth is "smaller" than a diminished fifth.
	 */
	[[nodiscard]] bool operator< (const Interval& other) const noexcept;

	/** @name Addition */
	///@{

	/** Adds another interval object to this one. */
	[[nodiscard]] Interval operator+ (const Interval& other) const noexcept;

	/** Adds a number of semitones to this interval. */
	[[nodiscard]] Interval operator+ (int semitonesToAdd) const noexcept;

	/** Adds another interval object to this one. */
	Interval& operator+= (const Interval& other) noexcept;

	/** Adds a number of semitones to this interval. */
	Interval& operator+= (int semitonesToAdd) noexcept;

	/** Increments this interval by one semitone, preserving the kind by manipulating the quality where possible.
		For example, a major second will increment to an augmented second, rather than a minor third.
		A perfect octave will increment to an augmented octave.
		Calling the increment operator on an augmented octave will do nothing.
	 */
	constexpr Interval& operator++() noexcept;

	///@}

	/** @name Subtraction */
	///@{

	/** Subtracts another interval object from this one. */
	[[nodiscard]] Interval operator- (const Interval& other) const noexcept;

	/** Subtracts a number of semitones from this interval. */
	[[nodiscard]] Interval operator- (int semitonesToSubtract) const noexcept;

	/** Subtracts another interval object from this one. */
	Interval& operator-= (const Interval& other) noexcept;

	/** Subtracts a number of semitones from this interval. */
	Interval& operator-= (int semitonesToSubtract) noexcept;

	/** Decrements this interval by one semitone, preserving the kind by manipulating the quality where possible.
		For example, a minor second will decrement to a diminished second, rather than a perfect unison.
		In the special case of a diminished second, it will decrement to a perfect unison (even though these two intervals are enharmonically equivalent).
		Calling the decrement operator on a perfect unison will do nothing.
	 */
	constexpr Interval& operator--() noexcept;

	///@}

	/** Returns the interval that is the inverse of this one, taking enharmonic spellings into account.
		An interval and its inverse add together to form a perfect octave -- for example, a major second's inverse is a minor seventh, a perfect fourth's inverse is a perfect fifth, etc.
	 */
	[[nodiscard]] Interval getInverse() const noexcept;

	/** Returns the number of semitones that this interval represents.
		@see getKind()
	 */
	[[nodiscard]] virtual int getNumSemitones() const noexcept;

	/** Returns the kind of this interval as an integer.
		0 represents unison, 2 is a second, 3 is a third, etc.
		Note that this will never return 1.
		@see getNumSemitones()
	 */
	[[nodiscard]] virtual int getKind() const noexcept;

	/** Returns the quality of this interval. */
	[[nodiscard]] virtual Quality getQuality() const noexcept;

	/** @name Cents measurements */
	///@{

	/** Returns an array of possible cents values this interval can represent in just intonation, taking enharmonic spelling into account.
		@see getJustRatios, getCents_EqualTemperament
	 */
	[[nodiscard]] virtual std::vector<double> getCents_JustIntonation() const;

	/** Returns an array of fraction objects representing possible just intervals for this interval.
		@see getCents_JustIntonation, getCents_EqualTemperament
	 */
	[[nodiscard]] std::vector<math::Fraction<int>> getJustRatios() const;

	/** Returns the number of cents this interval represents in equal temperament.
		Note that, unlike just intonation, enharmonic spellings represent equal numbers of cents in equal temperament.
		@see getCents_JustIntonation
	 */
	[[nodiscard]] int getCents_EqualTemperament() const noexcept;

	///@}

	/** Returns a new pitch object that represents the note this interval's distance above or below the passed pitch object.
		@param other The pitch object to start from.
		@param above When true, the returned pitch will be this interval's distance above the passed pitch. When false, the returned pitch will be this interval's distance below the passed pitch.
	 */
	[[nodiscard]] Pitch applyToPitch (const Pitch& other, bool above) const noexcept;

	/** Returns true if the quality is valid for the given kind of interval.
		For example, a perfect fourth or fifth is valid, while a perfect third or perfect second is not. A major or minor third is valid, while a major fourth or minor fifth is not.
		Note the special case that a diminished unison is invalid; unisons can only be perfect or augmented.
		Note that a kind of 1 is invalid.

		Here is a table of all valid kind/quality combinations

		Kind as integer | Name    | Valid qualities
		--------------- | ------- | ------------------------------------
		0               | Unison  | Perfect, augmented
		2               | Second  | Diminished, minor, major, augmented
		3               | Third   | Diminished, minor, major, augmented
		4               | Fourth  | Diminished, perfect, augmented
		5               | Fifth   | Diminished, perfect, augmented
		6               | Sixth   | Diminished, minor, major, augmented
		7               | Seventh | Diminished, minor, major, augmented
		8               | Octave  | Diminished, perfect, augmented

	 */
	[[nodiscard]] static constexpr bool isValidQualityForKind (Quality quality, int kind) noexcept;

private:

	[[nodiscard]] constexpr bool intervalIsPerfectKind() const noexcept;

	Quality quality { Quality::Major };

	int kind { 0 };
};


/** Adds an interval to the given pitch and returns a new pitch object at the resulting pitch.
	@ingroup music_intervals
 */
LIMES_EXPORT Pitch operator+ (const Pitch& pitch, const Interval& interval) noexcept;

/** Subtracts an interval from the given pitch and returns a new pitch object at the resulting pitch.
	@ingroup music_intervals
 */
LIMES_EXPORT Pitch operator- (const Pitch& pitch, const Interval& interval) noexcept;

}  // namespace music

LIMES_END_NAMESPACE

#include "Interval_impl.h"	// IWYU pragma: export
