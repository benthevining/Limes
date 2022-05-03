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
#include "fileutil.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <limes_core.h>

namespace fileutil
{

void displayUsage()
{
	limes::printLimesASCII();

	std::cout << "Limes FileUtil: provides a universal, OS-agnostic interface for common filesystem operations\n\n";

	std::cout << "FileUtil <mode> [<additionalArgs...>]\n\n";

	std::cout << "absolute <path> [<basePath>]                : If <path> is an absolute path, prints <path>; otherwise, prints <basePath>/<path>.\n";
	std::cout << "                                              If <basePath> is not specified, it defaults to the current working directory.\n";
	std::cout << "append <fileName> <content> [--strict]      : Appends the <content> to the specified file\n";
	std::cout << "                                              If the --strict option is given, raises an error if the file did not already exist.\n";
	std::cout << "cat <files...> [--output <outputFile>]      : Concatenates the various given files and prints all the output\n";
	std::cout << "                                              If <outputFile> is specified, the concatenated output is written to that file.\n";
	std::cout << "cd <directory>                              : Change the current working directory\n";
	std::cout << "copy <filesOrDirs...> <dest>                : Copies the files or directories to the given destination.\n";
	std::cout << "                                              If only one input file is specified, the destination may be a filename; if multiple inputs are specified, the destination must be a directory.\n";
	std::cout << "                                              If the destination is a directory that doesn't exist, it will be created.\n";
	std::cout << "equiv <path1> <path2> [--error]             : Prints 'yes' if the two paths are equivalent - ie, refer to the same filesystem object; otherwise, prints 'no'.\n";
	std::cout << "                                              If the --error option is given, the result of the comparison is indicated by an exit code of 0 or 1 instead of printing to standard output.\n";
	std::cout << "exists <filesOrDirs...> [--error]           : Prints 'yes' if every item in the list of files or directories exists; otherwise prints 'no'.\n";
	std::cout << "                                              If the --error option is given, the result of the comparison is indicated by an exit code of 0 or 1 instead of printing to standard output.\n";
	std::cout << "follow_symlink <symLink> [--no-recurse]     : Follows the symlink (and any other symlinks it points to, recursively) and prints the absolute path of the target to standard output\n";
	std::cout << "ln <symLinkPath> <symLinkTarget>            : Create a symlink at <symLinkPath> referencing <symLinkTarget>\n";
	std::cout << "ls [<directory>]                            : Print directory contents\n";
	std::cout << "mkdir <directory>                           : Create a directory\n";
	std::cout << "modtime <fileOrDir>                         : Prints the time the file or directory was last modified\n";
	std::cout << "                                              The time is printed in the form HH:MM:SS Day Month Year, with the time in 24-hour format and the month expressed as a 3-letter abbreviation.\n";
	std::cout << "native <path>                               : Converts any directory separators in <path> to the preferred directory separator for the current platform.\n";
	std::cout << "                                              Unlike most other commands, this one does not make the passed path absolute if a relative path was given.\n";
	std::cout << "path                                        : Prints the contents of the PATH environment variable as a list of absolute paths to directories, one per line of output.\n";
	std::cout << "prepend <fileName> <content> [--strict]     : Prepends the <content> to the specified file\n";
	std::cout << "                                              If the --strict option is given, raises an error if the file did not already exist.\n";
	std::cout << "pwd                                         : Print absolute path of current working directory\n";
	std::cout << "relative <path> [<basePath>]                : Prints the <path> made relative to <basePath>\n";
	std::cout << "                                              If <basePath> is not specified, it defaults to the current working directory.\n";
	std::cout << "rename <oldName> <newName>                  : Rename a file or directory\n";
	std::cout << "rm <filesOrDirs...>                         : Remove files, directories, or symlinks\n";
	std::cout << "sep                                         : Prints the current platform's preferred directory separator.\n";
	std::cout << "size [<fileOrDirectory>]                    : Prints the size of the file or directory, in bytes\n";
	std::cout << "                                              If no file or directory is specified, prints the capacity of the entire filesystem.\n";
	std::cout << "space                                       : Prints the available space on the filesystem, in bytes\n";
	std::cout << "touch <filesOrDirs...> [--no-create]        : Update last modification time of files or directories, creating them if they don't exist, unless --no-create is specified\n";
	std::cout << "type <path>                                 : Prints the type of the filesystem entry found at <path>, as one of 'file', 'directory', or 'symlink'.\n";
	std::cout << "which [<programName>]                       : Searches for an executable file named <programName> in each of the directories in the PATH environment variable and prints the absolute path of the first match found.\n";
	std::cout << "                                              Omit the <programName> to search for the FileUtil executable itself.\n";
	std::cout << "write <fileName> <content> [--no-overwrite] : Writes the <content> to the specified file. If the file already existed, it will be overwritten.\n";
	std::cout << "                                              If the --no-overwrite option is given, raises an error if the file already existed.\n\n";

	std::cout << "help|--help|-help|-h : print this message and exit\n\n";
	std::cout << "version|--version|-version|-v : print version information and exit\n";

	std::cout << std::endl;
}

void displayVersionInfo()
{
	limes::printLimesASCII();

	std::cout << "Limes FileUtil: version " << limes::getLimesVersion().toString() << "\n\n";
	std::cout << "Built on " << limes::time::Date::getCompilationDate().toString() << "\n"
			  << std::endl;
}

void parseAndExecute (int argc, char** argv)
{
	if (argc == 1)
	{
		displayUsage();
		return;
	}

	const auto mode = std::string { argv[1] };

	if (mode == "help" || mode == "--help" || mode == "-help" || mode == "-h")
	{
		displayUsage();
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
		std::cout << std::filesystem::path::preferred_separator << std::endl;
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
