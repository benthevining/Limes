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

#include <type_traits>
#include <limes_export.h>

namespace limes::vecops
{

template <typename T>
concept Scalar = std::is_scalar_v<T>;

template <typename T>
concept Integral = Scalar<T> && std::is_integral_v<T>;

template <Scalar DataType>
LIMES_EXPORT struct Complex final
{
	DataType real, imag;

	using Type = DataType;
};

namespace constants
{

//#if __has_include(<numbers>)
//#	include <numbers>
//
// template <Scalar T>
// LIMES_EXPORT static constexpr T pi = std::numbers::pi_v<T>;
//
//#else

template <Scalar T>
LIMES_EXPORT static constexpr T pi = static_cast<T> (3.1415916);

//#endif

template <Scalar T>
LIMES_EXPORT static constexpr T two_pi = pi<T>* T (2.);

template <Scalar T>
LIMES_EXPORT static constexpr T blackman_alpha = T (0.16);

}  // namespace constants

}  // namespace limes::vecops
