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

#include "PitchClass.h"
#include <array>
#include <cassert>
#include <limes_core.h>
#include <limes_export.h>
#include <limes_namespace.h>

/** @defgroup music_harmony Harmony
	Utilities for working with pitch and harmony related music theory.
	@ingroup limes_music
 */

/** @dir libs/limes_music/harmony
	This directory contains utilities for working with pitch and harmony related music theory.
	@ingroup music_harmony
 */

/** @file
	This file contains miscellaneous pitch utilities.
	@ingroup music_harmony
 */

LIMES_BEGIN_NAMESPACE

/** This namespace contains music theory utilities.
	@ingroup limes_music
 */
namespace music
{

/** @ingroup music_harmony
	@{
 */

/** Integer constant representing the number of semitones in an octave (12). */
LIMES_EXPORT constinit const auto semitonesInOctave = 12;

/** Returns the octave number of a given MIDI pitch.
	The formula is @f$ O=\lfloor\frac{note}{12}-1\rfloor @f$.
	This formula puts note 69 in octave 4.

	Here are the MIDI octaves, as defined by this formula:

	Octave number | Note number range | Pitch range
	------------- | ----------------- | ------------
	0             | 12 - 23           | C0 - B0
	1             | 24 - 35           | C1 - B1
	2             | 36 - 47           | C2 - B2
	3             | 48 - 59           | C3 - B3
	4             | 60 - 71           | C4 - B4
	5             | 72 - 83           | C5 - B5
	6             | 84 - 95           | C6 - B6
	7             | 96 - 107          | C7 - B7

	@see lowestNoteOfMidiOctave
 */
LIMES_EXPORT constexpr int octaveNumberOfMidiNote (int midiNote) noexcept
{
	LIMES_ASSERT (midiNote >= 0);
	return std::floor (static_cast<float> (midiNote) / 12.f - 1.f);
}

/** Returns the lowest note of a given MIDI octave number (that octave's C key).
	The formula is @f$ note=(O+1)*12 @f$.
	This formula puts 60 as the lowest note of octave 4.

	Here are the MIDI octaves, as defined by this formula:

	Octave number | Note number range | Pitch range
	------------- | ----------------- | ------------
	0             | 12 - 23           | C0 - B0
	1             | 24 - 35           | C1 - B1
	2             | 36 - 47           | C2 - B2
	3             | 48 - 59           | C3 - B3
	4             | 60 - 71           | C4 - B4
	5             | 72 - 83           | C5 - B5
	6             | 84 - 95           | C6 - B6
	7             | 96 - 107          | C7 - B7

	@see octaveNumberOfMidiNote
 */
LIMES_EXPORT constexpr int lowestNoteOfMidiOctave (int octaveNumber) noexcept
{
	LIMES_ASSERT (octaveNumber >= 0);
	return (octaveNumber + 1) * 12;
}

/** Returns an array of PitchClass objects representing each sharp in the order they appear in key signatures -- F, C, G, D, A, E, B.
	Each pitch class represents the raised pitch class -- so the first two pitch classes in the returned array are 6 and 1, for \c F# and \c C# , respectively.
 */
LIMES_EXPORT consteval std::array<PitchClass, 7> getSharpsOrder() noexcept
{
	return { PitchClass { 6 }, PitchClass { 1 }, PitchClass { 8 }, PitchClass { 3 }, PitchClass { 10 }, PitchClass { 5 }, PitchClass { 12 } };
}

/** Returns an array of pitch class objects representing each flat in the order they appear in key signatures -- B, E, A, D, G, C, F.
	Each pitch class represents the lowered pitch class -- so the first two pitch classes in the returned array are 10 and 3, for \c Bb and \c Eb , respectively.
 */
LIMES_EXPORT consteval std::array<PitchClass, 7> getFlatsOrder() noexcept
{
	return { PitchClass { 10 }, PitchClass { 3 }, PitchClass { 8 }, PitchClass { 1 }, PitchClass { 6 }, PitchClass { 11 }, PitchClass { 4 } };
}

/** @} */

}  // namespace music

LIMES_END_NAMESPACE
