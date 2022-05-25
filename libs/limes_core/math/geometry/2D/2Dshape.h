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

/** @defgroup geometry
	Classes representing geometric shapes.
	@ingroup math
 */

/** @defgroup 2Dgeometry 2-D geometry
	Two-dimensional geometric shapes.
	@ingroup geometry
 */

/** @file
	This file defines the TwoDShape interface class.
	@ingroup 2Dgeometry
 */

LIMES_BEGIN_NAMESPACE

/** This namespace contains utilities for working with geometric shapes and calculations.
	@ingroup geometry
 */
namespace math::geometry
{

/** A base class for any kind of 2-dimensional shape.
	@ingroup 2Dgeometry
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
