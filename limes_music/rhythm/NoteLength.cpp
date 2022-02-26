/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

#include "NoteLength.h"

namespace lemons::music
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

}  // namespace lemons::music
