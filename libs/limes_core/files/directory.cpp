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
#include <limes_namespace.h>
#include <filesystem>
#include <type_traits>

LIMES_BEGIN_NAMESPACE

namespace files
{

Path Directory::getRelativePath (const Path& inputPath) const
{
	return std::filesystem::relative (inputPath, getAbsolutePath());
}

File Directory::createChildFile (const std::string& filename) const
{
	File file { getAbsolutePath() / filename };

	file.createIfDoesntExist();

	return file;
}

Directory Directory::createChildDirectory (const std::string& subdirectoryName) const
{
	Directory dir { getAbsolutePath() / subdirectoryName };

	dir.createIfDoesntExist();

	return dir;
}

SymLink Directory::createChildSymLink (const std::string&	  symLinkName,
									   const FilesystemEntry& symLinkTarget) const
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

}  // namespace files

LIMES_END_NAMESPACE
