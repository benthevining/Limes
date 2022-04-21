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
#include <type_traits>
#include <cstddef>
#include <vector>

LIMES_BEGIN_NAMESPACE

class LIMES_EXPORT MemoryPool final
{
public:

	explicit MemoryPool (std::size_t storageSizeBytes, std::size_t chunkSize = 32);

	template <typename ObjectType>
	explicit MemoryPool (std::size_t desiredCapacity)
		: MemoryPool (desiredCapacity * sizeof (ObjectType), sizeof (ObjectType))
	{
	}

	~MemoryPool();

	MemoryPool (const MemoryPool&) = delete;

	MemoryPool (MemoryPool&&) = delete;

	MemoryPool& operator= (const MemoryPool&) = delete;

	MemoryPool& operator= (MemoryPool&&) = delete;

	[[nodiscard]] std::size_t getTotalSize() const noexcept;

	template <typename ObjectType>
	[[nodiscard]] int getTotalCapacity() const noexcept
	{
		return static_cast<int> (getTotalSize() / sizeof (ObjectType));
	}

	[[nodiscard]] std::size_t getRemainingSpace() const noexcept;

	template <typename ObjectType>
	[[nodiscard]] int getRemainingCapacity() const noexcept
	{
		return static_cast<int> (getRemainingSpace() / sizeof (ObjectType));
	}

	[[nodiscard]] void* allocate (std::size_t numBytesToAllocate);

	template <typename ObjectType>
	[[nodiscard]] void* allocate()
	{
		return allocate (sizeof (ObjectType));
	}

	template <typename ObjectType, typename... Args>
	[[nodiscard]] ObjectType* construct (Args&&... constructorArgs)
	{
		using Type = typename std::remove_cv<ObjectType>::type;

		if (auto* address = allocate<Type>())
			return new (address) Type (std::forward<Args> (constructorArgs)...);

		return nullptr;
	}

	template <typename ObjectType, typename... Args>
	[[nodiscard]] ObjectType& constructOrBust (Args&&... constructorArgs)
	{
		using Type = typename std::remove_cv<ObjectType>::type;

		if (auto* address = allocate<Type>())
			return *new (address) Type (std::forward<Args> (constructorArgs)...);

		return *new Type (std::forward<Args> (constructorArgs)...);
	}

	void deallocate (void* ptr, std::size_t numBytes);

	template <typename ObjectType>
	void deallocate (ObjectType* const object)
	{
		deallocate (static_cast<void*> (&object), sizeof (object));
	}

	template <typename ObjectType>
	void destruct (ObjectType& object)
	{
		deallocate (&object);
		object.~ObjectType();
	}

	[[nodiscard]] bool contains (void* ptr) const noexcept;

	template <typename ObjectType>
	[[nodiscard]] bool owns (const ObjectType& object)
	{
		return contains (static_cast<void*> (&object));
	}

	[[nodiscard]] const void* const getMemoryRootLocation() const noexcept;

private:

	struct Chunk final
	{
		explicit Chunk (char* const ptr);

		char* const location;

		bool isAvailable { true };
	};

	std::vector<Chunk> chunks;

	const std::size_t totalSizeBytes;
	const std::size_t chunkSizeBytes;

	char* const memory;
};

LIMES_END_NAMESPACE
