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
#include <limes_export.h>
#include <limes_platform.h>
#include "../../../misc/preprocessor.h"
#include "../2D/triangle.h"
#include "3Dshape.h"

LIMES_BEGIN_NAMESPACE

namespace math::geometry
{

template <typename ValueType>
class LIMES_EXPORT TriangularPrism final : public ThreeDShape<ValueType>
{
public:

	explicit constexpr TriangularPrism (const Triangle<ValueType>& face, ValueType length) noexcept;

	explicit constexpr TriangularPrism (ValueType face_base, ValueType face_height, ValueType face_hypotenuse, ValueType prismLength) noexcept;

	LIMES_CONSTEXPR_COPYABLE (TriangularPrism);
	LIMES_CONSTEXPR_MOVABLE (TriangularPrism);

	[[nodiscard]] constexpr ValueType surfaceArea() const noexcept final;

	[[nodiscard]] constexpr ValueType volume() const noexcept final;

	[[nodiscard]] constexpr ValueType length() const noexcept;

	[[nodiscard]] constexpr Triangle<ValueType> getFace() const noexcept;

	[[nodiscard]] constexpr bool operator== (const TriangularPrism& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const TriangularPrism& other) const noexcept;

private:

	ValueType m_length;

	Triangle<ValueType> m_face;
};


template <typename ValueType>
constexpr TriangularPrism<ValueType>::TriangularPrism (const Triangle<ValueType>& face, ValueType length) noexcept
	: m_length (length), m_face (face)
{
	LIMES_ASSERT (m_length > 0);
}

template <typename ValueType>
constexpr TriangularPrism<ValueType>::TriangularPrism (ValueType face_base, ValueType face_height, ValueType face_hypotenuse, ValueType prismLength) noexcept
	: m_length (prismLength), m_face (face_base, face_height, face_hypotenuse)
{
	LIMES_ASSERT (m_length > 0);
}

template <typename ValueType>
constexpr ValueType TriangularPrism<ValueType>::surfaceArea() const noexcept
{
	return (m_face.perimeter() * m_length) + (m_face.base() * m_face.altitude());
}

template <typename ValueType>
constexpr ValueType TriangularPrism<ValueType>::volume() const noexcept
{
	return m_face.area() * m_length;
}

template <typename ValueType>
constexpr ValueType TriangularPrism<ValueType>::length() const noexcept
{
	return m_length;
}

template <typename ValueType>
constexpr Triangle<ValueType> TriangularPrism<ValueType>::getFace() const noexcept
{
	return m_face;
}

template <typename ValueType>
constexpr bool TriangularPrism<ValueType>::operator== (const TriangularPrism& other) const noexcept
{
	return m_length == other.m_length && m_face == other.m_face;
}

template <typename ValueType>
constexpr bool TriangularPrism<ValueType>::operator!= (const TriangularPrism& other) const noexcept
{
	return ! (*this == other);
}

}  // namespace math::geometry

LIMES_END_NAMESPACE
