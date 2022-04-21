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
#include "../mathHelpers.h"

LIMES_BEGIN_NAMESPACE

namespace math::perimeter
{

template <Scalar T>
LIMES_EXPORT [[nodiscard]] constexpr T square (T length) noexcept
{
	return T (4) * length;
}

template <Scalar T>
LIMES_EXPORT [[nodiscard]] constexpr T rectangle (T length, T width) noexcept
{
	return T (2) * (length + width);
}

template <Scalar T>
LIMES_EXPORT [[nodiscard]] constexpr T triangle (T base, T height, T hypotenuse) noexcept
{
	return base + height + hypotenuse;
}

template <Scalar T>
LIMES_EXPORT [[nodiscard]] constexpr T circle (T radius) noexcept
{
	return T (2) * constants::pi<T> * radius;
}

template <Scalar T>
LIMES_EXPORT [[nodiscard]] constexpr T cube_surface (T side_length) noexcept
{
	return T (12) * side_length;
}

template <Scalar T>
LIMES_EXPORT [[nodiscard]] constexpr T cylinder_surface (T radius, T height) noexcept
{
	return (T (2) * radius) + (T (2) * height);
}

}  // namespace math::perimeter

LIMES_END_NAMESPACE
