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

#include "mathHelpers.h"
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace math
{

/*
- abs
- negate
- round
- limit
- map
- isPowerOf2
- isPrime
- periodInSamples
- freqFromPeriod
- sampsToMs
- msToSamps
*/

static_assert (isDivisibleBy (9, 3), "isDivisibleBy test");
static_assert (isDivisibleBy (15, 5), "isDivisibleBy test");
static_assert (isDivisibleBy (32, 8), "isDivisibleBy test");
static_assert (! isDivisibleBy (8, 7), "isDivisibleBy test");
static_assert (! isDivisibleBy (9, 2), "isDivisibleBy test");

static_assert (numberIsEven (16), "numberIsEven test");
static_assert (numberIsEven (126), "numberIsEven test");
static_assert (! numberIsEven (3), "numberIsEven test");
static_assert (! numberIsEven (521), "numberIsEven test");

}  // namespace math

LIMES_END_NAMESPACE
