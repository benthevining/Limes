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

#include "../limes_namespace.h"
#include <limes_export.h>
#include "./MemoryPool.h"

/** @file
	This file defines the MemoryPoolPointer class.
	@ingroup memory
 */

LIMES_BEGIN_NAMESPACE

namespace memory
{

/** A smart pointer to an object that lives within a MemoryPool.
	@tparam ObjectType The type of the owned object
	@tparam AllowOverflowAllocation If true, then if allocating from the memory pool fails, this class will fall back to using a regular \c new call. If this parameter is false, then if allocating from the pool fails, this object will be in a null state.
	@ingroup memory
	@see MemoryPool
 */
template <typename ObjectType, bool AllowOverflowAllocation = false>
class LIMES_EXPORT MemoryPoolPointer final
{
public:
	/** You can access the type of the owned object via this typedef. */
	using OwnedType = ObjectType;

	/** Creates the owned object by allocating its memory from the passed memory pool.
		If allocation from the pool is unsuccessful and the template parameter \c AllowOverflowAllocation is true, then this class will fall back to calling \c new to allocate the owned object.
		@param pool Memory pool to use
		@param constructorArgs Arguments to forward to the constructor of \c ObjectType
	 */
	template <typename... Args>
	explicit MemoryPoolPointer (MemoryPool& pool, Args&&... constructorArgs) noexcept ((! AllowOverflowAllocation) && noexcept (ObjectType (std::forward<Args> (constructorArgs)...)));

	/** Destructor.
		This destroys the owned object and frees its memory.
	 */
	~MemoryPoolPointer();

	LIMES_NON_COPYABLE (MemoryPoolPointer)
	LIMES_DEFAULT_MOVABLE (MemoryPoolPointer)

	/** Accessors
		Returns a pointer to the owned object.
	 */
	///@{
	[[nodiscard]] ObjectType* get() const noexcept { return object; }
	[[nodiscard]] ObjectType* operator()() const noexcept { return object; }
							  operator ObjectType*() const noexcept { return object; }
	ObjectType*				  operator->() const noexcept { return object; }
	///@}

	/** Returns a reference to the owned object.
		Be careful, the owned pointer may be null!
	 */
	[[nodiscard]] ObjectType& operator*() const
	{
		LIMES_ASSERT (object != nullptr);
		return *object;
	}

	/** Returns true if the object exists and was allocated from the memory pool. */
	[[nodiscard]] bool isOwnedByPool() const noexcept;

private:
	MemoryPool& memoryPool;

	ObjectType* object;
};

/*-----------------------------------------------------------------------------------------------------------------*/

template <typename ObjectType, bool AllowOverflowAllocation>
template <typename... Args>
MemoryPoolPointer<ObjectType, AllowOverflowAllocation>::MemoryPoolPointer (MemoryPool& pool, Args&&... constructorArgs) noexcept ((! AllowOverflowAllocation) && noexcept (ObjectType (std::forward<Args> (constructorArgs)...)))
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
		return object != nullptr;
	else
	{
		if (object == nullptr)
			return false;

		return memoryPool.owns (*object);
	}
}

}  // namespace memory

LIMES_END_NAMESPACE
