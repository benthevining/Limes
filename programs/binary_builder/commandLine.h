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

#pragma once

#include <vector>
#include <string>
#include <limes_core.h>

namespace binary_builder
{

using namespace limes::files;

struct Options final
{
	// list of files to embed
	std::vector<File> inputFiles;

	// output directory for generated files
	Directory outputDir;

	// output file names
	std::string headerFileName;
	std::string cppFileName;

	// C++ namespace to use (if any)
	std::string namespaceName;
};

static constexpr char s_defaultOutputBase[] = "BinaryData";

void displayUsage();

[[nodiscard]] Options parseCommandLine (int argc, char** argv);

}  // namespace binary_builder
