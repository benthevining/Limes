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
#include "mathHelpers.h"
#include <limes_export.h>
#include "../misc/preprocessor.h"

LIMES_BEGIN_NAMESPACE

namespace math
{

template <Scalar ValueType = int>
struct LIMES_EXPORT Fraction final
{
	constexpr Fraction() = default;

	explicit constexpr Fraction (ValueType num, ValueType denom) noexcept;

	template <typename T>
	explicit constexpr Fraction (const Fraction<T>& other) noexcept;

	LIMES_CONSTEXPR_COPYABLE (Fraction);
	LIMES_CONSTEXPR_MOVABLE (Fraction);

	template <typename T>
	constexpr Fraction& operator= (const Fraction<T>& other) noexcept;

	template <typename T>
	[[nodiscard]] constexpr operator T() const noexcept;

	template <typename T>
	[[nodiscard]] constexpr bool isEquivalentTo (const Fraction<T>& other) const noexcept;

	[[nodiscard]] constexpr bool operator== (const Fraction& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Fraction& other) const noexcept;

	template <typename T>
	[[nodiscard]] constexpr bool operator> (const Fraction<T>& other) const noexcept;

	template <typename T>
	[[nodiscard]] constexpr bool operator< (const Fraction<T>& other) const noexcept;

	template <typename T>
	[[nodiscard]] constexpr bool operator> (T value) const noexcept;

	template <typename T>
	[[nodiscard]] constexpr bool operator< (T value) const noexcept;

	[[nodiscard]] constexpr Fraction operator+ (const Fraction& other) const noexcept;
	[[nodiscard]] constexpr Fraction operator- (const Fraction& other) const noexcept;

	[[nodiscard]] constexpr Fraction operator+ (ValueType add) const noexcept;
	[[nodiscard]] constexpr Fraction operator- (ValueType subtract) const noexcept;

	constexpr Fraction& operator+= (const Fraction& other) noexcept;
	constexpr Fraction& operator-= (const Fraction& other) noexcept;

	constexpr Fraction& operator+= (ValueType add) noexcept;
	constexpr Fraction& operator-= (ValueType subtract) noexcept;

	[[nodiscard]] constexpr Fraction operator* (const Fraction& other) const noexcept;
	[[nodiscard]] constexpr Fraction operator/ (const Fraction& other) const noexcept;

	[[nodiscard]] constexpr Fraction operator* (ValueType multiply) const noexcept;
	[[nodiscard]] constexpr Fraction operator/ (ValueType divide) const noexcept;

	constexpr Fraction& operator*= (const Fraction& other) noexcept;
	constexpr Fraction& operator/= (const Fraction& other) noexcept;

	constexpr Fraction& operator*= (ValueType multiply) noexcept;
	constexpr Fraction& operator/= (ValueType divide) noexcept;

	[[nodiscard]] constexpr ValueType leastCommonDenominatorWith (const Fraction& other) const noexcept;

	[[nodiscard]] constexpr ValueType divided() const noexcept;

	[[nodiscard]] constexpr Fraction reduce() const noexcept;

	[[nodiscard]] constexpr Fraction withDenominator (ValueType newDenominator) const noexcept;

	[[nodiscard]] constexpr bool isNegative() const noexcept;

	[[nodiscard]] constexpr Fraction getReciprocal() const noexcept;

	ValueType numerator { 0 }, denominator { 0 };
};

}  // namespace math

LIMES_END_NAMESPACE

#include "Fraction_impl.h"	// IWYU pragma: export
