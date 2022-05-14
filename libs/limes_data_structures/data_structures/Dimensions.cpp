
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

#include "Dimensions.h"
#include <limes_platform.h>
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace misc
{

Dimensions& Dimensions::setWidth (int newWidth)
{
	LIMES_ASSERT (newWidth > 0);
	width = newWidth;
	return *this;
}

Dimensions& Dimensions::setHeight (int newHeight)
{
	LIMES_ASSERT (newHeight > 0);
	height = newHeight;
	return *this;
}

bool Dimensions::operator== (const Dimensions& other) const noexcept
{
	return width == other.width && height == other.height;
}

bool Dimensions::isValid() const noexcept
{
	return width > 0 && height > 0;
}

int Dimensions::getWidth() const noexcept
{
	return width;
}

int Dimensions::getHeight() const noexcept
{
	return height;
}

double Dimensions::getAspectRatio() const noexcept
{
	if (! isValid())
		return 0.;

	return static_cast<double> (width) / static_cast<double> (height);
}

bool Dimensions::hasSameAspectRatioAs (const Dimensions& other) const noexcept
{
	return getAspectRatio() == other.getAspectRatio();
}

// String Dimensions::toString() const noexcept
//{
//	return String (width.load()) + "x" + String (height.load());
// }
//
// Dimensions Dimensions::fromString (const String& string)
//{
//	const auto arr = juce::StringArray::fromTokens (string, "x", "");
//
//	jassert (arr.size() == 2);
//
//	return Dimensions { arr[0].getIntValue(), arr[1].getIntValue() };
// }

}  // namespace misc

LIMES_END_NAMESPACE
