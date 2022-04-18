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

#include "Random.h"
#include <limes_platform.h>
#include <atomic>
#include <limits>

namespace limes::math
{

static std::atomic<int64_t> globalSeed { 0 };

static Random sysRand;

Random::Random (int64_t seedValue) noexcept
	: seed (seedValue)
{
}

Random::Random()
	: seed (1)
{
	setSeedRandomly();
}

void Random::setSeed (int64_t newSeed) noexcept
{
	if (this == &getSystemRandom())
	{
		LIMES_ASSERT_FALSE;
		return;
	}

	seed.store (newSeed);
}

void Random::setSeedRandomly()
{
	const auto ptrValue = static_cast<int64_t> (reinterpret_cast<std::intptr_t> (this));

	seed ^= (static_cast<int64_t> (nextInt()) ^ (globalSeed ^ ptrValue));

	globalSeed ^= seed;
}

Random& Random::getSystemRandom() noexcept
{
	return sysRand;
}

int Random::nextInt() noexcept
{
	seed.store (static_cast<int64_t> (((static_cast<uint64_t> (seed) * 0x5deece66dLL) + 11) & 0xffffffffffffLL));

	return static_cast<int> (seed.load() >> 16);
}

int Random::nextInt (int maxValue) noexcept
{
	LIMES_ASSERT (maxValue > 0);
	return static_cast<int> ((static_cast<uint64_t> (nextInt()) * static_cast<uint64_t> (maxValue)) >> 32);
}

int Random::nextInt (int minValue, int maxValue) noexcept
{
	return minValue + nextInt (maxValue - minValue);
}

bool Random::nextBool() noexcept
{
	return (nextInt() & 0x40000000) != 0;
}

float Random::nextFloat() noexcept
{
	const auto result = static_cast<float> (static_cast<uint32_t> (nextInt()))
					  / (static_cast<float> (std::numeric_limits<uint32_t>::max()) + 1.f);

	if (result == 1.f)
		return 1.f - std::numeric_limits<float>::epsilon();

	return result;
}

double Random::nextDouble() noexcept
{
	const auto result = static_cast<double> (static_cast<uint32_t> (nextInt()))
					  / (static_cast<double> (std::numeric_limits<uint32_t>::max()) + 1.);

	if (result == 1.)
		return 1. - std::numeric_limits<double>::epsilon();

	return result;
}


}  // namespace limes::math
