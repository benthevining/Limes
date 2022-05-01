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

namespace math::volume
{

template <Scalar T>
LIMES_EXPORT [[nodiscard]] constexpr T rectangular_prism (T length, T width, T height) noexcept
{
	return length * width * height;
}

template <Scalar T>
LIMES_EXPORT [[nodiscard]] inline T cone (T radius, T height) noexcept
{
	return std::pow (radius, 2) * constants::pi<T> * height / T (3);
}

template <Scalar T>
LIMES_EXPORT [[nodiscard]] constexpr T triangular_prism (T baseSideLength, T height, T depth) noexcept
{
	return baseSideLength * height * depth / T (2);
}

template <Scalar T>
LIMES_EXPORT [[nodiscard]] constexpr T pyramid (T length, T width, T height) noexcept
{
	return length * width * height / T (3);
}

}  // namespace math::volume

LIMES_END_NAMESPACE
