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

#pragma once
#include <limes_core.h>

namespace lemons::music
{

constexpr bool Interval::isValidQualityForKind (Quality quality, int kind) noexcept
{
	const auto baseKind = [k = kind]
	{
		const auto base = k % 8;

		if (k > 8)
			return base + (k / 8);

		return base;
	}();

	if (baseKind == 0 || baseKind == 4 || baseKind == 5)
	{
		const auto base = ! (quality == Quality::Major || quality == Quality::Minor);

		if (kind == 0)
			return base && quality != Quality::Diminished;

		return base;
	}

	return quality != Quality::Perfect;
}

constexpr bool Interval::intervalIsPerfectKind() const noexcept
{
	return kind == 0 || kind == 4 || kind == 5 || kind == 8;
}

constexpr Interval::Interval (int kindToUse, Quality qualityToUse) noexcept
	: quality (qualityToUse), kind (kindToUse % 9)
{
	LIMES_ASSERT (kind != 1);
	LIMES_ASSERT (kind >= 0 && kind <= 8);
	LIMES_ASSERT (isValidQualityForKind (quality, kind));
}

constexpr Interval::Interval() noexcept
	: Interval (0, Quality::Perfect)
{
}

constexpr Interval::Interval (const Interval& other) noexcept
	: Interval (other.kind, other.quality)
{
}

constexpr Interval& Interval::operator= (const Interval& other) noexcept
{
	kind	= other.kind;
	quality = other.quality;
	return *this;
}

constexpr bool Interval::operator== (const Interval& other) const noexcept
{
	return kind == other.kind && quality == other.quality;
}

constexpr bool Interval::operator!= (const Interval& other) const noexcept
{
	return ! (*this == other);
}

constexpr Interval& Interval::operator++() noexcept
{
	if (kind == 8 && quality == Quality::Augmented)
		return *this;

	auto incrementKind = [this]
	{
		const auto wasFourth = kind == 4;

		++kind;

		if (kind == 1) kind = 2;

		if (intervalIsPerfectKind())
		{
			if (wasFourth)
				quality = Quality::Perfect;
			else
				quality = Quality::Augmented;
		}
		else
		{
			quality = Quality::Major;
		}
	};

	if (intervalIsPerfectKind())
	{
		switch (quality)
		{
			case (Quality::Perfect) :
			{
				quality = Quality::Augmented;
				return *this;
			}
			case (Quality::Augmented) :
			{
				incrementKind();
				return *this;
			}
			case (Quality::Diminished) :
			{
				quality = Quality::Perfect;
				return *this;
			}
			default : LIMES_ASSERT_FALSE; return *this;
		}
	}

	switch (quality)
	{
		case (Quality::Major) :
		{
			quality = Quality::Augmented;
			return *this;
		}
		case (Quality::Minor) :
		{
			quality = Quality::Major;
			return *this;
		}
		case (Quality::Augmented) :
		{
			incrementKind();
			return *this;
		}
		case (Quality::Diminished) :
		{
			quality = Quality::Minor;
			return *this;
		}
		default : LIMES_ASSERT_FALSE; return *this;
	}
}

constexpr Interval& Interval::operator--() noexcept
{
	if (kind == 0 && quality == Quality::Perfect)
		return *this;

	if (kind == 2 && quality == Quality::Diminished)
	{
		kind	= 0;
		quality = Quality::Perfect;
		return *this;
	}

	auto decrementKind = [this]
	{
		const auto wasFifth = kind == 5;

		--kind;

		if (kind == 1) kind = 0;

		if (intervalIsPerfectKind())
		{
			if (wasFifth)
				quality = Quality::Perfect;
			else
				quality = Quality::Diminished;
		}
		else
		{
			quality = Quality::Minor;
		}
	};

	if (intervalIsPerfectKind())
	{
		switch (quality)
		{
			case (Quality::Perfect) :
			{
				quality = Quality::Diminished;
				return *this;
			}
			case (Quality::Augmented) :
			{
				quality = Quality::Perfect;
				return *this;
			}
			case (Quality::Diminished) :
			{
				decrementKind();
				return *this;
			}
			default : LIMES_ASSERT_FALSE; return *this;
		}
	}

	switch (quality)
	{
		case (Quality::Major) :
		{
			quality = Quality::Minor;
			return *this;
		}
		case (Quality::Minor) :
		{
			quality = Quality::Diminished;
			return *this;
		}
		case (Quality::Augmented) :
		{
			quality = Quality::Major;
			return *this;
		}
		case (Quality::Diminished) :
		{
			decrementKind();
			return *this;
		}
		default : LIMES_ASSERT_FALSE; return *this;
	}
}

}  // namespace lemons::music
