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
#include "triangle.h"
#include <cmath>

LIMES_BEGIN_NAMESPACE

namespace math::geometry
{

template <Scalar ValueType>
class LIMES_EXPORT Rectangle final
{
public:

	explicit constexpr Rectangle (ValueType lengthToUse, ValueType widthToUse) noexcept;

	LIMES_CONSTEXPR_COPYABLE (Rectangle);
	LIMES_CONSTEXPR_MOVABLE (Rectangle);

	[[nodiscard]] constexpr ValueType area() const noexcept;

	[[nodiscard]] constexpr ValueType perimeter() const noexcept;

	[[nodiscard]] constexpr ValueType length() const noexcept;

	[[nodiscard]] constexpr ValueType width() const noexcept;

	[[nodiscard]] constexpr bool isSquare() const noexcept;

	[[nodiscard]] Triangle<ValueType> bisect() const noexcept;

	[[nodiscard]] constexpr bool operator== (const Rectangle& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Rectangle& other) const noexcept;

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
Triangle<ValueType> Rectangle<ValueType>::bisect() const noexcept
{
	const auto diag = std::sqrt ((m_length * m_length) + (m_width * m_width));

	return Triangle<ValueType> { m_length, m_width, diag };
}

template <Scalar ValueType>
constexpr bool Rectangle<ValueType>::operator== (const Rectangle& other) const noexcept
{
	return m_length == other.m_length && m_width == other.m_width;
}

template <Scalar ValueType>
constexpr bool Rectangle<ValueType>::operator!= (const Rectangle& other) const noexcept
{
	return ! (*this == other);
}

}  // namespace math::geometry

LIMES_END_NAMESPACE
