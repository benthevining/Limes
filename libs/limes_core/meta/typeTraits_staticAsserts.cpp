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

#include "TypeTraits.h"
#include <limes_namespace.h>
#include <vector>
#include <list>

LIMES_BEGIN_NAMESPACE

static_assert (is_specialization<std::vector<int>, std::vector>(), "is_specialization test");
static_assert (! is_specialization<std::vector<int>, std::list>(), "is_specialization test");

static_assert (is_one_of<int, double, float, int>::value, "is_one_of test");
static_assert (is_none_of<int, float, double, std::vector<int>>::value, "is_none_of test");

// covariance_check

LIMES_END_NAMESPACE
