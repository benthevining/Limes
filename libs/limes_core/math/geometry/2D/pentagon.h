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
class LIMES_EXPORT Pentagon final : public TwoDShape<ValueType>
{
public:

	explicit constexpr Pentagon (ValueType sideLength) noexcept;

	LIMES_CONSTEXPR_COPYABLE (Pentagon);
	LIMES_CONSTEXPR_MOVABLE (Pentagon);

	[[nodiscard]] ValueType area() const noexcept final;

	[[nodiscard]] constexpr ValueType perimeter() const noexcept final;

	[[nodiscard]] ValueType diagonal() const noexcept;

	[[nodiscard]] constexpr ValueType sideLength() const noexcept;

	[[nodiscard]] constexpr bool operator== (const Pentagon& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Pentagon& other) const noexcept;

	[[nodiscard]] static constexpr Angle<ValueType> interiorAngle() noexcept;
	[[nodiscard]] static constexpr Angle<ValueType> exteriorAngle() noexcept;

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
constexpr Angle<ValueType> Pentagon<ValueType>::interiorAngle() noexcept
{
	return Angle<ValueType>{ 108 };
}

template <Scalar ValueType>
constexpr Angle<ValueType> Pentagon<ValueType>::exteriorAngle() noexcept
{
	return Angle<ValueType>{ 72 };
}

}  // namespace math::geometry

LIMES_END_NAMESPACE
