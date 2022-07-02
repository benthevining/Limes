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

#include <limes_export.h>
#include <limes_namespace.h>
#include <limes_core.h>

/** @defgroup music_rhythm Rhythm
	Utilities for working with rhythm and musical durations.
	@ingroup limes_music
 */

/** @dir libs/limes_music/rhythm
	This directory contains utilities for working with rhythm and musical durations.
	@ingroup music_rhythm
 */

/** @file
	This file defines the NoteDuration class.
	@ingroup music_rhythm
 */

LIMES_BEGIN_NAMESPACE

namespace music
{

/** This class represents the duration of a kind of written musical note -- eighth, quarter, half, etc.
	@ingroup music_rhythm
	@see NoteLength

	@todo triplets
	@todo special: double whole note
 */
struct LIMES_EXPORT NoteDuration final
{
public:
	/** Creates a NoteDuration of the desired kind.
		1 represents a whole note, 2 represents a half note, 4 is a quarter note, 8 is an eighth note, and so on.
	 */
	constexpr explicit NoteDuration (int kind, int dots = 0);

	LIMES_CONSTEXPR_MOVABLE (NoteDuration)
	LIMES_CONSTEXPR_COPYABLE (NoteDuration)

	/** Returns true if the two note durations are equal. */
	[[nodiscard]] constexpr bool operator== (const NoteDuration& other) const noexcept;

	/** Returns true if the two note durations are not equal. */
	[[nodiscard]] constexpr bool operator!= (const NoteDuration& other) const noexcept;

	/** Returns true if this duration is longer than the other one. Dots are taken into account. */
	[[nodiscard]] constexpr bool operator> (const NoteDuration& other) const noexcept;

	/** Returns true if this duration is shorter than the other one. Dots are taken into account. */
	[[nodiscard]] constexpr bool operator<(const NoteDuration& other) const noexcept;

	/** Increments the duration value to the next shortest notehead kind.
		Whole notes are incremented to half notes, half notes to quarter notes, quarter notes to eighth notes, etc.
		The number of dots is unaffected.
	 */
	constexpr NoteDuration& operator++() noexcept;

	/** Decrements the duration value to the next longest notehead kind.
		Half notes are decremented to whole notes, quarter notes to half notes, eighth notes to quarter notes, etc.
		Decrementing a whole note has no effect.
		The number of dots is unaffected.
	 */
	constexpr NoteDuration& operator--() noexcept;

	/** Returns true if the notehead representing this duration has a stem.
		This is only false for whole notes (1).
	 */
	[[nodiscard]] constexpr bool hasStem() const noexcept;

	/** Returns true if the notehead representing this duration is filled.
		This is only false for whole notes and half notes (1 and 2).
	 */
	[[nodiscard]] constexpr bool noteHeadIsFilled() const noexcept;

	/** Returns the number of dots this note duration has. */
	[[nodiscard]] constexpr int getNumDots() const noexcept;

	/** Returns the percentage of a whole note represented by this object.
		1 represents a whole note, 2 represents a half note, 4 is a quarter note, 8 is an eighth note, and so on.
		This initial integer value is scaled according to the number of dots this duration has, and returned as a percentage of a whole note.
		A whole note will return 1, a half note 0.5, a quarter note 0.25, etc.
		@see getRatioToOtherDuration()
	 */
	[[nodiscard]] constexpr double getDuration() const noexcept;

	/** Returns the ratio of this duration to the other one.
		Dots are taken into account.
		@see getDuration()
	 */
	[[nodiscard]] constexpr double getRatioToOtherDuration (const NoteDuration& other) const noexcept;

private:
	int duration { 4 };

	int numDots { 0 };
};

}  // namespace music

LIMES_END_NAMESPACE

#include "./NoteDuration_impl.h"
