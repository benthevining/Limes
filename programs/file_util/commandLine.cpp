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

	std::cout << "append <fileName> <content>             : Appends the <content> to the specified file\n";
	std::cout << "cat <files...>                          : Concatenates the various given files and prints all the output\n";
	std::cout << "cd <directory>                          : Change the current working directory\n";
	std::cout << "copy <filesOrDirs...> <dest>            : Copies the files or directories to the given destination.\n";
	std::cout << "                                          If only one input file is specified, the destination may be a filename; if multiple inputs are specified, the destination must be a directory.\n";
	std::cout << "                                          If the destination is a directory that doesn't exist, it will be created.\n";
	std::cout << "exists <filesOrDirs...>                 : Prints 'yes' if every item in the list of files or directories exists; otherwise prints 'no'.\n";
	std::cout << "follow_symlink <symLink> [--no-recurse] : Follows the symlink (and any other symlinks it points to, recursively) and prints the absolute path of the target to standard output\n";
	std::cout << "ln <symLinkPath> <symLinkTarget>        : Create a symlink at <symLinkPath> referencing <symLinkTarget>\n";
	std::cout << "ls [<directory>]                        : Print directory contents\n";
	std::cout << "mkdir <directory>                       : Create a directory\n";
	std::cout << "modtime <fileOrDir>                     : Prints the time the file or directory was last modified\n";
	std::cout << "                                          The time is printed in the form HH:MM:SS Day Month Year, with the time in 24-hour format and the month expressed as a 3-letter abbreviation.\n";
	std::cout << "pwd                                     : Print absolute path of current working directory\n";
	std::cout << "relative <path> <basePath>              : Prints the <path> made relative to <basePath>\n";
	std::cout << "rename <oldName> <newName>              : Rename a file or directory\n";
	std::cout << "rm <files/dirs...>                      : Remove files, directories, or symlinks\n";
	std::cout << "size <fileOrDirectory>                  : Prints the size of the file or directory, in bytes\n";
	std::cout << "space                                   : Prints the available space on the filesystem, in bytes\n";
	std::cout << "touch <files/dirs...>                   : Update last modification time of files or directories, creating them if they don't exist\n";
	std::cout << "touch_nocreate <files/dirs...>          : Update last modification time of files or directories; do nothing if they don't exist\n";
	std::cout << "type <path>                             : Prints the type of the filesystem entry found at <path>, as one of 'file', 'directory', or 'symlink'.\n";
	std::cout << "write <fileName> <content>              : Writes the <content> to the specified file. If the file already existed, it will be overwritten.\n";
}

void parseAndExecute (int argc, char** argv)
{
	if (argc == 1)
	{
		displayUsage();
		return;
	}

	const auto mode = std::string { argv[1] };

	auto getVectorOfInputs = [argc, argv]
	{
		std::vector<std::string> items;

		for (auto i = 2; i < argc; ++i)
			items.emplace_back (argv[i]);

		return items;
	};

	if (mode == "help" || mode == "--help" || mode == "-help" || mode == "-h")
	{
		displayUsage();
		return;
	}

	if (mode == "append")
	{
		if (argc < 3)
		{
			std::cerr << "Error: filename and content must be specified in call to append" << std::endl;
			std::exit (EXIT_FAILURE);
		}

		fileutil::append (std::string { argv[2] }, std::string { argv[3] });
		return;
	}

	if (mode == "cat")
	{
		fileutil::cat (getVectorOfInputs());
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

	if (mode == "exists")
	{
		fileutil::exists (getVectorOfInputs());
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

	if (mode == "pwd")
	{
		fileutil::pwd();
		return;
	}

	if (mode == "relative")
	{
		if (argc < 3)
		{
			std::cerr << "Error: path and base path must be specified in call to relative" << std::endl;
			std::exit (EXIT_FAILURE);
		}

		fileutil::relative (std::string { argv[2] }, std::string { argv[3] });
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
		fileutil::rm (getVectorOfInputs());
		return;
	}

	if (mode == "size")
	{
		if (argc < 2)
		{
			std::cerr << "Error: Name not specified in call to size" << std::endl;
			std::exit (EXIT_FAILURE);
		}

		fileutil::size (std::string { argv[2] });
		return;
	}

	if (mode == "space")
	{
		fileutil::space();
		return;
	}

	if (mode == "touch" || mode == "touch_nocreate")
	{
		const auto allowCreation = mode == "touch";

		fileutil::touch (getVectorOfInputs(), allowCreation);
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

	if (mode == "write")
	{
		if (argc < 3)
		{
			std::cerr << "Error: filename and content must be specified in call to write" << std::endl;
			std::exit (EXIT_FAILURE);
		}

		fileutil::write (std::string { argv[2] }, std::string { argv[3] });
		return;
	}

	std::cerr << "Error: Unknown mode requested: " << mode << std::endl;
	std::cout << "Run with no arguments or with 'help' to view usage." << std::endl;
	std::exit (EXIT_FAILURE);
}

}  // namespace fileutil
