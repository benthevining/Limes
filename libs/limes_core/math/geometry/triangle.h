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
#include "../../misc/preprocessor.h"
#include "angle.h"
#include <cmath>

LIMES_BEGIN_NAMESPACE

namespace math::geometry
{

template <Scalar ValueType>
class LIMES_EXPORT Triangle final
{
public:

	using Angle = Angle<ValueType>;

	explicit constexpr Triangle (ValueType base, ValueType height, ValueType hypotenuse) noexcept;

	explicit Triangle (ValueType sideLength, const Angle& oppositeAngle, const Angle& anotherAngle);

	explicit Triangle (ValueType sideOne, ValueType sideTwo, const Angle& angleBetweenThem);

	LIMES_CONSTEXPR_COPYABLE (Triangle);
	LIMES_CONSTEXPR_MOVABLE (Triangle);

	[[nodiscard]] constexpr ValueType area() const noexcept;

	[[nodiscard]] constexpr ValueType perimeter() const noexcept;

	[[nodiscard]] constexpr ValueType base() const noexcept;

	[[nodiscard]] constexpr ValueType height() const noexcept;

	[[nodiscard]] constexpr ValueType hypotenuse() const noexcept;

	[[nodiscard]] constexpr bool isEquilateral() const noexcept;

	[[nodiscard]] constexpr bool isIsosceles() const noexcept;

	[[nodiscard]] constexpr bool isScalene() const noexcept;

	[[nodiscard]] constexpr bool isRight() const noexcept;

	[[nodiscard]] constexpr bool isAcute() const noexcept;

	[[nodiscard]] constexpr bool isObtuse() const noexcept;

	[[nodiscard]] bool isSimilarTo (const Triangle& other) const noexcept;

	[[nodiscard]] constexpr bool operator== (const Triangle& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Triangle& other) const noexcept;

	[[nodiscard]] Angle getBaseAngle() const noexcept;

	[[nodiscard]] Angle getHeightAngle() const noexcept;

	[[nodiscard]] Angle getHypotenuseAngle() const noexcept;

	[[nodiscard]] Triangle bisect() const noexcept;

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
Triangle<ValueType>::Triangle (ValueType sideLength, const Angle& oppositeAngle, const Angle& anotherAngle)
{
	const auto sinA = std::sin (oppositeAngle.degrees());

	const auto sideTwo = (sideLength * std::sin (anotherAngle.degrees())) / sinA;

	const auto thirdAngle = ValueType (180) - oppositeAngle.degrees() - anotherAngle.degrees();

	const auto sideThree = (sideLength * std::sin (thirdAngle)) / sinA;

	m_hypotenuse = std::max ({ sideLength, sideTwo, sideThree });

	m_base = std::min ({ sideLength, sideTwo, sideThree });

	// select the middle value of the 3 side lengths
	m_height = std::max ({ std::min (sideLength, sideTwo),
						   std::min (std::max (sideLength, sideTwo)),
						   sideThree });
}

template <Scalar ValueType>
Triangle<ValueType>::Triangle (ValueType sideOne, ValueType sideTwo, const Angle& angleBetweenThem)
{
	const auto thirdSide = std::sqrt ((sideOne * sideOne) + (sideTwo * sideTwo) - (ValueType (2) * sideOne * sideTwo * std::cos (angleBetweenThem.degrees())));

	m_hypotenuse = std::max ({ sideOne, sideTwo, thirdSide });

	m_base = std::min ({ sideOne, sideTwo, thirdSide });

	// select the middle value of the 3 side lengths
	m_height = std::max ({ std::min (sideOne, sideTwo),
						   std::min (std::max (sideOne, sideTwo)),
						   thirdSide });
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
bool Triangle<ValueType>::isSimilarTo (const Triangle& other) const noexcept
{
	return getBaseAngle() == other.getBaseAngle()
		&& getHeightAngle() == other.getHeightAngle()
		&& getHypotenuseAngle() == other.getHypotenuseAngle();
}

template <Scalar ValueType>
Triangle<ValueType> Triangle<ValueType>::bisect() const noexcept
{
	return Triangle { m_base / ValueType (2), m_height, getHypotenuseAngle() };
}

}  // namespace math::geometry

LIMES_END_NAMESPACE
