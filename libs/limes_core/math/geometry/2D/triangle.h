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

#include "../../../limes_namespace.h"
#include <limes_export.h>
#include <limes_platform.h>
#include "./2Dshape.h"
#include "../../../misc/preprocessor.h"
#include "./angle.h"
#include "../../mathFunctions.h"
#include <cmath>

/** @file
	This file defines the Triangle class.
	@ingroup TwoDgeometry
 */

LIMES_BEGIN_NAMESPACE

namespace math::geometry
{

/** Represents a %triangle.
	@ingroup TwoDgeometry

	@todo make entire API constexpr
	@todo static assertion tests
 */
template <Scalar ValueType>
class LIMES_EXPORT Triangle final : public TwoDShape<ValueType>
{
public:

	/** @name Constructors */
	///@{
	/** Creates a triangle with all three side lengths explicitly specified. */
	explicit constexpr Triangle (ValueType base, ValueType height, ValueType hypotenuse) noexcept;

	/** Creates a triangle from one side length and two angles.
		@param sideLength The length of one of the sides.
		@param oppositeAngle The angle opposite the side \c sideLength
		@param anotherAngle Another angle of the triangle
	 */
	explicit Triangle (ValueType sideLength, const Angle<ValueType>& oppositeAngle, const Angle<ValueType>& anotherAngle);

	/** Creates a triangle from two side lengths and the angle between them. */
	explicit Triangle (ValueType sideOne, ValueType sideTwo, const Angle<ValueType>& angleBetweenThem);
	///@}

	LIMES_CONSTEXPR_COPYABLE (Triangle)
	LIMES_CONSTEXPR_MOVABLE (Triangle)

	/** Returns the area of the %triangle.
		The formula is @f$ A=\frac{base*height}{2} @f$.
	 */
	[[nodiscard]] constexpr ValueType area() const noexcept final;

	/** Returns the perimeter of the %triangle.
		The formula is @f$ P=base+height+hypot @f$.
	 */
	[[nodiscard]] constexpr ValueType perimeter() const noexcept final;

	/** Identifies the sides of a %triangle. */
	enum class Side
	{
		base,
		height,
		hypotenuse
	};

	/** @name Side length queries */
	///@{
	/** Returns the length of the triangle's base side. */
	[[nodiscard]] constexpr ValueType base() const noexcept;

	/** Returns the length of the triangle's height side. */
	[[nodiscard]] constexpr ValueType height() const noexcept;

	/** Returns the length of the triangle's hypotenuse side. */
	[[nodiscard]] constexpr ValueType hypotenuse() const noexcept;

	/** Returns the length of the requested side. */
	[[nodiscard]] ValueType getSide (Side side) const noexcept;
	///@}

	/** Returns the length of the vertical altitude from the triangle's base to the point opposite the base.
		The formula is @f$ a=2*\frac{base*height}{2*base} @f$.
	 */
	[[nodiscard]] constexpr ValueType altitude() const noexcept;

	/** @name %Triangle type queries */
	///@{
	/** Returns true if all three sides are the same length. */
	constexpr bool isEquilateral() const noexcept;

	/** Returns true if two sides have the same length. */
	constexpr bool isIsosceles() const noexcept;

	/** Returns true if no two sides are the same length. */
	[[nodiscard]] constexpr bool isScalene() const noexcept;

	/** Returns true if one of the angles is a right angle. */
	[[nodiscard]] constexpr bool isRight() const noexcept;

	/** Returns true if all three angles are acute. */
	[[nodiscard]] constexpr bool isAcute() const noexcept;

	/** Returns true if one angle is obtuse. */
	[[nodiscard]] constexpr bool isObtuse() const noexcept;
	///@}

	/** Returns true if the two triangles have the same angle measures, regardless of their side lengths. */
	[[nodiscard]] bool isSimilarTo (const Triangle& other) const noexcept;

	/** @name Equality comparisons
		Triangles are considered equal if all three sides have the same length.
	 */
	///@{
	[[nodiscard]] constexpr bool operator== (const Triangle& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Triangle& other) const noexcept;
	///@}

	/** @name Angle queries */
	///@{
	/** Returns the angle opposite the base side. */
	[[nodiscard]] Angle<ValueType> getBaseAngle() const noexcept;

	/** Returns the angle opposite the height side. */
	[[nodiscard]] Angle<ValueType> getHeightAngle() const noexcept;

	/** Returns the angle opposite the hypotenuse side. */
	[[nodiscard]] Angle<ValueType> getHypotenuseAngle() const noexcept;

	/** Returns the angle opposite the requested side. */
	[[nodiscard]] Angle<ValueType> getAngle (Side side) const noexcept;
	///@}

	/** Returns a new %triangle created by bisecting the requested side of the current %triangle. */
	[[nodiscard]] Triangle bisect (Side side = Side::base) const noexcept;

private:

