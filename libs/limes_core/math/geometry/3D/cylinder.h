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

/** @file
	This file defines the Cylinder class.
	@ingroup ThreeDgeometry
 */

LIMES_BEGIN_NAMESPACE

namespace math::geometry
{

/** This class represents a %cylinder.
	@ingroup ThreeDgeometry
 */
template <Scalar ValueType>
class LIMES_EXPORT Cylinder final : public ThreeDShape<ValueType>
{
public:

	/** @name Constructors */
	///@{
	/** Creates a cylinder with the given radius and height. */
	explicit constexpr Cylinder (ValueType radiusToUse, ValueType heightToUse) noexcept;

	/** Creates a cylinder with the given face and height. */
	explicit constexpr Cylinder (const Circle<ValueType>& face, ValueType heightToUse) noexcept;
	///@}

	LIMES_CONSTEXPR_COPYABLE (Cylinder);
	LIMES_CONSTEXPR_MOVABLE (Cylinder);

	/** Returns the volume of the %cylinder.
		The formula is @f$ V=\pi*radius^2*height @f$.
	 */
	[[nodiscard]] constexpr ValueType volume() const noexcept final;

	/** Returns the surface area of the %cylinder.
		The formula is @f$ A=2*\pi*radius*height+2*\pi*radius^2 @f$.
	 */
	[[nodiscard]] constexpr ValueType surfaceArea() const noexcept final;

	/** Returns the surface perimeter of the %cylinder.
		The formula is @f$ P=(2*radius)+(2*height) @f$.
	 */
	[[nodiscard]] constexpr ValueType surfacePerimeter() const noexcept;

	/** Returns the circumference of the %cylinder.
		The formula is @f$ C=2*\pi*radius @f$.
		@see Circle::circumference
	 */
	[[nodiscard]] constexpr ValueType circumference() const noexcept;

	/** Returns the diameter of the %cylinder.
		The formula is @f$ D=2*radius @f$.
		@see Circle::diameter
	 */
	[[nodiscard]] constexpr ValueType diameter() const noexcept;

	/** Returns the radius of the %cylinder. */
	[[nodiscard]] constexpr ValueType radius() const noexcept;

	/** Returns a Circle representing a face of this %cylinder. */
	[[nodiscard]] constexpr Circle<ValueType> getFace() const noexcept;

	/** @name Equality comparisons
		Cylinders are considered equal if their radii and heights are the same.
	 */
	///@{
	[[nodiscard]] constexpr bool operator== (const Cylinder& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Cylinder& other) const noexcept;
	///@}

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
