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
#include "../2D/square.h"
#include "3Dshape.h"
#include <cmath>

/** @file
	This file defines the RectangularPrism class.
	@ingroup ThreeDgeometry
 */

LIMES_BEGIN_NAMESPACE

namespace math::geometry
{

/** This class represents a rectagular prism.
	Rectangular prisms may or may not be cubes.
	@see Cube
	@ingroup ThreeDgeometry
 */
template <Scalar ValueType>
class LIMES_EXPORT RectangularPrism final : public ThreeDShape<ValueType>
{
public:

	/** Creates a RectangularPrism with the given dimensions. */
	explicit constexpr RectangularPrism (ValueType length, ValueType width, ValueType height) noexcept;

	LIMES_CONSTEXPR_COPYABLE (RectangularPrism);
	LIMES_CONSTEXPR_MOVABLE (RectangularPrism);

	/** Returns the volume of the prism.
		The formula is @f$ V=length*width*height @f$.
	 */
	[[nodiscard]] constexpr ValueType volume() const noexcept final;

	/** Returns the surface area of the prism.
		The formula is @f$ A=2*((length*width)+(length*height)+(height*width)) @f$.
	 */
	[[nodiscard]] constexpr ValueType surfaceArea() const noexcept final;

	/** Returns the space diagonal of the prism.
		The formula is @f$ d=\sqrt{length^2+width^2+height^2} @f$.
	 */
	[[nodiscard]] ValueType spaceDiagonal() const noexcept;

	/** Returns true if this rectangular prism is a cube. */
	[[nodiscard]] constexpr bool isCube() const noexcept;

	/** Returns the length of the prism. */
	[[nodiscard]] constexpr ValueType length() const noexcept;

	/** Returns the width of the prism. */
	[[nodiscard]] constexpr ValueType width() const noexcept;

	/** Returns the height of the prism. */
	[[nodiscard]] constexpr ValueType height() const noexcept;

	/** @name Equality comparisons */
	///@{
	[[nodiscard]] constexpr bool operator== (const RectangularPrism& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const RectangularPrism& other) const noexcept;
	///@}

private:

	ValueType m_length, m_width, m_height;
};


template <Scalar ValueType>
constexpr RectangularPrism<ValueType>::RectangularPrism (ValueType length, ValueType width, ValueType height) noexcept
	: m_length (length), m_width (width), m_height (height)
{
	LIMES_ASSERT (m_length > 0);
	LIMES_ASSERT (m_width > 0);
	LIMES_ASSERT (m_height > 0);
}

template <Scalar ValueType>
constexpr ValueType RectangularPrism<ValueType>::volume() const noexcept
{
	return m_length * m_width * m_height;
}

template <Scalar ValueType>
constexpr ValueType RectangularPrism<ValueType>::surfaceArea() const noexcept
{
	return ValueType (2) * ((m_width * m_length) + (m_height * m_length) + (m_height * m_width));
}

template <Scalar ValueType>
ValueType RectangularPrism<ValueType>::spaceDiagonal() const noexcept
{
	return std::sqrt ((m_length * m_length) + (m_width * m_width) + (m_height * m_height));
}

template <Scalar ValueType>
constexpr bool RectangularPrism<ValueType>::isCube() const noexcept
{
	return m_length == m_width && m_length == m_height;
}

template <Scalar ValueType>
constexpr ValueType RectangularPrism<ValueType>::length() const noexcept
{
	return m_length;
}

template <Scalar ValueType>
constexpr ValueType RectangularPrism<ValueType>::width() const noexcept
{
	return m_width;
}

template <Scalar ValueType>
constexpr ValueType RectangularPrism<ValueType>::height() const noexcept
{
	return m_height;
}

template <Scalar ValueType>
constexpr bool RectangularPrism<ValueType>::operator== (const RectangularPrism& other) const noexcept
{
	return m_length == other.m_length && m_width == other.m_width && m_height == other.m_height;
}

template <Scalar ValueType>
constexpr bool RectangularPrism<ValueType>::operator!= (const RectangularPrism& other) const noexcept
{
	return ! (*this == other);
}

}  // namespace math::geometry

LIMES_END_NAMESPACE
