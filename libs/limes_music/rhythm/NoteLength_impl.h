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
#include <limes_core.h>

namespace limes::music
{

constexpr NoteLength::NoteLength (int numBeats, NoteDuration kind, int dots, bool isGraceNote)
	: duration (kind), num (numBeats), numDots (dots), grace (isGraceNote)
{
	if (grace)
		LIMES_ASSERT (duration.getDuration() >= 8);
}

constexpr NoteLength::NoteLength (const NoteLength& other) noexcept
	: duration (other.duration), num (other.num), numDots (other.numDots), grace (other.grace), tie (other.tie)
{
}

constexpr bool NoteLength::operator== (const NoteLength& other) const noexcept
{
	return num == other.num && duration == other.duration && numDots == other.numDots && grace == other.grace;
}

constexpr bool NoteLength::operator!= (const NoteLength& other) const noexcept
{
	return ! (*this == other);
}

constexpr bool NoteLength::operator> (const NoteLength& other) const noexcept
{
	if (! grace && other.grace)
		return true;

	if (duration < other.duration)
		return false;

	if (num < other.num)
		return false;

	return numDots > other.numDots;
}

constexpr bool NoteLength::operator< (const NoteLength& other) const noexcept
{
	if (grace && ! other.grace)
		return true;

	if (duration > other.duration)
		return false;

	if (num > other.num)
		return false;

	return numDots < other.numDots;
}

constexpr math::Fraction<int> NoteLength::getRelativeValue() const noexcept
{
	math::Fraction<int> frac { num, duration.getDuration() };

	for (auto i = 0; i < numDots; ++i)
	{
		frac.denominator *= 2;
		frac.numerator = (frac.numerator * 2) + 1;
	}

	return frac.reduce();
}

constexpr bool NoteLength::hasStem() const noexcept
{
	return duration.hasStem();
}

constexpr bool NoteLength::noteHeadIsFilled() const noexcept
{
	return duration.noteHeadIsFilled();
}

constexpr NoteDuration NoteLength::getDuration() const noexcept
{
	return duration;
}

constexpr int NoteLength::getNumDots() const noexcept
{
	return numDots;
}

constexpr bool NoteLength::isDotted() const noexcept
{
	return numDots > 0;
}

constexpr bool NoteLength::isGraceNote() const noexcept
{
	return grace;
}

constexpr double NoteLength::getNumNotesOfDuration (const NoteDuration& destDuration) const noexcept
{
	const auto ratio = duration.getRatioToOtherDuration (destDuration);

	auto base = static_cast<double> (num) * ratio;

	for (auto i = 1; i <= numDots; ++i)
		base += (ratio * (1. / static_cast<double> (2 * i)));

	return base;
}

constexpr bool NoteLength::isTied() const noexcept
{
	return tie.hasBeforeConnection() || tie.hasAfterConnection();
}

constexpr bool NoteLength::isFirstNoteInTie() const noexcept
{
	return tie.hasBeforeConnection() && ! tie.hasAfterConnection();
}

constexpr bool NoteLength::isLastNoteInTie() const noexcept
{
	return ! tie.hasAfterConnection() && tie.hasBeforeConnection();
}

constexpr NoteLength& NoteLength::getFirstNoteInTie() const noexcept
{
	return tie.getFirstObjectInList();
}

constexpr NoteLength& NoteLength::getLastNoteInTie() const noexcept
{
	return tie.getLastObjectInList();
}

constexpr void NoteLength::removeTies() noexcept
{
	tie.removeBothConnections();
}

constexpr void NoteLength::tieToNotes (NoteLength* noteBefore, NoteLength* noteAfter) noexcept
{
	LIMES_ASSERT (! grace);

	using LL = LinkedListModel<NoteLength>;

	auto* before = [nb = noteBefore]() -> LL*
	{
		if (nb == nullptr)
			return nullptr;

		return &nb->tie;
	}();

	auto* after = [na = noteAfter]() -> LL*
	{
		if (na == nullptr)
			return nullptr;

		return &na->tie;
	}();

	tie.replaceConnections (before, after);
}

}  // namespace limes::music
