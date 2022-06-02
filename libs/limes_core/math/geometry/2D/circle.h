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
#include "../../mathFunctions.h"

/** @file
	This file defines the Circle class.
	@ingroup TwoDgeometry
 */

LIMES_BEGIN_NAMESPACE

namespace math::geometry
{

/** Represents a %circle.
	@ingroup TwoDgeometry
 */
template <Scalar ValueType>
class LIMES_EXPORT Circle final : public TwoDShape<ValueType>
{
public:

	/** Creates a circle with the given radius (in unspecified units). */
	explicit constexpr Circle (ValueType radiusToUse) noexcept;

	LIMES_CONSTEXPR_COPYABLE (Circle);
	LIMES_CONSTEXPR_MOVABLE (Circle);

	/** Returns the area of the %circle.
		The formula is @f$ A=\pi*radius^2 @f$.
	 */
	[[nodiscard]] constexpr ValueType area() const noexcept final;

	/** Returns the perimeter of the %circle.
		This is the same as \c diameter() .
		The formula is @f$ P=2*radius @f$.
		@see diameter()
	 */
	[[nodiscard]] constexpr ValueType perimeter() const noexcept final;

	/** Returns the radius of the %circle. */
	[[nodiscard]] constexpr ValueType radius() const noexcept;

	/** Returns the diameter of the %circle.
	 This is the same as \c perimeter() , but is provided for more ergonomic use.
	 The formula is @f$ P=2*radius @f$.
	 @see perimeter()
	 */
	[[nodiscard]] constexpr ValueType diameter() const noexcept;

	/** @name Equality comparisons
		Compares circles for equality based on their radii.
	 */
	///@{
	[[nodiscard]] constexpr bool operator== (const Circle& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Circle& other) const noexcept;
	///@}

	/** @name Greater/less than comparisons
		Compares circles based on their radii.
	 */
	///@{
	[[nodiscard]] constexpr bool operator> (const Circle& other) const noexcept;
	[[nodiscard]] constexpr bool operator< (const Circle& other) const noexcept;
	///@}

private:

	ValueType m_radius;
};


template <Scalar ValueType>
constexpr Circle<ValueType>::Circle (ValueType radiusToUse) noexcept
	: m_radius (radiusToUse)
{
	LIMES_ASSERT (m_radius > 0);
}

template <Scalar ValueType>
constexpr ValueType Circle<ValueType>::area() const noexcept
{
	return constants::pi<ValueType> * m_radius * m_radius;
}

template <Scalar ValueType>
constexpr ValueType Circle<ValueType>::perimeter() const noexcept
{
	return ValueType (2) * constants::pi<ValueType> * m_radius;
}

template <Scalar ValueType>
constexpr ValueType Circle<ValueType>::radius() const noexcept
{
	return m_radius;
}

template <Scalar ValueType>
constexpr ValueType Circle<ValueType>::diameter() const noexcept
{
	return ValueType (2) * m_radius;
}

template <Scalar ValueType>
constexpr bool Circle<ValueType>::operator== (const Circle& other) const noexcept
{
	return m_radius == other.m_radius;
}

template <Scalar ValueType>
constexpr bool Circle<ValueType>::operator!= (const Circle& other) const noexcept
{
	return m_radius != other.m_radius;
}

template <Scalar ValueType>
constexpr bool Circle<ValueType>::operator> (const Circle& other) const noexcept
{
	return m_radius > other.m_radius;
}

template <Scalar ValueType>
constexpr bool Circle<ValueType>::operator< (const Circle& other) const noexcept
{
	return m_radius < other.m_radius;
}

}  // namespace math::geometry

LIMES_END_NAMESPACE
