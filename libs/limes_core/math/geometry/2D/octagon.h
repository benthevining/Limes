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
	This file defines the Octagon class.
	@ingroup TwoDgeometry
 */

LIMES_BEGIN_NAMESPACE

namespace math::geometry
{

/** Represents an %octagon.
	@ingroup TwoDgeometry
 */
template <Scalar ValueType>
class LIMES_EXPORT Octagon final : public TwoDShape<ValueType>
{
public:

	/** Creates an octagon with the specified side length (in unspecified units). */
	explicit constexpr Octagon (ValueType sideLength) noexcept;

	LIMES_CONSTEXPR_COPYABLE (Octagon);
	LIMES_CONSTEXPR_MOVABLE (Octagon);

	/** Returns the area of the %octagon.
		The formula is @f$ A=side^2*2*(1+\sqrt{2}) @f$.
	 */
	[[nodiscard]] ValueType area() const noexcept final;

	/** Returns the perimeter of the %octagon.
		The formula is @f$ P=side*8 @f$.
	 */
	[[nodiscard]] constexpr ValueType perimeter() const noexcept final;

	/** Returns the side length of the %octagon. */
	[[nodiscard]] constexpr ValueType sideLength() const noexcept;

	/** @name Equality comparisons
		Octagons are considered equal if their side length is the same.
	 */
	///@{
	[[nodiscard]] constexpr bool operator== (const Octagon& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Octagon& other) const noexcept;
	///@}

	/** @name Greater/less than comparisons
		Compares octagons based on their areas.
	 */
	///@{
	[[nodiscard]] constexpr bool operator> (const Octagon& other) const noexcept;
	[[nodiscard]] constexpr bool operator<(const Octagon& other) const noexcept;
	///@}

	/** Returns the interior angle of any %octagon (135 degrees). */
	[[nodiscard]] LIMES_PURE_FUNCTION static constexpr Angle<ValueType> interiorAngle() noexcept;

	/** Returns the exterior angle of any %octagon (45 degrees). */
	[[nodiscard]] LIMES_PURE_FUNCTION static constexpr Angle<ValueType> exteriorAngle() noexcept;

private:

	ValueType side_length;
};


template <Scalar ValueType>
constexpr Octagon<ValueType>::Octagon (ValueType sideLength) noexcept
	: side_length (sideLength)
{
	LIMES_ASSERT (side_length > 0);
}

template <Scalar ValueType>
constexpr ValueType Octagon<ValueType>::sideLength() const noexcept
{
	return side_length;
}

template <Scalar ValueType>
ValueType Octagon<ValueType>::area() const noexcept
{
	return side_length * side_length * ValueType (2) * (ValueType (1) + std::sqrt (ValueType (2)));
}

template <Scalar ValueType>
constexpr ValueType Octagon<ValueType>::perimeter() const noexcept
{
	return side_length * ValueType (8);
}

template <Scalar ValueType>
constexpr bool Octagon<ValueType>::operator== (const Octagon& other) const noexcept
{
	return side_length == other.side_length;
}

template <Scalar ValueType>
constexpr bool Octagon<ValueType>::operator!= (const Octagon& other) const noexcept
{
	return side_length != other.side_length;
}

template <Scalar ValueType>
constexpr bool Octagon<ValueType>::operator> (const Octagon& other) const noexcept
{
	return area() > other.area();
}

template <Scalar ValueType>
constexpr bool Octagon<ValueType>::operator<(const Octagon& other) const noexcept
{
	return area() < other.area();
}

template <Scalar ValueType>
constexpr Angle<ValueType> Octagon<ValueType>::interiorAngle() noexcept
{
	return Angle<ValueType> { 135 };
}

template <Scalar ValueType>
constexpr Angle<ValueType> Octagon<ValueType>::exteriorAngle() noexcept
{
	return Angle<ValueType> { 45 };
}

}  // namespace math::geometry

LIMES_END_NAMESPACE
