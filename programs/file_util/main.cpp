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
#include <string>
#include <vector>
#include <limes_core.h>

namespace fileutil
{

void displayUsage()
{
	std::cout << "append <fileName> <content>      : Appends the <content> to the specified file\n";
	std::cout << "cat <files...>                   : Concatenates the various given files and prints all the output\n";
	std::cout << "cd <directory>                   : Change the current working directory\n";
	std::cout << "copy <filesOrDirs...> <dest>     : Copies the files or directories to the given destination.\n";
	std::cout << "                                   If only one input file is specified, the destination may be a filename; if multiple inputs are specified, the destination must be a directory.\n";
	std::cout << "                                   If the destination is a directory that doesn't exist, it will be created.\n";
	std::cout << "follow_symlink <symLink>         : Follows the symlink (and any other symlinks it points to, recursively) and prints the absolute path of the target to standard output\n";
	std::cout << "ln <symLinkPath> <symLinkTarget> : Create a symlink at <symLinkPath> referencing <symLinkTarget>\n";
	std::cout << "ls [<directory>]                 : Print directory contents\n";
	std::cout << "mkdir <directory>                : Create a directory\n";
	std::cout << "modtime <fileOrDir>              : Prints the time the file or directory was last modified\n";
	std::cout << "                                   The time is printed in the form HH:MM:SS Day Month Year, with the time in 24-hour format and the month expressed as a 3-letter abbreviation.\n";
	std::cout << "pwd                              : Print absolute path of current working directory\n";
	std::cout << "rename <oldName> <newName>       : Rename a file or directory\n";
	std::cout << "rm <files/dirs...>               : Remove files, directories, or symlinks\n";
	std::cout << "size <fileOrDirectory>           : Prints the size of the file or directory, in bytes\n";
	std::cout << "space                            : Prints the available space on the filesystem, in bytes\n";
	std::cout << "touch <files/dirs...>            : Update last modification time of files or directories, creating them if they don't exist\n";
	std::cout << "touch_nocreate <files/dirs...>   : Update last modification time of files or directories; do nothing if they don't exist\n";
	std::cout << "write <fileName> <content>       : Writes the <content> to the specified file. If the file already existed, it will be overwritten.\n";
}

[[nodiscard]] inline limes::files::Directory getCWD()
{
	return limes::files::Directory::getCurrentWorkingDirectory();
}

void pwd()
{
	std::cout << getCWD().getAbsolutePath().native() << std::endl;
}

void ls (const std::string& dir)
{
	const auto directory = [&dir]
	{
		const auto cwd = getCWD();

		if (dir.empty())
			return cwd;

		limes::files::Directory d { dir };

		d.makeAbsoluteRelativeTo (cwd.getAbsolutePath());

		return d;
	}();

	auto children = directory.getAllChildren (false);

	limes::alg::sort (children);

	for (const auto& child : children)
		std::cout << child.getName() << std::endl;
}

void rm(const std::vector<std::string>& items)
{
	if (items.empty())
	{
		std::cerr << "Error: No items selected to remove" << std::endl;
		std::exit (EXIT_FAILURE);
	}

	const auto cwd = getCWD().getAbsolutePath();

	for (const auto& item : items)
	{
		limes::files::FilesystemEntry entry { item };

		entry.makeAbsoluteRelativeTo (cwd);

		entry.deleteIfExists();
	}
}

void mkdir (const std::string& dirName)
{
	limes::files::Directory dir { dirName };

	dir.makeAbsoluteRelativeToCWD();

	dir.createIfDoesntExist();
}

void rename (const std::string& oldName, const std::string& newName)
{
	limes::files::FilesystemEntry oldEntry { oldName };
	limes::files::FilesystemEntry newEntry { newName };

	const auto cwd = getCWD().getAbsolutePath();

	oldEntry.makeAbsoluteRelativeTo (cwd);
	newEntry.makeAbsoluteRelativeTo (cwd);

	if (! oldEntry.exists())
	{
		std::cerr << "Error: original filesystem object " << oldEntry.getAbsolutePath() << " does not exist!" << std::endl;
		std::exit (EXIT_FAILURE);
	}

	oldEntry.rename (newEntry.getAbsolutePath());
}

void touch(const std::vector<std::string>& items, bool allowCreation)
{
	if (items.empty())
	{
		std::cerr << "Error: No items selected to touch" << std::endl;
		std::exit (EXIT_FAILURE);
	}

	const auto cwd = getCWD().getAbsolutePath();

	for (const auto& item : items)
	{
		limes::files::FilesystemEntry entry { item };

		entry.makeAbsoluteRelativeTo (cwd);

		if (allowCreation)
			entry.touch();
		else
			entry.touch_noCreate();
	}
}

void size (const std::string& name)
{
	limes::files::FilesystemEntry entry { name };

	entry.makeAbsoluteRelativeToCWD();

	if (! entry.exists())
	{
		std::cerr << "Error: filesystem object " << entry.getAbsolutePath() << " does not exist!" << std::endl;
		std::exit (EXIT_FAILURE);
	}

	std::cout << entry.sizeInBytes() << " bytes" << std::endl;
}

void cat (const std::vector<std::string>& items)
{
	if (items.empty())
	{
		std::cerr << "Error: No items specified in call to cat" << std::endl;
		std::exit (EXIT_FAILURE);
	}

	const auto cwd = getCWD().getAbsolutePath();

	std::string result;

	for (const auto& item : items)
	{
		limes::files::File file { item };

		file.makeAbsoluteRelativeTo (cwd);

		if (! file.isFile())
		{
			std::cerr << "Error: object is not a file: " << file.getAbsolutePath() << std::endl;
			std::exit (EXIT_FAILURE);
		}

		if (! file.exists())
		{
			std::cerr << "Error: file does not exist: " << file.getAbsolutePath() << std::endl;
			std::exit (EXIT_FAILURE);
		}

		const auto fileContent = file.loadAsString();

		if (fileContent.empty())
			continue;

		if (! result.empty())
			result += "\n";

		result += fileContent;
	}

	std::cout << result << std::endl;
}

void cd(const std::string& name)
{
	limes::files::Directory dir { name };

	dir.makeAbsoluteRelativeToCWD();

	if (! (dir.isDirectory() && dir.exists()))
	{
		std::cerr << "Error: directory does not exist: " << dir.getAbsolutePath() << std::endl;
		std::exit (EXIT_FAILURE);
	}

	dir.setAsWorkingDirectory();
}

void ln(const std::string& linkName, const std::string& targetName)
{
	limes::files::FilesystemEntry link { linkName };
	limes::files::FilesystemEntry target { targetName };

	const auto cwd = getCWD().getAbsolutePath();

	link.makeAbsoluteRelativeTo (cwd);
	target.makeAbsoluteRelativeTo (cwd);

	if (! target.exists())
	{
		std::cerr << "Error: symlink target " << target.getAbsolutePath() << " does not exist" << std::endl;
		std::exit (EXIT_FAILURE);
	}

	limes::files::SymLink::create (link.getAbsolutePath(), target.getAbsolutePath());
}

void follow_symlink(const std::string& linkName)
{
	limes::files::SymLink link { linkName };

	link.makeAbsoluteRelativeToCWD();

	if (! link.exists())
	{
		std::cerr << "Error: symbolic link does not exist: " << link.getAbsolutePath() << std::endl;
		std::exit (EXIT_FAILURE);
	}

	std::cout << link.follow().getAbsolutePath() << std::endl;
}

void write (const std::string& fileName, const std::string& content)
{
	limes::files::File file { fileName };

	file.makeAbsoluteRelativeToCWD();

	if (content.empty())
		file.deleteIfExists();
	else
		file.overwriteWithText (content);
}

void append (const std::string& fileName, const std::string& content)
{
	if (content.empty())
		return;

	limes::files::File file { fileName };

	file.makeAbsoluteRelativeToCWD();

	if (! (content.starts_with("\n") || content.starts_with("\r\n")))
	{
		const auto fileText = file.loadAsString();

		if (! (fileText.ends_with("\n") || fileText.ends_with("\r\n")))
			file.appendText ("\n");
	}

	file.appendText (content);
}

void copy (const std::vector<std::string>& items, const std::string& destName)
{
	const auto cwd = getCWD().getAbsolutePath();

	limes::files::FilesystemEntry dest { destName };

	dest.makeAbsoluteRelativeTo (cwd);

	if (items.size() == 1)
	{
		limes::files::FilesystemEntry item { items[0] };

		item.makeAbsoluteRelativeTo (cwd);

		if (! item.exists())
		{
			std::cerr << "Error: Input item does not exist: " << item.getAbsolutePath() << std::endl;
			std::exit (EXIT_FAILURE);
		}

		if (dest.isFile())
			item.copyTo (dest);
		else
			item.copyToDirectory (dest.getAbsolutePath());

		return;
	}

	const auto destPath = dest.getAbsolutePath();

	if (destPath.has_extension())
	{
		std::cerr << "Error: when specifying multiple inputs, the destination must be a directory" << std::endl;
		std::exit (EXIT_FAILURE);
	}

	for (const auto& item : items)
	{
		limes::files::FilesystemEntry entry { item };

		entry.makeAbsoluteRelativeTo (cwd);

		if (! entry.exists())
		{
			std::cerr << "Error: Input item does not exist: " << entry.getAbsolutePath() << std::endl;
			std::exit (EXIT_FAILURE);
		}

		entry.copyToDirectory (destPath);
	}
}

void modtime (const std::string& name)
{
	limes::files::FilesystemEntry item { name };

	item.makeAbsoluteRelativeToCWD();

	if (! item.exists())
	{
		std::cerr << "Error: item does not exist: " << item.getAbsolutePath() << std::endl;
		std::exit (EXIT_FAILURE);
	}

	const auto time = item.getLastModificationTime();

	constexpr const std::string_view months[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

	if (time.tm_hour < 10)
		std::cout << 0;

	std::cout << time.tm_hour << ":";

	if (time.tm_min < 10)
		std::cout << 0;

	std::cout << time.tm_min << ":";

	if (time.tm_sec < 10)
		std::cout << 0;

	std::cout << time.tm_sec << " " << time.tm_mday << " " << months[time.tm_mon] << " " << time.tm_year + 1900 << std::endl;
}

}


int main (int argc, char** argv)
{
	if (argc == 1)
	{
		fileutil::displayUsage();
		return EXIT_SUCCESS;
	}

	try
	{
		const auto mode = std::string{ argv[1] };

		auto getVectorOfInputs = [argc, argv]
		{
			std::vector<std::string> items;

			for (auto i = 2; i < argc; ++i)
				items.emplace_back (argv[i]);

			return items;
		};

		if(mode == "help" || mode == "--help" || mode == "-help" || mode == "-h")
		{
			fileutil::displayUsage();
			return EXIT_SUCCESS;
		}

		if (mode == "pwd")
		{
			fileutil::pwd();
			return EXIT_SUCCESS;
		}

		if(mode == "ls")
		{
			if (argc > 2)
				fileutil::ls(std::string{ argv[2] });
			else
				fileutil::ls(std::string{});

			return EXIT_SUCCESS;
		}

		if(mode == "rm")
		{
			fileutil::rm (getVectorOfInputs());
			return EXIT_SUCCESS;
		}

		if(mode == "mkdir")
		{
			if (argc < 2)
			{
				std::cerr << "Error: Directory name not specified in call to mkdir" << std::endl;
				return EXIT_FAILURE;
			}

			fileutil::mkdir (std::string{ argv[2] });
			return EXIT_SUCCESS;
		}

		if(mode == "rename")
		{
			if (argc < 3)
			{
				std::cerr << "Error: old name and new name must be specified in call to rename" << std::endl;
				return EXIT_FAILURE;
			}

			fileutil::rename (std::string{ argv[2] }, std::string{ argv[3] });
			return EXIT_SUCCESS;
		}

		if(mode == "touch" || mode == "touch_nocreate")
		{
			const auto allowCreation = mode == "touch";

			fileutil::touch (getVectorOfInputs(), allowCreation);
			return EXIT_SUCCESS;
		}

		if(mode == "size")
		{
			if (argc < 2)
			{
				std::cerr << "Error: Name not specified in call to size" << std::endl;
				return EXIT_FAILURE;
			}

			fileutil::size (std::string{ argv[2] });
			return EXIT_SUCCESS;
		}

		if (mode == "cat")
		{
			fileutil::cat (getVectorOfInputs());
			return EXIT_SUCCESS;
		}

		if (mode == "cd")
		{
			if (argc < 2)
			{
				std::cerr << "Error: Directory name not specified in call to cd" << std::endl;
				return EXIT_FAILURE;
			}

			fileutil::cd (std::string{ argv[2] });
			return EXIT_SUCCESS;
		}

		if (mode == "ln")
		{
			if (argc < 3)
			{
				std::cerr << "Error: link name and target must be specified in call to ln" << std::endl;
				return EXIT_FAILURE;
			}

			fileutil::ln (std::string{ argv[2] }, std::string{ argv[3] });
			return EXIT_SUCCESS;
		}

		if (mode == "follow_symlink")
		{
			if (argc < 2)
			{
				std::cerr << "Error: Symlink name not specified in call to follow_symlink" << std::endl;
				return EXIT_FAILURE;
			}

			fileutil::follow_symlink (std::string{ argv[2] });
			return EXIT_SUCCESS;
		}

		if (mode == "write")
		{
			if (argc < 3)
			{
				std::cerr << "Error: filename and content must be specified in call to write" << std::endl;
				return EXIT_FAILURE;
			}

			fileutil::write (std::string{ argv[2] }, std::string{ argv[3] });
			return EXIT_SUCCESS;
		}

		if (mode == "append")
		{
			if (argc < 3)
			{
				std::cerr << "Error: filename and content must be specified in call to append" << std::endl;
				return EXIT_FAILURE;
			}

			fileutil::append (std::string{ argv[2] }, std::string{ argv[3] });
			return EXIT_SUCCESS;
		}

		if (mode == "copy")
		{
			if (argc < 3)
			{
				std::cerr << "Error: at least one input and a destination must be specified in call to copy" << std::endl;
				return EXIT_FAILURE;
			}

			std::vector<std::string> items;

			for (auto i = 2; i < argc - 1; ++i)
				items.emplace_back (argv[i]);

			fileutil::copy (items, std::string{ argv[argc - 1] });
			return EXIT_SUCCESS;
		}

		if (mode == "space")
		{
			std::cout << limes::files::getAvailableSpaceOnFilesystem() << " bytes" << std::endl;
			return EXIT_SUCCESS;
		}

		if (mode == "modtime")
		{
			if (argc < 2)
			{
				std::cerr << "Error: Item name not specified in call to modtime" << std::endl;
				return EXIT_FAILURE;
			}

			fileutil::modtime (std::string{ argv[2] });
			return EXIT_SUCCESS;
		}

		std::cerr << "Error: Unknown mode requested: " << mode << std::endl;
		std::cout << "Run with no arguments or with 'help' to view usage." << std::endl;
		return EXIT_FAILURE;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
