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

/** @file
	This file defines the MemoryPool class.
	@ingroup memory
 */

LIMES_BEGIN_NAMESPACE

namespace memory
{

#pragma mark MemoryPool

/** This class is a pool of preallocated memory that can be doled out in smaller chunks, allowing objects to be dynamically created and destroyed without incurring memory allocations.
	The pool's total size must be declared upon construction.
	Internally, the pool manages its memory using the concept of "chunks" -- each time some memory from the pool is requested, the pool rounds the requested number of bytes up to the next multiple of its chunk size, then it finds the first occurrence of the needed number of contiguous available chunks.
	If you're going to use a pool to create one kind of object, then I'd recommend making the chunk size the size of the object -- in fact, the function createFor() exists for just this purpose.
	It's also possible to store any number of disparate types within the same memory pool.
	@ingroup memory
	@see MemoryPoolPointer
 */
class LIMES_EXPORT MemoryPool final
{
public:

	/** Creates a MemoryPool.
		@param storageSizeBytes The total size of the preallocated memory the pool will own, in bytes. It is recommended for this to be a multiple of /c chunkSize .
		@param chunkSize The size of the individual memory "chunks" that can be doled out as needed. It is recommended for this to be a divisor of /c storageSizeBytes .
		@throws std::runtime_error An exception is thrown if the allocation of the memory pool's internal memory block fails.
	 */
	explicit MemoryPool (std::size_t storageSizeBytes, std::size_t chunkSize = 32);

	/** Creates a MemoryPool that can hold a specified number of objects of a certain type.
		The created pool's total size will be \c  desiredCapacity*sizeof(ObjectType) and its chunk size will be \c sizeof(ObjectType) .
		If initial memory allocation fails, a nullptr will be returned.
	 */
	template <typename ObjectType>
	static std::unique_ptr<MemoryPool> createFor (std::size_t desiredCapacity) noexcept;

	/** Destructor. */
	~MemoryPool();

	LIMES_NON_COPYABLE (MemoryPool)
	LIMES_NON_MOVABLE (MemoryPool)

	/** Returns the total size, in bytes, of the memory pool's preallocated memory. */
	[[nodiscard]] std::size_t getTotalSize() const noexcept;

	/** Returns the total size of this memory pool, expressed as the number of \c ObjectType objects it can hold. */
	template <typename ObjectType>
	[[nodiscard]] int getTotalCapacity() const noexcept;

	/** Returns the total remaining free space in the memory pool, in bytes. */
	[[nodiscard]] std::size_t getRemainingSpace() const noexcept;

	/** Returns the remaining space in the pool, expressed as the number of additional \c ObjectType objects it can hold. */
	template <typename ObjectType>
	[[nodiscard]] int getRemainingCapacity() const noexcept;

	/** Returns the pool's internal chunk size, in bytes. */
	[[nodiscard]] std::size_t getChunkSize() const noexcept;

	/** @name Allocation */
	///@{
	/** Allocates the desired number of bytes from the pool's preallocated memory.
		The actual number of bytes allocated will be rounded up to the next multiple of the pool's \c chunkSize .
		The allocated memory will be contiguous.
		@returns A pointer to the allocated memory location within the pool, or nullptr if allocation is unsuccessful
	 */
	[[nodiscard]] void* allocate (std::size_t numBytesToAllocate) noexcept;

	/** Allocates \c sizeof(ObjectType) bytes, and returns a typed pointer to the allocated memory location.
		Note that this function does not construct the object, it simply casts the \c void* to an \c ObjectType* .
		@see construct
	 */
	template <typename ObjectType>
	[[nodiscard]] ObjectType* allocate() noexcept;

	/** Allocates space for an object and constructs it in the allocated memory space.
		@param constructorArgs Arguments to forward to the constructor of \c ObjectType
		@return Pointer to the newly constructed object, or if allocation was unsuccessful, nullptr.
		@see constructOrBust
	 */
	template <typename ObjectType, typename... Args>
	[[nodiscard]] ObjectType* construct (Args&&... constructorArgs) noexcept (noexcept (ObjectType (std::forward<Args> (constructorArgs)...)));

	/** Similar to construct(), but if allocation from the memory pool fails, falls back to a regular \c new call.
		@param constructorArgs Arguments to forward to the constructor of \c ObjectType
		@see construct()
	 */
	template <typename ObjectType, typename... Args>
	[[nodiscard]] ObjectType& constructOrBust (Args&&... constructorArgs);
	///@}

	/** @name Deallocation */
	///@{
	/** Deallocates a certain number of bytes at a specific location within the pool. */
	void deallocate (void* ptr, std::size_t numBytes) noexcept;

	/** Deallocates the passed object.
		Note that this function does not call the object's destructor, it only frees its memory from the pool.
		@see destruct
	 */
	template <typename ObjectType>
	void deallocate (ObjectType* const object) noexcept;

	/** Calls an object's destructor and frees its memory from the pool. */
	template <typename ObjectType>
	void destruct (ObjectType& object) noexcept;
	///@}

	/** Returns true if the memory pool's preallocated memory contains the passed memory location. */
	[[nodiscard]] bool contains (void* ptr) const noexcept;

	/** Returns true if the pool owns the specified object -- ie, if the pointer to the object is within the pool's preallocated memory block. */
	template <typename ObjectType>
	[[nodiscard]] bool owns (const ObjectType& object) const noexcept;

	/** Returns the start location of the pool's preallocated memory block. */
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

#pragma mark Implementation

template <typename ObjectType>
std::unique_ptr<MemoryPool> MemoryPool::createFor (std::size_t desiredCapacity) noexcept
{
	try
	{
		return std::make_unique<MemoryPool> (desiredCapacity * sizeof (ObjectType), sizeof (ObjectType));
	}
	catch (std::exception&)
	{
		return nullptr;
	}
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
ObjectType* MemoryPool::construct (Args&&... constructorArgs) noexcept (noexcept (ObjectType (std::forward<Args> (constructorArgs)...)))
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

}  // namespace memory

LIMES_END_NAMESPACE
