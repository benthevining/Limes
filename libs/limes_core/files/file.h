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

#include <limes_export.h>  // for LIMES_EXPORT
#include "../limes_namespace.h"
#include <cstddef>	// for size_t
#include <string>	// for string
#include <string_view>
#include <vector>				// for vector
#include "../hashes/hash.h"		// for hash::Type
#include "../memory/RawData.h"	// for RawData
#include "./FilesystemEntry.h"	// for FilesystemEntry, Path
#include <functional>			// for std::hash
#include "../misc/preprocessor.h"
#include "./CFile.h"

/** @file
	This file defines the File and TempFile classes.
	@ingroup files
 */

LIMES_BEGIN_NAMESPACE

namespace files
{

#pragma mark File

/** This class represents a %file on the filesystem.
	@ingroup files
	@see CFile

	@todo std in/out streams for this class
 */
class LIMES_EXPORT File : public FilesystemEntry
{
public:
	using FilesystemEntry::FilesystemEntry;

	LIMES_DEFAULT_COPYABLE (File)
	LIMES_DEFAULT_MOVABLE (File)

	/** @name Assignment */
	///@{
	/** Assigns this object to refer to a new path. */
	File& operator= (const Path& newPath);
	File& operator= (const std::string_view& newPath);
	///@}

	/** Returns this file's filename. */
	[[nodiscard]] std::string getFilename (bool includeExtension = false) const;

	/** Returns this file's %file extension, if any. */
	[[nodiscard]] std::string getFileExtension() const;

	/** Returns true if this %file has the specified file extension. */
	[[nodiscard]] bool hasFileExtension (const std::string_view& extension) const;

	/** Returns true if this %file has a file extension. */
	[[nodiscard]] bool hasFileExtension() const;

	/** Replaces the filename in the path this object refers to.
		@param newFileExtension The new %file extension to use.
		@param renameOnDisk If true, the %file is actually renamed on the filesystem. If false, this function only changes the internal representation of the path.
		@see FilesystemEntry::rename()
	 */
	File& replaceFileExtension (const std::string_view& newFileExtension,
								bool					renameOnDisk = true);

	/** @name Overwriting with content */
	///@{
	/** Replaces the file's contents with the given data.
		@returns True if writing the data was successful
	 */
	bool overwrite (const memory::RawData& data) const noexcept;
	bool overwrite (const char* const data, std::size_t numBytes) const noexcept;
	bool overwrite (const std::string_view& text) const noexcept;
	///@}

	/** @name Appending content */
	///@{
	/** Appends the given data to the file's current contents.
		@returns True if writing the data was successful
	 */
	bool append (const memory::RawData& data) const noexcept;
	bool append (const char* const data, std::size_t numBytes) const noexcept;
	bool append (const std::string_view& text) const noexcept;
	///@}

	/** @name Prepending content */
	///@{
	/** Prepends the given data to the file's current contents.
		@returns True if writing the data was successful
	 */
	bool prepend (const memory::RawData& data) const noexcept;
	bool prepend (const char* const data, std::size_t numBytes) const noexcept;
	bool prepend (const std::string_view& text) const noexcept;
	///@}

	/** Duplicates this file within its directory.
		This creates a new file containing the same content as this file, in this file's directory, and automatically names the new file.
		The new file will be named \c \<thisFileName\>_copy.\<thisFileXtn\> , unless a file with that name already exists, in which case it will be named \c \<thisFileName\>_copy2.\<thisFileXtn\> , and so on, until a unique name in the current directory is reached.
		@returns The new file that was created, or a null file object if duplication fails.
	 */
	File duplicate() const noexcept;

	/** @name Loading the file */
	///@{
	/** Loads the file's contents as a RawData object. */
	[[nodiscard]] memory::RawData loadAsData() const noexcept;

	/** Loads the file's contents as a string. */
	[[nodiscard]] std::string loadAsString() const noexcept;

	/** Loads the file's contents as a vector of strings, with each string containing the contents of one line of the %file. */
	[[nodiscard]] std::vector<std::string> loadAsLines() const;
	///@}

	/** Calculates a hash of the file's content using the specified hash type. */
	[[nodiscard]] std::string hash (hash::Type hashType) const;

	/** Returns a CFile referring to this %file.
		This will return a nullptr if the %file does not exist.
		@see CFile
	 */
	[[nodiscard]] CFile getCfile (CFile::Mode mode = CFile::Mode::Read) const noexcept;

	/** Returns a file representing the location of the executable %file that launched the current process.
		Note that the %file returned by this function may not exist anymore; on some Unixes, it is perfectly legal to delete an executable %file while the executable is still running.
		@see getCurrentModule()
	 */
	[[nodiscard]] static File getCurrentExecutable();

	/** Returns a %file representing the location of the current code module.
		If the current process is a dynamic library, this will return the path to the library %file. Otherwise, this may return the same thing as getCurrentExecutable().
		@see getCurrentExecutable()
	 */
	[[nodiscard]] static File getCurrentModule();

private:
	[[nodiscard]] bool write_data (const char* const data, std::size_t numBytes, bool overwrite) const noexcept;
};

/*-------------------------------------------------------------------------------------------------------------------------*/

#pragma mark TempFile

/** Represents a temporary %file.
	This object will create the temporary %file when the object is constructed, and by default destroy the %file when the object is destroyed (though this behavior can be turned off).
	The temporary %file will reside in the %directory returned by \c Directory::getTempFileDirectory() .
	If you don't care about filenames and just need a temporary %file to work with, use the static method \c getNextFile() .
	@ingroup files
	@see Directory::getTempFileDirectory(), CFile::createTempFile()
 */
class LIMES_EXPORT TempFile final : public File
{
public:
	/** Creates a temporary %file with the specified filename.
		The %file will be created on the filesystem when this object is constructed.
		If \c filepath is a relative path, then the %directory returned by \c Directory::getTempFileDirectory() will be prepended to this path.

		@param filename The filename of the temporary %file.
		@param destroyOnDelete If true, the %file will be deleted from the filesystem when this object is destroyed.
	 */
	explicit TempFile (const Path& filepath,
					   bool		   destroyOnDelete = true);

	/** The temporary %file will be deleted from the filesystem when this object is destroyed, if this object was constructed with the \c destroyOnDelete parameter being \c true . */
	~TempFile() final;

	LIMES_NON_COPYABLE (TempFile)

	/** @name Moving */
	///@{
	/** Move constructor. */
	TempFile (TempFile&& other) noexcept;

	/** Move assignment operator. */
	TempFile& operator= (TempFile&& other) noexcept;
	///@}

	/** Returns a new temporary %file, with the name generated using the C function \c tmpnam() . */
	[[nodiscard]] static TempFile getNextFile();

private:
	bool shouldDelete { true };
};

}  // namespace files

LIMES_END_NAMESPACE

namespace std
{

/** A specialization of \c std::hash for File objects.
	The hash value is computed using the file's contents.
	@ingroup files
 */
template <>
struct LIMES_EXPORT hash<limes::files::File> final
{
	hash() = default;

	LIMES_DEFAULT_COPYABLE (hash)
	LIMES_DEFAULT_MOVABLE (hash)

	size_t operator() (const limes::files::File& f) const noexcept;
};
}  // namespace std
