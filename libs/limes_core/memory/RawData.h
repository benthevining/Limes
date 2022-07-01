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
#include <cstddef>	 // for size_t
#include <iostream>	 // for basic_istream, basic_ostream
#include <string>	 // for string
#include <string_view>
#include "../hashes/hash.h"	 // for hash::Type
#include "../misc/preprocessor.h"
#include <functional>  // for std::hash

/** @defgroup memory Memory
	Utilities for memory management.
	This module contains allocators, a memory pool, and more.
	@ingroup limes_core
 */

/** @dir libs/limes_core/memory
	This directory contains utilities for working with memory allocation and management.
	@ingroup memory
 */

/** @file
	This file defines the RawData class.
	@ingroup memory
 */

LIMES_BEGIN_NAMESPACE

/** This namespace contains utilities for working with memory allocation and management.
	@ingroup memory
 */
namespace memory
{

/** This class is a wrapper around a pointer to some raw, opaque data on the heap.
	@ingroup memory

	@todo refactor to use array_pointer internally?
	@todo allow for non-owning RawData objects that simply reference other memory
 */
class LIMES_EXPORT RawData final
{
public:

	/** @name Constructors */
	///@{

	/** Constructs an empty RawData object that owns no memory. */
	RawData() = default;

	/** Constructs a RawData objects that is allocated some initial memory, optionally initialized to zeroes.
		@throws std::bad_alloc An exception is thrown if the allocation of this object's internal memory fails.
	 */
	explicit RawData (std::size_t initialSize, bool initializeToZero = true);

	/** Constructs a RawData object that refers to the specified data.
		@attention The RawData object will take ownership of the passed pointer, so you should not delete it after using this constructor, or you'll get double-delete bugs!
	 */
	explicit RawData (char* dataToUse, std::size_t dataSize);

	/** Constructs a RawData object whose memory is initialized by reading the entire contents of the input stream.
		@throws std::bad_alloc An exception is thrown if the allocation of this object's internal memory fails.
	 */
	explicit RawData (std::basic_istream<char>& inputStream);

	/** Constructs a RawData object whose memory is initialized with the contents of the string.
		@throws std::bad_alloc An exception is thrown if the allocation of this object's internal memory fails.
	 */
	explicit RawData (const std::string_view& string);

	/** Copy constructor.
		@throws std::bad_alloc An exception is thrown if the allocation of this object's internal memory fails.
	 */
	explicit RawData (const RawData& other);

	/** Move constructor. */
	explicit RawData (RawData&& other) noexcept;

	///@}

	/** Destructor. */
	~RawData();

	/** @name Assignment */
	///@{
	RawData& operator= (RawData&& other);

	/**
		@throws std::bad_alloc An exception is thrown if the allocation of this object's internal memory fails.
	 */
	RawData& operator= (const RawData& other);
	///@}

	/** Creates a new RawData object with a copy of this one's data. */
	[[nodiscard]] RawData makeCopy() const;

	/** @name Data accessors
		Returns a pointer to this object's data.
	 */
	///@{
	[[nodiscard]] char*		  getData() noexcept;
	[[nodiscard]] const char* getData() const noexcept;
	///@}

	/** Returns the character at the specified position in the data.
		An assertion will be thrown if the index is out of range.
	 */
	char operator[] (std::size_t index) const noexcept;

	/** Returns the size (in bytes) of the data that this object owns. */
	[[nodiscard]] std::size_t getSize() const noexcept;

	/** Returns true if this data object is empty. */
	[[nodiscard]] bool isEmpty() const noexcept;

	/** Returns a string representation of the data this object contains. */
	[[nodiscard]] std::string toString() const noexcept;

	/** Writes this object's data to the passed output stream.
		@return True if writing to the stream succeeded.
	 */
	bool writeToStream (std::basic_ostream<char>& outputStream) const noexcept;

