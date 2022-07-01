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
#include <limes_platform.h>
#include <cstdlib>
#include <exception>
#include <new>
#include <memory>
#include "../misc/preprocessor.h"

#if LIMES_MSVC || LIMES_WINDOWS
#	include <malloc.h>
#endif

/** @file
	This file contains functions for aligned memory allocation.
	@ingroup memory
 */


LIMES_BEGIN_NAMESPACE

namespace memory
{

/** Allocates the desired number of objects with the desired alignment.
	On Windows or when using MSVC, \c _aligned_malloc will be used; otherwise, \c std::aligned_alloc will be used.
	@tparam T The type of objects to allocate
	@param count The number of objects to allocate
	@param alignment The alignment of the allocation
	@param args Arguments to forward to the constructor of \c T for each allocated object
	@see deallocate_aligned, aligned_pointer
	@ingroup memory
 */
template <typename T, typename... Args>
LIMES_EXPORT [[nodiscard]] T* allocate_aligned (std::size_t count = 1, std::size_t alignment = 32, Args&&... args) noexcept
{
	try
	{
		const auto sizeBytes = count * sizeof (T);

#if LIMES_MSVC || LIMES_WINDOWS
		auto* const ptr = _aligned_malloc (sizeBytes, alignment);
#else
		auto* const ptr = std::aligned_alloc (alignment, sizeBytes);
#endif

		if (ptr == nullptr)
			return nullptr;

		auto* const typed_ptr = static_cast<T*> (ptr);

		for (size_t i = 0; i < count; ++i)
			std::construct_at (typed_ptr + i, std::forward<Args> (args)...);

		return typed_ptr;
	}
	catch (const std::exception&)
	{
		return nullptr;
	}
}

/** Deallocates a pointer that was previously allocated with \c allocate_aligned() .
	@return False if deallocation throws an exception, true otherwise
	@see allocate_aligned, aligned_pointer
	@ingroup memory
 */
template <typename T>
LIMES_EXPORT bool deallocate_aligned (T* ptr) noexcept
{
	if (ptr == nullptr)
		return true;

	try
	{
#if LIMES_MSVC || LIMES_WINDOWS
		_aligned_free (ptr);
#else
		std::free (ptr);
#endif

		return true;
	}
	catch (const std::exception&)
	{
		return false;
	}
}

#pragma mark aligned_pointer

/** An RAII object that calls \c allocate_aligned() on construction and \c deallocate_aligned() on destruction.
	@tparam T The type of object that will be owned
	@tparam UseExceptions Whether allocation failure should throw an exception
	@ingroup memory
 */
template <typename T, bool UseExceptions = true>
class LIMES_EXPORT aligned_pointer final
{
public:

	/** You can access the type of the owned object using this typdef. */
	using OwnedType = T;

	/** Creates an aligned_pointer.
		@param count The number of \c T objects to allocate
		@param alignment The alignment of the allocation
		@param args Arguments to forward to the constructor of \c T for each allocated object
		@throws std::bad_alloc An exception is thrown if allocation fails and the \c UseExceptions template parameter is true.
	 */
	template <typename... Args>
	explicit aligned_pointer (std::size_t count = 1, std::size_t alignment = 32, Args&&... args) noexcept (! UseExceptions)
		: ptr (allocate_aligned<T> (count, alignment, std::forward<Args> (args)...))
	{
		if constexpr (UseExceptions)
			if (ptr == nullptr)
				throw std::bad_alloc();	 // cppcheck-suppress throwInNoexceptFunction
	}

	/** Destructor. */
	~aligned_pointer()
	{
		deallocate_aligned (ptr);
	}

	LIMES_NON_COPYABLE (aligned_pointer)
	LIMES_DEFAULT_MOVABLE (aligned_pointer)

	/** @name Accessors
		Returns a pointer to the owned object.
	 */
	///@{
	[[nodiscard]] T* get() const noexcept { return ptr; }
	[[nodiscard]] T* operator()() const noexcept { return ptr; }
					 operator T*() const noexcept { return ptr; }
	T*				 operator->() const noexcept { return ptr; }
	///@}

	/** Returns a reference to the owned object.
		Be careful, the owned pointer may be null!
	 */
	[[nodiscard]] T& operator*() const
	{
		LIMES_ASSERT (ptr != nullptr);
		return *ptr;
	}

	/** Indexes into the pointer like a C-style array. */
	[[nodiscard]] decltype (auto) operator[] (std::size_t idx) { return ptr[idx]; }

private:

	T* ptr;
};

}  // namespace memory

LIMES_END_NAMESPACE
