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
#include <iostream>
#include <string>
#include <exception>
#include <cstdlib>
#include <functional>
#include <limes_core.h>

namespace binary_builder
{

void displayUsage()
{
	limes::printLimesASCII();

	std::cout << "Limes BinaryBuilder: generates C++ source code which embed several external (binary) files.\n\n";

	std::cout << "Supported options:\n";
	std::cout << " <input>	: path to an input file or directory to embed in C++ code.\n";
	std::cout << "			  If it's a directory, its content will be recursively iterated.\n";
	std::cout << "			  Note: several inputs can be passed on the command line.\n";
	std::cout << " -h		    : this help message.\n";
	std::cout << " -d <path>  : directory where to save the generated files.\n";
	std::cout << " -o <name>  : base name to be used for the generated .h/.cpp files.\n";
	std::cout << "			  => '-o generated' will produce 'generated.h' and 'generated.cpp' files.\n";
	std::cout << "			  Default value is '" << s_defaultOutputBase << "'.\n";
	std::cout << " -ns <name> : name of the namespace to be used in generated code (recommended).\n";
	std::cout << "			  Default is empty (no namespace).\n";
}

Options parseCommandLine (int argc, char** argv)
{
	auto parseNamedArgument = [] (const std::string& argName, const std::string& argValue, Options& options)
	{
		LIMES_ASSERT (argName.front() == '-');
		LIMES_ASSERT (! argValue.empty());

		if (argName == "-d")
		{
			Directory dir { argValue };

			if (! dir.exists())
				if (! dir.createIfDoesntExist())
					throw std::runtime_error { "Invalid output directory: " + argValue };

			options.outputDir = dir;
		}
		else if (argName == "-o")
		{
			options.headerFileName = argValue + ".h";
			options.cppFileName	   = argValue + ".cpp";
		}
		else if (argName == "-ns")
		{
			options.namespaceName = argValue;
		}
		else
		{
			throw std::runtime_error { "Invalid option name: " + argName };
		}
	};

	auto parsePositionalArgument = [] (const std::string& value, Options& options)
	{
		std::function<void (const FilesystemEntry& entry)> processEntry = [&options, &processEntry] (const FilesystemEntry& entry)
		{
			if (entry.isDirectory())
			{
				entry.getDirectoryObject()->iterateFiles ([&options] (const File& f)
														  { options.inputFiles.emplace_back (f); },
														  true);
			}
			else if (entry.isFile())
			{
				options.inputFiles.emplace_back (entry.getAbsolutePath());
			}
			else if (entry.isSymLink())
			{
				processEntry (entry.getSymLinkObject()->follow (true));
			}
			else
			{
				throw std::runtime_error { "Can't find file or directory named " + entry.getAbsolutePath().string() };
			}
		};

		processEntry (FilesystemEntry { value });
	};

	//

	Options options;

	if (argc == 1)
	{
		displayUsage();
		std::exit (EXIT_SUCCESS);
	}

	for (auto i = 1; i < argc; ++i)
	{
		const std::string arg { argv[i] };

		if (arg.front() == '-')
		{
			if (arg == "-h")
			{
				displayUsage();
				std::exit (EXIT_SUCCESS);
			}
			else if (i == argc - 1)
			{
				throw std::runtime_error { "Missing value for option " + arg };
			}
			else
			{
				parseNamedArgument (arg, argv[i + 1], options);
				i += 1;
			}
		}
		else
		{
			parsePositionalArgument (arg, options);
		}
	}

	if (options.cppFileName.empty() || options.headerFileName.empty())
	{
		options.headerFileName = std::string { s_defaultOutputBase } + ".h";
		options.cppFileName	   = std::string { s_defaultOutputBase } + ".cpp";
	}

	return options;
}

}  // namespace binary_builder
