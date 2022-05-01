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
#include "../2D/circle.h"
#include "3Dshape.h"
#include <cmath>

LIMES_BEGIN_NAMESPACE

namespace math::geometry
{

template <Scalar ValueType>
class LIMES_EXPORT Cone final : public ThreeDShape<ValueType>
{
public:

	explicit constexpr Cone (ValueType radius, ValueType height) noexcept;

	explicit constexpr Cone (const Circle<ValueType>& face, ValueType height) noexcept;

	LIMES_CONSTEXPR_COPYABLE (Cone);
	LIMES_CONSTEXPR_MOVABLE (Cone);

	[[nodiscard]] constexpr ValueType radius() const noexcept;

	[[nodiscard]] constexpr ValueType diameter() const noexcept;

	[[nodiscard]] constexpr ValueType height() const noexcept;

	[[nodiscard]] constexpr ValueType volume() const noexcept final;

	[[nodiscard]] ValueType surfaceArea() const noexcept final;

	[[nodiscard]] ValueType slantHeight() const noexcept;

	[[nodiscard]] constexpr Circle<ValueType> getFace() const noexcept;

	[[nodiscard]] constexpr bool operator== (const Cone& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Cone& other) const noexcept;

private:

	ValueType m_radius, m_height;
};


template <Scalar ValueType>
constexpr Cone<ValueType>::Cone (ValueType radius, ValueType height) noexcept
	: m_radius (radius), m_height (height)
{
	LIMES_ASSERT (m_radius > 0);
	LIMES_ASSERT (m_height > 0);
}

template <Scalar ValueType>
constexpr Cone<ValueType>::Cone (const Circle<ValueType>& face, ValueType height) noexcept
	: m_radius (face.radius()), m_height (height)
{
	LIMES_ASSERT (m_height > 0);
}

template <Scalar ValueType>
constexpr ValueType Cone<ValueType>::radius() const noexcept
{
	return m_radius;
}

template <Scalar ValueType>
constexpr ValueType Cone<ValueType>::diameter() const noexcept
{
	return m_radius * ValueType (2);
}

template <Scalar ValueType>
constexpr ValueType Cone<ValueType>::height() const noexcept
{
	return m_height;
}

template <Scalar ValueType>
constexpr ValueType Cone<ValueType>::volume() const noexcept
{
	return m_radius * m_radius * constants::pi<ValueType> * m_height / ValueType (3);
}

template <Scalar ValueType>
ValueType Cone<ValueType>::surfaceArea() const noexcept
{
	return constants::pi<ValueType> * m_radius * (m_radius + std::sqrt ((m_radius * m_radius) + (m_height * m_height)));
}

template <Scalar ValueType>
ValueType Cone<ValueType>::slantHeight() const noexcept
{
	return std::sqrt ((m_radius * m_radius) + (m_height * m_height));
}

template <Scalar ValueType>
constexpr Circle<ValueType> Cone<ValueType>::getFace() const noexcept
{
	return Circle<ValueType> { m_radius };
}

template <Scalar ValueType>
constexpr bool Cone<ValueType>::operator== (const Cone& other) const noexcept
{
	return m_radius == other.m_radius && m_height == other.m_height;
}

template <Scalar ValueType>
constexpr bool Cone<ValueType>::operator!= (const Cone& other) const noexcept
{
	return ! (*this == other);
}

}  // namespace math::geometry

LIMES_END_NAMESPACE
