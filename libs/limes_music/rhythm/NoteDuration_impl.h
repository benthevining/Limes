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
#include "NoteDuration.h"
#include <limes_namespace.h>

/** @file
	This file contains implementation details for the NoteDuration class.
	@ingroup music_rhythm
 */

LIMES_BEGIN_NAMESPACE

namespace music
{

/// @cond

constexpr NoteDuration::NoteDuration (int kind, int dots)
	: duration (kind), numDots (dots)
{
	LIMES_ASSERT (duration > 0 && (duration == 1 || math::isPowerOf2 (duration)));
	LIMES_ASSERT (numDots >= 0);
}

constexpr bool NoteDuration::operator== (const NoteDuration& other) const noexcept
{
	return duration == other.duration && numDots == other.numDots;
}

constexpr bool NoteDuration::operator!= (const NoteDuration& other) const noexcept
{
	return ! (*this == other);
}

constexpr bool NoteDuration::operator> (const NoteDuration& other) const noexcept
{
	if (duration == other.duration)
		return numDots > other.numDots;

	return duration > other.duration;
}

constexpr bool NoteDuration::operator<(const NoteDuration& other) const noexcept
{
	if (duration == other.duration)
		return numDots < other.numDots;

	return duration < other.duration;
}

constexpr NoteDuration& NoteDuration::operator++() noexcept
{
	duration = [dur = duration]() -> int
	{
		if (dur == 1)
			return 2;

		return dur * 2;
	}();

	return *this;
}

constexpr NoteDuration& NoteDuration::operator--() noexcept
{
	duration = [dur = duration]() -> int
	{
		if (dur == 1)
			return dur;

		if (dur == 2)
			return 1;

		return dur / 2;
	}();

	return *this;
}

constexpr bool NoteDuration::hasStem() const noexcept
{
	return duration > 1;
}

constexpr bool NoteDuration::noteHeadIsFilled() const noexcept
{
	return duration > 2;
}

constexpr int NoteDuration::getNumDots() const noexcept
{
	return numDots;
}

constexpr double NoteDuration::getDuration() const noexcept
{
	auto dur = 1. / static_cast<double> (duration);

	auto multiple = 0.5;

	for (auto i = 0; i < numDots; ++i)
	{
		dur *= (1. + multiple);

		multiple /= 2.;
	}

	return dur;
}

constexpr double NoteDuration::getRatioToOtherDuration (const NoteDuration& other) const noexcept
{
	return other.getDuration() / getDuration();
}

/// @endcond

}  // namespace music

LIMES_END_NAMESPACE
