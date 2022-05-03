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
#include <iostream>
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace cstring
{

bool areSame (const char* string1, const char* string2)
{
	return std::strcmp (string1, string2) == 0;
}

int length (const char* string)
{
	return static_cast<int> (std::strlen (string));
}

}  // namespace cstring

void printLimesASCII()
{
	std::cout << "  __    ____  __  __  ____  ___\n";
	std::cout << " (  )  (_  _)(  \\/  )( ___)/ __)\n";
	std::cout << "  )(__  _)(_  )    (  )__) \\__ \\\n";
	std::cout << " (____)(____)(_/\\/\\_)(____)(___/\n"
			  << std::endl;
}

LIMES_END_NAMESPACE
