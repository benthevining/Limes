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

/*
- run command in new working dir
- get permissions
- set permissions
- get specific parts of a path
- convert an input path to preferred dir separators for platform
- check if a path is absolute
- oldest/newest out of list of files
*/

#include <iostream>
#include <exception>
#include <cstdlib>
#include "commandLine.h"


int main (int argc, char** argv)
{
	try
	{
		fileutil::parseAndExecute (argc, argv);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
