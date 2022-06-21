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
#include <limes_platform.h>
#include "../../../misc/preprocessor.h"
#include "../2D/square.h"
#include "rectangular_prism.h"
#include "3Dshape.h"
#include <cmath>

/** @file
	This file defines the Cube class.
	@ingroup ThreeDgeometry
 */

LIMES_BEGIN_NAMESPACE

namespace math::geometry
{

/** This class represents a %cube.
	@ingroup ThreeDgeometry
	@see RectangularPrism
 */
template <Scalar ValueType>
class LIMES_EXPORT Cube final : public ThreeDShape<ValueType>
{
public:

	/** @name Constructors */
	///@{
	/** Creates a cube with the given side length. */
	explicit constexpr Cube (ValueType sideLengthToUse) noexcept;

	/** Creates a cube with the given face. */
	explicit constexpr Cube (const Square<ValueType>& face) noexcept;
	///@}

	LIMES_CONSTEXPR_COPYABLE (Cube)
	LIMES_CONSTEXPR_MOVABLE (Cube)

	/** Returns the volume of the %cube.
		The formula is @f$ V=side^3 @f$.
	 */
	[[nodiscard]] constexpr ValueType volume() const noexcept final;

	/** Returns the surface area of the %cube.
		The formula is @f$ A=6*side^2 @f$.
	 */
	[[nodiscard]] constexpr ValueType surfaceArea() const noexcept final;

	/** Returns the surface perimeter of the %cube.
		The formula is @f$ P=12*side @f$.
	 */
	[[nodiscard]] constexpr ValueType surfacePerimeter() const noexcept;

	/** Returns the side length of the %cube. */
	[[nodiscard]] constexpr ValueType sideLength() const noexcept;

	/** Returns the space diagonal of the %cube.
		The formula is @f$ side*\sqrt{3} @f$.
	 */
	[[nodiscard]] ValueType spaceDiagonal() const noexcept;

	/** @name Equality comparisons */
	///@{
	[[nodiscard]] constexpr bool operator== (const Cube& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Cube& other) const noexcept;
	///@}

	/** @name Greater/less than comparisons
		Compares cubes based on their side lengths.
	 */
	///@{
	[[nodiscard]] constexpr bool operator> (const Cube& other) const noexcept;
	[[nodiscard]] constexpr bool operator<(const Cube& other) const noexcept;
	///@}

	/** Returns a Square object representing a face of this %cube. */
	[[nodiscard]] constexpr Square<ValueType> getFace() const noexcept;

	/** Returns a RectangularPrism object with the same dimensions as this %cube. */
	[[nodiscard]] constexpr RectangularPrism<ValueType> getRectangularPrism() const noexcept;

private:

	ValueType side_length;
};


template <Scalar ValueType>
constexpr Cube<ValueType>::Cube (ValueType sideLengthToUse) noexcept
	: side_length (sideLengthToUse)
{
	LIMES_ASSERT (side_length > 0);
}

template <Scalar ValueType>
constexpr Cube<ValueType>::Cube (const Square<ValueType>& face) noexcept
	: side_length (face.sideLength())
{
}

template <Scalar ValueType>
constexpr ValueType Cube<ValueType>::volume() const noexcept
{
	return side_length * side_length * side_length;
}

template <Scalar ValueType>
constexpr ValueType Cube<ValueType>::surfaceArea() const noexcept
{
	return ValueType (6) * side_length * side_length;
}

template <Scalar ValueType>
constexpr ValueType Cube<ValueType>::surfacePerimeter() const noexcept
{
	return ValueType (12) * side_length;
}

template <Scalar ValueType>
constexpr ValueType Cube<ValueType>::sideLength() const noexcept
{
	return side_length;
}

template <Scalar ValueType>
ValueType Cube<ValueType>::spaceDiagonal() const noexcept
{
	return side_length * std::sqrt (ValueType (3));
}

template <Scalar ValueType>
constexpr bool Cube<ValueType>::operator== (const Cube& other) const noexcept
{
	return side_length == other.side_length;
}

template <Scalar ValueType>
constexpr bool Cube<ValueType>::operator!= (const Cube& other) const noexcept
{
	return side_length != other.side_length;
}

template <Scalar ValueType>
constexpr bool Cube<ValueType>::operator> (const Cube& other) const noexcept
{
	return side_length > other.side_length;
}

template <Scalar ValueType>
constexpr bool Cube<ValueType>::operator<(const Cube& other) const noexcept
{
	return side_length < other.side_length;
}

template <Scalar ValueType>
constexpr Square<ValueType> Cube<ValueType>::getFace() const noexcept
{
	return Square<ValueType> { side_length };
}

template <Scalar ValueType>
constexpr RectangularPrism<ValueType> Cube<ValueType>::getRectangularPrism() const noexcept
{
	return RectangularPrism<ValueType> { side_length, side_length, side_length };
}

}  // namespace math::geometry

LIMES_END_NAMESPACE
