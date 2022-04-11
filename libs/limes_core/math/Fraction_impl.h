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

#include "Fraction.h"
#include <numeric>

namespace limes::math
{

template <Scalar ValueType>
constexpr Fraction<ValueType>::Fraction (ValueType num, ValueType denom) noexcept
	: numerator (num), denominator (denom)
{
	LIMES_ASSERT (denominator != ValueType (0));
}

template <Scalar ValueType>
template <typename T>
constexpr Fraction<ValueType>::Fraction (const Fraction<T>& other) noexcept
	: Fraction (static_cast<ValueType> (other.numerator), static_cast<ValueType> (other.denominator))
{
}

template <Scalar ValueType>
template <typename T>
constexpr Fraction<ValueType>& Fraction<ValueType>::operator= (const Fraction<T>& other) noexcept
{
	numerator	= static_cast<ValueType> (other.numerator);
	denominator = static_cast<ValueType> (other.denominator);
	return *this;
}

template <Scalar ValueType>
template <typename T>
constexpr Fraction<ValueType>::operator T() const noexcept
{
	return static_cast<T> (numerator) / static_cast<T> (denominator);
}

template <Scalar ValueType>
template <typename T>
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
constexpr bool Fraction<ValueType>::operator> (const Fraction& other) const noexcept
{
	if (other.isNegative() && ! isNegative())
		return true;

	return (double) *this > (double) other;	 // NOLINT
}

template <Scalar ValueType>
constexpr bool Fraction<ValueType>::operator< (const Fraction& other) const noexcept
{
	if (isNegative() && ! other.isNegative())
		return true;

	return (double) *this < (double) other;	 // NOLINT
}

template <Scalar ValueType>
constexpr Fraction<ValueType> Fraction<ValueType>::operator+ (const Fraction& other) const noexcept
{
	if (denominator != other.denominator)
	{
		const auto lcm = leastCommonDenominatorWith (other);
		return withDenominator (lcm) + other.withDenominator (lcm);
	}

	return Fraction<ValueType> { numerator + other.numerator, denominator }.reduce();
}

template <Scalar ValueType>
constexpr Fraction<ValueType> Fraction<ValueType>::operator- (const Fraction& other) const noexcept
{
	if (denominator != other.denominator)
	{
		const auto lcm = leastCommonDenominatorWith (other);
		return withDenominator (lcm) - other.withDenominator (lcm);
	}

	return Fraction<ValueType> { numerator - other.numerator, denominator }.reduce();
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
constexpr Fraction<ValueType> Fraction<ValueType>::operator* (const Fraction<ValueType>& other) const noexcept
{
	return Fraction { numerator * other.numerator, denominator * other.denominator }.reduce();
}

template <Scalar ValueType>
constexpr Fraction<ValueType> Fraction<ValueType>::operator/ (const Fraction<ValueType>& other) const noexcept
{
	return *this * other.getReciprocal();
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
constexpr ValueType Fraction<ValueType>::leastCommonDenominatorWith (const Fraction& other) const noexcept
{
	return std::max (1, std::lcm (denominator, other.denominator));
}

template <Scalar ValueType>
constexpr ValueType Fraction<ValueType>::divided() const noexcept
{
	return numerator / denominator;
}

template <Scalar ValueType>
constexpr Fraction<ValueType> Fraction<ValueType>::reduce() const noexcept
{
	const auto lcm = std::max (1, std::lcm (numerator, denominator));
	return Fraction { numerator / lcm, denominator / lcm };
}

template <Scalar ValueType>
constexpr Fraction<ValueType> Fraction<ValueType>::withDenominator (ValueType newDenominator) const noexcept
{
	return Fraction { numerator * (newDenominator / denominator), newDenominator };
}

template <Scalar ValueType>
constexpr bool Fraction<ValueType>::isNegative() const noexcept
{
	if constexpr (std::is_unsigned<ValueType>::value)
		return false;
	else
		return numerator < 0 || denominator < 0;
}

template <Scalar ValueType>
constexpr Fraction<ValueType> Fraction<ValueType>::getReciprocal() const noexcept
{
	return Fraction { denominator, numerator };
}

}  // namespace limes::math
