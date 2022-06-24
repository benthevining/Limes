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

#include "./Fraction.h"
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace math
{

static_assert (Fraction { 2, 3 }.isEquivalentTo (Fraction { 4, 6 }), "Fraction::isEquivalentTo");
static_assert (Fraction<double> { 5., 9. }.isEquivalentTo (Fraction<float> { 15.f, 27.f }), "Fraction::isEquivalentTo");

static_assert (Fraction { 12, 3 }.getReciprocal() == Fraction { 3, 12 }, "Fraction::getReciprocal");
static_assert (Fraction<float> { 186.f, 73.2f }.getReciprocal() == Fraction<float> { 73.2f, 186.f }, "Fraction::getReciprocal");

static_assert (Fraction { 8, 4 }.divided() == 2, "Fraction::divided");
static_assert (Fraction<double> { 60., 20. }.divided() == 3., "Fraction::divided");

static_assert (Fraction { 8, 16 }.reduce() == Fraction { 1, 2 }, "Fraction::reduce");
static_assert (Fraction { 5, 25 }.reduce() == Fraction { 1, 5 }, "Fraction::reduce");
static_assert (Fraction { 5, 26 }.reduce() == Fraction { 5, 26 }, "Fraction::reduce");
static_assert (Fraction { -2, -3 }.reduce() == Fraction { 2, 3 }, "Fraction::reduce");

static_assert (Fraction { 1, 6 }.leastCommonDenominatorWith (Fraction { 3, 8 }) == 2, "Fraction::leastCommonDenominatorWith");
static_assert (Fraction { 4, 5 }.leastCommonDenominatorWith (Fraction { 6, 15 }) == 5, "Fraction::leastCommonDenominatorWith");
static_assert (Fraction { 12, 3 }.leastCommonDenominatorWith (Fraction { 2, 3 }) == 3, "Fraction::leastCommonDenominatorWith");

static_assert (Fraction { 2, 3 }.withDenominator (6) == Fraction { 4, 6 }, "Fraction::withDenominator");
static_assert (Fraction { 4, 5 }.withDenominator (25) == Fraction { 20, 25 }, "Fraction::withDenominator");
static_assert (Fraction { 15, 20 }.withDenominator (4) == Fraction { 3, 4 }, "Fraction::withDenominator");
static_assert (Fraction { 12, 21 }.withDenominator (7) == Fraction { 4, 7 }, "Fraction::withDenominator");

static_assert (! Fraction { 2, 15 }.isNegative(), "Fraction::isNegative");
static_assert (Fraction { 4, -23 }.isNegative(), "Fraction::isNegative");
static_assert (Fraction { -12, 35 }.isNegative(), "Fraction::isNegative");
static_assert (! Fraction { -17, -38 }.isNegative(), "Fraction::isNegative");

}  // namespace math

LIMES_END_NAMESPACE
