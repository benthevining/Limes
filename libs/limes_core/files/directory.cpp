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

#include "directory.h"
#include <limes_namespace.h>  // for LIMES_BEGIN_NAMESPACE, LIMES_END_...
#include <cstdlib>			  // for getenv, size_t
#include <filesystem>		  // for begin, end, operator!=, operator/
#include <type_traits>		  // for conditional_t
#include <vector>			  // for vector
#include <string>			  // for string
#include "FilesystemEntry.h"  // for FilesystemEntry, Path
#include "file.h"			  // for File
#include "sym_link.h"		  // for SymLink
#include "misc.h"			  // for PATHseparator


LIMES_BEGIN_NAMESPACE

namespace files
{

Directory& Directory::operator= (const Path& newPath)
{
	assignPath (newPath);
	return *this;
}

Directory& Directory::operator= (const std::string_view& newPath)
{
	assignPath (newPath);
	return *this;
}

FilesystemEntry Directory::operator/ (const std::string_view& subpathName) const
{
	return FilesystemEntry { getAbsolutePath() / subpathName };
}

FilesystemEntry& Directory::operator/= (const std::string_view& subpathName)
{
	assignPath (getAbsolutePath() / subpathName);
	return *this;
}

bool Directory::contains (const FilesystemEntry& entry) const
{
	return entry.getDirectory().getAbsolutePath() == getAbsolutePath();
}

bool Directory::contains (const std::string_view& childName) const
{
	for (const auto& entry : getAllChildren())
		if (entry.getName() == childName)
			return true;

	return false;
}

bool Directory::createIfDoesntExist() const
{
	if (! isValid())
		return false;

	if (exists())
		return false;

	return std::filesystem::create_directories (getAbsolutePath());
}

Path Directory::getRelativePath (const Path& inputPath) const
{
	return std::filesystem::relative (inputPath, getAbsolutePath());
}

bool Directory::isEmpty() const
{
	return getAllChildren().empty();
}

FilesystemEntry Directory::getChild (const std::string_view& childName, bool createIfNeeded) const
{
	return FilesystemEntry { getAbsolutePath() / childName, createIfNeeded };
}

File Directory::getChildFile (const std::string_view& filename, bool createIfNeeded) const
{
	return File { getAbsolutePath() / filename, createIfNeeded };
}

Directory Directory::getChildDirectory (const std::string_view& subdirectoryName, bool createIfNeeded) const
{
	return Directory { getAbsolutePath() / subdirectoryName, createIfNeeded };
}

SymLink Directory::createChildSymLink (const std::string_view& symLinkName,
									   const FilesystemEntry&  symLinkTarget) const
{
	return SymLink { getAbsolutePath() / symLinkName, symLinkTarget };
}

template <bool Recursive>
using IteratorType = std::conditional_t<Recursive,
										std::filesystem::recursive_directory_iterator,
										std::filesystem::directory_iterator>;

template <bool Recursive>
[[nodiscard]] inline std::vector<FilesystemEntry> getAllDirChildren (const Path& path)
{
	std::vector<FilesystemEntry> entries;

	for (const auto& entry : IteratorType<Recursive> { path })
		entries.emplace_back (entry.path());  // cppcheck-suppress useStlAlgorithm

	return entries;
}

template <bool Recursive>
[[nodiscard]] inline std::vector<Directory> getDirChildDirectories (const Path& path)
{
	std::vector<Directory> entries;

	for (const auto& entry : IteratorType<Recursive> { path })
		if (entry.is_directory())
			entries.emplace_back (entry.path());

	return entries;
}

template <bool Recursive>
[[nodiscard]] inline std::vector<File> getDirChildFiles (const Path& path)
{
	std::vector<File> entries;

	for (const auto& entry : IteratorType<Recursive> { path })
		if (! (entry.is_directory() || entry.is_symlink()))
			entries.emplace_back (entry.path());

	return entries;
}

template <bool Recursive>
[[nodiscard]] inline std::vector<SymLink> getDirChildSymLinks (const Path& path)
{
	std::vector<SymLink> entries;

	for (const auto& entry : IteratorType<Recursive> { path })
		if (entry.is_symlink())
			entries.emplace_back (entry.path());

	return entries;
}

std::vector<File> Directory::getChildFiles (bool recurse) const
{
	if (recurse)
		return getDirChildFiles<true> (getAbsolutePath());

	return getDirChildFiles<false> (getAbsolutePath());
}

void Directory::iterateFiles (FileCallback&& callback, bool recurse) const
{
	for (const auto& file : getChildFiles (recurse))
		callback (file);
}

std::vector<Directory> Directory::getChildDirectories (bool recurse) const
{
	if (recurse)
		return getDirChildDirectories<true> (getAbsolutePath());

	return getDirChildDirectories<false> (getAbsolutePath());
}

void Directory::iterateDirectories (DirectoryCallback&& callback, bool recurse) const
{
	for (const auto& dir : getChildDirectories (recurse))
		callback (dir);
}

std::vector<SymLink> Directory::getChildSymLinks (bool recurse) const
{
	if (recurse)
		return getDirChildSymLinks<true> (getAbsolutePath());

	return getDirChildSymLinks<false> (getAbsolutePath());
}

void Directory::iterateSymLinks (SymLinkCallback&& callback, bool recurse) const
{
	for (const auto& link : getChildSymLinks (recurse))
		callback (link);
}

std::vector<FilesystemEntry> Directory::getAllChildren (bool recurse) const
{
	if (recurse)
		return getAllDirChildren<true> (getAbsolutePath());

	return getAllDirChildren<false> (getAbsolutePath());
}

void Directory::iterateAllChildren (FileCallback&&		fileCallback,
									DirectoryCallback&& directoryCallback,
									SymLinkCallback&&	symLinkCallback,
									bool				recurse) const
{
	for (const auto& entry : getAllChildren (recurse))
	{
		if (fileCallback != nullptr)
		{
			const auto file = entry.getFileObject();

			if (auto* filePtr = file.get())
			{
				fileCallback (*filePtr);
				continue;
			}
		}

		if (directoryCallback != nullptr)
		{
			const auto dir = entry.getDirectoryObject();

			if (auto* dirPtr = dir.get())
			{
				directoryCallback (*dirPtr);
				continue;
			}
		}

		if (symLinkCallback != nullptr)
		{
			const auto link = entry.getSymLinkObject();

			if (auto* linkPtr = link.get())
				symLinkCallback (*linkPtr);
		}
	}
}

std::uintmax_t Directory::sizeInBytes() const
{
	if (! exists())
		return 0;

	std::uintmax_t result = 0;

	for (const auto& child : getAllChildren())
		result += child.sizeInBytes();	// cppcheck-suppress useStlAlgorithm

	return result;
}

Directory Directory::getCurrentWorkingDirectory()
{
	return Directory { std::filesystem::current_path() };
}

void Directory::setCurrentWorkingDirectory (const Path& path)
{
	std::filesystem::current_path (std::filesystem::absolute (path));
}

void Directory::setCurrentWorkingDirectory (const Directory& directory)
{
	setCurrentWorkingDirectory (directory.getPath());
}

Directory Directory::getTempFileDirectory()
{
	return Directory { std::filesystem::temp_directory_path() };
}

void Directory::setAsWorkingDirectory() const
{
	setCurrentWorkingDirectory (*this);
}

std::vector<Directory> Directory::getPATH()
{
	std::vector<Directory> dirs;

	if (auto* path = std::getenv ("PATH"))
	{
		std::string fullPath { path };

		for (size_t pos = 0; pos != std::string::npos; pos = fullPath.find (PATHseparator()))
		{
			dirs.emplace_back (fullPath.substr (0, pos));
			fullPath.erase (0, pos + 1);
		}
	}

	for (auto& dir : dirs)
		dir.makeAbsoluteRelativeToCWD();

	return dirs;
}

}  // namespace files

LIMES_END_NAMESPACE
