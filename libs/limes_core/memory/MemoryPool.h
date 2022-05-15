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

#include <memory>				   // for addressof
#include <utility>				   // for forward
#include <limes_export.h>		   // for LIMES_EXPORT, LIMES_NO_EXPORT
#include <limes_namespace.h>	   // for LIMES_BEGIN_NAMESPACE, LIMES_END_...
#include <cstddef>				   // for size_t, byte
#include <exception>			   // for exception
#include <type_traits>			   // for remove_cv
#include <vector>				   // for vector
#include "../misc/preprocessor.h"  // for LIMES_NON_COPYABLE, LIMES_DEFAULT...
#include "../system/limes_assert.h"


LIMES_BEGIN_NAMESPACE

namespace memory
{

class LIMES_EXPORT MemoryPool final
{
public:

	explicit MemoryPool (std::size_t storageSizeBytes, std::size_t chunkSize = 32);

	template <typename ObjectType>
	static std::unique_ptr<MemoryPool> createFor (std::size_t desiredCapacity);

	~MemoryPool();

	LIMES_NON_COPYABLE (MemoryPool);
	LIMES_NON_MOVABLE (MemoryPool);

	[[nodiscard]] std::size_t getTotalSize() const noexcept;

	template <typename ObjectType>
	[[nodiscard]] int getTotalCapacity() const noexcept;

	[[nodiscard]] std::size_t getRemainingSpace() const noexcept;

	template <typename ObjectType>
	[[nodiscard]] int getRemainingCapacity() const noexcept;

	[[nodiscard]] void* allocate (std::size_t numBytesToAllocate) noexcept;

	template <typename ObjectType>
	[[nodiscard]] ObjectType* allocate() noexcept;

	template <typename ObjectType, typename... Args>
	[[nodiscard]] ObjectType* construct (Args&&... constructorArgs) noexcept;

	template <typename ObjectType, typename... Args>
	[[nodiscard]] ObjectType& constructOrBust (Args&&... constructorArgs);

	void deallocate (void* ptr, std::size_t numBytes) noexcept;

	template <typename ObjectType>
	void deallocate (ObjectType* const object) noexcept;

	template <typename ObjectType>
	void destruct (ObjectType& object) noexcept;

	[[nodiscard]] bool contains (void* ptr) const noexcept;

	template <typename ObjectType>
	[[nodiscard]] bool owns (const ObjectType& object) const noexcept;

	[[nodiscard]] const void* getMemoryRootLocation() const noexcept;

private:

	struct Chunk final
	{
		explicit Chunk (std::byte* const ptr) noexcept;

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

	LIMES_NON_COPYABLE (MemoryPoolPointer);

	LIMES_DEFAULT_MOVABLE (MemoryPoolPointer);

	[[nodiscard]] ObjectType* get() const noexcept { return object; }

	[[nodiscard]] ObjectType* operator()() const noexcept { return object; }

	operator ObjectType*() const noexcept { return object; }

	ObjectType* operator->() const noexcept { return object; }

	[[nodiscard]] ObjectType& operator*() const
	{
		LIMES_ASSERT (object != nullptr);
		return *object;
	}

	[[nodiscard]] bool isOwnedByPool() const noexcept;

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
ObjectType* MemoryPool::allocate() noexcept
{
	return static_cast<ObjectType*> (allocate (sizeof (ObjectType)));
}

template <typename ObjectType, typename... Args>
ObjectType* MemoryPool::construct (Args&&... constructorArgs) noexcept
{
	try
	{
		using Type = typename std::remove_cv<ObjectType>::type;

		if (auto* address = allocate<Type>())
			return std::construct_at (address, std::forward<Args> (constructorArgs)...);

		return nullptr;
	}
	catch (const std::exception&)
	{
		return nullptr;
	}
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
void MemoryPool::deallocate (ObjectType* const object) noexcept
{
	deallocate (static_cast<void*> (&object), sizeof (object));
}

template <typename ObjectType>
void MemoryPool::destruct (ObjectType& object) noexcept
{
	try
	{
		const auto* address = std::addressof (object);
		deallocate (address);
		std::destroy_at (address);
	}
	catch (std::exception&)
	{
		return;
	}
}

template <typename ObjectType>
bool MemoryPool::owns (const ObjectType& object) const noexcept
{
	return contains (static_cast<void*> (std::addressof (object)));
}

/*-----------------------------------------------------------------------------------------------------------------*/


template <typename ObjectType, bool AllowOverflowAllocation>
template <typename... Args>
MemoryPoolPointer<ObjectType, AllowOverflowAllocation>::MemoryPoolPointer (MemoryPool& pool, Args&&... constructorArgs)
	: memoryPool (pool)
{
	if constexpr (AllowOverflowAllocation)
		object = &(pool.constructOrBust (std::forward<Args> (constructorArgs)...));	 // cppcheck-suppress redundantAssignment
	else
	{
		object = pool.construct (std::forward<Args> (constructorArgs)...);	// cppcheck-suppress redundantAssignment
		LIMES_ASSERT (object != nullptr);
	}
}

template <typename ObjectType, bool AllowOverflowAllocation>
MemoryPoolPointer<ObjectType, AllowOverflowAllocation>::~MemoryPoolPointer()
{
	if (object != nullptr)
		memoryPool.destruct (*object);
}

template <typename ObjectType, bool AllowOverflowAllocation>
bool MemoryPoolPointer<ObjectType, AllowOverflowAllocation>::isOwnedByPool() const noexcept
{
	if constexpr (! AllowOverflowAllocation)
		return true;
	else
	{
		if (object == nullptr)
			return false;

		return memoryPool.owns (*object);
	}
}

}  // namespace memory

LIMES_END_NAMESPACE
