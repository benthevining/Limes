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
#include "../2D/triangle.h"
#include "../2D/rectangle.h"
#include "3Dshape.h"
#include <cmath>

LIMES_BEGIN_NAMESPACE

namespace math::geometry
{

template <Scalar ValueType>
class LIMES_EXPORT Pyramid final : public ThreeDShape<ValueType>
{
public:

	explicit constexpr Pyramid (ValueType baseLength, ValueType baseWidth, ValueType height) noexcept;

	LIMES_CONSTEXPR_COPYABLE (Pyramid);
	LIMES_CONSTEXPR_MOVABLE (Pyramid);

	[[nodiscard]] ValueType surfaceArea() const noexcept final;

	[[nodiscard]] constexpr ValueType volume() const noexcept final;

	[[nodiscard]] constexpr ValueType baseLength() const noexcept;

	[[nodiscard]] constexpr ValueType baseWidth() const noexcept;

	[[nodiscard]] constexpr ValueType height() const noexcept;

	[[nodiscard]] constexpr bool operator== (const Pyramid& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Pyramid& other) const noexcept;

private:

	ValueType m_length, m_width, m_height;
};


template <Scalar ValueType>
constexpr Pyramid<ValueType>::Pyramid (ValueType baseLength, ValueType baseWidth, ValueType height) noexcept
	: m_length (baseLength), m_width (baseWidth), m_height (height)
{
	LIMES_ASSERT (m_length > 0);
	LIMES_ASSERT (m_width > 0);
	LIMES_ASSERT (m_height > 0);
}

template <Scalar ValueType>
constexpr ValueType Pyramid<ValueType>::volume() const noexcept
{
	return (m_length * m_width * m_height) / ValueType (3);
}

template <Scalar ValueType>
ValueType Pyramid<ValueType>::surfaceArea() const noexcept
{
	const auto halfW = m_width / ValueType (2);
	const auto halfL = m_length / ValueType (2);

	const auto heightSquared = m_height * m_height;

	return (m_length * m_width)
		 + (m_length * std::sqrt ((halfW * halfW) + heightSquared + m_width))
		 + (m_width * std::sqrt ((halfL * halfL) + heightSquared));
}

template <Scalar ValueType>
constexpr ValueType Pyramid<ValueType>::baseLength() const noexcept
{
	return m_length;
}

template <Scalar ValueType>
constexpr ValueType Pyramid<ValueType>::baseWidth() const noexcept
{
	return m_width;
}

template <Scalar ValueType>
constexpr ValueType Pyramid<ValueType>::height() const noexcept
{
	return m_height;
}

template <Scalar ValueType>
constexpr bool Pyramid<ValueType>::operator== (const Pyramid& other) const noexcept
{
	return m_length == other.m_length && m_width == other.m_width && m_height == other.m_height;
}

template <Scalar ValueType>
constexpr bool Pyramid<ValueType>::operator!= (const Pyramid& other) const noexcept
{
	return ! (*this == other);
}

}  // namespace math::geometry

LIMES_END_NAMESPACE
