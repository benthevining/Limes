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

#include "alignment.h"
#include <limes_namespace.h>
#include <limes_export.h>

LIMES_BEGIN_NAMESPACE

template <typename T, std::size_t Alignment = 32>
class LIMES_EXPORT AlignedAllocator final
{
public:

	using pointer		  = T*;
	using const_pointer	  = const T*;
	using reference		  = T&;
	using const_reference = const T&;
	using value_type	  = T;
	using size_type		  = std::size_t;
	using difference_type = ptrdiff_t;

	AlignedAllocator() = default;

	AlignedAllocator (const AlignedAllocator&) = default;

	AlignedAllocator (AlignedAllocator&&) = default;

	template <typename OtherType>
	AlignedAllocator (const AlignedAllocator<OtherType>&)
	{
	}

	AlignedAllocator& operator= (const AlignedAllocator&) = delete;

	AlignedAllocator& operator= (AlignedAllocator&&) = delete;

	bool operator== (const AlignedAllocator& other) const noexcept
	{
		return Alignment == other.getAlignment();
	}

	bool operator!= (const AlignedAllocator& other) const noexcept
	{
		return ! (*this == other);
	}

	[[nodiscard]] T* allocate (std::size_t num) const
	{
		if (num == 0)
			return nullptr;

		if (num > max_size())
			return nullptr;

		return allocate_aligned<T> (num, Alignment);
	}

	template <typename U>
	[[nodiscard]] T* allocate (size_t num, const U*) const
	{
		return allocate (num);
	}

	void deallocate (T* const ptr, std::size_t) const
	{
		deallocate_aligned (ptr);
	}

	[[nodiscard]] T* address (T& r) const
	{
		return &r;
	}

	[[nodiscard]] const T* address (const T& s) const
	{
		return &s;
	}

	[[nodiscard]] std::size_t max_size() const
	{
		return (static_cast<size_t> (0) - static_cast<size_t> (1)) / sizeof (T);
	}

	template <typename U>
	struct rebind final
	{
		using other = AlignedAllocator<U, Alignment>;
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

	[[nodiscard]] size_t getAlignment() const noexcept
	{
		return Alignment;
	}
};

LIMES_END_NAMESPACE
