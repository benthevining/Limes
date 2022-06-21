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
#include "mathFunctions.h"
#include <limes_export.h>
#include "../misc/preprocessor.h"
#include <numeric>

/** @file
	This file defines the Fraction class.
	@ingroup core_math
 */

LIMES_BEGIN_NAMESPACE

namespace math
{

/** This class represents a %fraction.
	@ingroup core_math
	@test This class's API is tested with static assertions at compile-time.
 */
template <Scalar ValueType = int>
struct LIMES_EXPORT Fraction final
{
	/** @name Constructors */
	///@{

	/** Creates a default fraction with a numerator and denominator of 0. */
	constexpr Fraction() = default;

	/** Creates a fraction. */
	explicit constexpr Fraction (ValueType num, ValueType denom) noexcept;

	/** Creates a fraction. */
	template <Scalar T>
	explicit constexpr Fraction (const Fraction<T>& other) noexcept;
	///@}

	LIMES_CONSTEXPR_COPYABLE (Fraction)
	LIMES_CONSTEXPR_MOVABLE (Fraction)

	/** Assigns this %fraction to a new value. */
	template <Scalar T>
	constexpr Fraction& operator= (const Fraction<T>& other) noexcept;

	/** Converts this %fraction to a value of the desired type by dividing the numerator by the denominator. */
	template <Scalar T>
	[[nodiscard]] constexpr operator T() const noexcept;

	/** Returns true if the two fractions are equivalent, regardless of their exact representations.
		For example, \c 2/3 and \c 4/6 are equivalent even though their numerator and denominator values are different.
	 */
	template <Scalar T>
	[[nodiscard]] constexpr bool isEquivalentTo (const Fraction<T>& other) const noexcept;

	/** @name Equality comparisons */
	///@{
	[[nodiscard]] constexpr bool operator== (const Fraction& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Fraction& other) const noexcept;
	///@}

	/** @name Greater/less than comparisons */
	///@{
	template <Scalar T>
	[[nodiscard]] constexpr bool operator> (const Fraction<T>& other) const noexcept;

	template <Scalar T>
	[[nodiscard]] constexpr bool operator<(const Fraction<T>& other) const noexcept;

	template <Scalar T>
	[[nodiscard]] constexpr bool operator> (T value) const noexcept;

	template <Scalar T>
	[[nodiscard]] constexpr bool operator<(T value) const noexcept;
	///@}

	/** @name Addition */
	///@{
	[[nodiscard]] constexpr Fraction operator+ (const Fraction& other) const noexcept;
	[[nodiscard]] constexpr Fraction operator+ (ValueType add) const noexcept;
	constexpr Fraction&				 operator+= (const Fraction& other) noexcept;
	constexpr Fraction&				 operator+= (ValueType add) noexcept;
	///@}

	/** @name Subtraction */
	///@{
	[[nodiscard]] constexpr Fraction operator- (const Fraction& other) const noexcept;
	[[nodiscard]] constexpr Fraction operator- (ValueType subtract) const noexcept;
	constexpr Fraction&				 operator-= (const Fraction& other) noexcept;
	constexpr Fraction&				 operator-= (ValueType subtract) noexcept;
	///@}

	/** @name Multiplication */
	///@{
	[[nodiscard]] constexpr Fraction operator* (const Fraction& other) const noexcept;
	[[nodiscard]] constexpr Fraction operator* (ValueType multiply) const noexcept;
	constexpr Fraction&				 operator*= (const Fraction& other) noexcept;
	constexpr Fraction&				 operator*= (ValueType multiply) noexcept;
	///@}

	/** @name Division */
	///@{
	[[nodiscard]] constexpr Fraction operator/ (const Fraction& other) const noexcept;
	[[nodiscard]] constexpr Fraction operator/ (ValueType divide) const noexcept;
	constexpr Fraction&				 operator/= (const Fraction& other) noexcept;
	constexpr Fraction&				 operator/= (ValueType divide) noexcept;
	///@}

	/** Returns the least common denominator between this %fraction and the other one. */
	[[nodiscard]] constexpr ValueType leastCommonDenominatorWith (const Fraction& other) const noexcept;

	/** Divides the numerator by the denominator and returns the result. */
	[[nodiscard]] constexpr ValueType divided() const noexcept;

