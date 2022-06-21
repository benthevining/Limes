
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
#include <limes_namespace.h>
#include <string>
#include "../../misc/preprocessor.h"

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

	LIMES_DEFAULT_MOVABLE (Dimensions)
	LIMES_DEFAULT_COPYABLE (Dimensions)

	/** Sets the width of these dimensions. */
	Dimensions& setWidth (int newWidth);

	/** Sets the height of these dimensions. */
	Dimensions& setHeight (int newHeight);

	/** Returns true if the passed Dimensions object is equal to this one. */
	[[nodiscard]] bool operator== (const Dimensions& other) const noexcept;

	/** Returns true if the width and height are both greater than 0. */
	[[nodiscard]] bool isValid() const noexcept;

	/** Returns the width of these dimensions. */
	[[nodiscard]] int getWidth() const noexcept;

	/** Returns the height of these dimensions. */
	[[nodiscard]] int getHeight() const noexcept;

	/** Returns the aspect ratio of the represented dimensions, calculated as width / height.
		If isValid() return false, then this function will return 0.
	*/
	[[nodiscard]] double getAspectRatio() const noexcept;

	/** Returns true if the passed Dimensions object has the same aspect ratio as this one. */
	[[nodiscard]] bool hasSameAspectRatioAs (const Dimensions& other) const noexcept;

	/** Returns a string representation of these dimensions, eg '400x600'. */
	[[nodiscard]] std::string toString() const noexcept;

	/** Returns a Dimensions object representing the size 1060 x 640. */
	[[nodiscard]] static constexpr Dimensions getDefault() { return Dimensions { 1060, 640 }; }	 // NOLINT

private:

	int width { 0 }, height { 0 };
};

}  // namespace ds

LIMES_END_NAMESPACE
