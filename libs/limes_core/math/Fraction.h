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

/** @file
	This file defines the Fraction class.
	@ingroup math
 */

LIMES_BEGIN_NAMESPACE

namespace math
{

/** This class represents a %fraction.
	@ingroup math
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

	LIMES_CONSTEXPR_COPYABLE (Fraction);
	LIMES_CONSTEXPR_MOVABLE (Fraction);

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

}  // namespace math

LIMES_END_NAMESPACE

#include "Fraction_impl.h"	// IWYU pragma: export
