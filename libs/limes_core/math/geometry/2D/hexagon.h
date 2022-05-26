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
#include "2Dshape.h"
#include "../../../misc/preprocessor.h"
#include "../../../system/compiler_defs.h"
#include <cmath>

/** @file
	This file defines the Hexagon class.
	@ingroup TwoDgeometry
 */

LIMES_BEGIN_NAMESPACE

namespace math::geometry
{

/** Represents a %hexagon.
	@ingroup TwoDgeometry
 */
template <Scalar ValueType>
class LIMES_EXPORT Hexagon final : public TwoDShape<ValueType>
{
public:

	/** Creates a hexagon with the specified side length (in unspecified units). */
	explicit constexpr Hexagon (ValueType sideLength) noexcept;

	LIMES_CONSTEXPR_COPYABLE (Hexagon);
	LIMES_CONSTEXPR_MOVABLE (Hexagon);

	/** Returns the area of the %hexagon. */
	[[nodiscard]] ValueType area() const noexcept final;

	/** Returns the perimeter of the %hexagon. */
	[[nodiscard]] constexpr ValueType perimeter() const noexcept final;

	/** Returns the side length of the %hexagon. */
	[[nodiscard]] constexpr ValueType sideLength() const noexcept;

	/** @name Equality comparisons
		Hexagons are considered equal if their side length is the same.
	 */
	///@{
	[[nodiscard]] constexpr bool operator== (const Hexagon& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Hexagon& other) const noexcept;
	///@}

	/** @name Greater/less than comparisons
		Compares hexagons based on their areas.
	 */
	///@{
	[[nodiscard]] constexpr bool operator> (const Hexagon& other) const noexcept;
	[[nodiscard]] constexpr bool operator< (const Hexagon& other) const noexcept;
	///@}

	/** Returns the interior angle of any %hexagon (120 degrees). */
	[[nodiscard]] LIMES_PURE_FUNCTION static constexpr Angle<ValueType> interiorAngle() noexcept;

	/** Returns the exterior angle of any %hexagon (60 degrees). */
	[[nodiscard]] LIMES_PURE_FUNCTION static constexpr Angle<ValueType> exteriorAngle() noexcept;

private:

	ValueType side_length;
};


template <Scalar ValueType>
constexpr Hexagon<ValueType>::Hexagon (ValueType sideLength) noexcept
	: side_length (sideLength)
{
	LIMES_ASSERT (side_length > 0);
}

template <Scalar ValueType>
constexpr ValueType Hexagon<ValueType>::sideLength() const noexcept
{
	return side_length;
}

template <Scalar ValueType>
ValueType Hexagon<ValueType>::area() const noexcept
{
	return side_length * side_length * ValueType (3) * std::sqrt (ValueType (3)) / ValueType (2);
}

template <Scalar ValueType>
constexpr ValueType Hexagon<ValueType>::perimeter() const noexcept
{
	return side_length * ValueType (6);
}

template <Scalar ValueType>
constexpr bool Hexagon<ValueType>::operator== (const Hexagon& other) const noexcept
{
	return side_length == other.side_length;
}

template <Scalar ValueType>
constexpr bool Hexagon<ValueType>::operator!= (const Hexagon& other) const noexcept
{
	return side_length != other.side_length;
}

template <Scalar ValueType>
constexpr bool Hexagon<ValueType>::operator> (const Hexagon& other) const noexcept
{
	return area() > other.area();
}

template <Scalar ValueType>
constexpr bool Hexagon<ValueType>::operator< (const Hexagon& other) const noexcept
{
	return area() < other.area();
}

template <Scalar ValueType>
constexpr Angle<ValueType> Hexagon<ValueType>::interiorAngle() noexcept
{
	return Angle<ValueType> { 120 };
}

template <Scalar ValueType>
constexpr Angle<ValueType> Hexagon<ValueType>::exteriorAngle() noexcept
{
	return Angle<ValueType> { 60 };
}

}  // namespace math::geometry

LIMES_END_NAMESPACE
