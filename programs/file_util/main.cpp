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
*/

#include <iostream>
#include <exception>
#include <cstdlib>
#include "commandLine.h"

#include <limes_core.h>

int main (int argc, char** argv)
{
	// limes::files::File jsonFile { "/Users/benvining/Documents/MyRepos/Citrus/Limes/CMakePresets.json" };

	// const auto json = limes::json::Node { jsonFile.loadAsString() };

	// std::cout << json.getJsonText();

	// return;

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
