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
#include <variant>

LIMES_BEGIN_NAMESPACE

namespace typelist
{

using TypeListA = TypeList<int, double, float>;

static_assert (TypeListA::size == 3);
static_assert (! TypeListA::empty);

static_assert (TypeListA::equal<TypeList<int, double, float>>);

static_assert (TypeListA::equal_ignore_order<TypeList<float, int, double>>);

static_assert (TypeListA::replace_at<1, size_t>::equal<TypeList<int, size_t, float>>);

static_assert (TypeListA::insert_at<1, size_t>::equal<TypeList<int, size_t, double, float>>);

static_assert (TypeListA::swap_at<1, 2>::equal<TypeList<int, float, double>>);

static_assert (TypeListA::swap<double, int>::equal<TypeList<double, int, float>>);

static_assert (TypeListA::reverse::equal<TypeList<float, double, int>>);

static_assert (TypeListA::remove<int, float>::equal<TypeList<double>>);

static_assert (TypeListA::remove_if<std::is_floating_point>::equal<TypeList<int>>);

static_assert (TypeListA::remove_if_not<std::is_floating_point>::equal<TypeList<double, float>>);

static_assert (TypeListA::contains<int>);
static_assert (TypeListA::contains<double>);
static_assert (TypeListA::contains<float>);
static_assert (TypeListA::contains<int, double, float>);
static_assert (! TypeListA::contains<size_t>);
static_assert (! TypeListA::contains<int, double, float, size_t>);
static_assert (! TypeListA::contains<std::vector<int>>);

static_assert (TypeListA::contains_or<int, size_t>);
static_assert (! TypeListA::contains_or<size_t, std::vector<double>>);

static_assert (TypeListA::num_of<int> == 1);

static_assert (TypeListA::count_if<std::is_floating_point> == 2);
static_assert (TypeListA::count_if_not<std::is_floating_point> == 1);

using TypeListB = TypeListA::add<int>;

static_assert (TypeListB::size == 4);
static_assert (TypeListB::num_of<int> == 2);
static_assert (TypeListB::contains_duplicates);

using PrunedTypeListB = TypeListB::remove_duplicates;

static_assert (PrunedTypeListB::size == 3);
static_assert (PrunedTypeListB::num_of<int> == 1);

static_assert (! TypeListA::equal<TypeListB>);

using TypeListC = TypeListA::addIfAbsent<double>;

static_assert (TypeListC::equal<TypeListA>);

static_assert (TypeListA::common_with<TypeListC>::equal<TypeListA>);

static_assert (std::is_same_v<TypeListA::front, int>);
static_assert (std::is_same_v<TypeListA::back, float>);
static_assert (std::is_same_v<TypeListA::at<1>, double>);

static_assert (TypeListA::index_of<double> == 1);

using OnlyInt = TypeListA::remove<float>::TypeID::remove<double>;

static_assert (TypeListA::add_from<OnlyInt>::equal<TypeList<int, double, float, int>>);

static_assert (TypeListA::common_with<OnlyInt>::equal<TypeList<int>>);

static_assert (TypeListA::not_in<OnlyInt>::equal<TypeList<double, float>>);

static_assert (OnlyInt::size == 1);
static_assert (! OnlyInt::contains<double>);
static_assert (! OnlyInt::contains<float>);
static_assert (OnlyInt::contains<int>);

using TypeListD = TypeListA::replace<double, size_t>;

static_assert (TypeListD::equal<TypeList<int, size_t, float>>);

static_assert (TypeListD::count_if<std::is_floating_point> == 1);
static_assert (TypeListD::count_if_not<std::is_floating_point> == 2);

static_assert (is_typelist<TypeListD>);

static_assert (TypeListD::prepend<double>::equal<TypeList<double, int, size_t, float>>);

static_assert (TypeListD::append<double>::equal<TypeList<int, size_t, float, double>>);

using TypeListE = TypeListD::remove_at<1>;

static_assert (TypeListE::equal<TypeList<int, float>>);

static_assert (TypeListE::remove_last::equal<TypeList<int>>);

using EmptyList = OnlyInt::remove<int>;

static_assert (EmptyList::empty);

static_assert (EmptyList::equal<TypeListE::clear>);

static_assert (EmptyList::reverse::equal<Empty>);

using variant_type = std::variant<int, float, double>;

using CreatedList = make_type_list_from_t<variant_type>;

static_assert (CreatedList::equal<TypeList<int, float, double>>);

static_assert (CreatedList::reverse::equal<TypeList<double, float, int>>);

static_assert (std::is_same_v<variant_type, CreatedList::apply_to<std::variant>>);

using TypeListF = TypeList<std::vector<int>, std::variant<size_t, double, float>, size_t>;

static_assert (TypeListF::index_of<std::vector<int>> == 0);

}  // namespace typelist

LIMES_END_NAMESPACE
