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

#include "./alignment.h"
#include "../limes_namespace.h"
#include <limes_export.h>
#include "../misc/preprocessor.h"
#include <new>
#include <type_traits>
#include <stdexcept>
#include <memory>

/** @file
	This file defines the AlignedAllocator class.
	@ingroup memory
 */

LIMES_BEGIN_NAMESPACE

namespace memory
{

/** An STL-style allocator that allocates aligned pointers.
	@ingroup memory
	@see allocate_aligned, deallocate_aligned
 */
template <typename T, std::size_t Alignment = sizeof (T), bool UseExceptions = false>
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

	constexpr AlignedAllocator() = default;

	template <typename OtherType, std::size_t OtherAlignment, bool OtherExceptions>
	constexpr AlignedAllocator (const AlignedAllocator<OtherType, OtherAlignment, OtherExceptions>&) noexcept;

	LIMES_CONSTEXPR_COPYABLE (AlignedAllocator)
	LIMES_CONSTEXPR_MOVABLE (AlignedAllocator)

	consteval bool operator== (const AlignedAllocator&) const noexcept;
	consteval bool operator!= (const AlignedAllocator&) const noexcept;

	template <typename OtherType, std::size_t OtherAlignment, bool OtherExceptions>
	consteval bool operator== (const AlignedAllocator<OtherType, OtherAlignment, OtherExceptions>&) const noexcept;

	template <typename OtherType, std::size_t OtherAlignment, bool OtherExceptions>
	consteval bool operator!= (const AlignedAllocator<OtherType, OtherAlignment, OtherExceptions>&) const noexcept;

	[[nodiscard]] T* allocate (std::size_t num) const noexcept (! UseExceptions);

	template <typename U>
	[[nodiscard]] T* allocate (size_t num, const U*) const noexcept (! UseExceptions);

	void deallocate (T* const ptr, std::size_t) const noexcept (! UseExceptions);

	[[nodiscard]] T* address (T& r) const noexcept;

	[[nodiscard]] const T* address (const T& s) const noexcept;

	[[nodiscard]] static constexpr std::size_t max_size() noexcept;

	template <typename U>
	struct rebind final
	{
		using other = AlignedAllocator<U, Alignment, UseExceptions>;
	};

	void construct (T* const ptr, const T& t) const noexcept (! UseExceptions || std::is_nothrow_copy_constructible_v<T>);

	template <typename... Args>
	void construct (T* const ptr, Args&&... args) const noexcept (! UseExceptions);

	void destroy (T* const ptr) const noexcept;

	[[nodiscard]] static consteval size_t getAlignment() noexcept;
};


/*-------------------------------------------------------------------------------------------------------------------*/

template <typename T, std::size_t Alignment, bool UseExceptions>
template <typename OtherType, std::size_t OtherAlignment, bool OtherExceptions>
constexpr AlignedAllocator<T, Alignment, UseExceptions>::AlignedAllocator (const AlignedAllocator<OtherType, OtherAlignment, OtherExceptions>&) noexcept
{
	static_assert (OtherAlignment == Alignment);
	static_assert (OtherExceptions == UseExceptions);
}

template <typename T, std::size_t Alignment, bool UseExceptions>
consteval bool AlignedAllocator<T, Alignment, UseExceptions>::operator== (const AlignedAllocator&) const noexcept
{
	return true;
}

template <typename T, std::size_t Alignment, bool UseExceptions>
consteval bool AlignedAllocator<T, Alignment, UseExceptions>::operator!= (const AlignedAllocator&) const noexcept
{
	return false;
}

template <typename T, std::size_t Alignment, bool UseExceptions>
template <typename OtherType, std::size_t OtherAlignment, bool OtherExceptions>
consteval bool AlignedAllocator<T, Alignment, UseExceptions>::operator== (const AlignedAllocator<OtherType, OtherAlignment, OtherExceptions>&) const noexcept
{
	return Alignment == OtherAlignment;
}

