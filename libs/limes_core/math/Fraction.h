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

#include "mathHelpers.h"
#include <limes_export.h>

namespace limes::math
{

template <Scalar ValueType = int>
struct LIMES_EXPORT Fraction final
{
	explicit constexpr Fraction (ValueType num, ValueType denom) noexcept;

	template <typename T>
	explicit constexpr Fraction (const Fraction<T>& other) noexcept;

	template <typename T>
	constexpr Fraction& operator= (const Fraction<T>& other) noexcept;

	template <typename T>
	[[nodiscard]] constexpr operator T() const noexcept;

	template <typename T>
	[[nodiscard]] constexpr bool isEquivalentTo (const Fraction<T>& other) const noexcept;

	[[nodiscard]] constexpr bool operator== (const Fraction& other) const noexcept;

	[[nodiscard]] constexpr bool operator!= (const Fraction& other) const noexcept;

	[[nodiscard]] constexpr bool operator> (const Fraction& other) const noexcept;

	[[nodiscard]] constexpr bool operator< (const Fraction& other) const noexcept;

	[[nodiscard]] constexpr Fraction operator+ (const Fraction& other) const noexcept;

	[[nodiscard]] constexpr Fraction operator- (const Fraction& other) const noexcept;

	constexpr Fraction& operator+= (const Fraction& other) noexcept;

	constexpr Fraction& operator-= (const Fraction& other) noexcept;

	[[nodiscard]] constexpr Fraction operator* (const Fraction& other) const noexcept;

	[[nodiscard]] constexpr Fraction operator/ (const Fraction& other) const noexcept;

	constexpr Fraction& operator*= (const Fraction& other) noexcept;

	constexpr Fraction& operator/= (const Fraction& other) noexcept;

	[[nodiscard]] constexpr ValueType leastCommonDenominatorWith (const Fraction& other) const noexcept;

	[[nodiscard]] constexpr ValueType divided() const noexcept;

	[[nodiscard]] constexpr Fraction reduce() const noexcept;

	[[nodiscard]] constexpr Fraction withDenominator (ValueType newDenominator) const noexcept;

	[[nodiscard]] constexpr bool isNegative() const noexcept;

	[[nodiscard]] constexpr Fraction getReciprocal() const noexcept;

	ValueType numerator, denominator;
};

}  // namespace limes::math


#include "Fraction_impl.h"
