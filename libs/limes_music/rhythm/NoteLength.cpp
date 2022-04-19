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

#include "NoteLength.h"

#include <algorithm>				// for for_each
#include <limes_core.h>				// for Fraction::operator+=, Fraction::...
#include "NoteDuration.h"			// for NoteDuration
#include <limes_data_structures.h>	// for LinkedListModel
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace music
{

double NoteLength::getNumNotesOfDurationForWholeTie (const NoteDuration& destDuration) const noexcept
{
	auto val = getFirstNoteInTie().getNumNotesOfDuration (destDuration);

	std::for_each (tie.begin(), tie.end(),
				   [&val, dur = destDuration] (const NoteLength& l)
				   { val += l.getNumNotesOfDuration (dur); });

	return val;
}

math::Fraction<int> NoteLength::getRelativeValueForWholeTie() const noexcept
{
	auto frac = getFirstNoteInTie().getRelativeValue();

	std::for_each (tie.begin(), tie.end(),
				   [&frac] (const NoteLength& l)
				   { frac += l.getRelativeValue(); });

	return frac.reduce();
}

}  // namespace music

LIMES_END_NAMESPACE
