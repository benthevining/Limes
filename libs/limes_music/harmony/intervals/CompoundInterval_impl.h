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

// IWYU pragma: private
// IWYU pragma: friend CompoundInterval.h

#pragma once
#include <limes_core.h>
#include <limes_namespace.h>

/** @file
	This file contains implementation details for the CompoundInterval class.
	@see CompoundInterval.h
	@ingroup music_intervals
 */

LIMES_BEGIN_NAMESPACE

namespace music
{

/// @cond

constexpr CompoundInterval::CompoundInterval (int kindToUse, Interval::Quality qualityToUse) noexcept
{
	LIMES_ASSERT (Interval::isValidQualityForKind (qualityToUse, kindToUse));

	if (kindToUse <= 8)
	{
		numOctaves = 0;
		interval   = Interval { kindToUse, qualityToUse };
		return;
	}

	numOctaves = (kindToUse - 1) / 7;

	const auto intervalKind = kindToUse - (7 * numOctaves);

	if (intervalKind <= 0)
		interval = Interval();
	else
		interval = Interval { intervalKind, qualityToUse };
}

constexpr bool CompoundInterval::operator== (const CompoundInterval& other) const noexcept
{
	return numOctaves == other.numOctaves && interval == other.interval;
}

constexpr bool CompoundInterval::operator!= (const CompoundInterval& other) const noexcept
{
	return ! (*this == other);
}

constexpr bool CompoundInterval::operator> (const CompoundInterval& other) const noexcept
{
	if (numOctaves < other.numOctaves)
		return false;

	return interval > other.interval;
}

constexpr bool CompoundInterval::operator<(const CompoundInterval& other) const noexcept
{
	if (numOctaves > other.numOctaves)
		return false;

	return interval < other.interval;
}

constexpr int CompoundInterval::getNumOctaves() const noexcept
{
	return numOctaves;
}

constexpr bool CompoundInterval::hasSameSimpleInterval (const CompoundInterval& other) const noexcept
{
	return interval == other.interval;
}

/// @endcond

}  // namespace music

LIMES_END_NAMESPACE
