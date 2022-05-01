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
#include "../2D/square.h"
#include "3Dshape.h"
#include <cmath>

LIMES_BEGIN_NAMESPACE

namespace math::geometry
{

template <Scalar ValueType>
class LIMES_EXPORT RectangularPrism final : public ThreeDShape<ValueType>
{
public:

	explicit constexpr RectangularPrism (ValueType length, ValueType width, ValueType height) noexcept;

	LIMES_CONSTEXPR_COPYABLE (RectangularPrism);
	LIMES_CONSTEXPR_MOVABLE (RectangularPrism);

	[[nodiscard]] constexpr ValueType volume() const noexcept final;

	[[nodiscard]] constexpr ValueType surfaceArea() const noexcept final;

	[[nodiscard]] ValueType spaceDiagonal() const noexcept;

	[[nodiscard]] constexpr bool isCube() const noexcept;

	[[nodiscard]] constexpr ValueType length() const noexcept;

	[[nodiscard]] constexpr ValueType width() const noexcept;

	[[nodiscard]] constexpr ValueType height() const noexcept;

	[[nodiscard]] constexpr bool operator== (const RectangularPrism& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const RectangularPrism& other) const noexcept;

private:

	ValueType m_length, m_width, m_height;
};


template <Scalar ValueType>
constexpr RectangularPrism<ValueType>::RectangularPrism (ValueType length, ValueType width, ValueType height) noexcept
	: m_length (length), m_width (width), m_height (height)
{
	LIMES_ASSERT (m_length > 0);
	LIMES_ASSERT (m_width > 0);
	LIMES_ASSERT (m_height > 0);
}

template <Scalar ValueType>
constexpr ValueType RectangularPrism<ValueType>::volume() const noexcept
{
	return m_length * m_width * m_height;
}

template <Scalar ValueType>
constexpr ValueType RectangularPrism<ValueType>::surfaceArea() const noexcept
{
	return ValueType (2) * ((m_width * m_length) + (m_height * m_length) + (m_height * m_width));
}

template <Scalar ValueType>
ValueType RectangularPrism<ValueType>::spaceDiagonal() const noexcept
{
	return std::sqrt ((m_length * m_length) + (m_width * m_width) + (m_height * m_height));
}

template <Scalar ValueType>
constexpr bool RectangularPrism<ValueType>::isCube() const noexcept
{
	return m_length == m_width && m_length == m_height;
}

template <Scalar ValueType>
constexpr ValueType RectangularPrism<ValueType>::length() const noexcept
{
	return m_length;
}

template <Scalar ValueType>
constexpr ValueType RectangularPrism<ValueType>::width() const noexcept
{
	return m_width;
}

template <Scalar ValueType>
constexpr ValueType RectangularPrism<ValueType>::height() const noexcept
{
	return m_height;
}

template <Scalar ValueType>
constexpr bool RectangularPrism<ValueType>::operator== (const RectangularPrism& other) const noexcept
{
	return m_length == other.m_length && m_width == other.m_width && m_height == other.m_height;
}

template <Scalar ValueType>
constexpr bool RectangularPrism<ValueType>::operator!= (const RectangularPrism& other) const noexcept
{
	return ! (*this == other);
}

}  // namespace math::geometry

LIMES_END_NAMESPACE
