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

#include <memory>		   // for unique_ptr
#include <limes_export.h>  // for LIMES_EXPORT
#include "../limes_namespace.h"
#include <cstdint>				   // for uintmax_t
#include <filesystem>			   // for copy_options, path, perm_options
#include <string>				   // for string
#include "../misc/preprocessor.h"  // for LIMES_DEFAULT_COPYABLE, LIMES_DEFA...
#include "../time/DateTime.h"	   // for DateTime
#include <functional>			   // for std::hash

/** @defgroup files Files
	Utilities for working with the filesystem.
	Limes's filesystem library is built on top of the \c std::filesystem library.
	I wanted a strongly-typed interface for working with filesystem objects, primarily to differentiate between files and directories.
	You can still construct paths that wouldn't be considered canonical for a given filesystem object type --
	for instance, the Directory class won't prevent you from creating one referencing \c /usr/documents/a_file.txt --
	but I believe that having strongly-typed classes for each kind of filesystem object provides a cleaner API and allows the programmer to more explicitly express their intent.

	Another design principle of this library is that most operations are \c noexcept -- success or failure is usually indicated by a boolean return value.

	@ingroup limes_core
	@todo FileWatcher class
 */

/** @dir libs/limes_core/files
	This directory contains filesystem utilities.
	@ingroup files
 */

/** @file
	This file defines the FilesystemEntry class and some typedefs used throughout the files namespace.
	@ingroup files
 */

LIMES_BEGIN_NAMESPACE

/** This namespace contains filesystem utilities.
	@ingroup files
 */
namespace files
{

class Directory;
class File;
class SymLink;

/** Convenience typedef for filesystem paths. */
using Path = std::filesystem::path;

/** The base class for any kind of object in the filesystem.
	This class is essentially a wrapper around \c std::filesystem::path .
	@ingroup files
	@see File, Directory, SymLink
 */
class LIMES_EXPORT FilesystemEntry
{
public:

	using Permissions = std::filesystem::perms;
	using PermOptions = std::filesystem::perm_options;
	using CopyOptions = std::filesystem::copy_options;

	/** @name Constructors */
	///@{
	/** Creates a FilesystemEntry with an empty path. */
	FilesystemEntry() = default;

	/** Creates a FilesystemEntry referring to the specified path.
		@param pathToUse The path to this filesystem entry (which may or may not exist)
	 */
	explicit FilesystemEntry (const Path& pathToUse);
	///@}

	/** Destructor. */
	virtual ~FilesystemEntry() = default;

	LIMES_DEFAULT_COPYABLE (FilesystemEntry)
	LIMES_DEFAULT_MOVABLE (FilesystemEntry)

	/** @name Assignment */
	///@{
	/** Assigns the path this filesystem entry represents. */
	FilesystemEntry& operator= (const Path& newPath);
	FilesystemEntry& operator= (const std::string_view& newPath);
	FilesystemEntry& assignPath (const Path& newPath) noexcept;
	///@}

	/** Returns a new FilesystemEntry referring to a subpath of this one. */
	[[nodiscard]] FilesystemEntry operator/ (const std::string_view& subpathName) const;

	/** Assigns this object's path to a subpath of the current path. */
	FilesystemEntry& operator/= (const std::string_view& subpathName);

	/** Changes the last section of the path (the filename). */
	FilesystemEntry& changeName (const std::string_view& newName);

	/** @name Equality comparison */
	///@{
	/** Compares this filesystem entry's absolute path with the other one's for equality. */
	[[nodiscard]] bool operator== (const FilesystemEntry& other) const noexcept;
	[[nodiscard]] bool operator== (const Path& other) const noexcept;
	[[nodiscard]] bool operator!= (const FilesystemEntry& other) const noexcept;
	[[nodiscard]] bool operator!= (const Path& other) const noexcept;
	///@}

	/** @name Lexicographic comparison */
	///@{
	/** Lexicographically compares this filesystem entry's absolute path with another path. */
	[[nodiscard]] bool operator<(const FilesystemEntry& other) const noexcept;
	[[nodiscard]] bool operator> (const FilesystemEntry& other) const noexcept;
	[[nodiscard]] bool operator<(const Path& other) const noexcept;
	[[nodiscard]] bool operator> (const Path& other) const noexcept;
	///@}

	/** Returns the current path held by this object.
		If this object was constructed with or assigned to a relative path, this function will return that relative path.
		@param makePreferred If true, this converts any directory separators present in the path to the platform's preferred directory separator.
	 */
	[[nodiscard]] Path getPath (bool makePreferred = false) const noexcept;

	/** Returns the full, absolute path of this filesystem entry.
		@param makePreferred If true, this converts any directory separators present in the path to the platform's preferred directory separator.
	 */
	[[nodiscard]] Path getAbsolutePath (bool makePreferred = false) const noexcept;

	/** Converts this filesystem entry to its absolute path. */
	operator Path() const noexcept;

	/** Returns the last section of the path (the filename). */
	[[nodiscard]] std::string getName() const noexcept;

	/** Returns the directory containing this filesystem entry. */
	[[nodiscard]] Directory getDirectory() const;

	/** Returns the parent %directory of this filesystem entry. */
	[[nodiscard]] Directory getParentDirectory() const;

	/** Returns true if this object holds a valid filesystem path. This is essentially only false if the path is empty. */
	[[nodiscard]] bool isValid() const noexcept;

	/** Returns true if the filesystem object this object refers to exists on disk. */
	[[nodiscard]] bool exists() const noexcept;

