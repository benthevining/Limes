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

#include <iostream>
#include <exception>
#include <cstdlib>
#include "commandLine.h"

#include <limes_core.h>

int main (int argc, char** argv)
{
	try
	{
		limes::files::File jsonFile { "/Users/benvining/Documents/MyRepos/Citrus/Limes/CMakePresets.json" };

		const auto json = limes::serializing::parseJSON (jsonFile.loadAsString());

		std::cout << json.getJsonString();

		return;


		fileutil::parseAndExecute (argc, argv);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
