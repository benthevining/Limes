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

#include <limes_export.h>
#include "../limes_namespace.h"
#include <cstdio>
#include "../misc/preprocessor.h"
#include "./FilesystemEntry.h"	// for Path typedef

/** @file
	This file defines the CFile class.
	@ingroup files
 */

LIMES_BEGIN_NAMESPACE

namespace files
{

class File;

/** This class is a wrapper around a C-style \c FILE* that takes care of freeing the file when the object is destroyed, and provides a few other convenience methods.
	@ingroup files
	@see File
 */
class LIMES_EXPORT CFile final
{
public:
	/** Represents possible modes a file can be opened in. */
	enum class Mode
	{
		Read,
		Write,
		Append,
		ReadExtended,
		WriteExtended,
		AppendExtended
	};

	/** @name Constructors */
	///@{
	/** Creates a default CFile that holds a nullptr. Call \c open() to actually open a file. */
	CFile() = default;

	/** Creates a CFile referencing the given filepath, in the given mode.
		\c fopen() is called to open the file.
	 */
	explicit CFile (const Path& filepath, Mode mode) noexcept;

	/** Creates a CFile that takes ownership of the passed \c FILE* .
		After using this constructor, calling \c getPath() on the created object will return an empty path.

		@note The CFile object will take ownership of the passed pointer, so you should not delete it manually!
	 */
	explicit CFile (std::FILE* fileHandle) noexcept;

	/** Move constructor. */
	CFile (CFile&& other) noexcept;
	///@}

	/** Destructor. If the file is open when the destructor is called, the destructor will close the file.
	 */
	~CFile() noexcept;

	/** Move assignment operator. */
	CFile& operator= (CFile&& other) noexcept;

	LIMES_NON_COPYABLE (CFile)

	/** @name Accessors */
	///@{
	/** Returns the pointer this object holds.
		Note that the CFile object still retains ownership of this pointer, you should not free it!
	 */
	[[nodiscard]] std::FILE* get() const noexcept;

	/** Returns the pointer this object holds. */
	std::FILE* operator->() const noexcept;

	/** Dereferences the pointer this object holds.
		Note that the pointer may be null, so be careful!
	 */
	std::FILE& operator*() const;

	/** Returns the path of the file this object represents.
		Returns an empty path if no file is currently open, or if this object was constructed by passing it only a \c FILE* .
	 */
	[[nodiscard]] Path getPath() const noexcept;
	///@}

	/** If the file is currently open, this closes it by calling \c fclose() . */
	void close() noexcept;

	/** Closes the current file (if one is open) and opens the file at the specified path.
		\c fopen() is called to open the file.
		@return True if opening the file was successful
	 */
	bool open (const Path& filepath, Mode mode) noexcept;

	/** Returns true if the file is currently open. */
	[[nodiscard]] bool isOpen() const noexcept;

	/** Evaluates to true if the file is currently open. */
	explicit operator bool() const noexcept;

	/** Returns a File object representing this %file.

		If this CFile was constructed without a path (by passing it just a \c std::FILE* ),
		or if no file is currently open, a File object holding an empty path will be returned.
	 */
	[[nodiscard]] File getFile() const;

	/** Creates an automatically named, self-deleting temporary file using the C function \c std::tmpfile .
		@see TempFile
	 */
	[[nodiscard]] static CFile createTempFile();

private:
	std::FILE* ptr { nullptr };

	Path path {};
};

}  // namespace files

LIMES_END_NAMESPACE
