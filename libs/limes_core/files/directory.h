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

#pragma once

#include <limes_namespace.h>
#include <limes_export.h>
#include "FilesystemEntry.h"
#include "file.h"
#include "sym_link.h"
#include <vector>
#include <functional>
#include <string>

LIMES_BEGIN_NAMESPACE

namespace files
{

class LIMES_EXPORT Directory final : public FilesystemEntry
{
public:

	using FilesystemEntry::FilesystemEntry;

	using FileCallback		= std::function<void (const File&)>;
	using DirectoryCallback = std::function<void (const Directory&)>;
	using SymLinkCallback	= std::function<void (const SymLink&)>;

	bool createIfDoesntExist() const final;

	[[nodiscard]] FilesystemEntry getChild (const std::string& childName, bool createIfNeeded = false) const;

	[[nodiscard]] Path getRelativePath (const Path& inputPath) const;

	File createChildFile (const std::string& filename) const;

	Directory createChildDirectory (const std::string& subdirectoryName) const;

	SymLink createChildSymLink (const std::string&	   symLinkName,
								const FilesystemEntry& symLinkTarget) const;

	[[nodiscard]] std::vector<File> getChildFiles (bool recurse = true) const;

	void iterateFiles (FileCallback&& callback, bool recurse = true) const;

	[[nodiscard]] std::vector<Directory> getChildDirectories (bool recurse = true) const;

	void iterateDirectories (DirectoryCallback&& callback, bool recurse = true) const;

	[[nodiscard]] std::vector<SymLink> getChildSymLinks (bool recurse = true) const;

	void iterateSymLinks (SymLinkCallback&& callback, bool recurse = true) const;

	[[nodiscard]] std::vector<FilesystemEntry> getAllChildren (bool recurse = true) const;

	void iterateAllChildren (FileCallback&&		 fileCallback,
							 DirectoryCallback&& directoryCallback,
							 SymLinkCallback&&	 symLinkCallback,
							 bool				 recurse = true) const;

	[[nodiscard]] std::uintmax_t sizeInBytes() const final;

	void setAsWorkingDirectory() const;

	[[nodiscard]] static Directory getCurrentWorkingDirectory();

	static void setCurrentWorkingDirectory (const Path& path);
	static void setCurrentWorkingDirectory (const Directory& directory);

	[[nodiscard]] static Directory getTempFileDirectory();

	[[nodiscard]] static std::vector<Directory> getPATH();
};

}  // namespace files

LIMES_END_NAMESPACE
