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
#include "square.h"

LIMES_BEGIN_NAMESPACE

namespace math::geometry
{

template <Scalar ValueType>
class LIMES_EXPORT Cube final
{
public:

	explicit constexpr Cube (ValueType sideLengthToUse) noexcept;

	LIMES_CONSTEXPR_COPYABLE (Cube);
	LIMES_CONSTEXPR_MOVABLE (Cube);

	[[nodiscard]] constexpr ValueType volume() const noexcept;

	[[nodiscard]] constexpr ValueType surfaceArea() const noexcept;

	[[nodiscard]] constexpr ValueType surfacePerimeter() const noexcept;

	[[nodiscard]] constexpr ValueType sideLength() const noexcept;

	[[nodiscard]] constexpr bool operator== (const Cube& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Cube& other) const noexcept;

	[[nodiscard]] constexpr bool operator> (const Cube& other) const noexcept;
	[[nodiscard]] constexpr bool operator< (const Cube& other) const noexcept;

	[[nodiscard]] constexpr Square<ValueType> getFace() const noexcept;

private:

	ValueType side_length;
};


template <Scalar ValueType>
constexpr Cube<ValueType>::Cube (ValueType sideLengthToUse) noexcept
	: side_length (sideLengthToUse)
{
	LIMES_ASSERT (side_length > 0);
}

template <Scalar ValueType>
constexpr ValueType Cube<ValueType>::volume() const noexcept
{
	return side_length * side_length * side_length;
}

template <Scalar ValueType>
constexpr ValueType Cube<ValueType>::surfaceArea() const noexcept
{
	return ValueType (6) * side_length * side_length;
}

template <Scalar ValueType>
constexpr ValueType Cube<ValueType>::surfacePerimeter() const noexcept
{
	return ValueType (12) * side_length;
}

template <Scalar ValueType>
constexpr ValueType Cube<ValueType>::sideLength() const noexcept
{
	return side_length;
}

template <Scalar ValueType>
constexpr bool Cube<ValueType>::operator== (const Cube& other) const noexcept
{
	return side_length == other.side_length;
}

template <Scalar ValueType>
constexpr bool Cube<ValueType>::operator!= (const Cube& other) const noexcept
{
	return side_length != other.side_length;
}

template <Scalar ValueType>
constexpr bool Cube<ValueType>::operator> (const Cube& other) const noexcept
{
	return side_length > other.side_length;
}

template <Scalar ValueType>
constexpr bool Cube<ValueType>::operator< (const Cube& other) const noexcept
{
	return side_length < other.side_length;
}

template <Scalar ValueType>
constexpr Square<ValueType> Cube<ValueType>::getFace() const noexcept
{
	return Square<ValueType> { side_length };
}

}  // namespace math::geometry

LIMES_END_NAMESPACE
