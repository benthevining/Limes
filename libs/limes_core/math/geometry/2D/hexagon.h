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
#include <cmath>

LIMES_BEGIN_NAMESPACE

namespace math::geometry
{

template <Scalar ValueType>
class LIMES_EXPORT Hexagon final : public TwoDShape<ValueType>
{
public:

	explicit constexpr Hexagon (ValueType sideLength) noexcept;

	LIMES_CONSTEXPR_COPYABLE (Hexagon);
	LIMES_CONSTEXPR_MOVABLE (Hexagon);

	[[nodiscard]] ValueType area() const noexcept final;

	[[nodiscard]] constexpr ValueType perimeter() const noexcept final;

	[[nodiscard]] constexpr ValueType sideLength() const noexcept;

	[[nodiscard]] constexpr bool operator== (const Hexagon& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Hexagon& other) const noexcept;

	[[nodiscard]] static constexpr Angle<ValueType> interiorAngle() noexcept;
	[[nodiscard]] static constexpr Angle<ValueType> exteriorAngle() noexcept;

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
constexpr Angle<ValueType> Hexagon<ValueType>::interiorAngle() noexcept
{
	return Angle<ValueType>{ 120 };
}

template <Scalar ValueType>
constexpr Angle<ValueType> Hexagon<ValueType>::exteriorAngle() noexcept
{
	return Angle<ValueType>{ 60 };
}

}  // namespace math::geometry

LIMES_END_NAMESPACE
