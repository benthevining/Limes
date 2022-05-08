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

#include <memory>				   // for unique_ptr
#include <limes_export.h>		   // for LIMES_EXPORT
#include <limes_namespace.h>	   // for LIMES_BEGIN_NAMESPACE, LIMES_END_N...
#include <cstdint>				   // for uintmax_t
#include <filesystem>			   // for copy_options, path, perm_options
#include <string>				   // for string
#include "../misc/preprocessor.h"  // for LIMES_DEFAULT_COPYABLE, LIMES_DEFA...
#include "../time/DateTime.h"	   // for DateTime

LIMES_BEGIN_NAMESPACE

namespace files
{

class Directory;
class File;
class SymLink;

using Path = std::filesystem::path;

class LIMES_EXPORT FilesystemEntry
{
public:

	using Permissions = std::filesystem::perms;

	using PermOptions = std::filesystem::perm_options;
	using CopyOptions = std::filesystem::copy_options;

	FilesystemEntry() = default;

	explicit FilesystemEntry (const Path& pathToUse, bool createIfNeeded = false);

	virtual ~FilesystemEntry() = default;

	LIMES_DEFAULT_COPYABLE (FilesystemEntry);
	LIMES_DEFAULT_MOVABLE (FilesystemEntry);

	FilesystemEntry& operator= (const Path& newPath);

	FilesystemEntry& assignPath (const Path& newPath);

	FilesystemEntry& changeName (const std::string& newName);

	[[nodiscard]] FilesystemEntry operator/ (const std::string& subpathName) const;

	FilesystemEntry& operator/= (const std::string& subpathName);

	[[nodiscard]] bool operator== (const FilesystemEntry& other) const noexcept;
	[[nodiscard]] bool operator!= (const FilesystemEntry& other) const noexcept;

	[[nodiscard]] bool operator< (const FilesystemEntry& other) const noexcept;
	[[nodiscard]] bool operator> (const FilesystemEntry& other) const noexcept;

	[[nodiscard]] Path getPath() const noexcept;
	[[nodiscard]] Path getAbsolutePath() const noexcept;

	[[nodiscard]] std::string getName() const noexcept;

	[[nodiscard]] Directory getDirectory() const;
	[[nodiscard]] Directory getParentDirectory() const;

	[[nodiscard]] bool isValid() const noexcept;
	[[nodiscard]] bool exists() const noexcept;
	[[nodiscard]] bool isFile() const noexcept;
	[[nodiscard]] bool isDirectory() const noexcept;
	[[nodiscard]] bool isSymLink() const noexcept;

	[[nodiscard]] bool isAbsolutePath() const noexcept;
	[[nodiscard]] bool isRelativePath() const noexcept;

	bool makeAbsoluteRelativeTo (const Path& basePath) noexcept;
	bool makeAbsoluteRelativeToCWD() noexcept;

	[[nodiscard]] std::unique_ptr<File>		 getFileObject() const;
	[[nodiscard]] std::unique_ptr<Directory> getDirectoryObject() const;
	[[nodiscard]] std::unique_ptr<SymLink>	 getSymLinkObject() const;

	virtual bool createIfDoesntExist() const;
	bool		 deleteIfExists() const;

	void touch() const;
	bool touch_noCreate() const;

	[[nodiscard]] virtual std::uintmax_t sizeInBytes() const;

	[[nodiscard]] bool setPermissions (Permissions permissions, PermOptions options = PermOptions::replace) const noexcept;

	[[nodiscard]] Permissions getPermissions() const;

	[[nodiscard]] bool isHidden() const;

	[[nodiscard]] time::DateTime getLastModificationTime() const noexcept;

	bool rename (const Path& newPath) noexcept;

	bool rename (const std::string& newFilenameWithinDirectory);

	bool copyTo (const Path& dest, CopyOptions options = CopyOptions::update_existing) const noexcept;
	bool copyTo (const FilesystemEntry& dest, CopyOptions options = CopyOptions::update_existing) const noexcept;

	bool copyToDirectory (const Path& destDirectory, CopyOptions options = CopyOptions::update_existing) const noexcept;

	bool copyFrom (const Path& source, CopyOptions options = CopyOptions::update_existing) const noexcept;
	bool copyFrom (const FilesystemEntry& source, CopyOptions options = CopyOptions::update_existing) const noexcept;

private:

	Path path;
};

}  // namespace files

LIMES_END_NAMESPACE
