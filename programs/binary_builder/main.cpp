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

#include "commandLine.h"
#include "generator.h"
#include <iostream>
#include <exception>

int main (int argc, char** argv)
{
	try
	{
		const auto options = binary_builder::parseCommandLine (argc, argv);

		if (options.inputFiles.empty())
			std::cerr << "Warning: no input file to process, will generate empty C++ output!\n";
		else
			std::cout << "Ready to process " << options.inputFiles.size() << " file(s).\n";

		binary_builder::generateHeaderFile (options);
		binary_builder::generateBodyFile (options);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
