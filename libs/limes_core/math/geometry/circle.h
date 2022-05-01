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
#include "../mathHelpers.h"

LIMES_BEGIN_NAMESPACE

namespace math::geometry
{

template <Scalar ValueType>
class LIMES_EXPORT Circle final
{
public:

	explicit constexpr Circle (ValueType radiusToUse) noexcept;

	LIMES_CONSTEXPR_COPYABLE (Circle);
	LIMES_CONSTEXPR_MOVABLE (Circle);

	[[nodiscard]] constexpr ValueType area() const noexcept;

	[[nodiscard]] constexpr ValueType perimeter() const noexcept;

	[[nodiscard]] constexpr ValueType radius() const noexcept;

	[[nodiscard]] constexpr ValueType diameter() const noexcept;

	[[nodiscard]] constexpr bool operator== (const Circle& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Circle& other) const noexcept;

	[[nodiscard]] constexpr bool operator> (const Circle& other) const noexcept;
	[[nodiscard]] constexpr bool operator< (const Circle& other) const noexcept;

private:

	ValueType m_radius;
};


template <Scalar ValueType>
constexpr Circle<ValueType>::Circle (ValueType radiusToUse) noexcept
	: m_radius (radiusToUse)
{
	LIMES_ASSERT (m_radius > 0);
}

template <Scalar ValueType>
constexpr ValueType Circle<ValueType>::area() const noexcept
{
	return constants::pi<ValueType> * m_radius * m_radius;
}

template <Scalar ValueType>
constexpr ValueType Circle<ValueType>::perimeter() const noexcept
{
	return ValueType (2) * constants::pi<ValueType> * m_radius;
}

template <Scalar ValueType>
constexpr ValueType Circle<ValueType>::radius() const noexcept
{
	return m_radius;
}

template <Scalar ValueType>
constexpr ValueType Circle<ValueType>::diameter() const noexcept
{
	return ValueType (2) * m_radius;
}

template <Scalar ValueType>
constexpr bool Circle<ValueType>::operator== (const Circle& other) const noexcept
{
	return m_radius == other.m_radius;
}

template <Scalar ValueType>
constexpr bool Circle<ValueType>::operator!= (const Circle& other) const noexcept
{
	return m_radius != other.m_radius;
}

template <Scalar ValueType>
constexpr bool Circle<ValueType>::operator> (const Circle& other) const noexcept
{
	return m_radius > other.m_radius;
}

template <Scalar ValueType>
constexpr bool Circle<ValueType>::operator< (const Circle& other) const noexcept
{
	return m_radius < other.m_radius;
}

}  // namespace math::geometry

LIMES_END_NAMESPACE
