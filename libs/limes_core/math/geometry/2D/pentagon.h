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
	This file defines the Pentagon class.
	@ingroup TwoDgeometry
 */

LIMES_BEGIN_NAMESPACE

namespace math::geometry
{

/** Represents a %pentagon.
	@ingroup TwoDgeometry
 */
template <Scalar ValueType>
class LIMES_EXPORT Pentagon final : public TwoDShape<ValueType>
{
public:

	/** Creates a pentagon with the specified side length (in unspecified units). */
	explicit constexpr Pentagon (ValueType sideLength) noexcept;

	LIMES_CONSTEXPR_COPYABLE (Pentagon);
	LIMES_CONSTEXPR_MOVABLE (Pentagon);

	/** Returns the area of the %pentagon. */
	[[nodiscard]] ValueType area() const noexcept final;

	/** Returns the perimeter of the %pentagon. */
	[[nodiscard]] constexpr ValueType perimeter() const noexcept final;

	/** Returns the length of the internal diagonal of the %pentagon. */
	[[nodiscard]] ValueType diagonal() const noexcept;

	/** Returns the side length of the %pentagon. */
	[[nodiscard]] constexpr ValueType sideLength() const noexcept;

	/** @name Equality comparisons
		Pentagons are considered equal if their side length is the same.
	 */
	///@{
	[[nodiscard]] constexpr bool operator== (const Pentagon& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Pentagon& other) const noexcept;
	///@}

	/** @name Greater/less than comparisons
		Compares pentagons based on their areas.
	 */
	///@{
	[[nodiscard]] constexpr bool operator> (const Pentagon& other) const noexcept;
	[[nodiscard]] constexpr bool operator< (const Pentagon& other) const noexcept;
	///@}

	/** Returns the interior angle of any %pentagon (108 degrees). */
	[[nodiscard]] LIMES_PURE_FUNCTION static constexpr Angle<ValueType> interiorAngle() noexcept;

	/** Returns the exterior angle of any %pentagon (72 degrees). */
	[[nodiscard]] LIMES_PURE_FUNCTION static constexpr Angle<ValueType> exteriorAngle() noexcept;

private:

	ValueType side_length;
};


template <Scalar ValueType>
constexpr Pentagon<ValueType>::Pentagon (ValueType sideLength) noexcept
	: side_length (sideLength)
{
	LIMES_ASSERT (side_length > 0);
}

template <Scalar ValueType>
ValueType Pentagon<ValueType>::area() const noexcept
{
	const auto magicConstant = ValueType (5) + (ValueType (2) * std::sqrt (5));

	return std::sqrt (ValueType (5) * side_length * side_length * (magicConstant)) / ValueType (4);
}

template <Scalar ValueType>
constexpr ValueType Pentagon<ValueType>::perimeter() const noexcept
{
	return side_length * ValueType (5);
}

template <Scalar ValueType>
ValueType Pentagon<ValueType>::diagonal() const noexcept
{
	return side_length * ((ValueType (1) + std::sqrt (ValueType (5))) / ValueType (2));
}

template <Scalar ValueType>
constexpr ValueType Pentagon<ValueType>::sideLength() const noexcept
{
	return side_length;
}

template <Scalar ValueType>
constexpr bool Pentagon<ValueType>::operator== (const Pentagon& other) const noexcept
{
	return side_length == other.side_length;
}

template <Scalar ValueType>
constexpr bool Pentagon<ValueType>::operator!= (const Pentagon& other) const noexcept
{
	return side_length != other.side_length;
}

template <Scalar ValueType>
constexpr bool Pentagon<ValueType>::operator> (const Pentagon& other) const noexcept
{
	return area() > other.area();
}

template <Scalar ValueType>
constexpr bool Pentagon<ValueType>::operator< (const Pentagon& other) const noexcept
{
	return area() < other.area();
}

template <Scalar ValueType>
constexpr Angle<ValueType> Pentagon<ValueType>::interiorAngle() noexcept
{
	return Angle<ValueType> { 108 };
}

template <Scalar ValueType>
constexpr Angle<ValueType> Pentagon<ValueType>::exteriorAngle() noexcept
{
	return Angle<ValueType> { 72 };
}

}  // namespace math::geometry

LIMES_END_NAMESPACE
