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
#include "usage.h"
#include "fileutil.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <limes_core.h>

namespace fileutil
{

void displayVersionInfo()
{
	limes::strings::printLimesASCII();

	const auto newline = limes::strings::new_line;

	std::cout << "Limes FileUtil: version " << limes::getLimesVersion().toString() << newline << newline;
	std::cout << "Built on " << limes::time::Date::getCompilationDate().toString() << newline
			  << std::endl;
}

void parseAndExecute (int argc, char** argv)
{
	if (argc == 1)
	{
		displayUsageOverview();
		return;
	}

	const auto mode = std::string { argv[1] };

	if (mode == "--help" || mode == "-help" || mode == "-h")
	{
		displayUsageOverview();
		return;
	}

	if (mode == "help")
	{
		if (argc > 2)
			displayCommandHelp (std::string { argv[2] });
		else
			displayUsageOverview();

		return;
	}

	if (mode == "version" || mode == "--version" || mode == "-version" || mode == "-v")
	{
		displayVersionInfo();
		return;
	}

	if (mode == "absolute")
	{
		if (argc < 2)
		{
			std::cerr << "Error: Input path not specified in call to absolute" << std::endl;
			std::exit (EXIT_FAILURE);
		}

		std::string basePath;

		if (argc > 3)
			basePath = argv[3];

		fileutil::absolute (std::string { argv[2] }, basePath);
		return;
	}

	if (mode == "append")
	{
		if (argc < 3)
		{
			std::cerr << "Error: filename and content must be specified in call to append" << std::endl;
			std::exit (EXIT_FAILURE);
		}

		bool strict = false;

		if (argc > 4)
		{
			const auto fourthArg = std::string { argv[4] };

			if (fourthArg == "--strict")
				strict = true;
		}

		fileutil::append (std::string { argv[2] }, std::string { argv[3] }, strict);
		return;
	}

	if (mode == "cat")
	{
		std::vector<std::string> items;

		std::string outputFile;

		for (auto i = 2; i < argc; ++i)
		{
			const auto arg = std::string { argv[i] };

			if (arg == "--output")
			{
				if (i + 1 < argc)
				{
					outputFile = argv[i + 1];
					++i;
				}
				else
				{
					std::cerr << "Error: argument --output requires a filename" << std::endl;
					std::exit (EXIT_FAILURE);
				}
			}
			else
			{
				items.emplace_back (arg);
			}
		}

		fileutil::cat (items, outputFile);
		return;
	}

	if (mode == "cd")
	{
		if (argc < 2)
		{
			std::cerr << "Error: Directory name not specified in call to cd" << std::endl;
			std::exit (EXIT_FAILURE);
		}

		fileutil::cd (std::string { argv[2] });
		return;
	}

	if (mode == "copy")
	{
		if (argc < 3)
		{
			std::cerr << "Error: at least one input and a destination must be specified in call to copy" << std::endl;
			std::exit (EXIT_FAILURE);
		}

		std::vector<std::string> items;

		for (auto i = 2; i < argc - 1; ++i)
			items.emplace_back (argv[i]);

		fileutil::copy (items, std::string { argv[argc - 1] });
		return;
	}

	if (mode == "equiv")
	{
		if (argc < 3)
		{
			std::cerr << "Error: two paths must be specified in call to equiv" << std::endl;
			std::exit (EXIT_FAILURE);
		}

		bool error = false;

		if (argc > 4)
		{
			const auto fourthArg = std::string { argv[4] };

			if (fourthArg == "--error")
				error = true;
		}

		fileutil::equivalent (std::string { argv[2] }, std::string { argv[3] }, error);
		return;
	}

	if (mode == "exists")
	{
		std::vector<std::string> items;

		bool error = false;

		for (auto i = 2; i < argc; ++i)
		{
			const auto arg = std::string { argv[i] };

			if (arg == "--error")
				error = true;
			else
				items.emplace_back (arg);
		}

		fileutil::exists (items, error);
		return;
	}

	if (mode == "follow_symlink")
	{
		if (argc < 2)
		{
			std::cerr << "Error: Symlink name not specified in call to follow_symlink" << std::endl;
			std::exit (EXIT_FAILURE);
		}

		const auto recurse = [argc, argv]
		{
			if (argc > 3)
			{
				const auto thirdArg = std::string { argv[3] };

				if (thirdArg == "--no-recurse")
					return false;
			}

			return true;
		}();

		fileutil::follow_symlink (std::string { argv[2] }, recurse);
		return;
	}

	if (mode == "glob")
	{
		bool recurse = false;
		bool error	 = false;

		std::string dir;
		std::string expr;

		for (auto i = 2; i < argc; ++i)
		{
			const auto arg = std::string { argv[i] };

			if (arg == "--recurse")
			{
				recurse = true;
				continue;
			}

			if (arg == "--error")
			{
				error = true;
				continue;
			}

			if (arg == "--dir")
			{
				if (i + 1 >= argc)
				{
					std::cerr << "Error: directory not specified with --dir option" << std::endl;
					std::exit (EXIT_FAILURE);
				}

				dir = argv[i + 1];
				++i;
				continue;
			}

			expr = arg;
		}

		if (expr.empty())
		{
			std::cerr << "Error: globbing expression not specified in call to glob" << std::endl;
			std::exit (EXIT_FAILURE);
		}

		fileutil::glob (expr, dir, recurse, error);
		return;
	}

	if (mode == "hash")
	{
		if (argc < 3)
		{
			std::cerr << "Error: hash type and filename must be specified in call to hash" << std::endl;
			std::exit (EXIT_FAILURE);
		}

		fileutil::hash (std::string { argv[2] }, std::string { argv[3] });
		return;
	}

	if (mode == "ln")
	{
		if (argc < 3)
		{
			std::cerr << "Error: link name and target must be specified in call to ln" << std::endl;
			std::exit (EXIT_FAILURE);
		}

		fileutil::ln (std::string { argv[2] }, std::string { argv[3] });
		return;
	}

	if (mode == "ls")
	{
		if (argc > 2)
			fileutil::ls (std::string { argv[2] });
		else
			fileutil::ls (std::string {});

		return;
	}

	if (mode == "mkdir")
	{
		if (argc < 2)
		{
			std::cerr << "Error: Directory name not specified in call to mkdir" << std::endl;
			std::exit (EXIT_FAILURE);
		}

		fileutil::mkdir (std::string { argv[2] });
		return;
	}

	if (mode == "modtime")
	{
		if (argc < 2)
		{
			std::cerr << "Error: Item name not specified in call to modtime" << std::endl;
			std::exit (EXIT_FAILURE);
		}

		fileutil::modtime (std::string { argv[2] });
		return;
	}

	if (mode == "native")
	{
		if (argc < 2)
		{
			std::cerr << "Error: Item name not specified in call to native" << std::endl;
			std::exit (EXIT_FAILURE);
		}

		fileutil::native (std::string { argv[2] });
		return;
	}

	if (mode == "path")
	{
		fileutil::path();
		return;
	}

	if (mode == "prepend")
	{
		if (argc < 3)
		{
			std::cerr << "Error: filename and content must be specified in call to prepend" << std::endl;
			std::exit (EXIT_FAILURE);
		}

		bool strict = false;

		if (argc > 4)
		{
			const auto fourthArg = std::string { argv[4] };

			if (fourthArg == "--strict")
				strict = true;
		}

		fileutil::prepend (std::string { argv[2] }, std::string { argv[3] }, strict);
		return;
	}

	if (mode == "pwd")
	{
		fileutil::pwd();
		return;
	}

	if (mode == "relative")
	{
		if (argc < 2)
		{
			std::cerr << "Error: input path must be specified in call to relative" << std::endl;
			std::exit (EXIT_FAILURE);
		}

		std::string basePath;

		if (argc > 3)
			basePath = std::string { argv[3] };

		fileutil::relative (std::string { argv[2] }, basePath);
		return;
	}

	if (mode == "rename")
	{
		if (argc < 3)
		{
			std::cerr << "Error: old name and new name must be specified in call to rename" << std::endl;
			std::exit (EXIT_FAILURE);
		}

		fileutil::rename (std::string { argv[2] }, std::string { argv[3] });
		return;
	}

	if (mode == "rm")
	{
		std::vector<std::string> items;

		for (auto i = 2; i < argc; ++i)
			items.emplace_back (argv[i]);

		fileutil::rm (items);
		return;
	}

	if (mode == "sep")
	{
		std::cout << limes::files::dirSeparator() << std::endl;
		return;
	}

	if (mode == "size")
	{
		std::string filename;

		if (argc > 2)
			filename = argv[2];

		fileutil::size (filename);
		return;
	}

	if (mode == "space")
	{
		fileutil::space();
		return;
	}

	if (mode == "touch")
	{
		bool allowCreation = true;

		std::vector<std::string> items;

		for (auto i = 2; i < argc; ++i)
		{
			const auto arg = std::string { argv[i] };

			if (arg == "--no-create")
				allowCreation = false;
			else
				items.emplace_back (arg);
		}

		fileutil::touch (items, allowCreation);
		return;
	}

	if (mode == "type")
	{
		if (argc < 2)
		{
			std::cerr << "Error: path must be specified in call to type" << std::endl;
			std::exit (EXIT_FAILURE);
		}

		fileutil::type (std::string { argv[2] });
		return;
	}

	if (mode == "which")
	{
		std::string programName;

		if (argc > 2)
			programName = argv[2];
		else
			programName = "FileUtil";

		fileutil::which (programName);
		return;
	}

	if (mode == "write")
	{
		if (argc < 3)
		{
			std::cerr << "Error: filename and content must be specified in call to write" << std::endl;
			std::exit (EXIT_FAILURE);
		}

		bool allowOverwrite = true;

		if (argc > 4)
		{
			const auto fourthArg = std::string { argv[4] };

			if (fourthArg == "--no-overwrite")
				allowOverwrite = false;
		}

		fileutil::write (std::string { argv[2] }, std::string { argv[3] }, allowOverwrite);
		return;
	}

	std::cerr << "Error: Unknown mode requested: " << mode << std::endl;
	std::cout << "Run with no arguments or with 'help' to view usage." << std::endl;
	std::exit (EXIT_FAILURE);
}

}  // namespace fileutil
