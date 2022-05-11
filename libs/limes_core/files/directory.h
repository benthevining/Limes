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

#include <functional>		  // for function
#include <limes_export.h>	  // for LIMES_EXPORT
#include <limes_namespace.h>  // for LIMES_BEGIN_NAMESPACE, LIMES_END_...
#include <stdint.h>			  // for uintmax_t
#include <string>			  // for string
#include <vector>			  // for vector
#include "FilesystemEntry.h"  // for FilesystemEntry, Path
#include "file.h"			  // for File
#include "sym_link.h"		  // for SymLink


LIMES_BEGIN_NAMESPACE

namespace files
{

class LIMES_EXPORT Directory final : public FilesystemEntry
{
public:

	using FilesystemEntry::FilesystemEntry;

	LIMES_DEFAULT_COPYABLE (Directory);
	LIMES_DEFAULT_MOVABLE (Directory);

	Directory& operator= (const Path& newPath);
	Directory& operator= (const std::string_view& newPath);

	using FileCallback		= std::function<void (const File&)>;
	using DirectoryCallback = std::function<void (const Directory&)>;
	using SymLinkCallback	= std::function<void (const SymLink&)>;

	[[nodiscard]] FilesystemEntry operator/ (const std::string_view& subpathName) const;

	FilesystemEntry& operator/= (const std::string_view& subpathName);

	[[nodiscard]] bool contains (const FilesystemEntry& entry) const;
	[[nodiscard]] bool contains (const std::string_view& childName) const;

	bool createIfDoesntExist() const final;

	[[nodiscard]] bool isEmpty() const;

	[[nodiscard]] Path getRelativePath (const Path& inputPath) const;

	[[nodiscard]] FilesystemEntry getChild (const std::string_view& childName, bool createIfNeeded = false) const;

	[[nodiscard]] File getChildFile (const std::string_view& filename, bool createIfNeeded = false) const;

	[[nodiscard]] Directory getChildDirectory (const std::string_view& subdirectoryName, bool createIfNeeded = false) const;

	[[nodiscard]] SymLink createChildSymLink (const std::string_view& symLinkName,
											  const FilesystemEntry&  symLinkTarget) const;

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
