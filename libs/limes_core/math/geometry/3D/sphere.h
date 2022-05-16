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
#include "../../mathHelpers.h"
#include "3Dshape.h"

LIMES_BEGIN_NAMESPACE

namespace math::geometry
{

template <Scalar ValueType>
class LIMES_EXPORT Sphere final : public ThreeDShape<ValueType>
{
public:

	explicit constexpr Sphere (ValueType radiusToUse) noexcept;

	LIMES_CONSTEXPR_COPYABLE (Sphere);
	LIMES_CONSTEXPR_MOVABLE (Sphere);

	[[nodiscard]] constexpr ValueType volume() const noexcept final;

	[[nodiscard]] constexpr ValueType surfaceArea() const noexcept final;

	[[nodiscard]] constexpr ValueType circumference() const noexcept;

	[[nodiscard]] constexpr ValueType diameter() const noexcept;

	[[nodiscard]] constexpr ValueType radius() const noexcept;

	[[nodiscard]] constexpr bool operator== (const Sphere& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Sphere& other) const noexcept;

	[[nodiscard]] constexpr bool operator> (const Sphere& other) const noexcept;
	[[nodiscard]] constexpr bool operator<(const Sphere& other) const noexcept;

private:

	ValueType m_radius;
};


template <Scalar ValueType>
constexpr Sphere<ValueType>::Sphere (ValueType radiusToUse) noexcept
	: m_radius (radiusToUse)
{
	LIMES_ASSERT (m_radius > 0);
}

template <Scalar ValueType>
constexpr ValueType Sphere<ValueType>::volume() const noexcept
{
	return constants::pi<ValueType> * m_radius * m_radius * m_radius * ValueType (4) / ValueType (3);
}

template <Scalar ValueType>
constexpr ValueType Sphere<ValueType>::surfaceArea() const noexcept
{
	return ValueType (4) * constants::pi<ValueType> * m_radius * m_radius;
}

template <Scalar ValueType>
constexpr ValueType Sphere<ValueType>::circumference() const noexcept
{
	return ValueType (2) * constants::pi<ValueType> * m_radius;
}

template <Scalar ValueType>
constexpr ValueType Sphere<ValueType>::diameter() const noexcept
{
	return ValueType (2) * m_radius;
}

template <Scalar ValueType>
constexpr ValueType Sphere<ValueType>::radius() const noexcept
{
	return m_radius;
}

template <Scalar ValueType>
constexpr bool Sphere<ValueType>::operator== (const Sphere& other) const noexcept
{
	return m_radius == other.m_radius;
}

template <Scalar ValueType>
constexpr bool Sphere<ValueType>::operator!= (const Sphere& other) const noexcept
{
	return m_radius != other.m_radius;
}

template <Scalar ValueType>
constexpr bool Sphere<ValueType>::operator> (const Sphere& other) const noexcept
{
	return m_radius > other.m_radius;
}

template <Scalar ValueType>
constexpr bool Sphere<ValueType>::operator<(const Sphere& other) const noexcept
{
	return m_radius < other.m_radius;
}

}  // namespace math::geometry

LIMES_END_NAMESPACE
