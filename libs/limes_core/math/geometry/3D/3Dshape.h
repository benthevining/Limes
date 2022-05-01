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

#include <limes_namespace.h>
#include <limes_export.h>

LIMES_BEGIN_NAMESPACE

namespace math::geometry
{

template <Scalar ValueType>
class LIMES_EXPORT ThreeDShape
{
public:

	virtual ~ThreeDShape() = default;

	[[nodiscard]] ValueType surfaceArea() = 0;

	[[nodiscard]] ValueType volume() = 0;
};

}  // namespace math::geometry

LIMES_END_NAMESPACE
