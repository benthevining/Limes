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

#include <limes_export.h>
#include <limes_namespace.h>
#include <limes_platform.h>
#include "../../mathHelpers.h"

LIMES_BEGIN_NAMESPACE

namespace math::geometry
{

template <Scalar ValueType>
class LIMES_EXPORT Angle final
{
public:

	explicit constexpr Angle (ValueType degrees) noexcept;

	[[nodiscard]] constexpr ValueType degrees() const noexcept;

	[[nodiscard]] constexpr ValueType radians() const noexcept;

	[[nodiscard]] constexpr bool isRight() const noexcept;

	[[nodiscard]] constexpr bool isAcute() const noexcept;

	[[nodiscard]] constexpr bool isObtuse() const noexcept;

	[[nodiscard]] constexpr bool isStraight() const noexcept;

	[[nodiscard]] constexpr bool isReflex() const noexcept;

	[[nodiscard]] constexpr bool isFull() const noexcept;

	[[nodiscard]] constexpr bool isOblique() const noexcept;

	[[nodiscard]] constexpr bool isComplementaryWith (const Angle& other) const noexcept;

	[[nodiscard]] constexpr Angle getComplement() const noexcept;

	[[nodiscard]] constexpr bool isSupplementaryWith (const Angle& other) const noexcept;

	[[nodiscard]] constexpr Angle getSupplement() const noexcept;

	[[nodiscard]] constexpr bool isExplementaryWith (const Angle& other) const noexcept;

	[[nodiscard]] constexpr Angle getExplement() const noexcept;

	[[nodiscard]] constexpr Angle bisect() const noexcept;

	[[nodiscard]] constexpr bool operator== (const Angle& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Angle& other) const noexcept;

	[[nodiscard]] constexpr bool operator> (const Angle& other) const noexcept;
	[[nodiscard]] constexpr bool operator< (const Angle& other) const noexcept;

	[[nodiscard]] constexpr Angle& operator+= (ValueType degreesToAdd) noexcept;
	[[nodiscard]] constexpr Angle& operator-= (ValueType degreesToSubtract) noexcept;

	[[nodiscard]] constexpr Angle& operator+= (const Angle& other) noexcept;
	[[nodiscard]] constexpr Angle& operator-= (const Angle& other) noexcept;

	[[nodiscard]] constexpr Angle operator+ (ValueType degreesToAdd) const noexcept;
	[[nodiscard]] constexpr Angle operator- (ValueType degreesToSubtract) const noexcept;

	[[nodiscard]] constexpr Angle operator+ (const Angle& other) const noexcept;
	[[nodiscard]] constexpr Angle operator- (const Angle& other) const noexcept;

private:

