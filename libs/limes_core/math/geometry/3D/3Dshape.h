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

/** @defgroup ThreeDgeometry 3-D geometry
	Three-dimensional geometric shapes.
	@ingroup geometry
 */

/** @file
	This file defines the ThreeDShape interface class.
	@ingroup ThreeDgeometry
 */

LIMES_BEGIN_NAMESPACE

namespace math::geometry
{

/** A base class for any kind of 3-dimensional shape.
	@ingroup ThreeDgeometry
 */
template <Scalar ValueType>
class LIMES_EXPORT ThreeDShape
{
public:

	/** Destructor. */
	virtual ~ThreeDShape() = default;

	/** Returns the surface area of the shape. */
	[[nodiscard]] ValueType surfaceArea() = 0;

	/** Returns the volume of the shape. */
	[[nodiscard]] ValueType volume() = 0;
};

}  // namespace math::geometry

LIMES_END_NAMESPACE
