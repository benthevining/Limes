
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

namespace limes
{

Dimensions& Dimensions::operator= (const Dimensions& other)
{
	width.store (other.width.load());
	height.store (other.height.load());
	return *this;
}

Dimensions::Dimensions (const Dimensions& other)
{
	width.store (other.width.load());
	height.store (other.height.load());
}

void Dimensions::set (int newWidth, int newHeight)
{
	width.store (newWidth);
	height.store (newHeight);
}

bool Dimensions::operator== (const Dimensions& other) const noexcept
{
	return width.load() == other.width.load() && height.load() == other.height.load();
}

bool Dimensions::isValid() const noexcept
{
	return width.load() > 0 && height.load() > 0;
}

int Dimensions::getWidth() const noexcept
{
	return width.load();
}

int Dimensions::getHeight() const noexcept
{
	return height.load();
}

double Dimensions::getAspectRatio() const noexcept
{
	if (! isValid())
		return 0.;

	return static_cast<double> (width.load()) / static_cast<double> (height.load());
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

}  // namespace limes
