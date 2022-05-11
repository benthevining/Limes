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

#include "TypeList.h"
#include <limes_namespace.h>
#include <vector>
#include <type_traits>

LIMES_BEGIN_NAMESPACE

namespace typelist
{

using TypeListA = TypeList<int, double, float>;

static_assert (TypeListA::size == 3);
static_assert (! TypeListA::empty);

static_assert (TypeListA::equal<TypeList<int, double, float>>);

static_assert (TypeListA::replace_at<1, size_t>::TypeID::equal<TypeList<int, size_t, float>>);

// static_assert(TypeListA::reverse::TypeID::equal<TypeList<float, double, int>>);

static_assert (TypeListA::remove<int, float>::TypeID::equal<TypeList<double>>);

static_assert (TypeListA::contains<int>);
static_assert (TypeListA::contains<double>);
static_assert (TypeListA::contains<float>);
static_assert (! TypeListA::contains<size_t>);
static_assert (! TypeListA::contains<std::vector<int>>);

static_assert (TypeListA::num_of<int> == 1);

using TypeListB = TypeListA::add<int>;

static_assert (TypeListB::num_of<int> == 2);
static_assert (TypeListB::contains_duplicates);

using PrunedTypeListB = TypeListB::remove_duplicates;

static_assert (PrunedTypeListB::size == 3);
static_assert (PrunedTypeListB::num_of<int> == 1);

static_assert (! TypeListA::equal<TypeListB>);

using TypeListC = TypeListA::addIfAbsent<double>;

static_assert (TypeListC::equal<TypeListA>);

static_assert (std::is_same_v<TypeListA::front, int>);
static_assert (std::is_same_v<TypeListA::back, float>);
static_assert (std::is_same_v<TypeListA::at<1>, double>);

static_assert (TypeListA::index_of<double> == 1);

using OnlyInt = TypeListA::remove<float>::TypeID::remove<double>;

static_assert (OnlyInt::size == 1);
static_assert (! OnlyInt::contains<double>);
static_assert (! OnlyInt::contains<float>);
static_assert (OnlyInt::contains<int>);

using TypeListD = TypeListA::replace<double, size_t>;

static_assert (TypeListD::equal<TypeList<int, size_t, float>>);

static_assert (is_typelist<TypeListD>);

using TypeListE = TypeListD::remove_at<1>;

static_assert (TypeListE::equal<TypeList<int, float>>);

static_assert (TypeListE::remove_last::TypeID::equal<TypeList<int>>);

using EmptyList = OnlyInt::remove<int>;

// static_assert(EmptyList::empty);

}  // namespace typelist

LIMES_END_NAMESPACE
