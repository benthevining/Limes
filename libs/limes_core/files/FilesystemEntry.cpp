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

#include "FilesystemEntry.h"
#include <limes_namespace.h>
#include <string>
#include <filesystem>
#include <fstream>
#include "directory.h"
#include "file.h"
#include "sym_link.h"
#include "../misc/Functions.h"

LIMES_BEGIN_NAMESPACE

namespace files
{

FilesystemEntry::FilesystemEntry (const Path& pathToUse, bool createIfNeeded)
	: path (pathToUse)
{
	if (createIfNeeded)
		createIfDoesntExist();
}

FilesystemEntry& FilesystemEntry::operator= (const Path& newPath)
{
	path = newPath;
	return *this;
}

FilesystemEntry FilesystemEntry::operator/ (const std::string& subpathName) const
{
	FilesystemEntry newEntry { *this };

	newEntry.path.append (subpathName);

	return newEntry;
}

FilesystemEntry& FilesystemEntry::operator/= (const std::string& subpathName)
{
	path.append (subpathName);
	return *this;
}

bool FilesystemEntry::operator== (const FilesystemEntry& other) const noexcept
{
	return getAbsolutePath() == other.getAbsolutePath();
}

bool FilesystemEntry::operator!= (const FilesystemEntry& other) const noexcept
{
	return ! (*this == other);
}

Path FilesystemEntry::getPath() const noexcept
{
	return path;
}

Path FilesystemEntry::getAbsolutePath() const noexcept
{
	return std::filesystem::absolute (path);
}

Directory FilesystemEntry::getDirectory() const
{
	if (isDirectory())
		return Directory { getAbsolutePath() };

	return Directory { getAbsolutePath().parent_path() };
}

Directory FilesystemEntry::getParentDirectory() const
{
	return Directory { getDirectory().getPath().parent_path() };
}

bool FilesystemEntry::isValid() const noexcept
{
	return ! path.empty();
}

bool FilesystemEntry::exists() const noexcept
{
	if (! isValid())
		return false;

	return std::filesystem::exists (path);
}

bool FilesystemEntry::isFile() const noexcept
{
	return ! (isDirectory() || isSymLink());
}

bool FilesystemEntry::isDirectory() const noexcept
{
	return std::filesystem::is_directory (path);
}

bool FilesystemEntry::isSymLink() const noexcept
{
	return std::filesystem::is_symlink (path);
}

std::unique_ptr<File> FilesystemEntry::getFileObject() const
{
	if (! isValid())
		return nullptr;

	if (! isFile())
		return nullptr;

	return std::make_unique<File> (path);
}

std::unique_ptr<Directory> FilesystemEntry::getDirectoryObject() const
{
	if (! isValid())
		return nullptr;

	if (! isDirectory())
		return nullptr;

	return std::make_unique<Directory> (path);
}

std::unique_ptr<SymLink> FilesystemEntry::getSymLinkObject() const
{
	if (! isValid())
		return nullptr;

	if (! isSymLink())
		return nullptr;

	return std::make_unique<SymLink> (path);
}

bool FilesystemEntry::isAbsolutePath() const noexcept
{
	return path.is_absolute();
}

bool FilesystemEntry::isRelativePath() const noexcept
{
	return path.is_relative();
}

bool FilesystemEntry::createIfDoesntExist() const
{
	if (! isValid())
		return false;

	if (isSymLink())
	{
		const auto abs_path = getAbsolutePath();

		SymLink::create (abs_path, std::filesystem::read_symlink (abs_path));

		return exists();
	}

	if (exists())
		return false;

	if (isDirectory())
		return std::filesystem::create_directories (path);

	std::ofstream output (getAbsolutePath());

	return exists();
}

bool FilesystemEntry::deleteIfExists() const
{
	if (! exists() || ! isValid())
		return false;

	const auto filesRemoved = std::filesystem::remove_all (path);

	return filesRemoved > 0;
}

void FilesystemEntry::touch() const
{
	std::ofstream output (getAbsolutePath());
}

std::uintmax_t FilesystemEntry::sizeInBytes() const
{
	if (! exists())
		return 0;

	return std::filesystem::file_size (path);
}

bool FilesystemEntry::setPermissions (Permissions permissions, PermOptions options) const noexcept
{
	return try_call ([p = getAbsolutePath(), permissions, options]
					 { std::filesystem::permissions (p, permissions, options); });
}

FilesystemEntry::Permissions FilesystemEntry::getPermissions() const
{
	return std::filesystem::status (getAbsolutePath()).permissions();
}

bool FilesystemEntry::isHidden() const
{
	return getAbsolutePath().stem().string().starts_with ('.');
}

FilesystemEntry::TimeType FilesystemEntry::getLastModificationTime() const noexcept
{
	return std::filesystem::last_write_time (path);
}

bool FilesystemEntry::rename (const Path& newPath) noexcept
{
	return try_call ([this, newPath]
					 { std::filesystem::rename (path, newPath);
		path = newPath; });
}

bool FilesystemEntry::rename (const std::string& newFilenameWithinDirectory)
{
	return rename (getAbsolutePath().replace_filename (newFilenameWithinDirectory));
}

bool FilesystemEntry::copyTo (const Path& dest, CopyOptions options) const noexcept
{
	return try_call ([p = getAbsolutePath(), dest, options]
					 { std::filesystem::copy (p, dest, options); });
}

bool FilesystemEntry::copyTo (const FilesystemEntry& dest, CopyOptions options) const noexcept
{
	return copyTo (dest.getAbsolutePath(), options);
}

bool FilesystemEntry::copyFrom (const Path& source, CopyOptions options) const noexcept
{
	return try_call ([p = getAbsolutePath(), source, options]
					 { std::filesystem::copy (source, p, options); });
}

bool FilesystemEntry::copyFrom (const FilesystemEntry& source, CopyOptions options) const noexcept
{
	return copyFrom (source.getAbsolutePath(), options);
}

}  // namespace files

LIMES_END_NAMESPACE
