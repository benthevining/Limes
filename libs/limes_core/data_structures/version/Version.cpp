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

#include "./Version.h"
#include "../../limes_namespace.h"
#include <sstream>	// for operator<<, basic_ostream, stringstream

LIMES_BEGIN_NAMESPACE

namespace ds
{

std::string Version::toString (char separator) const
{
	std::stringstream stream;

	stream << major << separator << minor << separator << patch;

	return stream.str();
}

}  // namespace ds

LIMES_END_NAMESPACE

namespace std
{
size_t hash<limes::ds::Version>::operator() (const limes::ds::Version& v) const noexcept
{
	return hash<string> {}(v.toString ('.'));
}
}  // namespace std
