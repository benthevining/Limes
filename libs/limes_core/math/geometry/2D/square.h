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
#include "rectangle.h"
#include "triangle.h"
#include <cmath>

/** @file
	This file defines the Square class.
	@ingroup 2Dgeometry
 */

LIMES_BEGIN_NAMESPACE

namespace math::geometry
{

/** Represents a square.
	@ingroup 2Dgeometry
 */
template <Scalar ValueType>
class LIMES_EXPORT Square final : public TwoDShape<ValueType>
{
public:

	using Rect = Rectangle<ValueType>;

	/** Creates a square with the specified side length (in unspecified units). */
	explicit constexpr Square (ValueType sideLengthToUse) noexcept;

	LIMES_CONSTEXPR_COPYABLE (Square);
	LIMES_CONSTEXPR_MOVABLE (Square);

	/** Returns the area of the square. */
	[[nodiscard]] constexpr ValueType area() const noexcept final;

	/** Returns the perimeter of the square. */
	[[nodiscard]] constexpr ValueType perimeter() const noexcept final;

	/** Returns the side length of the square. */
	[[nodiscard]] constexpr ValueType sideLength() const noexcept;

	/** Returns a rectangle object with the same dimensions as this square. */
	[[nodiscard]] constexpr Rect getRectangle() const noexcept;

	/** Returns the length of the internal diagonal of the rectangle. */
	[[nodiscard]] ValueType diagonal() const noexcept;

	/** Returns a triangle representing the rectangle bisected along its internal diagonal. */
	[[nodiscard]] Triangle<ValueType> bisectDiagonal() const noexcept;

	/** @name Equality comparisons
		Squares are considered equal if their side lengths are equal.
	 */
	///@{
	[[nodiscard]] constexpr bool operator== (const Square& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Square& other) const noexcept;
	[[nodiscard]] constexpr bool operator== (const Rect& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Rect& other) const noexcept;
	///@}

	/** @name Greater/less than comparisons
		Compares squares based on their areas.
	 */
	///@{
	[[nodiscard]] constexpr bool operator> (const Square& other) const noexcept;
	[[nodiscard]] constexpr bool operator< (const Square& other) const noexcept;
	[[nodiscard]] constexpr bool operator> (const Rect& other) const noexcept;
	[[nodiscard]] constexpr bool operator< (const Rect& other) const noexcept;
	///@}

private:

	ValueType side_length;
};


template <Scalar ValueType>
LIMES_EXPORT [[nodiscard]] constexpr bool operator== (const Rectangle<ValueType>& rect, const Square<ValueType>& square) noexcept
{
	if (! rect.isSquare())
		return false;

	return rect.length() == square.sideLength();
}

template <Scalar ValueType>
LIMES_EXPORT [[nodiscard]] constexpr bool operator!= (const Rectangle<ValueType>& rect, const Square<ValueType>& square) noexcept
{
	return ! (rect == square);
}


template <Scalar ValueType>
constexpr Square<ValueType>::Square (ValueType sideLengthToUse) noexcept
	: side_length (sideLengthToUse)
{
	LIMES_ASSERT (side_length > 0);
}

template <Scalar ValueType>
constexpr ValueType Square<ValueType>::area() const noexcept
{
	return side_length * side_length;
}

template <Scalar ValueType>
constexpr ValueType Square<ValueType>::perimeter() const noexcept
{
	return ValueType (4) * side_length;
}

template <Scalar ValueType>
constexpr ValueType Square<ValueType>::sideLength() const noexcept
{
	return side_length;
}

template <Scalar ValueType>
constexpr bool Square<ValueType>::operator== (const Square& other) const noexcept
{
	return side_length == other.side_length;
}

template <Scalar ValueType>
constexpr bool Square<ValueType>::operator!= (const Square& other) const noexcept
{
	return side_length != other.side_length;
}

template <Scalar ValueType>
constexpr bool Square<ValueType>::operator> (const Square& other) const noexcept
{
	return side_length > other.side_length;
}

template <Scalar ValueType>
constexpr bool Square<ValueType>::operator< (const Square& other) const noexcept
{
	return side_length < other.side_length;
}

template <Scalar ValueType>
constexpr bool Square<ValueType>::operator> (const Rect& other) const noexcept
{
	return area() > other.area();
}

template <Scalar ValueType>
constexpr bool Square<ValueType>::operator< (const Rect& other) const noexcept
{
	return area() < other.area();
}

template <Scalar ValueType>
constexpr Rectangle<ValueType> Square<ValueType>::getRectangle() const noexcept
{
	return Rect { side_length, side_length };
}

template <Scalar ValueType>
ValueType Square<ValueType>::diagonal() const noexcept
{
	const auto sideSquared = side_length * side_length;

	return std::sqrt (sideSquared * ValueType (2));
}

template <Scalar ValueType>
Triangle<ValueType> Square<ValueType>::bisectDiagonal() const noexcept
{
	const auto diag = std::sqrt (ValueType (2) * (side_length * side_length));

	return Triangle<ValueType> { side_length, side_length, diag };
}

template <Scalar ValueType>
constexpr bool Square<ValueType>::operator== (const Rect& other) const noexcept
{
	if (! other.isSquare())
		return false;

	return side_length == other.length();
}

template <Scalar ValueType>
constexpr bool Square<ValueType>::operator!= (const Rect& other) const noexcept
{
	return ! (*this == other);
}

}  // namespace math::geometry

LIMES_END_NAMESPACE
