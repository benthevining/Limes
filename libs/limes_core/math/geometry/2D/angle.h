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

/** @file
	This file defines the Angle class.
	@ingroup TwoDgeometry
 */

LIMES_BEGIN_NAMESPACE

namespace math::geometry
{

/** This class represents a geometric %angle.
	Internally, the %angle is represented in degrees.
	@ingroup TwoDgeometry
	@test This class's API is tested with static assertions at compile-time.
 */
template <Scalar ValueType = int>
class LIMES_EXPORT Angle final
{
public:

	/** Creates an Angle representing the given number of degrees. */
	explicit constexpr Angle (ValueType degrees) noexcept;

	/** @name Accessors */
	///@{
	/** Returns the angle's measure in degrees. */
	constexpr operator ValueType() const noexcept;

	/** Returns the angle's measure in degrees. */
	[[nodiscard]] constexpr ValueType degrees() const noexcept;

	/** Returns the angle's measure in radians. */
	[[nodiscard]] constexpr ValueType radians() const noexcept;
	///@}

	/** @name Queries of %angle type */
	///@{
	/** Returns true if the %angle is exactly 90 degrees. */
	[[nodiscard]] constexpr bool isRight() const noexcept;

	/** Returns true if the %angle is less than 90 degrees. */
	[[nodiscard]] constexpr bool isAcute() const noexcept;

	/** Returns true if the %angle is larger than 90 degrees and smaller than 180 degrees. */
	[[nodiscard]] constexpr bool isObtuse() const noexcept;

	/** Returns true if the %angle is exactly 180 degrees. */
	[[nodiscard]] constexpr bool isStraight() const noexcept;

	/** Returns true if the %angle is larger than 180 degrees and smaller than 360 degrees. */
	[[nodiscard]] constexpr bool isReflex() const noexcept;

	/** Returns true if the %angle is exactly 360 degrees. */
	[[nodiscard]] constexpr bool isFull() const noexcept;

	/** Returns true if the angle's measure in degrees is not divisible by 90. */
	[[nodiscard]] constexpr bool isOblique() const noexcept;
	///@}

	/** @name %Angle relations */
	///@{
	/** Returns true if this %angle and the other's measures in degrees sum to 90. */
	[[nodiscard]] constexpr bool isComplementaryWith (const Angle& other) const noexcept;

	/** Returns an %angle object such that the new angle's measure in degrees added to this angle's measure will equal 90. */
	[[nodiscard]] constexpr Angle getComplement() const noexcept;

	/** Returns true if this %angle and the other's measures in degrees sum to 180. */
	[[nodiscard]] constexpr bool isSupplementaryWith (const Angle& other) const noexcept;

	/** Returns an %angle object such that the new angle's measure in degrees added to this angle's measure will equal 180. */
	[[nodiscard]] constexpr Angle getSupplement() const noexcept;

	/** Returns true if this %angle and the other's measures in degrees sum to 360. */
	[[nodiscard]] constexpr bool isExplementaryWith (const Angle& other) const noexcept;

	/** Returns an %angle object such that the new angle's measure in degrees added to this angle's measure will equal 360. */
	[[nodiscard]] constexpr Angle getExplement() const noexcept;
	///@}

	/** Returns a new %angle object representing half of this angle's measure. */
	[[nodiscard]] constexpr Angle bisect() const noexcept;

	/** @name Equality comparisons
		Returns true if this %angle has the same degree measure as the other.
	 */
	///@{
	[[nodiscard]] constexpr bool operator== (const Angle& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Angle& other) const noexcept;
	[[nodiscard]] constexpr bool operator== (ValueType other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (ValueType other) const noexcept;
	///@}

	/** @name Greater/less than comparisons
	 */
	///@{
	[[nodiscard]] constexpr bool operator> (const Angle& other) const noexcept;
	[[nodiscard]] constexpr bool operator< (const Angle& other) const noexcept;
	[[nodiscard]] constexpr bool operator> (ValueType other) const noexcept;
	[[nodiscard]] constexpr bool operator< (ValueType other) const noexcept;
	///@}

	/** @name Addition and subtraction
		Adds or subtracts a specified number of degrees from this %angle.
	 */
	///@{
	[[nodiscard]] constexpr Angle& operator+= (ValueType degreesToAdd) noexcept;
	[[nodiscard]] constexpr Angle& operator-= (ValueType degreesToSubtract) noexcept;
	[[nodiscard]] constexpr Angle& operator+= (const Angle& other) noexcept;
	[[nodiscard]] constexpr Angle& operator-= (const Angle& other) noexcept;
	[[nodiscard]] constexpr Angle  operator+ (ValueType degreesToAdd) const noexcept;
	[[nodiscard]] constexpr Angle  operator- (ValueType degreesToSubtract) const noexcept;
	[[nodiscard]] constexpr Angle  operator+ (const Angle& other) const noexcept;
	[[nodiscard]] constexpr Angle  operator- (const Angle& other) const noexcept;
	///@}

	/** @name Creation functions */
	///@{
	/** Convenience function for creating a right %angle object (90 degrees). */
	[[nodiscard]] static constexpr Angle right() noexcept;

	/** Convenience function for creating a straight %angle object (180 degrees). */
	[[nodiscard]] static constexpr Angle straight() noexcept;

	/** Convenience function for creating a full %angle object (360 degrees). */
	[[nodiscard]] static constexpr Angle full() noexcept;
	///@}

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
constexpr Angle<ValueType>::operator ValueType() const noexcept
{
	return m_degrees;
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
constexpr bool Angle<ValueType>::operator== (ValueType other) const noexcept
{
	return m_degrees == other;
}

template <Scalar ValueType>
constexpr bool Angle<ValueType>::operator!= (ValueType other) const noexcept
{
	return m_degrees != other;
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
constexpr bool Angle<ValueType>::operator> (ValueType other) const noexcept
{
	return m_degrees > other;
}

template <Scalar ValueType>
constexpr bool Angle<ValueType>::operator< (ValueType other) const noexcept
{
	return m_degrees < other;
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

template <Scalar ValueType>
constexpr Angle<ValueType> Angle<ValueType>::right() noexcept
{
	return Angle { 90 };
}

template <Scalar ValueType>
constexpr Angle<ValueType> Angle<ValueType>::straight() noexcept
{
	return Angle { 180 };
}

template <Scalar ValueType>
constexpr Angle<ValueType> Angle<ValueType>::full() noexcept
{
	return Angle { 360 };
}

}  // namespace math::geometry

LIMES_END_NAMESPACE
