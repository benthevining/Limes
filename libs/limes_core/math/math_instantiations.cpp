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
#include "./StatsComputer.h"
#include "./geometry/2D/angle.h"
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace math
{

template <>
struct Fraction<int>;

template <>
struct Fraction<float>;

template <>
struct Fraction<double>;

template <>
class StatsComputer<int>;

template <>
class StatsComputer<float>;

template <>
class StatsComputer<double>;

namespace geometry
{

template <>
class Angle<int>;

template <>
class Angle<float>;

template <>
class Angle<double>;

}  // namespace geometry

}  // namespace math

LIMES_END_NAMESPACE
