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
#include <cmath>
#include "../mathHelpers.h"

LIMES_BEGIN_NAMESPACE

namespace math::area
{

template <Scalar T>
LIMES_EXPORT [[nodiscard]] constexpr T square (T length) noexcept
{
	return length * length;
}

template <Scalar T>
LIMES_EXPORT [[nodiscard]] constexpr T rectangle (T length, T width) noexcept
{
	return length * width;
}

template <Scalar T>
LIMES_EXPORT [[nodiscard]] constexpr T triangle (T base, T height) noexcept
{
	return base * height * T (0.5);
}

template <Scalar T>
LIMES_EXPORT [[nodiscard]] inline T circle (T radius) noexcept
{
	return constants::pi<T> * std::pow (radius, 2);
}

template <Scalar T>
LIMES_EXPORT [[nodiscard]] constexpr T cube_surface (T side_length) noexcept
{
	return 6 * side_length * side_length;
}

template <Scalar T>
LIMES_EXPORT [[nodiscard]] inline T sphere_surface (T radius) noexcept
{
	return 4 * constants::pi<T> * std::pow (radius, 2);
}

template <Scalar T>
LIMES_EXPORT [[nodiscard]] inline T cylinder_surface (T radius, T height) noexcept
{
	return T (2) * constants::pi<T> * radius * height + T (2) * constants::pi<T> * std::pow (radius, 2);
}

}  // namespace math::area

LIMES_END_NAMESPACE
