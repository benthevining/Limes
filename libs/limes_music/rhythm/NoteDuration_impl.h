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

namespace limes::music
{

constexpr NoteDuration::NoteDuration (int kind)
	: duration (kind)
{
	LIMES_ASSERT (duration > 0 && (duration == 1 || math::isPowerOf2 (duration)));
}

constexpr NoteDuration::NoteDuration (const NoteDuration& other) noexcept
	: duration (other.duration)
{
}

constexpr NoteDuration& NoteDuration::operator= (const NoteDuration& other) noexcept
{
	duration = other.duration;

	return *this;
}

constexpr bool NoteDuration::operator== (const NoteDuration& other) const noexcept
{
	return duration == other.duration;
}

constexpr bool NoteDuration::operator!= (const NoteDuration& other) const noexcept
{
	return ! (*this == other);
}

constexpr bool NoteDuration::operator> (const NoteDuration& other) const noexcept
{
	return duration > other.duration;
}

constexpr bool NoteDuration::operator< (const NoteDuration& other) const noexcept
{
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

constexpr int NoteDuration::getDuration() const noexcept
{
	return duration;
}

constexpr double NoteDuration::getRatioToOtherDuration (const NoteDuration& other) const noexcept
{
	return static_cast<double> (other.duration) / static_cast<double> (duration);
}

}  // namespace limes::music
