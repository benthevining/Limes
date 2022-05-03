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

#include <limes_namespace.h>
#include "Version.h"
#include <string>
#include <sstream>

LIMES_BEGIN_NAMESPACE

std::string Version::toString (char separator) const
{
	std::stringstream stream;

	stream << major << separator << minor << separator << patch;

	return stream.str();
}

LIMES_END_NAMESPACE
