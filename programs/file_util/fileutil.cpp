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

#include "fileutil.h"

#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <string_view>
#include <filesystem>
#include <regex>
#include <limes_core.h>

namespace fileutil
{

[[nodiscard]] inline limes::files::Directory getCWD()
{
	return limes::files::Directory::getCurrentWorkingDirectory();
}

void absolute (const std::string& path, const std::string& basePath)
{
	limes::files::FilesystemEntry target { path };

	if (target.isAbsolutePath())
	{
		std::cout << target.getAbsolutePath() << std::endl;
		return;
	}

	const auto base = [&basePath]
	{
		if (basePath.empty())
			return getCWD();

		limes::files::Directory dir { basePath };

		if (! dir.isAbsolutePath())
		{
			std::cerr << "Error: base path must be absolute: " << dir.getPath() << std::endl;
			std::exit (EXIT_FAILURE);
		}

		return dir;
	}();

	target.makeAbsoluteRelativeTo (base.getAbsolutePath());

	std::cout << target.getAbsolutePath() << std::endl;
}

void append (const std::string& fileName, std::string content, bool strict)
{
	if (content.empty())
		return;

	limes::files::File file { fileName };

	file.makeAbsoluteRelativeToCWD();

	if (file.exists())
	{
		if (! (content.starts_with ('\n') || content.starts_with ("\r\n")))
		{
			const auto fileText = file.loadAsString();

			if (! (fileText.ends_with ('\n') || fileText.ends_with ("\r\n")))
				content.insert (0, 1, '\n');
		}
	}
	else
	{
		if (strict)
		{
			std::cerr << "Error: cannot append to file that does not exist: " << file.getAbsolutePath() << std::endl;
			std::exit (EXIT_FAILURE);
		}
	}

	file.appendText (content);
}

void cat (const std::vector<std::string>& items, const std::string& output)
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
			result += '\n';

		result += fileContent;
	}

	if (output.empty())
	{
		std::cout << result << std::endl;
		return;
	}

	limes::files::File outFile { output };

	outFile.makeAbsoluteRelativeToCWD();

	outFile.overwriteWithText (result);
}

void cd (const std::string& name)
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

void equivalent (const std::string& path1, const std::string& path2, bool error)
{
	const auto cwd = getCWD().getAbsolutePath();

	limes::files::FilesystemEntry entry1 { path1 };
	limes::files::FilesystemEntry entry2 { path2 };

	entry1.makeAbsoluteRelativeTo (cwd);
	entry2.makeAbsoluteRelativeTo (cwd);

	const auto result = entry1.getAbsolutePath() == entry2.getAbsolutePath();

	if (error)
	{
		if (result)
			std::exit (0);

		std::exit (1);
	}

	if (result)
		std::cout << "yes" << std::endl;
	else
		std::cout << "no" << std::endl;
}

void exists (const std::vector<std::string>& items, bool error)
{
	if (items.empty())
	{
		std::cerr << "Error: no items passed to call to exists" << std::endl;
		std::exit (EXIT_FAILURE);
	}

	const auto exists = [&items]
	{
		const auto cwd = getCWD().getAbsolutePath();

		for (const auto& item : items)
		{
			limes::files::FilesystemEntry entry { item };

			entry.makeAbsoluteRelativeTo (cwd);

			if (! entry.exists())
				return false;
		}

		return true;
	}();

	if (error)
	{
		if (exists)
			std::exit (0);

		std::exit (1);
	}

	if (exists)
		std::cout << "yes" << std::endl;
	else
		std::cout << "no" << std::endl;
}

void follow_symlink (const std::string& linkName, bool recurse)
{
	limes::files::SymLink link { linkName };

	link.makeAbsoluteRelativeToCWD();

	if (! link.exists())
	{
		std::cerr << "Error: symbolic link does not exist: " << link.getAbsolutePath() << std::endl;
		std::exit (EXIT_FAILURE);
	}

	std::cout << link.follow (recurse).getAbsolutePath() << std::endl;
}

void glob (const std::string& expr, const std::string& dir, bool recurse, bool error)
{
	const auto baseDir = [&dir]
	{
		if (dir.empty())
			return getCWD();

		limes::files::Directory base { dir };

		base.makeAbsoluteRelativeToCWD();

		if (! base.exists())
		{
			std::cerr << "Error: base directory does not exist: " << base.getAbsolutePath() << std::endl;
			std::exit (EXIT_FAILURE);
		}

		return base;
	}();

	std::regex regex { expr, std::regex_constants::ECMAScript | std::regex_constants::icase };

	bool atLeastOne = false;

	for (const auto& child : baseDir.getAllChildren (recurse))
	{
		if (std::regex_search (child.getName(), regex))
		{
			atLeastOne = true;
			std::cout << child.getAbsolutePath() << std::endl;
		}
	}

	if (! atLeastOne)
	{
		if (error)
			std::exit (EXIT_FAILURE);

		std::cout << "No files found" << std::endl;
	}
}

void hash (const std::string& hashType, const std::string& filename)
{
	limes::files::File file { filename };

	file.makeAbsoluteRelativeToCWD();

	if (! file.exists())
	{
		std::cerr << "Error: file does not exist: " << file.getAbsolutePath() << std::endl;
		std::exit (EXIT_FAILURE);
	}

	const auto hash_type = [&hashType]
	{
		if (hashType == "md5")
			return limes::hash::Type::md5;

		if (hashType == "sha1")
			return limes::hash::Type::sha1;

		if (hashType == "sha224")
			return limes::hash::Type::sha224;

		if (hashType == "sha256")
			return limes::hash::Type::sha256;

		if (hashType == "sha384")
			return limes::hash::Type::sha384;

		if (hashType == "sha512")
			return limes::hash::Type::sha512;

		std::cerr << "Error: unknown hash type requested: " << hashType << std::endl;
		std::exit (EXIT_FAILURE);

		return limes::hash::Type::md5;
	}();

	std::cout << file.hash (hash_type) << std::endl;
}

