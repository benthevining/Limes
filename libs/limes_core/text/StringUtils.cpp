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

#include <cstring>	// for strcmp, strlen

namespace limes::cstring
{

bool areSame (const char* string1, const char* string2)
{
	return std::strcmp (string1, string2) == 0;
}

int length (const char* string)
{
	return static_cast<int> (std::strlen (string));
}

}  // namespace limes::cstring
