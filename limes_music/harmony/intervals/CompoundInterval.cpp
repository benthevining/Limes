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

#include "CompoundInterval.h"

namespace lemons::music
{

CompoundInterval::CompoundInterval (int octaves, const Interval& simpleInterval) noexcept
	: interval (simpleInterval), numOctaves (octaves)
{
	constexpr auto octave_kind = 8;

	if (interval.getKind() == octave_kind)
	{
		const auto quality_ = interval.getQuality();

		if (quality_ != Interval::Quality::Diminished)
		{
			++numOctaves;
			interval = Interval { 0, quality_ };
		}
	}
}

CompoundInterval::CompoundInterval (int numSemitones) noexcept
{
	numOctaves = numSemitones / semitonesInOctave;
	interval   = Interval::fromNumSemitones (numSemitones - (numOctaves * 12));
}

CompoundInterval CompoundInterval::fromPitches (int midiPitch1, int midiPitch2) noexcept
{
	return CompoundInterval { math::abs (midiPitch2 - midiPitch1) };
}

Interval CompoundInterval::getSimpleInterval() const noexcept
{
	return interval;
}

std::vector<double> CompoundInterval::getCents_JustIntonation() const
{
	const auto octaveCents = static_cast<double> (numOctaves) * 1200.;

	auto array = interval.getCents_JustIntonation();

	for (auto i = 0; i < static_cast<int> (array.size()); ++i)
		array[static_cast<std::vector<double>::size_type> (i)] += octaveCents;

	return array;
}

CompoundInterval CompoundInterval::fromPitches (const Pitch& pitch1, const Pitch& pitch2) noexcept
{
	return CompoundInterval { std::abs (pitch2.getRoundedMidiPitch() - pitch1.getRoundedMidiPitch()) };
}

// String CompoundInterval::getStringDescription (bool useShort) const
//{
//	if (useShort)
//		return interval.getQualityAsString (true) + String (getKind());
//
//	return interval.getQualityAsString (false) + " " + String (getKind());
// }

int CompoundInterval::getKind() const noexcept
{
	const auto simple = [kind = interval.getKind()]
	{
		if (kind > 0)
			return kind - 1;

		return kind;
	}();

	return (numOctaves * 7) + simple + 1;
}

Interval::Quality CompoundInterval::getQuality() const noexcept
{
	return interval.getQuality();
}

int CompoundInterval::getNumSemitones() const noexcept
{
	return interval.getNumSemitones() + (numOctaves * semitonesInOctave);
}

}  // namespace lemons::music
