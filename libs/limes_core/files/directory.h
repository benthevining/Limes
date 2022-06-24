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

#include <functional>			// for function
#include <limes_export.h>		// for LIMES_EXPORT
#include <limes_namespace.h>	// for LIMES_BEGIN_NAMESPACE, LIMES_END_...
#include <stdint.h>				// for uintmax_t
#include <string>				// for string
#include <vector>				// for vector
#include "./FilesystemEntry.h"	// for FilesystemEntry, Path
#include "./file.h"				// for File
#include "./sym_link.h"			// for SymLink

/** @file
	This file defines the Directory class.
	@ingroup files
 */

LIMES_BEGIN_NAMESPACE

namespace files
{

/** This class represents a %directory on the filesystem.
	@ingroup files
	@todo Get special directory locations (desktop, documents, etc)
	@todo funcs involving child files - filename regex
 */
class LIMES_EXPORT Directory final : public FilesystemEntry
{
public:

	using FilesystemEntry::FilesystemEntry;

	LIMES_DEFAULT_COPYABLE (Directory)
	LIMES_DEFAULT_MOVABLE (Directory)

	/** @name Assignment */
	///@{
	/** Assigns this object to refer to a new path. */
	Directory& operator= (const Path& newPath);
	Directory& operator= (const std::string_view& newPath);
	///@}

	/** @name Typedefs
		Typedefs for callbacks used by the functions that iterate through this directory's children.
	 */
	///@{
	using FileCallback			  = std::function<void (const File&)>;
	using DirectoryCallback		  = std::function<void (const Directory&)>;
	using SymLinkCallback		  = std::function<void (const SymLink&)>;
	using FilesystemEntryCallback = std::function<void (const FilesystemEntry&)>;
	///@}

	/** Returns true if this %directory contains the passed FilesystemEntry. */
	[[nodiscard]] bool contains (const FilesystemEntry& entry) const;

	/** Returns true if this %directory contains a child with the specified name. */
	[[nodiscard]] bool contains (const std::string_view& childName) const;

	/** Creates the %directory this object refers to, if it doesn't already exist.
		@returns True if the %directory needed to be created, and was successfully created.
	 */
	bool createIfDoesntExist() const noexcept final;

	/** Returns true if this %directory contains no children. */
	[[nodiscard]] bool isEmpty() const;

	/** Returns the input path made relative to this directory's path. */
	[[nodiscard]] Path getRelativePath (const Path& inputPath) const;

	/** Returns a type-erased FilesystemEntry representing a filesystem object in this %directory with the specified name.
		@see getChildFile(), getChildDirectory(), createChildSymLink()
	 */
	[[nodiscard]] FilesystemEntry getChild (const std::string_view& childName, bool createIfNeeded = false) const;

	/** @name Child files */
	///@{

	/** Returns a File object representing a file in this %directory with the specified name.
		@see getChild()
	 */
	[[nodiscard]] File getChildFile (const std::string_view& filename, bool createIfNeeded = false) const;

	/** Returns all child files that exist in this %directory.
		@see iterateFiles()
	 */
	[[nodiscard]] std::vector<File> getChildFiles (bool recurse = true) const;

	/** Calls a function for each child %file that exists in this %directory.
		@see getChildFiles()
	 */
	void iterateFiles (FileCallback&& callback, bool recurse = true) const;

	///@}

	/** @name Child subdirectories */
	///@{

	/** Returns a Directory object that represents a subdirectory of this one with the specified name.
		@see getChild()
	 */
	[[nodiscard]] Directory getChildDirectory (const std::string_view& subdirectoryName, bool createIfNeeded = false) const;

	/** Returns all child directories that exist in this %directory.
		@see iterateDirectories()
	 */
	[[nodiscard]] std::vector<Directory> getChildDirectories (bool recurse = true) const;

	/** Calls a function for each child %directory that exists in this %directory.
		@see getChildDirectories()
	 */
	void iterateDirectories (DirectoryCallback&& callback, bool recurse = true) const;

	///@}

	/** Child symbolic links */
	///@{

	/** Returns a SymLink object that represents a symbolic link in the current %directory with the specified name.
		Note that this function always creates the symbolic link if it doesn't exist.
		@see getChild()
	 */
	[[nodiscard]] SymLink createChildSymLink (const std::string_view& symLinkName,
											  const FilesystemEntry&  symLinkTarget) const;

	/** Returns all child symbolic links that exist in this %directory.
		@see iterateSymLinks()
	 */
	[[nodiscard]] std::vector<SymLink> getChildSymLinks (bool recurse = true) const;

	/** Calls a function for each child symbolic link that exists in this %directory.
		@see getChildSymLinks()
	 */
	void iterateSymLinks (SymLinkCallback&& callback, bool recurse = true) const;

	///@}

	/** Returns all child filesystem entries that exist in this %directory.
		@see iterateAllChildren()
	 */
	[[nodiscard]] std::vector<FilesystemEntry> getAllChildren (bool recurse = true) const;

	/** Iterates through all child objects of this %directory, calling different callbacks for each object depending on if it is a %file, %directory, or symbolic link.
	 */
	void iterateAllChildren (FileCallback&&		 fileCallback,
							 DirectoryCallback&& directoryCallback,
							 SymLinkCallback&&	 symLinkCallback,
							 bool				 recurse = true) const;

	/** Iterates through all child objects of this %directory, calling a single type-erased callback for each one. */
	void iterateAllChildren (FilesystemEntryCallback&& callback,
							 bool					   recurse = true) const;

	/** Returns the size of this %directory, calculated as the cumulative size of all this directory's contents, recursively. */
	[[nodiscard]] std::uintmax_t sizeInBytes() const final;

	/** Sets this %directory as the current working %directory. */
	void setAsWorkingDirectory() const;

	/** Returns true if this %directory is the current working %directory. */
	[[nodiscard]] bool isCurrentWorkingDirectory() const;

	/** Returns a %directory object representing the current working %directory. */
	[[nodiscard]] static Directory getCurrentWorkingDirectory();

	/** @name Set the current working %directory */
	///@{
	static void setCurrentWorkingDirectory (const Path& path);
	static void setCurrentWorkingDirectory (const Directory& directory);
	///@}

	/** Returns a %directory appropriate for storing temporary files.
		@see TempFile
	 */
	[[nodiscard]] static Directory getTempFileDirectory();

	/** Returns the contents of the \c PATH environment variable as a vector of Directory objects. */
	[[nodiscard]] static std::vector<Directory> getPATH();
};

}  // namespace files

LIMES_END_NAMESPACE
