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
#include <memory>

LIMES_BEGIN_NAMESPACE

class LIMES_EXPORT MemoryPool final
{
public:

	explicit MemoryPool (std::size_t storageSizeBytes, std::size_t chunkSize = 32);

	template <typename ObjectType>
	static std::unique_ptr<MemoryPool> createFor (std::size_t desiredCapacity);

	~MemoryPool();

	MemoryPool (const MemoryPool&) = delete;

	MemoryPool (MemoryPool&&) = delete;

	MemoryPool& operator= (const MemoryPool&) = delete;

	MemoryPool& operator= (MemoryPool&&) = delete;

	[[nodiscard]] std::size_t getTotalSize() const noexcept;

	template <typename ObjectType>
	[[nodiscard]] int getTotalCapacity() const noexcept;

	[[nodiscard]] std::size_t getRemainingSpace() const noexcept;

	template <typename ObjectType>
	[[nodiscard]] int getRemainingCapacity() const noexcept;

	[[nodiscard]] void* allocate (std::size_t numBytesToAllocate);

	template <typename ObjectType>
	[[nodiscard]] ObjectType* allocate();

	template <typename ObjectType, typename... Args>
	[[nodiscard]] ObjectType* construct (Args&&... constructorArgs);

	template <typename ObjectType, typename... Args>
	[[nodiscard]] ObjectType& constructOrBust (Args&&... constructorArgs);

	void deallocate (void* ptr, std::size_t numBytes);

	template <typename ObjectType>
	void deallocate (ObjectType* const object);

	template <typename ObjectType>
	void destruct (ObjectType& object);

	[[nodiscard]] bool contains (void* ptr) const noexcept;

	template <typename ObjectType>
	[[nodiscard]] bool owns (const ObjectType& object) const noexcept;

	[[nodiscard]] const void* const getMemoryRootLocation() const noexcept;

private:

	struct Chunk final
	{
		explicit Chunk (std::byte* const ptr);

		std::byte* const location;

		bool isAvailable { true };
	};

	std::vector<Chunk> chunks;

	const std::size_t totalSizeBytes;
	const std::size_t chunkSizeBytes;

	std::byte* const memory;
};

/*-----------------------------------------------------------------------------------------------------------------*/

template <typename ObjectType, bool AllowOverflowAllocation = false>
class LIMES_EXPORT MemoryPoolPointer final
{
public:

	using OwnedType = ObjectType;

	template <typename... Args>
	explicit MemoryPoolPointer (MemoryPool& pool, Args&&... constructorArgs);

	~MemoryPoolPointer();

	[[nodiscard]] ObjectType* get() const noexcept { return object; }

	[[nodiscard]] ObjectType* operator()() const noexcept { return object; }

	operator ObjectType*() const noexcept { return object; }

	ObjectType* operator->() const noexcept { return object; }

	[[nodiscard]] ObjectType& operator*() const
	{
		LIMES_ASSERT (object != nullptr);
		return *object;
	}

private:

	MemoryPool& memoryPool;

	ObjectType* object;
};

/*-----------------------------------------------------------------------------------------------------------------*/

template <typename ObjectType>
std::unique_ptr<MemoryPool> createFor (std::size_t desiredCapacity)
{
	return std::make_unique<MemoryPool> (desiredCapacity * sizeof (ObjectType), sizeof (ObjectType));
}

template <typename ObjectType>
int MemoryPool::getTotalCapacity() const noexcept
{
	return static_cast<int> (getTotalSize() / sizeof (ObjectType));
}

template <typename ObjectType>
int MemoryPool::getRemainingCapacity() const noexcept
{
	return static_cast<int> (getRemainingSpace() / sizeof (ObjectType));
}

template <typename ObjectType>
ObjectType* MemoryPool::allocate()
{
	return static_cast<ObjectType*> (allocate (sizeof (ObjectType)));
}

template <typename ObjectType, typename... Args>
ObjectType* MemoryPool::construct (Args&&... constructorArgs)
{
	using Type = typename std::remove_cv<ObjectType>::type;

	if (auto* address = allocate<Type>())
		return std::construct_at (address, std::forward<Args> (constructorArgs)...);

	return nullptr;
}

template <typename ObjectType, typename... Args>
ObjectType& MemoryPool::constructOrBust (Args&&... constructorArgs)
{
	using Type = typename std::remove_cv<ObjectType>::type;

	if (auto* address = allocate<Type>())
		return std::construct_at (address, std::forward<Args> (constructorArgs)...);

	return *new Type (std::forward<Args> (constructorArgs)...);
}

template <typename ObjectType>
void MemoryPool::deallocate (ObjectType* const object)
{
	deallocate (static_cast<void*> (&object), sizeof (object));
}

template <typename ObjectType>
void MemoryPool::destruct (ObjectType& object)
{
	const auto* address = std::addressof (object);
	deallocate (address);
	std::destroy_at (address);
}

template <typename ObjectType>
bool MemoryPool::owns (const ObjectType& object) const noexcept
{
	return contains (static_cast<void*> (std::addressof (object)));
}

/*-----------------------------------------------------------------------------------------------------------------*/


template <typename ObjectType, bool AllowOverflowAllocation, typename... Args>
LIMES_NO_EXPORT inline ObjectType* mem_pool_ptr_construct_object (MemoryPool& pool, Args&&... constructorArgs)
{
	if constexpr (AllowOverflowAllocation)
		return &pool.constructOrBust (std::forward<Args> (constructorArgs)...);
	else
		return pool.construct (std::forward<Args> (constructorArgs)...);
}


template <typename ObjectType, bool AllowOverflowAllocation>
template <typename... Args>
MemoryPoolPointer<ObjectType, AllowOverflowAllocation>::MemoryPoolPointer (MemoryPool& pool, Args&&... constructorArgs)
	: memoryPool (pool), object (mem_pool_ptr_construct_object (memoryPool, std::forward<Args> (constructorArgs)...))
{
	LIMES_ASSERT (object != nullptr);
}

template <typename ObjectType, bool AllowOverflowAllocation>
MemoryPoolPointer<ObjectType, AllowOverflowAllocation>::~MemoryPoolPointer()
{
	if (object != nullptr)
		memoryPool.destruct (*object);
}

LIMES_END_NAMESPACE
