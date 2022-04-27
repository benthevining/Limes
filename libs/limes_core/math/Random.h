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

#include <limes_namespace.h>
#include <limes_export.h>
#include <cstdint>
#include <type_traits>
#include <atomic>
#include <random>
#include <limits>
#include "../misc/preprocessor.h"
#include "../misc/TypeTraits.h"
#include "mathHelpers.h"

LIMES_BEGIN_NAMESPACE

namespace math
{

class LIMES_EXPORT Random final
{
public:

	using EngineType = std::minstd_rand;

	using ValueType = EngineType::result_type;

	explicit Random (ValueType seedValue);

	template <Scalar T>
	explicit Random (T seedValue);

	Random();

	LIMES_DEFAULT_MOVABLE (Random);
	LIMES_DEFAULT_COPYABLE (Random);

	[[nodiscard]] ValueType nextValue();

	[[nodiscard]] bool nextBool();

	template <Scalar T>
	[[nodiscard]] T next (T min = std::numeric_limits<T>::min(),
						  T max = std::numeric_limits<T>::max());

	void setSeed (ValueType newSeed);

	[[nodiscard]] Random fork();

	[[nodiscard]] static Random& getSystem() noexcept;

private:

	EngineType engine;
};

/*-------------------------------------------------------------------------------------------------------------------------------*/

template <Scalar T>
Random::Random (T seedValue)
	: Random (static_cast<ValueType> (seedValue))
{
}

template <Scalar T>
T Random::next (T min, T max)
{
	return map (static_cast<T> (nextValue()),
				static_cast<T> (EngineType::min()), static_cast<T> (EngineType::max()),
				min, max);
}

}  // namespace math

LIMES_END_NAMESPACE
