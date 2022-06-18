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
#include "../math/mathFunctions.h"
#include "directory.h"

LIMES_BEGIN_NAMESPACE

namespace files
{

SymLink::SymLink (const Path& symLinkPath, const FilesystemEntry& linkTarget)
	: FilesystemEntry (symLinkPath)
{
	create (getAbsolutePath(), linkTarget);
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

bool SymLink::create (const Path& linkPath, const FilesystemEntry& target) noexcept
{
	if (! target.exists())
		return false;

	return func::try_call ([&target, &linkPath]
						   {
			const auto targetPath = target.getAbsolutePath();

			if (target.isDirectory())
				std::filesystem::create_directory_symlink (targetPath, linkPath);
			else
				std::filesystem::create_symlink (targetPath, linkPath); });
}

bool SymLink::create (const Path& linkPath, const Path& target) noexcept
{
	const FilesystemEntry targetEntry { target };

	return create (linkPath, targetEntry);
}

bool SymLink::create (const Directory& newDirectory, const FilesystemEntry& target) noexcept
{
	return create (newDirectory.getAbsolutePath() / target.getAbsolutePath().filename().string(),
				   target);
}

bool SymLink::create (const Directory& newDirectory, const Path& target) noexcept
{
	const FilesystemEntry targetEntry { target };

	return create (newDirectory, targetEntry);
}

}  // namespace files

LIMES_END_NAMESPACE

namespace std
{

size_t hash<limes::files::SymLink>::operator() (const limes::files::SymLink& l) const noexcept
{
	const auto linkHash	  = filesystem::hash_value (l.getAbsolutePath());
	const auto targetHash = filesystem::hash_value (l.follow (true));

	return limes::math::szudzikPair (linkHash, targetHash);
}

}  // namespace std