	ValueType m_base, m_height, m_hypotenuse;
};


template <Scalar ValueType>
constexpr Triangle<ValueType>::Triangle (ValueType base, ValueType height, ValueType hypotenuse) noexcept
	: m_base (base), m_height (height), m_hypotenuse (hypotenuse)
{
	LIMES_ASSERT (m_base > 0);
	LIMES_ASSERT (m_height > 0);
	LIMES_ASSERT ((m_hypotenuse > m_base && m_hypotenuse > m_height) || isEquilateral() || isIsosceles());
}

template <Scalar ValueType>
Triangle<ValueType>::Triangle (ValueType sideLength, const Angle<ValueType>& oppositeAngle, const Angle<ValueType>& anotherAngle)
{
	const auto sinA = std::sin (oppositeAngle.degrees());

	const auto sideTwo = (sideLength * std::sin (anotherAngle.degrees())) / sinA;

	const auto thirdAngle = ValueType (180) - oppositeAngle.degrees() - anotherAngle.degrees();

	const auto sideThree = (sideLength * std::sin (thirdAngle)) / sinA;

	m_hypotenuse = std::max ({ sideLength, sideTwo, sideThree });

	m_base = std::min ({ sideLength, sideTwo, sideThree });

	m_height = math::middleOfThree (sideLength, sideTwo, sideThree);
}

template <Scalar ValueType>
Triangle<ValueType>::Triangle (ValueType sideOne, ValueType sideTwo, const Angle<ValueType>& angleBetweenThem)
{
	const auto thirdSide = std::sqrt ((sideOne * sideOne) + (sideTwo * sideTwo) - (ValueType (2) * sideOne * sideTwo * std::cos (angleBetweenThem.degrees())));

	m_hypotenuse = std::max ({ sideOne, sideTwo, thirdSide });

	m_base = std::min ({ sideOne, sideTwo, thirdSide });

	m_height = math::middleOfThree (sideOne, sideTwo, thirdSide);
}

template <Scalar ValueType>
constexpr ValueType Triangle<ValueType>::area() const noexcept
{
	return m_base * m_height * ValueType (0.5);
}

template <Scalar ValueType>
constexpr ValueType Triangle<ValueType>::perimeter() const noexcept
{
	return m_base + m_height + m_hypotenuse;
}

template <Scalar ValueType>
constexpr ValueType Triangle<ValueType>::base() const noexcept
{
	return m_base;
}

template <Scalar ValueType>
constexpr ValueType Triangle<ValueType>::height() const noexcept
{
	return m_height;
}

template <Scalar ValueType>
constexpr ValueType Triangle<ValueType>::hypotenuse() const noexcept
{
	return m_hypotenuse;
}

template <Scalar ValueType>
constexpr ValueType Triangle<ValueType>::altitude() const noexcept
{
	return ValueType (2) * area() / m_base;
}

template <Scalar ValueType>
constexpr bool Triangle<ValueType>::isEquilateral() const noexcept
{
	return m_base == m_height && m_base == m_hypotenuse;
}

template <Scalar ValueType>
constexpr bool Triangle<ValueType>::isIsosceles() const noexcept
{
	return m_base == m_height || m_base == m_hypotenuse || m_height == m_hypotenuse;
}

template <Scalar ValueType>
constexpr bool Triangle<ValueType>::isScalene() const noexcept
{
	return ! (isEquilateral() || isIsosceles());
}

template <Scalar ValueType>
constexpr bool Triangle<ValueType>::isRight() const noexcept
{
	return (m_hypotenuse * m_hypotenuse) == ((m_base * m_base) + (m_height * m_height));
}

template <Scalar ValueType>
constexpr bool Triangle<ValueType>::isAcute() const noexcept
{
	return (m_hypotenuse * m_hypotenuse) > ((m_base * m_base) + (m_height * m_height));
}

template <Scalar ValueType>
constexpr bool Triangle<ValueType>::isObtuse() const noexcept
{
	return (m_hypotenuse * m_hypotenuse) < ((m_base * m_base) + (m_height * m_height));
}

template <Scalar ValueType>
constexpr bool Triangle<ValueType>::operator== (const Triangle& other) const noexcept
{
	return m_base == other.m_base && m_height == other.m_height && m_hypotenuse == other.m_hypotenuse;
}

template <Scalar ValueType>
constexpr bool Triangle<ValueType>::operator!= (const Triangle& other) const noexcept
{
	return ! (*this == other);
}

template <Scalar ValueType>
Angle<ValueType> Triangle<ValueType>::getBaseAngle() const noexcept
{
	return std::acos (((m_base * m_base) - (m_height * m_height) - (m_hypotenuse * m_hypotenuse)) / (ValueType (-2) * m_height * m_hypotenuse));
}

template <Scalar ValueType>
Angle<ValueType> Triangle<ValueType>::getHeightAngle() const noexcept
{
	return std::acos (((m_height * m_height) - (m_base * m_base) - (m_hypotenuse * m_hypotenuse)) / (ValueType (-2) * m_base * m_hypotenuse));
}

template <Scalar ValueType>
Angle<ValueType> Triangle<ValueType>::getHypotenuseAngle() const noexcept
{
	return std::acos (((m_hypotenuse * m_hypotenuse) - (m_base * m_base) - (m_height * m_height)) / (ValueType (-2) * m_base * m_height));
}

template <Scalar ValueType>
Angle<ValueType> Triangle<ValueType>::getAngle (Side side) const noexcept
{
	if (side == Side::base)
		return getBaseAngle();

	if (side == Side::height)
		return getHeightAngle();

	return getHypotenuseAngle();
}

template <Scalar ValueType>
ValueType Triangle<ValueType>::getSide (Side side) const noexcept
{
	if (side == Side::base)
		return m_base;

	if (side == Side::height)
		return m_height;

	return m_hypotenuse;
}

template <Scalar ValueType>
bool Triangle<ValueType>::isSimilarTo (const Triangle& other) const noexcept
{
	return getBaseAngle() == other.getBaseAngle()
		&& getHeightAngle() == other.getHeightAngle()
		&& getHypotenuseAngle() == other.getHypotenuseAngle();
}

template <Scalar ValueType>
Triangle<ValueType> Triangle<ValueType>::bisect (Side side) const noexcept
{
	if (side == Side::base)
		return Triangle { m_base / ValueType (2), m_height, getHypotenuseAngle() };

	if (side == Side::height)
		return Triangle { m_height / ValueType (2), m_hypotenuse, getBaseAngle() };

	return Triangle { m_hypotenuse / ValueType (2), m_base, getHeightAngle() };
}

}  // namespace math::geometry

LIMES_END_NAMESPACE
