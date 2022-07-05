
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

#include <limes_export.h>  // for LIMES_EXPORT
#include "../../limes_namespace.h"
#include <string>
#include "../../misc/preprocessor.h"
#include "../../system/limes_assert.h"
#include <functional>  // for std::hash

/** @dir libs/limes_core/data_structures/dimensions
	This directory contains the Dimensions class.
	@ingroup data_structures
 */

/** @file
	This file defines the Dimensions class.
	@ingroup data_structures
 */

LIMES_BEGIN_NAMESPACE

namespace ds
{

/** A simple struct that represents the dimensions of something.
	This class is typically used for working with GUIs.
	@ingroup data_structures
 */
struct LIMES_EXPORT Dimensions final
{
	/** @name Constructors */
	///@{
	/** Creates an invalid Dimensions object. */
	constexpr explicit Dimensions() = default;

	/** Creates a valid Dimensions object with a specified width and height. */
	constexpr explicit Dimensions (int widthToUse, int heightToUse)
		: width (widthToUse), height (heightToUse)
	{
	}
	///@}

	LIMES_CONSTEXPR_MOVABLE (Dimensions)
	LIMES_CONSTEXPR_COPYABLE (Dimensions)

	/** @name Setters */
	///@{
	/** Sets the width of these dimensions. */
	constexpr Dimensions& setWidth (int newWidth) noexcept
	{
		LIMES_ASSERT (newWidth > 0);
		width = newWidth;
		return *this;
	}

	/** Sets the height of these dimensions. */
	constexpr Dimensions& setHeight (int newHeight) noexcept
	{
		LIMES_ASSERT (newHeight > 0);
		height = newHeight;
		return *this;
	}
	///@}

	/** @name Getters */
	///@{
	/** Returns the width of these dimensions. */
	[[nodiscard]] constexpr int getWidth() const noexcept
	{
		return width;
	}

	/** Returns the height of these dimensions. */
	[[nodiscard]] constexpr int getHeight() const noexcept
	{
		return height;
	}
	///@}

	/** @name Equality comparison */
	///@{
	/** Returns true if the passed Dimensions object is equal to this one. */
	constexpr bool operator== (const Dimensions& other) const noexcept
	{
		return width == other.width && height == other.height;
	}

	/** Returns true if the passed Dimensions object is not equal to this one. */
	constexpr bool operator!= (const Dimensions& other) const noexcept
	{
		return ! (*this == other);
	}
	///@}

	/** Returns true if the width and height are both greater than 0. */
	constexpr bool isValid() const noexcept
	{
		return width > 0 && height > 0;
	}

	/** @name Aspect ratio */
	///@{
	/** Returns the aspect ratio of the represented dimensions, calculated as \c width/height.
		If \c isValid() return false, then this function will return 0.
	*/
	[[nodiscard]] constexpr double getAspectRatio() const noexcept
	{
		if (! isValid())
			return 0.;

		return static_cast<double> (width) / static_cast<double> (height);
	}

	/** Returns true if the passed Dimensions object has the same aspect ratio as this one. */
	[[nodiscard]] constexpr bool hasSameAspectRatioAs (const Dimensions& other) const noexcept
	{
		return getAspectRatio() == other.getAspectRatio();
	}
	///@}

	/** Returns a string representation of these dimensions, eg '400x600'. */
	[[nodiscard]] std::string toString() const noexcept;

	/** Returns a Dimensions object with the size 1060 x 640. */
	[[nodiscard]] static constexpr Dimensions getDefault() { return Dimensions { 1060, 640 }; }	 // NOLINT

private:
	int width { 0 }, height { 0 };
};

}  // namespace ds

LIMES_END_NAMESPACE

namespace std
{

/** A specialization of \c std::hash for Dimensions objects.
	@ingroup data_structures
 */
template <>
struct LIMES_EXPORT hash<limes::ds::Dimensions> final
{
	hash() = default;

	LIMES_DEFAULT_COPYABLE (hash)
	LIMES_DEFAULT_MOVABLE (hash)

	size_t operator() (const limes::ds::Dimensions& d) const noexcept;
};

}  // namespace std
