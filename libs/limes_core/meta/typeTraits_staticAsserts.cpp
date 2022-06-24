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

#include "./TypeTraits.h"
#include <limes_namespace.h>
#include <vector>
#include <list>

LIMES_BEGIN_NAMESPACE

static_assert (meta::is_specialization<std::vector<int>, std::vector>());
static_assert (! meta::is_specialization<std::vector<int>, std::list>());

static_assert (meta::is_one_of_v<int, double, float, int>);
static_assert (meta::is_none_of_v<int, float, double, std::vector<int>>);

struct Base
{
};

struct Derived : Base
{
};

static_assert (meta::is_covariant_v<Derived, Base>);

LIMES_END_NAMESPACE
