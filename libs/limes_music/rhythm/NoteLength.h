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
#include "NoteDuration.h"  // for NoteDuration
#include <limes_core.h>	   // for Fraction
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace music
{

struct LIMES_EXPORT NoteLength final
{
public:

	constexpr explicit NoteLength (int numBeats, NoteDuration kind, int dots = 0, bool isGraceNote = false);

	LIMES_DEFAULT_MOVABLE (NoteLength)
	LIMES_DEFAULT_COPYABLE (NoteLength)

	[[nodiscard]] constexpr bool operator== (const NoteLength& other) const noexcept;

	[[nodiscard]] constexpr bool operator!= (const NoteLength& other) const noexcept;

	[[nodiscard]] constexpr bool operator> (const NoteLength& other) const noexcept;

	[[nodiscard]] constexpr bool operator< (const NoteLength& other) const noexcept;

	[[nodiscard]] constexpr math::Fraction<int> getRelativeValue() const noexcept;

	[[nodiscard]] math::Fraction<int> getRelativeValueForWholeTie() const noexcept;

	[[nodiscard]] constexpr bool hasStem() const noexcept;

	[[nodiscard]] constexpr bool noteHeadIsFilled() const noexcept;

	[[nodiscard]] constexpr int getNumDots() const noexcept;

	[[nodiscard]] constexpr bool isDotted() const noexcept;

	[[nodiscard]] constexpr NoteDuration getDuration() const noexcept;

	[[nodiscard]] constexpr bool isGraceNote() const noexcept;

	[[nodiscard]] constexpr double getNumNotesOfDuration (const NoteDuration& destDuration) const noexcept;

	[[nodiscard]] double getNumNotesOfDurationForWholeTie (const NoteDuration& destDuration) const noexcept;

	[[nodiscard]] constexpr bool isTied() const noexcept;

	[[nodiscard]] constexpr NoteLength& getFirstNoteInTie() const noexcept;
	[[nodiscard]] constexpr NoteLength& getLastNoteInTie() const noexcept;

	constexpr void removeTies() noexcept;

	constexpr void tieToNotes (NoteLength* noteBefore, NoteLength* noteAfter) noexcept;

	[[nodiscard]] constexpr bool isFirstNoteInTie() const noexcept;
	[[nodiscard]] constexpr bool isLastNoteInTie() const noexcept;

private:

	NoteDuration duration { 4 };

	int num { 1 }, numDots { 0 };

	bool grace { false };

	ds::LinkedListModel<NoteLength> tie { *this };
};

}  // namespace music

LIMES_END_NAMESPACE

#include "NoteLength_impl.h"
