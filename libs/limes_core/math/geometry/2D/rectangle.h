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
#include "./triangle.h"
#include <cmath>

/** @file
	This file defines the Rectangle class.
	@ingroup TwoDgeometry
 */

LIMES_BEGIN_NAMESPACE

namespace math::geometry
{

/** Represents a %rectangle.
	A rectangle may or may not be square.
	@ingroup TwoDgeometry
	@see Square
 */
template <Scalar ValueType>
class LIMES_EXPORT Rectangle final : public TwoDShape<ValueType>
{
public:
	/** Creates a rectangle with the specified length and width (in unspecified units). */
	explicit constexpr Rectangle (ValueType lengthToUse, ValueType widthToUse) noexcept;

	LIMES_CONSTEXPR_COPYABLE (Rectangle)
	LIMES_CONSTEXPR_MOVABLE (Rectangle)

	/** Returns the area of the %rectangle.
		The formula is @f$ A=length*width @f$.
	 */
	[[nodiscard]] constexpr ValueType area() const noexcept final;

	/** Returns the perimeter of the %rectangle.
		The formula is @f$ A=2*(length+width) @f$.
	 */
	[[nodiscard]] constexpr ValueType perimeter() const noexcept final;

	/** Returns the length of the %rectangle. */
	[[nodiscard]] constexpr ValueType length() const noexcept;

	/** Returns the width of the %rectangle. */
	[[nodiscard]] constexpr ValueType width() const noexcept;

	/** Returns true if the %rectangle is square; ie, the length is equal to the width. */
	[[nodiscard]] constexpr bool isSquare() const noexcept;

	/** Returns the length of the internal diagonal of the %rectangle.
		The formula is @f$ d=\sqrt{length^2+width^2} @f$.
	 */
	[[nodiscard]] ValueType diagonal() const noexcept;

	/** Returns a triangle representing the %rectangle bisected along its internal diagonal. */
	[[nodiscard]] Triangle<ValueType> bisectDiagonal() const noexcept;

	/** @name Equality comparisons
		Rectangles are considered equal if their areas are equal.
	 */
	///@{
	[[nodiscard]] constexpr bool operator== (const Rectangle& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Rectangle& other) const noexcept;
	///@}

	/** @name Greater/less than comparisons
		Compares rectangles based on their areas.
	 */
	///@{
	[[nodiscard]] constexpr bool operator> (const Rectangle& other) const noexcept;
	[[nodiscard]] constexpr bool operator<(const Rectangle& other) const noexcept;
	///@}

private:
	ValueType m_length, m_width;
};


template <Scalar ValueType>
constexpr Rectangle<ValueType>::Rectangle (ValueType lengthToUse, ValueType widthToUse) noexcept
	: m_length (lengthToUse), m_width (widthToUse)
{
	LIMES_ASSERT (m_length > 0);
	LIMES_ASSERT (m_width > 0);
}

template <Scalar ValueType>
constexpr ValueType Rectangle<ValueType>::area() const noexcept
{
	return m_length * m_width;
}

template <Scalar ValueType>
constexpr ValueType Rectangle<ValueType>::perimeter() const noexcept
{
	return ValueType (2) * (m_length + m_width);
}

template <Scalar ValueType>
constexpr ValueType Rectangle<ValueType>::length() const noexcept
{
	return m_length;
}

template <Scalar ValueType>
constexpr ValueType Rectangle<ValueType>::width() const noexcept
{
	return m_width;
}

template <Scalar ValueType>
constexpr bool Rectangle<ValueType>::isSquare() const noexcept
{
	return m_length == m_width;
}

template <Scalar ValueType>
ValueType Rectangle<ValueType>::diagonal() const noexcept
{
	return std::sqrt ((m_length * m_length) + (m_width * m_width));
}

template <Scalar ValueType>
Triangle<ValueType> Rectangle<ValueType>::bisectDiagonal() const noexcept
{
	return Triangle<ValueType> { m_length, m_width, diagonal() };
}

template <Scalar ValueType>
constexpr bool Rectangle<ValueType>::operator== (const Rectangle& other) const noexcept
{
	return area() == other.area();
}

template <Scalar ValueType>
constexpr bool Rectangle<ValueType>::operator!= (const Rectangle& other) const noexcept
{
	return area() != other.area();
}

template <Scalar ValueType>
constexpr bool Rectangle<ValueType>::operator> (const Rectangle& other) const noexcept
{
	return area() > other.area();
}

template <Scalar ValueType>
constexpr bool Rectangle<ValueType>::operator<(const Rectangle& other) const noexcept
{
	return area() < other.area();
}

}  // namespace math::geometry

LIMES_END_NAMESPACE
