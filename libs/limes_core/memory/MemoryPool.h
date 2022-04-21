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
#include <cstdint>

LIMES_BEGIN_NAMESPACE

class LIMES_EXPORT MemoryPool final
{
public:

	explicit MemoryPool (size_t storageSizeBytes);

	~MemoryPool();

	[[nodiscard]] size_t getTotalSize() const noexcept;

	template <typename ObjectType>
	[[nodiscard]] int getTotalCapacity() const noexcept
	{
		return static_cast<int> (getTotalSize() / sizeof (ObjectType));
	}

	[[nodiscard]] size_t getRemainingSpace() const noexcept;

	template <typename ObjectType>
	[[nodiscard]] int getRemainingCapacity() const noexcept
	{
		return static_cast<int> (getRemainingSpace() / sizeof (ObjectType));
	}

	[[nodiscard]] void* allocate (size_t numBytesToAllocate, size_t alignmentBytes = 32);

	template <typename ObjectType>
	[[nodiscard]] void* allocate (size_t alignmentBytes = alignof (ObjectType))
	{
		return allocate (sizeof (ObjectType), alignmentBytes);
	}

	template <typename ObjectType, typename... Args>
	[[nodiscard]] ObjectType* construct (Args&&... constructorArgs) const
	{
		using Type = typename std::remove_cv<ObjectType>::type;

		if (auto* address = allocate<Type>())
			return new (address) Type (std::forward<Args> (constructorArgs)...);

		return nullptr;
	}

	template <typename ObjectType, typename... Args>
	[[nodiscard]] ObjectType& constructOrBust (Args&&... constructorArgs) const
	{
		using Type = typename std::remove_cv<ObjectType>::type;

		if (auto* address = allocate<Type>())
			return *new (address) Type (std::forward<Args> (constructorArgs)...);

		return *new Type (std::forward<Args> (constructorArgs)...);
	}

	void deallocate (void* ptr, size_t numBytes);

	template <typename ObjectType>
	void deallocate (ObjectType* const object)
	{
		if (object != nullptr)
			deallocate (static_cast<void*> (&object), sizeof (object));
	}

	template <typename ObjectType>
	void destruct (ObjectType& object)
	{
		if (owns (object))
			deallocate (&object);

		object.~ObjectType();
	}

	[[nodiscard]] bool contains (void* ptr) const noexcept;

	template <typename ObjectType>
	[[nodiscard]] bool owns (const ObjectType& object)
	{
		return contains (static_cast<void*> (&object));
	}

private:

	[[nodiscard]] size_t getCurrentPosition() const noexcept;

	const size_t sizeBytes;

	char* memory;

	char* marker = nullptr;
};

LIMES_END_NAMESPACE
