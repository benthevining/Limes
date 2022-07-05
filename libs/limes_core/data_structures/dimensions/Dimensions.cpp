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

#include "../../limes_namespace.h"
#include "./Dimensions.h"
#include <string>
#include <sstream>
#include "../../math/mathFunctions.h"

LIMES_BEGIN_NAMESPACE

namespace ds
{

std::string Dimensions::toString() const noexcept
{
	std::stringstream stream;

	stream << width << 'x' << height;

	return stream.str();
}

}  // namespace ds

LIMES_END_NAMESPACE

namespace std
{

size_t hash<limes::ds::Dimensions>::operator() (const limes::ds::Dimensions& d) const noexcept
{
	return static_cast<size_t> (::limes::math::szudzikPair (d.getWidth(), d.getHeight()));
}

}  // namespace std
