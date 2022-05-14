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
#include <exception>			// for exception
#include <filesystem>			// for create_directory_symlink, create_...
#include "../misc/Functions.h"	// for try_call
#include "FilesystemEntry.h"	// for FilesystemEntry, Path
#include <limes_namespace.h>	// for LIMES_BEGIN_NAMESPACE, LIMES_END_...

LIMES_BEGIN_NAMESPACE

namespace files
{

SymLink::SymLink (const Path& symLinkPath, const FilesystemEntry& linkTarget)
	: FilesystemEntry (symLinkPath)
{
	create (getAbsolutePath(), linkTarget);
}

bool SymLink::create (const Path& linkPath, const FilesystemEntry& target) noexcept
{
	if (! target.exists())
		return false;

	return try_call ([&target, &linkPath]
					 {
			const auto targetPath = target.getAbsolutePath();

			if (target.isDirectory())
				std::filesystem::create_directory_symlink (targetPath, linkPath);
			else
				std::filesystem::create_symlink (targetPath, linkPath); });
}

bool SymLink::create (const Path& linkPath, const Path& target) noexcept
{
	FilesystemEntry targetEntry { target };

	return create (linkPath, targetEntry);
}

FilesystemEntry SymLink::follow (bool recurse) const noexcept
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
	if (follow (false) == other.follow (false))
		return true;

	return follow (true) == other.follow (true);
}

}  // namespace files

LIMES_END_NAMESPACE
