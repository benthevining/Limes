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

#include <limes_export.h>  // for LIMES_EXPORT
#include "../limes_namespace.h"
#include <limits>				   // for numeric_limits
#include <random>				   // for minstd_rand
#include "../misc/preprocessor.h"  // for LIMES_DEFAULT_COPYABLE, LIMES_DEFA...
#include "./mathFunctions.h"	   // for map
#include <algorithm>			   // for max/min

/** @file
	This file defines the Random class.
	@ingroup core_math
 */

LIMES_BEGIN_NAMESPACE

namespace math
{

/** This class is a %random number generator.
	The generator used is \c std::minstd_rand .
	@ingroup core_math
	@todo get current seed
 */
class LIMES_EXPORT Random final
{
public:
	/** The type of %random number generation engine being used internally. */
	using EngineType = std::minstd_rand;

	/** The types of values the %random number engine outputs. */
	using ValueType = EngineType::result_type;

	/** @name Constructors */
	///@{
	/** Creates a %random number generator with a specified seed value. */
	explicit Random (ValueType seedValue);

	/** Creates a %random number generator with a specified seed value. */
	template <Scalar T>
	explicit Random (T seedValue);

	/** Creates a %random number generator with a %random seed value.
		The seed value will be generated using \c std::random_device .
	 */
	Random();
	///@}

	LIMES_DEFAULT_MOVABLE (Random)
	LIMES_DEFAULT_COPYABLE (Random)

	/** Returns the next value from the %random number generator. */
	[[nodiscard]] ValueType nextValue();

	/** Returns the next value from the %random number generator, interpreted as a boolean. */
	[[nodiscard]] bool nextBool();

	/** Returns the next value from the %random number generator as a specified type, optionally mapped to a specified output range. */
	template <Scalar T>
	[[nodiscard]] T next (T min = std::max (std::numeric_limits<T>::min(), static_cast<T> (EngineType::min())),
						  T max = std::min (std::numeric_limits<T>::max(), static_cast<T> (EngineType::max())));

	/** Sets the seed of the %random number generator. */
	void setSeed (ValueType newSeed);

	/** Creates a new %random number generator, which is seeded with this generator's next value. */
	[[nodiscard]] Random fork();

	/** Returns a static, system-wide %random number generator. */
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
