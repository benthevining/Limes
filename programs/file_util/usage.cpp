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

#include "usage.h"
#include <string>
#include <vector>
#include <iostream>
#include <limes_core.h>

namespace fileutil
{

struct Command final
{
	explicit Command (const std::string& nameToUse, const std::string& signatureToUse, const std::string& helpString)
		: name (nameToUse), signature (signatureToUse), help (helpString)
	{
	}

	LIMES_DEFAULT_COPYABLE (Command);
	LIMES_DEFAULT_MOVABLE (Command);

	std::string name, signature, help;
};

// version


[[nodiscard]] std::vector<Command> getAllCommands()
{
	const auto newLine = std::string { limes::strings::new_line };

	std::vector<Command> commands;

	commands.emplace_back ("absolute",
						   "<path> [<basePath>]",
						   "If <path> is an absolute path, prints <path>; otherwise, prints <basePath>/<path>." + newLine
							   + "If <basePath> is not specified, it defaults to the current working directory.");

	commands.emplace_back ("append",
						   "<fileName> <content> [--strict]",
						   "Appends the <content> to the specified file." + newLine
							   + "If the --strict option is given, raises an error if the file did not already exist.");

	commands.emplace_back ("cat",
						   "<files...> [--output <outputFile>]",
						   "Concatenates the various given files and prints all the output." + newLine
							   + "If <outputFile> is specified, the concatenated output is written to that file.");

	commands.emplace_back ("cd",
						   "<directory>",
						   "Change the current working directory.");

	commands.emplace_back ("copy",
						   "<filesOrDirs...> <dest>",
						   "Copies the files or directories to the given destination." + newLine
							   + "If only one input file is specified, the destination may be a filename; if multiple inputs are specified, the destination must be a directory." + newLine
							   + "If the destination is a directory that doesn't exist, it will be created.");

	commands.emplace_back ("equiv",
						   "<path1> <path2> [--error]",
						   "Prints 'yes' if the two paths are equivalent - ie, refer to the same filesystem object; otherwise, prints 'no'." + newLine
							   + "If the --error option is given, the result of the comparison is indicated by an exit code of 0 or 1 instead of printing to standard output.");

	commands.emplace_back ("exists",
						   "<filesOrDirs...> [--error]",
						   "Prints 'yes' if every item in the list of files or directories exists; otherwise prints 'no'." + newLine
							   + "If the --error option is given, the result of the comparison is indicated by an exit code of 0 or 1 instead of printing to standard output.");

	commands.emplace_back ("follow_symlink",
						   "<symLink> [--no-recurse]",
						   "Follows the symlink (and any other symlinks it points to, recursively, unless the --no-recurse option is given) and prints the absolute path of the target to standard output.");

	commands.emplace_back ("glob",
						   "<expr> [--recurse] [--dir <baseDir>] [--error]",
						   "Prints a list of absolute paths found that match the given globbing expression." + newLine
							   + "If the <baseDir> option is given, files will be searched for in that directory. Otherwise, it defaults to the current working directory." + newLine
							   + "If the --error flag is given, the program will exit with an error code if no files are found. Otherwise, 'No files found' will be printed.");

	commands.emplace_back ("hash",
						   "<hashType> <filename>",
						   "Prints a hash of the given file's contents. Raises an error if the file does not exist." + newLine
							   + "Available hash types are: md5, sha1, sha224, sha256, sha384, sha512");

	commands.emplace_back ("ln",
						   "<symLinkPath> <symLinkTarget>",
						   "Create a symlink at <symLinkPath> referencing <symLinkTarget>.");

	commands.emplace_back ("ls",
						   "[<directory>]",
						   "Print directory contents.");

	commands.emplace_back ("mkdir",
						   "<directory>",
						   "Create a directory.");

	commands.emplace_back ("modtime",
						   "<fileOrDir>",
						   "Prints the time the file or directory was last modified." + newLine
							   + "The time is printed in the form HH:MM:SS Day Month Year, with the time in 24-hour format and the month expressed as a 3-letter abbreviation.");

	commands.emplace_back ("native",
						   "<path>",
						   "Converts any directory separators in <path> to the preferred directory separator for the current platform." + newLine
							   + "Unlike most other commands, this one does not make the passed path absolute if a relative path was given.");

	commands.emplace_back ("path",
						   "",
						   "Prints the contents of the PATH environment variable as a list of absolute paths to directories, one per line of output.");

	commands.emplace_back ("prepend",
						   "<fileName> <content> [--strict]",
						   "Prepends the <content> to the specified file." + newLine
							   + "If the --strict option is given, raises an error if the file did not already exist.");

	commands.emplace_back ("pwd",
						   "",
						   "Prints the absolute path of the current working directory.");

	commands.emplace_back ("relative",
						   "<path> [<basePath>]",
						   "Prints the <path> made relative to <basePath>." + newLine
							   + "If <basePath> is not specified, it defaults to the current working directory.");

	commands.emplace_back ("rename",
						   "<oldName> <newName>",
						   "Rename a file or directory.");

	commands.emplace_back ("rm",
						   "<filesOrDirs...>",
						   "Removes files, directories, or symlinks.");

	commands.emplace_back ("sep",
						   "",
						   "Prints the current platform's preferred directory separator.");

	commands.emplace_back ("size",
						   "[<fileOrDirectory>]",
						   "Prints the size of the file or directory, in bytes." + newLine
							   + "If no file or directory is specified, prints the capacity of the entire filesystem.");

	commands.emplace_back ("space",
						   "",
						   "Prints the available space on the filesystem, in bytes.");

	commands.emplace_back ("touch",
						   "<filesOrDirs...> [--no-create]",
						   "Update last modification time of files or directories, creating them if they don't exist, unless --no-create is specified.");

	commands.emplace_back ("type",
						   "<path>",
						   "Prints the type of the filesystem entry found at <path>, as one of 'file', 'directory', or 'symlink'.");

	commands.emplace_back ("which",
						   "[<programName>]",
						   "Searches for an executable file named <programName> in each of the directories in the PATH environment variable and prints the absolute path of the first match found." + newLine
							   + "Omit the <programName> to print the path of the current FileUtil executable itself.");

	commands.emplace_back ("write",
						   "<fileName> <content> [--no-overwrite]",
						   "Writes the <content> to the specified file. If the file already existed, it will be overwritten." + newLine
							   + "If the --no-overwrite option is given, raises an error if the file already existed.");

	return commands;
}


void displayUsageOverview()
{
	limes::strings::printLimesASCII();

	const auto newLine = limes::strings::new_line;

	std::cout << "Limes FileUtil: provides a universal, OS-agnostic interface for common filesystem operations" << newLine << newLine;

	std::cout << "FileUtil <mode> [<additionalArgs...>]" << newLine << newLine;

	std::cout << "Run 'help <modeName>' to view details about a specific mode." << newLine << newLine;

	std::cout << "Run 'version' to display version info." << newLine << newLine;

	std::cout << "Available modes are: " << newLine;

	for (const auto& command : getAllCommands())
		std::cout << newLine << command.name << " " << command.signature << newLine;

	std::cout << std::endl;
}

void displayCommandHelp (const std::string& commandName)
{
	const auto allCommands = getAllCommands();

	const auto it = std::find_if (allCommands.begin(), allCommands.end(),
								  [&commandName] (const Command& c)
								  { return c.name == commandName; });

	if (it == allCommands.end())
	{
		std::cerr << "Error: No command named " << commandName << std::endl;
		std::cout << "Run with no arguments to display an overview of all commands." << std::endl;
		std::exit (EXIT_FAILURE);
	}

	const auto newLine = limes::strings::new_line;

	const auto& command = *it;

	std::cout << newLine << "FileUtil " << command.name << " " << command.signature << newLine << newLine
			  << command.help << newLine
			  << std::endl;
}

}  // namespace fileutil
