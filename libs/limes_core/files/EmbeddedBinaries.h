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
#include "../memory/RawData.h"
#include <cstddef>
#include <string>
#include <string_view>
#include <iterator>
#include "../misc/preprocessor.h"
#include "../misc/Algorithm.h"

/** @defgroup binaries Embedded binary data files
	Utilities for working with binary data files embedded into your source code.
	These classes are used automatically by the BinaryBuilder tool included with Limes, but any sources containing embedded data can use these classes to provide a runtime interface for accessing that data.
	@ingroup files
 */

/** @file
	This file defines the FileInfo and FileInfoList classes.
	@ingroup binaries
 */

LIMES_BEGIN_NAMESPACE

/** This namespace contains utilities for working with binary data files embedded into your application's source code.
	@ingroup binaries
 */
namespace binaries
{

/** This class is an interface to any kind of binary data embedded into your source code.
	To embed some external data as a source file, create a static extern array of FileInfo objects initialized with the filename, extension, and raw data of any binary files you wish to embed.
	Then you can create a free function returning an instance of the FileInfoList class which provides access to the array of FileInfo objects you've embedded in your code.
	A full example of how to do this can be seen in the BinaryBuilder source code.
	@ingroup binaries
	@see FileInfoList
 */
struct LIMES_EXPORT FileInfo final
{
	LIMES_NON_COPYABLE (FileInfo)
	LIMES_NON_MOVABLE (FileInfo)

	/** The original name of this file. */
	const char* const fileName;

	/** The original file extension of this file. */
	const char* const fileExtension;

	/** The raw data contained in this file. */
	const char* const fileData;

	/** The size of this file's data. */
	const std::size_t fileDataSize;

	/** Returns this file's name as a string.
		@see fileName, fullName()
	 */
	[[nodiscard]] constexpr std::string_view name() const { return { fileName }; }

	/** Returns this file's file extension as a string.
		@see fileExtension, hasFileExtension()
	 */
	[[nodiscard]] constexpr std::string_view xtn() const { return { fileExtension }; }

	/** Returns true if this file has a file extension.
		@see xtn()
	 */
	[[nodiscard]] constexpr bool hasFileExtension() const { return ! xtn().empty(); }

	/** Returns the full filename, including the extension, if any.
		@see name()
	 */
	[[nodiscard]] std::string fullName() const;

	/** Returns a constant view of this file's data as a \c string_view .
		@see string(), data(), fileData
	 */
	[[nodiscard]] constexpr std::string_view string_view() const { return std::string_view { fileData, fileDataSize }; }

	/** Returns a copy of this file's data as a \c string .
		@see string_view(), data(), fileData
	 */
	[[nodiscard]] std::string string() const;

	/** Returns a copy of this file's data as a RawData object.
		@see string(), fileData
	 */
	[[nodiscard]] memory::RawData data() const;
};


#pragma mark FileInfoList

/** This class provides an interface to a list of FileInfo objects.
	To embed some external data as a source file, create a static extern array of FileInfo objects which hold your files' data.
	Then you can create a free function returning an instance of the FileInfoList class which provides access to the FileInfo array you've embedded in your code.
	A full example of how to do this can be seen in the BinaryBuilder source code.
	@ingroup binaries
	@see FileInfo
 */
struct LIMES_EXPORT FileInfoList final
{
public:

	LIMES_NON_COPYABLE (FileInfoList)
	LIMES_NON_MOVABLE (FileInfoList)

	/** Constructors */
	///@{

	/** Creates a FileInfoList with pointers to the first and ending elements of an array of FileInfo objects.
		@param first The first object in the FileInfo array
		@param end A pointer to one-past-the-end of the FileInfo array
	 */
	constexpr explicit FileInfoList (const FileInfo* const first,
									 const FileInfo* const end) noexcept
		: m_begin (first), m_end (end)
	{
	}

	/** Creates a FileInfoList with a pointer to the beginning of an array of FileInfo objects and the size of the array.
		@param first The first object in the FileInfo array
		@param num The number of FileInfo objects in the array pointed to by \c first
	 */
	constexpr explicit FileInfoList (const FileInfo* const first,
									 std::size_t		   num) noexcept
		: m_begin (first), m_end (first + num)
	{
	}

	///@}

	/** Returns an iterator to the beginning of the array of FileInfo objects. */
	[[nodiscard]] constexpr const FileInfo* begin() const noexcept { return m_begin; }

	/** Returns an iterator to the end of the array of FileInfo objects. */
	[[nodiscard]] constexpr const FileInfo* end() const noexcept { return m_end; }

	/** Returns the number of FileInfo objects in this list. */
	[[nodiscard]] constexpr const std::size_t size() const noexcept { return std::distance (m_begin, m_end); }

	/** Searches for a file in the list with the given name.
		The list is first searched for an exact match with both the file's original name and extension; if none is found, then the list is searched for a match with just the filename portion.
		@see loadFile()
	 */
	[[nodiscard]] constexpr const FileInfo* findFile (const std::string_view& fileName) const noexcept
	{
		if (const auto* firstResult = alg::contains_or_null (*this, [&fileName] (const FileInfo& i)
															 { return i.fullName() == fileName; }))
			return firstResult;

		return alg::contains_or_null (*this, [&fileName] (const FileInfo& i)
									  { return i.name() == fileName; });
	}

	/** Searches for a file in the list with the given name, and returns a RawData object holding its data.
		If the file cannot be found, an empty RawData will be returned.
		@see findFile()
	 */
	[[nodiscard]] memory::RawData loadFile (const std::string_view& fileName) const noexcept;

private:

	const FileInfo* const m_begin;
	const FileInfo* const m_end;
};

}  // namespace binaries

LIMES_END_NAMESPACE
