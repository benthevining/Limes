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
#include <cstdlib>
#include <cstddef>
#include "../misc/Algorithm.h"

LIMES_BEGIN_NAMESPACE

MemoryPool::MemoryPool (std::size_t storageSizeBytes, std::size_t chunkSize)
	: totalSizeBytes (storageSizeBytes), chunkSizeBytes (chunkSize), memory (static_cast<char*> (malloc (storageSizeBytes)))
{
	const auto totalNumChunks = static_cast<int> (std::ceil (static_cast<float> (totalSizeBytes) / static_cast<float> (chunkSizeBytes)));

	chunks.reserve (totalNumChunks);

	auto* location = memory;

	for (auto i = 0; i < totalNumChunks; ++i)
	{
		chunks.emplace_back (location);
		location += chunkSizeBytes;
	}
}

MemoryPool::~MemoryPool()
{
	free (memory);
}

std::size_t MemoryPool::getTotalSize() const noexcept
{
	return totalSizeBytes;
}

std::size_t MemoryPool::getRemainingSpace() const noexcept
{
	const auto availableChunks = alg::num_of (chunks, [] (const Chunk& c)
											  { return c.isAvailable; });

	return availableChunks * chunkSizeBytes;
}

void* MemoryPool::allocate (std::size_t numBytesToAllocate)
{
	const auto numChunks = static_cast<int> (std::ceil (static_cast<float> (numBytesToAllocate) / static_cast<float> (chunkSizeBytes)));

	// find first occurrence of numChunks consecutive chunks that are all available
	const auto startingChunkIdx = [numChunks, this]
	{
		for (auto firstChunkIdx = 0; firstChunkIdx + numChunks < chunks.size(); ++firstChunkIdx)
		{
			const auto thisChunkWorks = [numChunks, firstChunkIdx, this]
			{
				for (auto i = 0; i < numChunks; ++i)
					if (! chunks[firstChunkIdx + i].isAvailable)
						return false;

				return true;
			}();

			if (thisChunkWorks)
				return firstChunkIdx;
		}

		return -1;
	}();

	if (startingChunkIdx == -1)
		return nullptr;

	for (auto i = 0; i < numChunks; ++i)
	{
		LIMES_ASSERT (chunks[startingChunkIdx + i].isAvailable);
		chunks[startingChunkIdx + i].isAvailable = false;
	}

	return static_cast<void*> (chunks[startingChunkIdx].location);
}

void MemoryPool::deallocate (void* ptr, std::size_t numBytes)
{
	if (ptr == nullptr)
		return;

	if (! contains (ptr))
		return;

	const auto byteDistance = reinterpret_cast<intptr_t> (ptr) - reinterpret_cast<intptr_t> (memory);

	const auto startChunkIdx = static_cast<int> (std::ceil (static_cast<float> (byteDistance) / static_cast<float> (chunkSizeBytes)));

	LIMES_ASSERT (chunks[startChunkIdx].location == static_cast<char*> (ptr));

	const auto numChunks = static_cast<int> (numBytes * chunkSizeBytes);

	for (auto i = 0; i < numChunks; ++i)
	{
		LIMES_ASSERT (! chunks[startChunkIdx + i].isAvailable);
		chunks[startChunkIdx + i].isAvailable = true;
	}
}

bool MemoryPool::contains (void* ptr) const noexcept
{
	const auto mem_start = reinterpret_cast<std::size_t> (memory);
	return reinterpret_cast<std::size_t> (ptr) >= mem_start && reinterpret_cast<std::size_t> (ptr) < mem_start + totalSizeBytes;
}

const void* const MemoryPool::getMemoryRootLocation() const noexcept
{
	return static_cast<void*> (memory);
}

MemoryPool::Chunk::Chunk (char* const ptr)
	: location (ptr)
{
}

LIMES_END_NAMESPACE