	ValueType m_degrees;
};


template <Scalar ValueType>
constexpr Angle<ValueType>::Angle (ValueType degrees) noexcept
	: m_degrees (degrees)
{
	LIMES_ASSERT (m_degrees >= 0 && m_degrees <= 360);
}

template <Scalar ValueType>
constexpr ValueType Angle<ValueType>::degrees() const noexcept
{
	return m_degrees;
}

template <Scalar ValueType>
constexpr ValueType Angle<ValueType>::radians() const noexcept
{
	return m_degrees * constants::pi<ValueType> / ValueType (180);
}

template <Scalar ValueType>
constexpr bool Angle<ValueType>::isRight() const noexcept
{
	return m_degrees == ValueType (90);
}

template <Scalar ValueType>
constexpr bool Angle<ValueType>::isAcute() const noexcept
{
	return m_degrees < ValueType (90);
}

template <Scalar ValueType>
constexpr bool Angle<ValueType>::isObtuse() const noexcept
{
	return m_degrees > ValueType (90) && m_degrees < ValueType (180);
}

template <Scalar ValueType>
constexpr bool Angle<ValueType>::isStraight() const noexcept
{
	return m_degrees == ValueType (180);
}

template <Scalar ValueType>
constexpr bool Angle<ValueType>::isReflex() const noexcept
{
	return m_degrees > ValueType (180) && m_degrees < ValueType (360);
}

template <Scalar ValueType>
constexpr bool Angle<ValueType>::isFull() const noexcept
{
	return m_degrees == ValueType (360);
}

template <Scalar ValueType>
constexpr bool Angle<ValueType>::isOblique() const noexcept
{
	return ! isDivisibleBy (m_degrees, ValueType (90));
}

template <Scalar ValueType>
constexpr bool Angle<ValueType>::isComplementaryWith (const Angle& other) const noexcept
{
	return m_degrees + other.m_degrees == ValueType (90);
}

template <Scalar ValueType>
constexpr Angle<ValueType> Angle<ValueType>::getComplement() const noexcept
{
	return Angle { ValueType (90) - m_degrees };
}

template <Scalar ValueType>
constexpr bool Angle<ValueType>::isSupplementaryWith (const Angle& other) const noexcept
{
	return m_degrees + other.m_degrees == ValueType (180);
}

template <Scalar ValueType>
constexpr Angle<ValueType> Angle<ValueType>::getSupplement() const noexcept
{
	return Angle { ValueType (180) - m_degrees };
}

template <Scalar ValueType>
constexpr bool Angle<ValueType>::isExplementaryWith (const Angle& other) const noexcept
{
	return m_degrees + other.m_degrees == ValueType (360);
}

template <Scalar ValueType>
constexpr Angle<ValueType> Angle<ValueType>::getExplement() const noexcept
{
	return Angle { ValueType (360) - m_degrees };
}

template <Scalar ValueType>
constexpr Angle<ValueType> Angle<ValueType>::bisect() const noexcept
{
	return Angle { m_degrees / ValueType (2) };
}

template <Scalar ValueType>
constexpr bool Angle<ValueType>::operator== (const Angle& other) const noexcept
{
	return m_degrees == other.m_degrees;
}

template <Scalar ValueType>
constexpr bool Angle<ValueType>::operator!= (const Angle& other) const noexcept
{
	return m_degrees != other.m_degrees;
}

template <Scalar ValueType>
constexpr bool Angle<ValueType>::operator> (const Angle& other) const noexcept
{
	return m_degrees > other.m_degrees;
}

template <Scalar ValueType>
constexpr bool Angle<ValueType>::operator< (const Angle& other) const noexcept
{
	return m_degrees < other.m_degrees;
}

template <Scalar ValueType>
constexpr Angle<ValueType>& Angle<ValueType>::operator+= (ValueType degreesToAdd) noexcept
{
	m_degrees += degreesToAdd;
	return *this;
}

template <Scalar ValueType>
constexpr Angle<ValueType>& Angle<ValueType>::operator-= (ValueType degreesToSubtract) noexcept
{
	m_degrees -= degreesToSubtract;
	return *this;
}

template <Scalar ValueType>
constexpr Angle<ValueType>& Angle<ValueType>::operator+= (const Angle& other) noexcept
{
	m_degrees += other.m_degrees;
	return *this;
}

template <Scalar ValueType>
constexpr Angle<ValueType>& Angle<ValueType>::operator-= (const Angle& other) noexcept
{
	m_degrees -= other.m_degrees;
	return *this;
}

template <Scalar ValueType>
constexpr Angle<ValueType> Angle<ValueType>::operator+ (ValueType degreesToAdd) const noexcept
{
	return Angle { m_degrees + degreesToAdd };
}

template <Scalar ValueType>
constexpr Angle<ValueType> Angle<ValueType>::operator- (ValueType degreesToSubtract) const noexcept
{
	return Angle { m_degrees - degreesToSubtract };
}

template <Scalar ValueType>
constexpr Angle<ValueType> Angle<ValueType>::operator+ (const Angle& other) const noexcept
{
	return Angle { m_degrees + other.m_degrees };
}

template <Scalar ValueType>
constexpr Angle<ValueType> Angle<ValueType>::operator- (const Angle& other) const noexcept
{
	return Angle { m_degrees - other.m_degrees };
}

}  // namespace math::geometry

LIMES_END_NAMESPACE
