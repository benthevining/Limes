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

#include "angle.h"
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace math::geometry
{

static_assert (Angle { 45 }.isComplementaryWith (Angle { 45 }), "Angle::isComplementaryWith");
static_assert (Angle { 60 }.getComplement().degrees() == 30, "Angle::getComplement");

static_assert (Angle { 100 }.isSupplementaryWith (Angle { 80 }), "Angle::isSupplementaryWith");
static_assert (Angle { 90 }.getSupplement().degrees() == 90, "Angle::getSupplement");

static_assert (Angle { 350 }.isExplementaryWith (Angle { 10 }), "Angle::isExplementaryWith");
static_assert (Angle { 60 }.getExplement().degrees() == 300, "Angle::getExplement");

static_assert (Angle<int>::right().bisect().degrees() == 45);

}  // namespace math::geometry

LIMES_END_NAMESPACE
