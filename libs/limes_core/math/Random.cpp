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

#include "./Random.h"
#include <limes_namespace.h>  // for LIMES_BEGIN_NAMESPACE, LIMES_END_NAMESPACE
#include <limes_platform.h>	  // for LIMES_ASSERT_FALSE


LIMES_BEGIN_NAMESPACE

namespace math
{

Random::Random (ValueType seedValue)
	: engine (seedValue)
{
}

static std::random_device random_device_seed;

Random::Random()
	: Random (random_device_seed())
{
}

void Random::setSeed (ValueType newSeed)
{
	if (this == &getSystem())
	{
		LIMES_ASSERT_FALSE;
		return;
	}

	engine.seed (newSeed);
}

static Random sysRand;

Random& Random::getSystem() noexcept
{
	return sysRand;
}

Random::ValueType Random::nextValue()
{
	return engine();
}

bool Random::nextBool()
{
	constexpr auto halfRange = EngineType::max() - EngineType::min();

	return nextValue() > halfRange;
}

template <>
bool Random::next (bool, bool)
{
	return nextBool();
}

Random Random::fork()
{
	return Random { nextValue() };
}

}  // namespace math

LIMES_END_NAMESPACE
