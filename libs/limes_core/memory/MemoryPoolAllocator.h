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

#include "MemoryPool.h"
#include <limes_namespace.h>
#include <limes_export.h>
#include <cstdlib>
#include "../misc/preprocessor.h"

LIMES_BEGIN_NAMESPACE

template <typename T, std::size_t MaxCapacity>
class LIMES_EXPORT MemoryPoolAllocator final
{
public:

	using pointer		  = T*;
	using const_pointer	  = const T*;
	using reference		  = T&;
	using const_reference = const T&;
	using value_type	  = T;
	using size_type		  = std::size_t;
	using difference_type = ptrdiff_t;

	MemoryPoolAllocator()
		: pool (MaxCapacity * sizeof (T), sizeof (T))
	{
	}

	LIMES_NON_COPYABLE (MemoryPoolAllocator);
	LIMES_NON_MOVABLE (MemoryPoolAllocator);

	bool operator== (const MemoryPoolAllocator& other) const noexcept
	{
		return pool.getMemoryRootLocation() == other.pool.getMemoryRootLocation();
	}

	bool operator!= (const MemoryPoolAllocator& other) const noexcept
	{
		return ! (*this == other);
	}

	[[nodiscard]] T* allocate (std::size_t num) noexcept
	{
		return static_cast<T*> (pool.allocate (num * sizeof (T)));
	}

	template <typename U>
	[[nodiscard]] T* allocate (size_t num, const U*) noexcept
	{
		return allocate (num);
	}

	void deallocate (T* const ptr, std::size_t num) noexcept
	{
		pool.deallocate (static_cast<void*> (ptr), num * sizeof (T));
	}

	[[nodiscard]] T* address (T& r) const noexcept
	{
		return &r;
	}

	[[nodiscard]] const T* address (const T& s) const noexcept
	{
		return &s;
	}

	[[nodiscard]] std::size_t max_size() const noexcept
	{
		return pool.getTotalSize();
	}

	template <typename U>
	struct rebind final
	{
		using other = MemoryPoolAllocator<U, MaxCapacity>;
	};

	void construct (T* const ptr, const T& t) const
	{
		void* const voidPtr = static_cast<void*> (ptr);
		new (voidPtr) T (t);
	}

	template <typename... Args>
	void construct (T* const ptr, Args&&... args) const
	{
		void* const voidPtr = static_cast<void*> (ptr);
		new (voidPtr) T (std::forward<Args> (args)...);
	}

	void destroy (T* const ptr) const
	{
		ptr->~T();
	}

private:

	MemoryPool pool;
};

LIMES_END_NAMESPACE