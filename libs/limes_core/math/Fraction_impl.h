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
#include "Fraction.h"
#include <numeric>
#include <algorithm>

/** @file
	This file contains implementation details for the Fraction class.
	@ingroup math
 */

/// @cond

LIMES_BEGIN_NAMESPACE

namespace math
{

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
	return (T) * this > value;	// NOLINT
}

template <Scalar ValueType>
template <Scalar T>
constexpr bool Fraction<ValueType>::operator<(T value) const noexcept
{
	return (T) * this < value;	// NOLINT
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

/// @endcond
