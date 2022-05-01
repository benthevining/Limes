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
#include "../../mathHelpers.h"
#include "../2D/circle.h"
#include "3Dshape.h"

LIMES_BEGIN_NAMESPACE

namespace math::geometry
{

template <Scalar ValueType>
class LIMES_EXPORT Cylinder final : public ThreeDShape<ValueType>
{
public:

	explicit constexpr Cylinder (ValueType radiusToUse, ValueType heightToUse) noexcept;

	explicit constexpr Cylinder (const Circle<ValueType>& face, ValueType heightToUse) noexcept;

	LIMES_CONSTEXPR_COPYABLE (Cylinder);
	LIMES_CONSTEXPR_MOVABLE (Cylinder);

	[[nodiscard]] constexpr ValueType volume() const noexcept final;

	[[nodiscard]] constexpr ValueType surfaceArea() const noexcept final;

	[[nodiscard]] constexpr ValueType surfacePerimeter() const noexcept;

	[[nodiscard]] constexpr ValueType circumference() const noexcept;

	[[nodiscard]] constexpr ValueType diameter() const noexcept;

	[[nodiscard]] constexpr ValueType radius() const noexcept;

	[[nodiscard]] constexpr Circle<ValueType> getFace() const noexcept;

	[[nodiscard]] constexpr bool operator== (const Cylinder& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Cylinder& other) const noexcept;

private:

	ValueType m_radius, height;
};


template <Scalar ValueType>
constexpr Cylinder<ValueType>::Cylinder (ValueType radiusToUse, ValueType heightToUse) noexcept
	: m_radius (radiusToUse), height (heightToUse)
{
	LIMES_ASSERT (m_radius > 0);
	LIMES_ASSERT (height > 0);
}

template <Scalar ValueType>
constexpr Cylinder<ValueType>::Cylinder (const Circle<ValueType>& face, ValueType heightToUse) noexcept
	: m_radius (face.radius()), height (heightToUse)
{
	LIMES_ASSERT (height > 0);
}

template <Scalar ValueType>
constexpr ValueType Cylinder<ValueType>::volume() const noexcept
{
	return constants::pi<ValueType> * m_radius * m_radius * height;
}

template <Scalar ValueType>
constexpr ValueType Cylinder<ValueType>::surfaceArea() const noexcept
{
	return ValueType (2) * constants::pi<ValueType> * m_radius * height + ValueType (2) * constants::pi<ValueType> * m_radius * m_radius;
}

template <Scalar ValueType>
constexpr ValueType Cylinder<ValueType>::surfacePerimeter() const noexcept
{
	return (ValueType (2) * m_radius) + (ValueType (2) * height);
}

template <Scalar ValueType>
constexpr ValueType Cylinder<ValueType>::circumference() const noexcept
{
	return ValueType (2) * constants::pi<ValueType> * m_radius;
}

template <Scalar ValueType>
constexpr ValueType Cylinder<ValueType>::diameter() const noexcept
{
	return ValueType (2) * m_radius;
}

template <Scalar ValueType>
constexpr ValueType Cylinder<ValueType>::radius() const noexcept
{
	return m_radius;
}

template <Scalar ValueType>
constexpr Circle<ValueType> Cylinder<ValueType>::getFace() const noexcept
{
	return Circle<ValueType> { m_radius };
}

template <Scalar ValueType>
constexpr bool Cylinder<ValueType>::operator== (const Cylinder& other) const noexcept
{
	return m_radius == other.m_radius && height == other.height;
}

template <Scalar ValueType>
constexpr bool Cylinder<ValueType>::operator!= (const Cylinder& other) const noexcept
{
	return ! (*this == other);
}

}  // namespace math::geometry

LIMES_END_NAMESPACE
