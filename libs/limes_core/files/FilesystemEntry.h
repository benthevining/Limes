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
#include <filesystem>
#include <string>
#include <cstdint>
#include <memory>

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

	using TimeType = std::filesystem::file_time_type;

	using Permissions = std::filesystem::perms;

	using PermOptions = std::filesystem::perm_options;
	using CopyOptions = std::filesystem::copy_options;

	FilesystemEntry() = default;

	explicit FilesystemEntry (const Path& pathToUse, bool createIfNeeded = false);

	virtual ~FilesystemEntry() = default;

	FilesystemEntry (const FilesystemEntry&) = default;
	FilesystemEntry (FilesystemEntry&&)		 = default;

	FilesystemEntry& operator= (const FilesystemEntry&) = default;
	FilesystemEntry& operator= (FilesystemEntry&&) = default;

	FilesystemEntry& operator= (const Path& newPath);

	[[nodiscard]] FilesystemEntry operator/ (const std::string& subpathName) const;

	FilesystemEntry& operator/= (const std::string& subpathName);

	[[nodiscard]] bool operator== (const FilesystemEntry& other) const noexcept;
	[[nodiscard]] bool operator!= (const FilesystemEntry& other) const noexcept;

	[[nodiscard]] Path getPath() const noexcept;
	[[nodiscard]] Path getAbsolutePath() const noexcept;

	[[nodiscard]] Directory getDirectory() const;
	[[nodiscard]] Directory getParentDirectory() const;

	[[nodiscard]] bool isValid() const noexcept;
	[[nodiscard]] bool exists() const noexcept;
	[[nodiscard]] bool isFile() const noexcept;
	[[nodiscard]] bool isDirectory() const noexcept;
	[[nodiscard]] bool isSymLink() const noexcept;

	[[nodiscard]] bool isAbsolutePath() const noexcept;
	[[nodiscard]] bool isRelativePath() const noexcept;

	[[nodiscard]] std::unique_ptr<File>		 getFileObject() const;
	[[nodiscard]] std::unique_ptr<Directory> getDirectoryObject() const;
	[[nodiscard]] std::unique_ptr<SymLink>	 getSymLinkObject() const;

	bool createIfDoesntExist() const;
	bool deleteIfExists() const;

	void touch() const;

	[[nodiscard]] std::uintmax_t sizeInBytes() const;

	[[nodiscard]] bool setPermissions (Permissions permissions, PermOptions options = PermOptions::replace) const noexcept;

	[[nodiscard]] Permissions getPermissions() const;

	[[nodiscard]] bool isHidden() const;

	[[nodiscard]] TimeType getLastModificationTime() const noexcept;

	bool rename (const Path& newPath) noexcept;

	bool rename (const std::string& newFilenameWithinDirectory);

	bool copyTo (const Path& dest, CopyOptions options = CopyOptions::update_existing) const noexcept;
	bool copyTo (const FilesystemEntry& dest, CopyOptions options = CopyOptions::update_existing) const noexcept;

	bool copyFrom (const Path& source, CopyOptions options = CopyOptions::update_existing) const noexcept;
	bool copyFrom (const FilesystemEntry& source, CopyOptions options = CopyOptions::update_existing) const noexcept;

private:

	Path path;
};

}  // namespace files

LIMES_END_NAMESPACE
