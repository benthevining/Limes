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
#include <limes_namespace.h>	 // for LIMES_BEGIN_NAMESPACE, LIMES_END_NAME...
#include <ctime>				 // for tm
#include <exception>			 // for exception
#include <filesystem>			 // for path, copy, operator/, absolute, crea...
#include <fstream>				 // for string, ofstream
#include <string>				 // for operator<, operator>
#include "../misc/Functions.h"	 // for try_call
#include "../time/time_utils.h"	 // for toTimeObj
#include "directory.h"			 // for Directory
#include "file.h"				 // for File
#include "sym_link.h"			 // for SymLink


LIMES_BEGIN_NAMESPACE

namespace files
{

FilesystemEntry::FilesystemEntry (const Path& pathToUse)
	: path (pathToUse)
{
}

FilesystemEntry& FilesystemEntry::operator= (const Path& newPath)
{
	path = newPath;
	return *this;
}

FilesystemEntry& FilesystemEntry::operator= (const std::string_view& newPath)
{
	path = newPath;
	return *this;
}

FilesystemEntry::operator Path() const noexcept
{
	return getAbsolutePath();
}

FilesystemEntry& FilesystemEntry::assignPath (const Path& newPath) noexcept
{
	path = newPath;
	return *this;
}

FilesystemEntry FilesystemEntry::operator/ (const std::string_view& subpathName) const
{
	return FilesystemEntry { getAbsolutePath() / subpathName };
}

FilesystemEntry& FilesystemEntry::operator/= (const std::string_view& subpathName)
{
	assignPath (getAbsolutePath() / subpathName);
	return *this;
}

FilesystemEntry& FilesystemEntry::changeName (const std::string_view& newName)
{
	path.replace_filename (newName);
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

bool FilesystemEntry::operator== (const Path& other) const noexcept
{
	return getAbsolutePath() == std::filesystem::absolute (other);
}

bool FilesystemEntry::operator!= (const Path& other) const noexcept
{
	return ! (*this == other);
}

bool FilesystemEntry::operator< (const FilesystemEntry& other) const noexcept
{
	return getAbsolutePath() < other.getAbsolutePath();
}

bool FilesystemEntry::operator> (const FilesystemEntry& other) const noexcept
{
	return getAbsolutePath() > other.getAbsolutePath();
}

bool FilesystemEntry::operator< (const Path& other) const noexcept
{
	return getAbsolutePath() > std::filesystem::absolute (other);
}

bool FilesystemEntry::operator> (const Path& other) const noexcept
{
	return getAbsolutePath() < std::filesystem::absolute (other);
}

Path FilesystemEntry::getPath (bool makePreferred) const noexcept
{
	if (! makePreferred)
		return path;

	Path pathCopy { path };

	return pathCopy.make_preferred();
}

Path FilesystemEntry::getAbsolutePath (bool makePreferred) const noexcept
{
	auto abs = std::filesystem::absolute (path);

	if (! makePreferred)
		return abs;

	return abs.make_preferred();
}

std::string FilesystemEntry::getName() const noexcept
{
	return path.filename().string();
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

bool FilesystemEntry::makeAbsoluteRelativeTo (const Path& basePath) noexcept
{
	if (isAbsolutePath())
		return false;

	if (! basePath.is_absolute())
		return false;

	path = basePath / path;

	return true;
}

bool FilesystemEntry::makeAbsoluteRelativeToCWD() noexcept
{
	return makeAbsoluteRelativeTo (Directory::getCurrentWorkingDirectory().getAbsolutePath());
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
	if (! createIfDoesntExist())
		std::ofstream output (getAbsolutePath());
}

bool FilesystemEntry::touch_noCreate() const
{
	if (! exists())
		return false;

	touch();
	return true;
}

std::uintmax_t FilesystemEntry::sizeInBytes() const
{
	if (! exists())
		return 0;

	if (isDirectory())
		return getDirectoryObject()->sizeInBytes();

	return std::filesystem::file_size (path);
}

bool FilesystemEntry::setPermissions (Permissions permissions, PermOptions options) const noexcept
{
	return func::try_call ([p = getAbsolutePath(), permissions, options]
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

time::DateTime FilesystemEntry::getLastModificationTime() const noexcept
{
	return time::DateTime { time::toTimeObj (std::filesystem::last_write_time (getAbsolutePath())) };
}

bool FilesystemEntry::rename (const Path& newPath) noexcept
{
	return func::try_call ([this, newPath]
						   { std::filesystem::rename (path, newPath);
		path = newPath; });
}

bool FilesystemEntry::rename (const std::string& newFilenameWithinDirectory)
{
	return rename (getAbsolutePath().replace_filename (newFilenameWithinDirectory));
}

bool FilesystemEntry::copyTo (const Path& dest, CopyOptions options) const noexcept
{
	return func::try_call ([p = getAbsolutePath(), dest, options]
						   { std::filesystem::copy (p, dest, options); });
}

bool FilesystemEntry::copyTo (const FilesystemEntry& dest, CopyOptions options) const noexcept
{
	return copyTo (dest.getAbsolutePath(), options);
}

bool FilesystemEntry::copyToDirectory (const Path& destDirectory, CopyOptions options) const noexcept
{
	const Directory dir { destDirectory };

	if (dir == getDirectory())
		return false;

	dir.createIfDoesntExist();

	return copyTo (dir.getAbsolutePath() / getName(), options);
}

bool FilesystemEntry::copyFrom (const Path& source, CopyOptions options) const noexcept
{
	return func::try_call ([p = getAbsolutePath(), source, options]
						   { std::filesystem::copy (source, p, options); });
}

bool FilesystemEntry::copyFrom (const FilesystemEntry& source, CopyOptions options) const noexcept
{
	return copyFrom (source.getAbsolutePath(), options);
}

}  // namespace files

LIMES_END_NAMESPACE

namespace std
{
size_t hash<limes::files::FilesystemEntry>::operator() (const limes::files::FilesystemEntry& e) const noexcept
{
	return filesystem::hash_value (e.getAbsolutePath());
}
}  // namespace std
