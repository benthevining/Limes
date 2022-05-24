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

/** This namespace contains utilities for working with geometric shapes and calculations. */
namespace math::geometry
{

/** A base class for any kind of 2-dimensional shape. */
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
