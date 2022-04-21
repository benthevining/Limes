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

#include "MemoryPool.h"
#include <limes_namespace.h>
#include <limes_platform.h>

LIMES_BEGIN_NAMESPACE

MemoryPool::MemoryPool (size_t storageSizeBytes)
	: sizeBytes (storageSizeBytes), memory (static_cast<char*> (malloc (storageSizeBytes))), marker (memory)
{
}

MemoryPool::~MemoryPool()
{
	free (memory);
}

size_t MemoryPool::getCurrentPosition() const noexcept
{
	return reinterpret_cast<size_t> (marker);
}

size_t MemoryPool::getTotalSize() const noexcept
{
	return sizeBytes;
}

size_t MemoryPool::getRemainingSpace() const noexcept
{
	return sizeBytes - getCurrentPosition();
}

void* MemoryPool::allocate (size_t numBytesToAllocate, size_t alignmentBytes)
{
	LIMES_ASSERT (marker != nullptr);

	if (numBytesToAllocate > getRemainingSpace())
		return nullptr;

	const auto allocationSize = [numBytesToAllocate, alignmentBytes]() -> size_t
	{
		const auto remainder = static_cast<size_t> (numBytesToAllocate % alignmentBytes);

		if (remainder != 0)
			return numBytesToAllocate + (alignmentBytes - remainder);

		return numBytesToAllocate;
	}();

	const auto pos = getCurrentPosition() + allocationSize - reinterpret_cast<size_t> (memory);

	if (pos > 0 && pos < sizeBytes)
	{
		auto* const newAddress = static_cast<void*> (marker);
		marker += allocationSize;
		return newAddress;
	}

	LIMES_ASSERT_FALSE;
	return nullptr;
}

void MemoryPool::deallocate (void* ptr, size_t numBytes)
{
	LIMES_ASSERT (contains (ptr));

	// if the freed memory is at the end of the pool, and the marker is anywhere within the freed memory block,
	// move the marker backwards to the beginning of the freed block

	const auto ptr_pos = reinterpret_cast<size_t> (ptr);

	if ((ptr_pos + numBytes) == reinterpret_cast<size_t> (memory) + sizeBytes)
	{
		const auto mark_pos = getCurrentPosition();

		if (mark_pos >= ptr_pos && mark_pos <= ptr_pos + numBytes)
			marker = static_cast<char*> (ptr);
	}
}

bool MemoryPool::contains (void* ptr) const noexcept
{
	const auto mem_start = reinterpret_cast<size_t> (memory);
	return reinterpret_cast<size_t> (ptr) >= mem_start && reinterpret_cast<size_t> (ptr) < mem_start + sizeBytes;
}

LIMES_END_NAMESPACE