template <typename T, std::size_t Alignment, bool UseExceptions>
template <typename OtherType, std::size_t OtherAlignment, bool OtherExceptions>
consteval bool AlignedAllocator<T, Alignment, UseExceptions>::operator!= (const AlignedAllocator<OtherType, OtherAlignment, OtherExceptions>&) const noexcept
{
	return Alignment != OtherAlignment;
}

template <typename T, std::size_t Alignment, bool UseExceptions>
T* AlignedAllocator<T, Alignment, UseExceptions>::allocate (std::size_t num) const noexcept (! UseExceptions)
{
	if (num == 0)
		return nullptr;

	if (num > max_size())
	{
		if constexpr (UseExceptions)
			throw std::bad_alloc();	 // cppcheck-suppress throwInNoexceptFunction
		else
			return nullptr;
	}

	auto* const result = allocate_aligned<T> (num, Alignment);

	if constexpr (UseExceptions)
		if (result == nullptr)
			throw std::bad_alloc();

	return result;
}

template <typename T, std::size_t Alignment, bool UseExceptions>
template <typename U>
[[nodiscard]] T* AlignedAllocator<T, Alignment, UseExceptions>::allocate (size_t num, const U*) const noexcept (! UseExceptions)
{
	return allocate (num);	// cppcheck-suppress throwInNoexceptFunction
}

template <typename T, std::size_t Alignment, bool UseExceptions>
void AlignedAllocator<T, Alignment, UseExceptions>::deallocate (T* const ptr, std::size_t) const noexcept (! UseExceptions)
{
	if (! deallocate_aligned (ptr))
		if constexpr (UseExceptions)
			throw std::runtime_error ("Error in call to deallocate_aligned");  // cppcheck-suppress throwInNoexceptFunction
}

template <typename T, std::size_t Alignment, bool UseExceptions>
T* AlignedAllocator<T, Alignment, UseExceptions>::address (T& r) const noexcept
{
	return &r;
}

template <typename T, std::size_t Alignment, bool UseExceptions>
const T* AlignedAllocator<T, Alignment, UseExceptions>::address (const T& s) const noexcept
{
	return &s;
}

template <typename T, std::size_t Alignment, bool UseExceptions>
constexpr std::size_t AlignedAllocator<T, Alignment, UseExceptions>::max_size() noexcept
{
	return size_t (0) - size_t (1) / sizeof (T);
}

template <typename T, std::size_t Alignment, bool UseExceptions>
void AlignedAllocator<T, Alignment, UseExceptions>::construct (T* const ptr, const T& t) const noexcept (! UseExceptions || std::is_nothrow_copy_constructible_v<T>)
{
	if constexpr (UseExceptions || std::is_nothrow_copy_constructible_v<T>)
	{
		std::construct_at (ptr, t);
	}
	else
	{
		try
		{
			std::construct_at (ptr, t);
		}
		catch (std::exception&)
		{
		}
	}
}

template <typename T, std::size_t Alignment, bool UseExceptions>
template <typename... Args>
void AlignedAllocator<T, Alignment, UseExceptions>::construct (T* const ptr, Args&&... args) const noexcept (! UseExceptions)
{
	if constexpr (UseExceptions)
		std::construct_at (ptr, std::forward<Args> (args)...);
	else
	{
		try
		{
			std::construct_at (ptr, std::forward<Args> (args)...);
		}
		catch (std::exception&)
		{
		}
	}
}

template <typename T, std::size_t Alignment, bool UseExceptions>
void AlignedAllocator<T, Alignment, UseExceptions>::destroy (T* const ptr) const noexcept
{
	std::destroy_at (ptr);
}

template <typename T, std::size_t Alignment, bool UseExceptions>
consteval size_t AlignedAllocator<T, Alignment, UseExceptions>::getAlignment() noexcept
{
	return Alignment;
}

}  // namespace memory

LIMES_END_NAMESPACE