	/** If the %fraction can be reduced, this function returns a new %fraction with the reduced numerator and denominator.
		For example, \c 4/6 would be reduced to \c 2/3 .
	 */
	[[nodiscard]] constexpr Fraction reduce() const noexcept;

	/** Returns a new %fraction representing the same numerical value as this one, but expressed in terms of the given denominator. */
	[[nodiscard]] constexpr Fraction withDenominator (ValueType newDenominator) const noexcept;

	/** Returns true if this %fraction is negative. */
	[[nodiscard]] constexpr bool isNegative() const noexcept;

	/** Returns the reciprocal of this %fraction. */
	[[nodiscard]] constexpr Fraction getReciprocal() const noexcept;

	/** The fraction's numerator. */
	ValueType numerator { 0 };

	/** The fraction's denominator. */
	ValueType denominator { 0 };
};

/*--------------------------------------------------------------------------------------------------------------*/

#pragma mark Implementation

template <Scalar ValueType>
constexpr Fraction<ValueType>::Fraction (ValueType num, ValueType denom) noexcept
	: numerator (num), denominator (denom)
{
	LIMES_ASSERT (denominator != ValueType (0));
}

template <Scalar ValueType>
template <Scalar T>
constexpr Fraction<ValueType>::Fraction (const Fraction<T>& other) noexcept
	: Fraction (static_cast<ValueType> (other.numerator), static_cast<ValueType> (other.denominator))
{
}

template <Scalar ValueType>
template <Scalar T>
constexpr Fraction<ValueType>& Fraction<ValueType>::operator= (const Fraction<T>& other) noexcept
{
	numerator	= static_cast<ValueType> (other.numerator);
	denominator = static_cast<ValueType> (other.denominator);
	return *this;
}

template <Scalar ValueType>
template <Scalar T>
constexpr Fraction<ValueType>::operator T() const noexcept
{
	LIMES_ASSERT (denominator != 0);

	return static_cast<T> (numerator) / static_cast<T> (denominator);
}

template <Scalar ValueType>
template <Scalar T>
constexpr bool Fraction<ValueType>::isEquivalentTo (const Fraction<T>& other) const noexcept
{
	return (double) *this == (double) other;  // NOLINT
}

template <Scalar ValueType>
constexpr bool Fraction<ValueType>::operator== (const Fraction& other) const noexcept
{
	return numerator == other.numerator && denominator == other.denominator;
}

template <Scalar ValueType>
constexpr bool Fraction<ValueType>::operator!= (const Fraction& other) const noexcept
{
	return ! (*this == other);
}

template <Scalar ValueType>
template <Scalar T>
constexpr bool Fraction<ValueType>::operator> (const Fraction<T>& other) const noexcept
{
	if (other.isNegative() && ! isNegative())
		return true;

	return (double) *this > (double) other;	 // NOLINT
}

template <Scalar ValueType>
template <Scalar T>
constexpr bool Fraction<ValueType>::operator<(const Fraction<T>& other) const noexcept
{
	if (isNegative() && ! other.isNegative())
		return true;

	return (double) *this < (double) other;	 // NOLINT
}

template <Scalar ValueType>
template <Scalar T>
constexpr bool Fraction<ValueType>::operator> (T value) const noexcept
{
	// clang-format off
	return (T) *this > value;	// NOLINT
	// clang-format on
}

template <Scalar ValueType>
template <Scalar T>
constexpr bool Fraction<ValueType>::operator<(T value) const noexcept
{
	// clang-format off
	return (T) *this < value;	// NOLINT
	// clang-format on
}

template <Scalar ValueType>
constexpr Fraction<ValueType> Fraction<ValueType>::operator+ (const Fraction& other) const noexcept
{
	if (denominator != other.denominator)
	{
		const auto lcm = leastCommonDenominatorWith (other);
		return withDenominator (lcm) + other.withDenominator (lcm);
	}

	return Fraction { numerator + other.numerator, denominator }.reduce();
}

template <Scalar ValueType>
constexpr Fraction<ValueType> Fraction<ValueType>::operator- (const Fraction& other) const noexcept
{
	if (denominator != other.denominator)
	{
		const auto lcm = leastCommonDenominatorWith (other);
		return withDenominator (lcm) - other.withDenominator (lcm);
	}

	return Fraction { numerator - other.numerator, denominator }.reduce();
}

template <Scalar ValueType>
constexpr Fraction<ValueType> Fraction<ValueType>::operator+ (ValueType add) const noexcept
{
	return Fraction { numerator + add, denominator }.reduce();
}

template <Scalar ValueType>
constexpr Fraction<ValueType> Fraction<ValueType>::operator- (ValueType subtract) const noexcept
{
	return Fraction { numerator - subtract, denominator }.reduce();
}

template <Scalar ValueType>
constexpr Fraction<ValueType>& Fraction<ValueType>::operator+= (const Fraction& other) noexcept
{
	return *this = *this + other;
}

template <Scalar ValueType>
constexpr Fraction<ValueType>& Fraction<ValueType>::operator-= (const Fraction& other) noexcept
{
	return *this = *this - other;
}

template <Scalar ValueType>
constexpr Fraction<ValueType>& Fraction<ValueType>::operator+= (ValueType add) noexcept
{
	numerator += add;
	return *this;
}

template <Scalar ValueType>
constexpr Fraction<ValueType>& Fraction<ValueType>::operator-= (ValueType subtract) noexcept
{
	numerator -= subtract;
	return *this;
}

template <Scalar ValueType>
constexpr Fraction<ValueType> Fraction<ValueType>::operator* (const Fraction<ValueType>& other) const noexcept
{
	return Fraction { numerator * other.numerator, denominator * other.denominator }.reduce();
}

template <Scalar ValueType>
constexpr Fraction<ValueType> Fraction<ValueType>::operator* (ValueType multiply) const noexcept
{
	return Fraction { numerator * multiply, denominator }.reduce();
}

template <Scalar ValueType>
constexpr Fraction<ValueType> Fraction<ValueType>::operator/ (const Fraction<ValueType>& other) const noexcept
{
	return (*this * other.getReciprocal()).reduce();
}

template <Scalar ValueType>
constexpr Fraction<ValueType> Fraction<ValueType>::operator/ (ValueType divide) const noexcept
{
	return Fraction { numerator / divide, denominator }.reduce();
}

template <Scalar ValueType>
constexpr Fraction<ValueType>& Fraction<ValueType>::operator*= (const Fraction& other) noexcept
{
	return *this = *this * other;
}

template <Scalar ValueType>
constexpr Fraction<ValueType>& Fraction<ValueType>::operator/= (const Fraction& other) noexcept
{
	return *this = *this / other;
}

template <Scalar ValueType>
constexpr Fraction<ValueType>& Fraction<ValueType>::operator*= (ValueType multiply) noexcept
{
	numerator *= multiply;
	return *this;
}

template <Scalar ValueType>
constexpr Fraction<ValueType>& Fraction<ValueType>::operator/= (ValueType divide) noexcept
{
	numerator /= divide;
	return *this;
}

template <Scalar ValueType>
constexpr ValueType Fraction<ValueType>::leastCommonDenominatorWith (const Fraction& other) const noexcept
{
	return std::max (ValueType (1), std::gcd (denominator, other.denominator));
}

template <Scalar ValueType>
constexpr ValueType Fraction<ValueType>::divided() const noexcept
{
	LIMES_ASSERT (denominator != 0);

	return numerator / denominator;
}

template <Scalar ValueType>
constexpr Fraction<ValueType> Fraction<ValueType>::reduce() const noexcept
{
	const auto gcd = std::max (ValueType (1), std::gcd (numerator, denominator));
	auto	   res = Fraction { numerator / gcd, denominator / gcd };

	if (res.numerator < 0 && res.denominator < 0)
	{
		res.numerator	= -res.numerator;
		res.denominator = -res.denominator;
	}

	return res;
}

template <Scalar ValueType>
constexpr Fraction<ValueType> Fraction<ValueType>::withDenominator (ValueType newDenominator) const noexcept
{
	if (newDenominator > denominator)
		return Fraction { numerator * (newDenominator / denominator), newDenominator };

	return Fraction { numerator / (denominator / newDenominator), newDenominator };
}

template <Scalar ValueType>
constexpr bool Fraction<ValueType>::isNegative() const noexcept
{
	if constexpr (std::is_unsigned<ValueType>::value)
		return false;
	else
	{
		if (numerator < 0 && denominator < 0)
			return false;

		return numerator < 0 || denominator < 0;
	}
}

template <Scalar ValueType>
constexpr Fraction<ValueType> Fraction<ValueType>::getReciprocal() const noexcept
{
	return Fraction { denominator, numerator };
}

}  // namespace math

LIMES_END_NAMESPACE