	/** Returns true if this filesystem entry refers to a %file. */
	[[nodiscard]] bool isFile() const noexcept;

	/** Returns true if this filesystem entry refers to a %directory. */
	[[nodiscard]] bool isDirectory() const noexcept;

	/** Returns true if this filesystem entry refers to a symbolic link. */
	[[nodiscard]] bool isSymLink() const noexcept;

	/** Returns true if the path this object holds is a relative path. */
	[[nodiscard]] bool isAbsolutePath() const noexcept;

	/** Returns true if the path this object holds is an absolute path. */
	[[nodiscard]] bool isRelativePath() const noexcept;

	/** If the path this object holds is already absolute, this function does nothing.
		Otherwise, this function assigns the path to \c basePath/currentPath (prepending the basePath to the previous path this object was already assigned to).
		@returns True if this function changed the path this object refers to.
		@see makeAbsoluteRelativeToCWD
	 */
	bool makeAbsoluteRelativeTo (const Path& basePath) noexcept;

	/** Similar to makeAbsoluteRelativeTo(), but uses the current working directory as the base path.
		@returns True if this function changed the path this object refers to.
		@see makeAbsoluteRelativeTo
	 */
	bool makeAbsoluteRelativeToCWD() noexcept;

	/** If this object refers to a %file, constructs a File object referring to the same %file. Otherwise, returns a nullptr. */
	[[nodiscard]] std::unique_ptr<File> getFileObject() const;

	/** If this object refers to a %directory, constructs a Directory object referring to the same %directory. Otherwise, returns a nullptr. */
	[[nodiscard]] std::unique_ptr<Directory> getDirectoryObject() const;

	/** If this object refers to a symbolic link, constructs a SymLink object referring to the same link. Otherwise, returns a nullptr. */
	[[nodiscard]] std::unique_ptr<SymLink> getSymLinkObject() const;

	/** Creates the filesystem entry this object refers to, if it doesn't already exist.
		@returns True if the filesystem object needed to be created, and was successfully created.
	 */
	virtual bool createIfDoesntExist() const noexcept;

	/** Deletes the filesystem entry this object refers to, if it exists.
		@returns True if the object existed and was successfully deleted.
	 */
	bool deleteIfExists() const noexcept;

	/** If the filesystem entry this object refers do doesn't exist, this function creates it.
		If it did already exist, this function updates its modification time to the current system time.
		@see touch_noCreate
	 */
	void touch() const;

	/** The same as touch(), except this function does not create the filesystem entry if it didn't already exist.
		@return True if the filesystem entry existed when this function was called.
		@see touch
	 */
	bool touch_noCreate() const;

	/** Returns the size, in bytes, of the filesystem entry referred to by this object.
		If the filesystem entry doesn't exist, this will return 0.
		For directories, this returns the cumulative size of all the directory's contents, recursively.
	 */
	[[nodiscard]] virtual std::uintmax_t sizeInBytes() const;

	/** Sets the permissions for the filesystem entry.
		@returns True if setting permissions was successful
	 */
	[[nodiscard]] bool setPermissions (Permissions permissions, PermOptions options = PermOptions::replace) const noexcept;

	/** Returns the current permissions of the filesystem entry this object refers to. */
	[[nodiscard]] Permissions getPermissions() const;

	/** Returns true if the filename of this object begins with a \c . character. */
	[[nodiscard]] bool isHidden() const;

	/** Returns the last modification time of the filesystem entry. */
	[[nodiscard]] time::DateTime getLastModificationTime() const noexcept;

	/** Renames this filesystem object to a new path. This can be used to move the entry to a different %directory.
		@returns True if renaming was successful.
	 */
	bool rename (const Path& newPath) noexcept;

	/** Renames this filesystem entry to a new filename within its current %directory.
		@returns True if renaming was successful.
	 */
	bool rename (const std::string& newFilenameWithinDirectory);

	/**	@name Creating copies */
	///@{
	/**
		Creates a copy of this filesystem entry at a new location.
		@returns True if copying was successful
	 */
	bool copyTo (const Path& dest, CopyOptions options = CopyOptions::update_existing) const noexcept;
	bool copyTo (const FilesystemEntry& dest, CopyOptions options = CopyOptions::update_existing) const noexcept;
	///@}

	/** Creates a copy of this filesystem object, with the same filename, in a different %directory.
		@returns True if copying was successful.
	 */
	bool copyToDirectory (const Path& destDirectory, CopyOptions options = CopyOptions::update_existing) const noexcept;

	/** Overwriting */
	//@{
	/** Overwrites this filesystem entry by copying from another location.
		@returns True if copying was successful.
	 */
	bool copyFrom (const Path& source, CopyOptions options = CopyOptions::update_existing) const noexcept;
	bool copyFrom (const FilesystemEntry& source, CopyOptions options = CopyOptions::update_existing) const noexcept;
	//@}

private:

	Path path;
};

}  // namespace files

LIMES_END_NAMESPACE

namespace std
{

/** A specialization of \c std::hash for filesystem entries.
	The hash value is computed based on the path of the filesystem entry.
	@ingroup files
 */
template <>
struct LIMES_EXPORT hash<limes::files::FilesystemEntry> final
{
	hash() = default;

	LIMES_DEFAULT_COPYABLE (hash)
	LIMES_DEFAULT_MOVABLE (hash)

	size_t operator() (const limes::files::FilesystemEntry& e) const noexcept;
};
}  // namespace std
