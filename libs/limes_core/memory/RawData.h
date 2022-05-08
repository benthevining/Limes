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

#include <limes_export.h>	  // for LIMES_EXPORT
#include <limes_namespace.h>  // for LIMES_BEGIN_NAMESPACE, LIMES_END_NAMESPACE
#include <cstddef>			  // for size_t
#include <iostream>			  // for basic_istream, basic_ostream
#include <string>			  // for string
#include "../hashes/hash.h"	  // for hash::Type

LIMES_BEGIN_NAMESPACE

class LIMES_EXPORT RawData final
{
public:

	RawData() = default;

	explicit RawData (std::size_t initialSize, bool initializeToZero = true);

	explicit RawData (char* const dataToUse, std::size_t dataSize);

	explicit RawData (std::basic_istream<char>& inputStream);

	explicit RawData (const std::string& string);

	explicit RawData (const RawData& other);

	explicit RawData (RawData&& other) noexcept;

	~RawData();

	RawData& operator= (const RawData& other);

	RawData& operator= (RawData&& other);

	[[nodiscard]] char* getData() noexcept;

	[[nodiscard]] const char* getData() const noexcept;

	[[nodiscard]] std::size_t getSize() const noexcept;

	[[nodiscard]] std::string toString() const;

	void writeToStream (std::basic_ostream<char>& outputStream) const;

	[[nodiscard]] bool isEmpty() const noexcept;

	void allocate (std::size_t newSize, bool preserveOldData = true, bool initializeToZero = true);

	void free();

	[[nodiscard]] char* release();

	void append (const char* const newData, std::size_t numBytes);

	void append (const RawData& other);

	void copyFrom (const char* const newData, std::size_t newSize);

	void copyFrom (const RawData& other);

	void copyTo (char* const dest, std::size_t maxNumBytes) const;

	void fill (char fillWith);

	void zero();

	char* begin() noexcept;

	const char* begin() const noexcept;

	char* end() noexcept;

	const char* end() const noexcept;

	[[nodiscard]] std::string hash (hash::Type hashType) const;

private:

	void throwOnAllocationFailure() const;

	std::size_t size { 0 };

	char* data { nullptr };
};

LIMES_END_NAMESPACE
