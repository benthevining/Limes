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

#include "./mathFunctions.h"
#include "../limes_namespace.h"

LIMES_BEGIN_NAMESPACE

namespace math
{

/*
- map
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

static_assert (abs (60.25) == 60.25, "abs test");
static_assert (abs (-414.1) == 414.1, "abs test");
static_assert (abs (111.2f) == 111.2f, "abs test");
static_assert (abs (-12.653f) == 12.653f, "abs test");
static_assert (abs (14) == 14, "abs test");
static_assert (abs (-23) == 23, "abs test");
static_assert (abs (0) == 0, "abs test");

static_assert (negate (-41.2) == 41.2, "negate test");
static_assert (negate (23.64) == -23.64, "negate test");
static_assert (negate (43.f) == -43.f, "negate test");
static_assert (negate (-101.6f) == 101.6f, "negate test");
static_assert (negate (4) == -4, "negate test");
static_assert (negate (-16) == 16, "negate test");
static_assert (negate (0) == 0, "negate test");

static_assert (round (43.2) == 43, "round test");
static_assert (round (68.7) == 69, "round test");
static_assert (round (101.) == 101, "round test");
static_assert (round (-21.8f) == -22, "round test");
static_assert (round (31.1f) == 31, "round test");
static_assert (round (1.f) == 1, "round test");
static_assert (round (42) == 42, "round test");

static_assert (limit (68.2, 69., 72.) == 69., "limit test");
static_assert (limit (21.3, 20., 21.) == 21., "limit test");
static_assert (limit (69.7, 21.8, 78.6) == 69.7, "limit test");
static_assert (limit (-18.5f, -21.f, -18.5f) == -18.5f, "limit test");
static_assert (limit (627.2f, 627.3f, 956.f) == 627.3f, "limit test");
static_assert (limit (19.3f, -68.f, 127.f) == 19.3f, "limit test");
static_assert (limit (-6, -5, -2) == -5, "limit test");
static_assert (limit (18, 12, 17) == 17, "limit test");
static_assert (limit (0, -1, 1) == 0, "limit test");

static_assert (power (2, 2) == 4, "power test");
static_assert (power (12, 5) == 248832, "power test");
static_assert (power (7, 3) == 343, "power test");

static_assert (isPowerOf2 (2), "isPowerOf2 test");
static_assert (! isPowerOf2 (3), "isPowerOf2 test");
static_assert (isPowerOf2 (4), "isPowerOf2 test");
static_assert (! isPowerOf2 (5), "isPowerOf2 test");
static_assert (! isPowerOf2 (0), "isPowerOf2 test");
static_assert (! isPowerOf2 (-2), "isPowerOf2 test");
static_assert (isPowerOf2 (65536), "isPowerOf2 test");

static_assert (factorial (1) == 1, "factorial test");
static_assert (factorial (0) == 1, "factorial test");
static_assert (factorial (5) == 120, "factorial test");
static_assert (factorial (8) == 40320, "factorial test");

static_assert (isPrime (2), "isPrime test");
static_assert (isPrime (3), "isPrime test");
static_assert (! isPrime (4), "isPrime test");
static_assert (! isPrime (111), "isPrime test");
static_assert (isPrime (113), "isPrime test");

}  // namespace math

LIMES_END_NAMESPACE
