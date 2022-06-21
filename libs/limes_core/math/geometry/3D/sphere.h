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
#include "../../mathFunctions.h"
#include "3Dshape.h"
#include "../2D/circle.h"

/** @file
	This file defines the Sphere class.
	@ingroup ThreeDgeometry
 */

LIMES_BEGIN_NAMESPACE

namespace math::geometry
{

/** This class represents a %sphere.
	@ingroup ThreeDgeometry
 */
template <Scalar ValueType>
class LIMES_EXPORT Sphere final : public ThreeDShape<ValueType>
{
public:

	/** @name Constructors */
	///@{
	/** Creates a sphere with the given radius. */
	explicit constexpr Sphere (ValueType radiusToUse) noexcept;

	/** Creates a sphere with the given great circle. The radius of the circle will be the radius of the sphere. */
	explicit constexpr Sphere (const Circle<ValueType>& greatCircle) noexcept;
	///@}

	LIMES_CONSTEXPR_COPYABLE (Sphere)
	LIMES_CONSTEXPR_MOVABLE (Sphere)

	/** Returns the volume of the %sphere.
		The formula is @f$ V=\pi*radius^3*\frac{4}{3} @f$.
	 */
	[[nodiscard]] constexpr ValueType volume() const noexcept final;

	/** Returns the surface area of the %sphere.
		The formula is @f$ A=4*\pi*radius^2 @f$.
	 */
	[[nodiscard]] constexpr ValueType surfaceArea() const noexcept final;

	/** Returns the circumference of the %sphere.
		The formula is @f$ C=2*\pi*radius @f$.
		@see Circle::circumference
	 */
	[[nodiscard]] constexpr ValueType circumference() const noexcept;

	/** Returns the diameter of the %sphere.
		The formula is @f$ d=2*radius @f$.
		@see Circle::diameter
	 */
	[[nodiscard]] constexpr ValueType diameter() const noexcept;

	/** Returns the radius of the %sphere. */
	[[nodiscard]] constexpr ValueType radius() const noexcept;

	/** Returns a Circle object with the same radius as this %sphere. */
	[[nodiscard]] constexpr Circle<ValueType> getGreatCircle() const noexcept;

	/** @name Equality comparison
		Spheres are considered equal if their radii are equal.
	 */
	///@{
	[[nodiscard]] constexpr bool operator== (const Sphere& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Sphere& other) const noexcept;
	///@}

	/** @name Greater/less than comparison
		Compares spheres based on their radii.
	 */
	///@{
	[[nodiscard]] constexpr bool operator> (const Sphere& other) const noexcept;
	[[nodiscard]] constexpr bool operator<(const Sphere& other) const noexcept;
	///@}

private:

	ValueType m_radius;
};


template <Scalar ValueType>
constexpr Sphere<ValueType>::Sphere (ValueType radiusToUse) noexcept
	: m_radius (radiusToUse)
{
	LIMES_ASSERT (m_radius > 0);
}

template <Scalar ValueType>
constexpr Sphere<ValueType>::Sphere (const Circle<ValueType>& greatCircle) noexcept
	: m_radius (greatCircle.radius())
{
}

template <Scalar ValueType>
constexpr ValueType Sphere<ValueType>::volume() const noexcept
{
	return constants::pi<ValueType> * m_radius * m_radius * m_radius * ValueType (4) / ValueType (3);
}

template <Scalar ValueType>
constexpr ValueType Sphere<ValueType>::surfaceArea() const noexcept
{
	return ValueType (4) * constants::pi<ValueType> * m_radius * m_radius;
}

template <Scalar ValueType>
constexpr ValueType Sphere<ValueType>::circumference() const noexcept
{
	return ValueType (2) * constants::pi<ValueType> * m_radius;
}

template <Scalar ValueType>
constexpr ValueType Sphere<ValueType>::diameter() const noexcept
{
	return ValueType (2) * m_radius;
}

template <Scalar ValueType>
constexpr ValueType Sphere<ValueType>::radius() const noexcept
{
	return m_radius;
}

template <Scalar ValueType>
constexpr Circle<ValueType> Sphere<ValueType>::getGreatCircle() const noexcept
{
	return Circle { m_radius };
}

template <Scalar ValueType>
constexpr bool Sphere<ValueType>::operator== (const Sphere& other) const noexcept
{
	return m_radius == other.m_radius;
}

template <Scalar ValueType>
constexpr bool Sphere<ValueType>::operator!= (const Sphere& other) const noexcept
{
	return m_radius != other.m_radius;
}

template <Scalar ValueType>
constexpr bool Sphere<ValueType>::operator> (const Sphere& other) const noexcept
{
	return m_radius > other.m_radius;
}

template <Scalar ValueType>
constexpr bool Sphere<ValueType>::operator<(const Sphere& other) const noexcept
{
	return m_radius < other.m_radius;
}

}  // namespace math::geometry

LIMES_END_NAMESPACE