void ln (const std::string& linkName, const std::string& targetName)
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

void ls (const std::string& dir)
{
	const auto directory = [&dir]
	{
		const auto cwd = getCWD();

		if (dir.empty())
			return cwd;

		limes::files::Directory d { dir };

		d.makeAbsoluteRelativeTo (cwd.getAbsolutePath());

		if (! d.exists())
		{
			std::cerr << "Error: directory does not exist: " << d.getAbsolutePath() << std::endl;
			std::exit (EXIT_FAILURE);
		}

		return d;
	}();

	auto children = directory.getAllChildren (false);

	limes::alg::sort (children);

	limes::TextTable table;

	int column = 0;

	for (const auto& child : children)
	{
		table.addColumnToCurrentRow (child.getName());

		++column;

		if (column > 2)
			table.startNewRow();

		column %= 3;
	}

	std::cout << table.toString ({}, " ") << std::endl;
}

void mkdir (const std::string& dirName)
{
	limes::files::Directory dir { dirName };

	dir.makeAbsoluteRelativeToCWD();

	dir.createIfDoesntExist();
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

	std::cout << time.toString() << std::endl;
}

void native (const std::string& name)
{
	std::filesystem::path location { name };

	location.make_preferred();

	std::cout << location.string() << std::endl;
}

void path()
{
	for (const auto& pathDir : limes::files::Directory::getPATH())
		std::cout << pathDir.getAbsolutePath() << std::endl;
}

void prepend (const std::string& fileName, std::string content, bool strict)
{
	if (content.empty())
		return;

	limes::files::File file { fileName };

	file.makeAbsoluteRelativeToCWD();

	if (file.exists())
	{
		if (! (content.ends_with ('\n') || content.ends_with ("\r\n")))
		{
			const auto fileText = file.loadAsString();

			if (! (fileText.starts_with ('\n') || fileText.starts_with ("\r\n")))
				content += '\n';
		}
	}
	else
	{
		if (strict)
		{
			std::cerr << "Error: cannot prepend to file that does not exist: " << file.getAbsolutePath() << std::endl;
			std::exit (EXIT_FAILURE);
		}
	}

	file.prependText (content);
}

void pwd()
{
	std::cout << getCWD().getAbsolutePath().string() << std::endl;
}

void relative (const std::string& path, const std::string& basePath)
{
	const auto cwd = getCWD().getAbsolutePath();

	const auto base = [&basePath, &cwd]
	{
		if (basePath.empty())
			return getCWD();

		limes::files::Directory dir { basePath };

		dir.makeAbsoluteRelativeTo (cwd);

		return dir;
	}();

	limes::files::FilesystemEntry target { path };

	target.makeAbsoluteRelativeTo (cwd);

	std::cout << std::filesystem::relative (target.getAbsolutePath(), base.getAbsolutePath()) << std::endl;
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

void rm (const std::vector<std::string>& items)
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

void size (const std::string& name)
{
	if (name.empty())
	{
		std::cout << std::filesystem::space (getCWD().getAbsolutePath()).capacity << " bytes" << std::endl;
		return;
	}

	limes::files::FilesystemEntry entry { name };

	entry.makeAbsoluteRelativeToCWD();

	if (! entry.exists())
	{
		std::cerr << "Error: filesystem object " << entry.getAbsolutePath() << " does not exist!" << std::endl;
		std::exit (EXIT_FAILURE);
	}

	std::cout << entry.sizeInBytes() << " bytes" << std::endl;
}

void space()
{
	std::cout << limes::files::getAvailableSpaceOnFilesystem() << " bytes" << std::endl;
}

void touch (const std::vector<std::string>& items, bool allowCreation)
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

void type (const std::string& name)
{
	limes::files::FilesystemEntry entry { name };

	entry.makeAbsoluteRelativeToCWD();

	if (! entry.exists())
	{
		std::cerr << "Error: item does not exist: " << entry.getAbsolutePath() << std::endl;
		std::exit (EXIT_FAILURE);
	}

	const auto typeString = [&entry]
	{
		if (entry.isSymLink())
			return "symlink";

		if (entry.isDirectory())
			return "directory";

		return "file";
	}();

	std::cout << typeString << std::endl;
}

void which (const std::string& programName)
{
	for (const auto& pathDir : limes::files::Directory::getPATH())
	{
		const auto child = pathDir.getChild (programName);

		if (child.exists())
		{
			std::cout << child.getAbsolutePath() << std::endl;
			return;
		}
	}

	std::cerr << programName << " not found" << std::endl;
	std::exit (EXIT_FAILURE);
}

void write (const std::string& fileName, const std::string& content, bool allowOverwrite)
{
	limes::files::File file { fileName };

	file.makeAbsoluteRelativeToCWD();

	if (! allowOverwrite && file.exists())
	{
		std::cerr << "Error: file already exists: " << file.getAbsolutePath() << std::endl;
		std::exit (EXIT_FAILURE);
	}

	file.overwriteWithText (content);
}

}  // namespace fileutil
