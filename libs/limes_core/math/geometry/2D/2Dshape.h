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

#include "../../../limes_namespace.h"
#include <limes_export.h>
#include "../../mathFunctions.h"  // for concept Scalar - IWYU pragma: keep

/** @defgroup geometry Geometry
	Classes representing geometric shapes.
	@ingroup core_math
 */

/** @dir libs/limes_core/math/geometry
	This directory contains utilities for working with geometry.
	@ingroup geometry
 */

/** @defgroup TwoDgeometry 2-D geometry
	Two-dimensional geometric shapes.
	@ingroup geometry
 */

/** @dir libs/limes_core/math/geometry/2D
	This directory contains utilities for working with 2-dimensional geometry.
	@ingroup TwoDgeometry
 */

/** @file
	This file defines the TwoDShape interface class.
	@ingroup TwoDgeometry
 */

LIMES_BEGIN_NAMESPACE

/** This namespace contains utilities for working with geometric shapes and calculations.
	@ingroup geometry
 */
namespace math::geometry
{

/** A base class for any kind of 2-dimensional shape.
	@ingroup TwoDgeometry
 */
template <Scalar ValueType>
class LIMES_EXPORT TwoDShape
{
public:
	/** Destructor. */
	virtual ~TwoDShape() = default;

	/** Returns the area of the shape. */
	[[nodiscard]] ValueType area() const noexcept = 0;

	/** Returns the perimeter of the shape. */
	[[nodiscard]] ValueType perimeter() const noexcept = 0;
};

}  // namespace math::geometry

LIMES_END_NAMESPACE
