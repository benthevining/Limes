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

#include "./RawData.h"
#include <iterator>	 // for istreambuf_iterator
#include "../limes_namespace.h"
#include <cstdlib>	// for malloc, calloc, free
#include <cstring>	// for memcpy, memset
#include <new>		// for bad_alloc
#include <string>	// for char_traits
#include <string_view>
#include "../hashes/hash.h"	 // for hash
#include "../system/limes_assert.h"

LIMES_BEGIN_NAMESPACE

namespace memory
{

RawData::RawData (std::size_t initialSize, bool initializeToZero)
	: size (initialSize), data (static_cast<char*> (initializeToZero
														? std::calloc (initialSize, std::size_t (1))
														: std::malloc (initialSize)))
{
	throwOnAllocationFailure();
}

RawData::RawData (char* const dataToUse, std::size_t dataSize)
	: size (dataSize), data (dataToUse)
{
}

RawData::RawData (std::basic_istream<char>& inputStream)
	: RawData (std::string { std::istreambuf_iterator<char> { inputStream }, {} })
{
}

RawData::RawData (const std::string_view& string)
	: size (static_cast<std::size_t> (string.size())),
	  data (static_cast<char*> (std::malloc (size)))
{
	throwOnAllocationFailure();
	std::memcpy (static_cast<void*> (data), static_cast<const void*> (string.data()), size);
}

RawData::RawData (const RawData& other)
	: size (other.size), data (static_cast<char*> (std::malloc (size)))
{
	throwOnAllocationFailure();
	std::memcpy (static_cast<void*> (data), static_cast<const void*> (other.data), size);
}

RawData::RawData (RawData&& other) noexcept
	: size (other.size), data (other.release())
{
}

RawData::~RawData()
{
	std::free (data);
}

RawData& RawData::operator= (const RawData& other)	// cppcheck-suppress operatorEqVarError
{
	if (other.isEmpty())
	{
		free();
		return *this;
	}

	size = other.size;

	data = static_cast<char*> (data == nullptr
								   ? std::malloc (size)
								   : std::realloc (static_cast<void*> (data), size));

	throwOnAllocationFailure();

	std::memcpy (static_cast<void*> (data), static_cast<const void*> (other.data), size);

	return *this;
}

RawData& RawData::operator= (RawData&& other)  // cppcheck-suppress operatorEqVarError
{
	size = other.size;
	data = other.release();

	return *this;
}

RawData RawData::makeCopy() const
{
	return RawData { *this };
}

bool RawData::writeToStream (std::basic_ostream<char>& outputStream) const noexcept
{
	try
	{
		outputStream.write (data, static_cast<std::streamsize> (size));
		return true;
	}
	catch (...)
	{
		return false;
	}
}

void RawData::allocate (std::size_t newSize, bool preserveOldData, bool initializeToZero)
{
	if (size == newSize)
		return;

	if (newSize == 0)
	{
		free();
		return;
	}

	size = newSize;

	if (preserveOldData)
	{
		data = static_cast<char*> ([d = data, initializeToZero, newSize]
								   {
			if (d == nullptr)
			{
				if (initializeToZero)
					return std::calloc (newSize, std::size_t (1));

				return std::malloc (newSize);
			}

			return std::realloc (static_cast<void*>(d), newSize); }());
	}
	else
	{
		std::free (data);

		data = static_cast<char*> (initializeToZero
									   ? std::calloc (size, std::size_t (1))
									   : std::malloc (size));
	}

	throwOnAllocationFailure();
}

void RawData::free()
{
	std::free (data);
	size = 0;
	data = nullptr;
}

char* RawData::release() noexcept
{
	auto* const ptr = data;

	data = nullptr;
	size = 0;

	return ptr;
}

void RawData::append (const char* const newData, std::size_t numBytes)
{
	if (newData == nullptr || numBytes == 0)
		return;

	const auto oldSize = size;

	allocate (oldSize + numBytes, true, false);

	if (data != nullptr)
	{
		std::memcpy (static_cast<void*> (data + oldSize),
					 static_cast<const void*> (newData),
					 numBytes);
	}
}

void RawData::append (const RawData& other)
{
	append (other.data, other.size);
}

void RawData::prepend (const char* const newData, std::size_t numBytes)
{
	if (newData == nullptr || numBytes == 0)
		return;

	const RawData orig { *this };

	const auto oldSize = size;

	allocate (oldSize + numBytes, false, false);

	if (data != nullptr)
	{
		std::memcpy (static_cast<void*> (data),
					 newData,
					 numBytes);

		std::memcpy (static_cast<void*> (data + numBytes),
					 orig.getData(),
					 oldSize);
	}
}

void RawData::prepend (const RawData& other)
{
	prepend (other.getData(), other.getSize());
}

void RawData::copyFrom (const char* const newData, std::size_t newSize)
{
	if (newData == nullptr || newSize == 0)
	{
		free();
		return;
	}

	allocate (newSize, false, false);

	std::memcpy (static_cast<void*> (data),
				 static_cast<const void*> (newData),
				 newSize);
}

void RawData::copyFrom (const RawData& other)
{
	copyFrom (other.data, other.size);
}

void RawData::copyTo (char* const dest, std::size_t maxNumBytes) const
{
	std::memcpy (static_cast<void*> (dest),
				 static_cast<const void*> (data),
				 maxNumBytes);
}

void RawData::copyTo (RawData& other, bool allowAllocation) const
{
	if (allowAllocation)
	{
		other.copyFrom (*this);
		return;
	}

	copyTo (other.data, std::min (size, other.size));
}

RawData RawData::createCopy() const
{
	return RawData { *this };
}

void RawData::fill (char fillWith)
{
	std::memset (static_cast<void*> (data), static_cast<int> (fillWith), size);
}

void RawData::zero()
{
	fill ('0');
}

void RawData::setData (char* dataToUse, std::size_t dataSize)
{
	free();

	data = dataToUse;
	size = dataSize;
}

void RawData::throwOnAllocationFailure() const
{
	if (size == 0)
		return;

	if (data == nullptr)
		throw std::bad_alloc();
}

char* RawData::getData() noexcept
{
	return data;
}

const char* RawData::getData() const noexcept
{
	return data;
}

char* RawData::begin() noexcept
{
	return data;
}

const char* RawData::begin() const noexcept
{
	return data;
}

char* RawData::end() noexcept
{
	return data + size;
}

const char* RawData::end() const noexcept
{
	return data + size;
}

char RawData::operator[] (std::size_t index) const noexcept
{
	LIMES_ASSERT (index < size);

	return data[size];
}

std::size_t RawData::getSize() const noexcept
{
	return size;
}

std::string RawData::toString() const noexcept
{
	try
	{
		return { data, size };
	}
	catch (...)
	{
		return {};
	}
}

bool RawData::isEmpty() const noexcept
{
	return size == 0 || data == nullptr;
}

std::string RawData::hash (hash::Type hashType) const
{
	return hash::hash (hashType, data, size);
}

}  // namespace memory

LIMES_END_NAMESPACE

namespace std
{

size_t hash<limes::memory::RawData>::operator() (const limes::memory::RawData& d) const noexcept
{
	return hash<std::string> {}(d.toString());
}

}  // namespace std
