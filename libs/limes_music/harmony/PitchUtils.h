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

LIMES_BEGIN_NAMESPACE

namespace music
{

LIMES_EXPORT constexpr auto semitonesInOctave = 12;

/** Returns the octave number of a given MIDI pitch.
 */
LIMES_EXPORT constexpr int octaveNumberOfMidiNote (int midiNote) noexcept
{
	LIMES_ASSERT (midiNote >= 0);
	return midiNote / 12 - 1;
}

/** Returns the lowest note of a given MIDI octave number (that octave's C key).
 */
LIMES_EXPORT constexpr int lowestNoteOfMidiOctave (int octaveNumber) noexcept
{
	LIMES_ASSERT (octaveNumber >= 0);
	return (octaveNumber + 1) * 12;
}

/** Returns an array of pitch class objects representing each sharp in the order they appear in key signatures -- F, C, G, D, A, E, B.
 */
LIMES_EXPORT consteval std::array<PitchClass, 7> getSharpsOrder() noexcept
{
	return { PitchClass { 6 }, PitchClass { 1 }, PitchClass { 8 }, PitchClass { 3 }, PitchClass { 10 }, PitchClass { 5 }, PitchClass { 12 } };
}

/** Returns an array of pitch class objects representing each flat in the order they appear in key signatures -- B, E, A, D, G, C, F.
 */
LIMES_EXPORT consteval std::array<PitchClass, 7> getFlatsOrder() noexcept
{
	return { PitchClass { 10 }, PitchClass { 3 }, PitchClass { 8 }, PitchClass { 1 }, PitchClass { 6 }, PitchClass { 11 }, PitchClass { 4 } };
}

}  // namespace music

LIMES_END_NAMESPACE