	/** Allocates new memory for this object.
		@param newSize The new total size of this object's internal memory, in bytes
		@param preserveOldData If true, the allocation will attempt to preserve any old memory that was allocated. If this is false, this function will free the old memory before allocating new memory.
		@param initializeToZero If true, all the newly allocated bytes are filled with zeroes.
		@throws std::bad_alloc An exception is thrown if the allocation of this object's internal memory fails.
	 */
	void allocate (std::size_t newSize, bool preserveOldData = true, bool initializeToZero = true);

	/** Frees all of this object's internal memory. */
	void free();

	/** Returns this object's memory pointer and releases ownership of it. This is analogous to \c std::unique_ptr::release()
		@attention Once this function is called, this object will not free this pointer, so you must do so!
	 */
	[[nodiscard]] char* release() noexcept;

	/** @name Appending
	 */
	///@{
	/**
		Appends some data to the end of this object's data, reallocating memory if necessary.
		@throws std::bad_alloc An exception is thrown if the allocation of this object's internal memory fails.
	 */
	void append (const char* const newData, std::size_t numBytes);
	void append (const RawData& other);
	///@}

	/** @name Prepending
	 */
	///@{
	/**
		Prepends some data to the beginning of this object's data, reallocating memory if necessary.
		@throws std::bad_alloc An exception is thrown if the allocation of this object's internal memory fails.
	 */
	void prepend (const char* const newData, std::size_t numBytes);
	void prepend (const RawData& other);
	///@}

	/** @name Overwriting
	 */
	///@{
	/**
		Overwrites this object's data with the input data, reallocating memory if necessary.
		@throws std::bad_alloc An exception is thrown if the allocation of this object's internal memory fails.
	 */
	void copyFrom (const char* const newData, std::size_t newSize);
	void copyFrom (const RawData& other);
	///@}

	/** @name Copying
		Copies this object's data to the specified destination.
	 */
	///@{
	/** Writes this object's data to the specified destination.
		@param dest The destination to copy this object's data to.
		@param maxNumBytes The maximum number of bytes this function will copy to the output.
	 */
	void copyTo (char* const dest, std::size_t maxNumBytes) const;

	/** Writes this object's data to the specified destination.
		@param other %RawData object to copy this object's memory into.
		@param allowAllocation If this is false, the data may be truncated if the destination object has a smaller preallocated memory size than the data size.
	 */
	void copyTo (RawData& other, bool allowAllocation = true) const;

	/** Returns a new %RawData object that contains a copy of this object's data. */
	[[nodiscard]] RawData createCopy() const;
	///@}

	/** Writes the specified value to every byte in this object's owned memory. */
	void fill (char fillWith);

	/** Fills this object's owned memory with zeroes. */
	void zero();

	/** @name begin() accessors
		Returns a pointer to the beginning of this object's owned memory.
		The returned pointer may be null if this object is empty.
	 */
	///@{
	char*		begin() noexcept;
	const char* begin() const noexcept;
	///@}

	/** @name end() accessors
		Returns a pointer to the end of this object's owned memory.
	 */
	///@{
	char*		end() noexcept;
	const char* end() const noexcept;
	///@}

	/** Deallocates any previously owned memory, and takes ownership of the passed pointer.
		@attention The RawData object will take ownership of the passed pointer, so you should not delete it manually after calling this function, or you'll get double-delete bugs!
	 */
	void setData (char* dataToUse, std::size_t dataSize);

	/** Computes a hash value for this object's data, based on the specified hash type. */
	[[nodiscard]] std::string hash (hash::Type hashType) const;

private:

	void throwOnAllocationFailure() const;

	std::size_t size { 0 };

	char* data { nullptr };
};

}  // namespace memory

LIMES_END_NAMESPACE

namespace std
{

/** A specialization of \c std::hash for RawData objects.
	The hash value is calculated based on the string representation of the data.
 */
template <>
struct LIMES_EXPORT hash<limes::memory::RawData> final
{
	hash() = default;

	LIMES_DEFAULT_COPYABLE (hash)
	LIMES_DEFAULT_MOVABLE (hash)

	size_t operator() (const limes::memory::RawData& d) const noexcept;
};

}  // namespace std
