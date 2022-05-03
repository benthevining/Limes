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
#include <limes_core.h>

namespace fileutil
{

[[nodiscard]] inline limes::files::Directory getCWD()
{
	return limes::files::Directory::getCurrentWorkingDirectory();
}

void append (const std::string& fileName, const std::string& content)
{
	if (content.empty())
		return;

	limes::files::File file { fileName };

	file.makeAbsoluteRelativeToCWD();

	if (file.exists())
	{
		if (! (content.starts_with ("\n") || content.starts_with ("\r\n")))
		{
			const auto fileText = file.loadAsString();

			if (! (fileText.ends_with ("\n") || fileText.ends_with ("\r\n")))
				file.appendText ("\n");
		}
	}

	file.appendText (content);
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

void exists (const std::vector<std::string>& items)
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

void pwd()
{
	std::cout << getCWD().getAbsolutePath().string() << std::endl;
}

void relative (const std::string& path, const std::string& basePath)
{
	const auto cwd = getCWD().getAbsolutePath();

	limes::files::FilesystemEntry target { path };
	limes::files::FilesystemEntry base { basePath };

	target.makeAbsoluteRelativeTo (cwd);
	base.makeAbsoluteRelativeTo (cwd);

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

void write (const std::string& fileName, const std::string& content)
{
	limes::files::File file { fileName };

	file.makeAbsoluteRelativeToCWD();

	file.overwriteWithText (content);
}

}  // namespace fileutil
