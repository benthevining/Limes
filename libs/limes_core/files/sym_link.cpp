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

#include "sym_link.h"
#include <exception>

LIMES_BEGIN_NAMESPACE

namespace files
{

SymLink::SymLink (const Path& symLinkPath, const FilesystemEntry& linkTarget)
	: FilesystemEntry (symLinkPath)
{
	create (getAbsolutePath(), linkTarget);
}

bool SymLink::create (const Path& linkPath, const FilesystemEntry& target)
{
	if (! target.exists())
		return false;

	try
	{
		const auto targetPath = target.getAbsolutePath();

		if (target.isDirectory())
			std::filesystem::create_directory_symlink (targetPath, linkPath);
		else
			std::filesystem::create_symlink (targetPath, linkPath);
	}
	catch (const std::exception&)
	{
		return false;
	}

	return true;
}

FilesystemEntry SymLink::follow (bool recurse) const
{
	try
	{
		FilesystemEntry target { std::filesystem::read_symlink (getAbsolutePath()) };

		if (! recurse)
			return target;

		if (! target.isSymLink())
			return target;

		return target.getSymLinkObject()->follow();
	}
	catch (std::exception&)
	{
		return {};
	}
}

bool SymLink::references (const FilesystemEntry& entry) const
{
	if (follow (false) == entry)
		return true;

	return follow (true) == entry;
}

bool SymLink::referencesSameLocationAs (const SymLink& other) const
{
	return follow() == other.follow();
}

}  // namespace files

LIMES_END_NAMESPACE
