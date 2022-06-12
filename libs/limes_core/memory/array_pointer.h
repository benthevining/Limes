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
#include "../misc/preprocessor.h"
#include <cstdlib>
#include <new>
#include <exception>

/** @file
	This file defines the array_pointer class.
	@ingroup memory
 */

LIMES_BEGIN_NAMESPACE

namespace memory
{

/** This class is a simple RAII wrapper around a pointer allocated on the heap.

	So, instead of writing:
	@code
	auto* intArray = static_cast<int*>(std::malloc(50 * sizeof(int)));
	@endcode
	You can just do:
	@code
	auto intArray = limes::memory::array_pointer<int>{ 50 };
	@endcode

	@ingroup memory
 */
template <typename Type, bool UseExceptions = false>
class LIMES_EXPORT array_pointer final
{
public:

	/** You can access the type of the owned object using this typdef. */
	using OwnedType = Type;

	/** Creates a pointer that owns no memory. */
	constexpr array_pointer() = default;

	/** Allocates an array with the specified number of elements. */
	explicit array_pointer (std::size_t arraySize) noexcept (! UseExceptions)
		: ptr (static_cast<Type*> (std::malloc (arraySize * sizeof (Type)))), size (static_cast<int> (arraySize))
	{
		if constexpr (UseExceptions)
			if (ptr == nullptr)
				throw std::bad_alloc();
	}

	/** Destructor. */
	~array_pointer()
	{
		free();
	}

	LIMES_NON_COPYABLE (array_pointer)
	LIMES_DEFAULT_MOVABLE (array_pointer)

	/** @name Accessors
	 */
	///@{
	/** Returns a pointer to the first object in the owned array. */
	[[nodiscard]] Type* get() const noexcept { return ptr; }
	[[nodiscard]] Type* operator()() const noexcept { return ptr; }
						operator Type*() const noexcept { return ptr; }
	Type*				operator->() const noexcept { return ptr; }
	///@}

	/** Returns the size of the pointed-to array. */
	int getSize() const noexcept { return size; }

	/** Frees the allocated memory. */
	void free() noexcept (! UseExceptions)
	{
		if constexpr (UseExceptions)
			std::free (ptr);
		else
		{
			try
			{
				std::free (ptr);
			}
			catch (std::exception&)
			{
			}
		}

		size = 0;
		ptr	 = nullptr;
	}

	/** Reallocates the array's memory. */
	bool reallocate (std::size_t newSize) noexcept (! UseExceptions)
	{
		if (newSize == size)
			return true;

		if (newSize == 0)
		{
			free();
			return true;
		}

		if constexpr (UseExceptions)
		{
			ptr = static_cast<Type*> (std::realloc (ptr, newSize * sizeof (Type)));

			if (ptr == nullptr)
				throw std::bad_alloc();
		}
		else
		{
			try
			{
				ptr = static_cast<Type*> (std::realloc (ptr, newSize * sizeof (Type)));
			}
			catch (std::exception&)
			{
				return false;
			}
		}

		if (ptr == nullptr)
			return false;

		size = newSize;
		return true;
	}

	/** Indexes into the array. */
	decltype (auto) operator[] (int i) { return ptr[static_cast<std::size_t> (i)]; }

private:

	Type* ptr { nullptr };

	int size { 0 };
};

}  // namespace memory

LIMES_END_NAMESPACE
