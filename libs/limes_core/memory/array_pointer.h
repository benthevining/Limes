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

	/** @name Constructors */
	///@{
	/** Creates a pointer that owns no memory. */
	constexpr array_pointer() = default;

	/** Allocates an array with the specified number of elements. */
	explicit array_pointer (std::size_t arraySize) noexcept (! UseExceptions)
		: ptr (static_cast<Type*> (std::malloc (arraySize * sizeof (Type)))), size (static_cast<int> (arraySize))
	{
		if constexpr (UseExceptions)
			if (ptr == nullptr)
				throw std::bad_alloc();	 // cppcheck-suppress throwInNoexceptFunction
	}

	/** Creates a non-owning array pointer that refers to the passed memory. */
	explicit array_pointer (Type* memoryToReference, std::size_t arraySize) noexcept
		: ptr (memoryToReference), size (arraySize), owning (false)
	{
	}
	///@}

	/** Destructor. */
	~array_pointer()
	{
		free();
	}

	/** @name Copying */
	///@{
	/** Creates a non-owning array pointer that references the same memory as the passed pointer. */
	array_pointer (const array_pointer& other) noexcept
		: ptr (other.ptr), size (other.size), owning (false)
	{
	}

	/** Deallocates this pointer's memory, and assigns it to refer to the memory owned by the other pointer. */
	array_pointer& operator= (const array_pointer& other) noexcept (! UseExceptions)  // cppcheck-suppress operatorEqVarError
	{
		return referenceOtherMemory (other.ptr, other.size);
	}
	///@}

	LIMES_DEFAULT_MOVABLE (array_pointer)

	/** Deallocates this pointer's memory, and assigns it to refer to the passed pointer. */
	array_pointer& referenceOtherMemory (Type* memoryToReference, std::size_t arraySize) noexcept (! UseExceptions)	 // cppcheck-suppress operatorEqRetRefThis
	{
		free();

		ptr	   = memoryToReference;
		size   = arraySize;
		owning = false;

		return *this;
	}

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
		if (! owning)
			return;

		if (ptr == nullptr || size == 0)
			return;

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

		size   = 0;
		ptr	   = nullptr;
		owning = false;
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
				throw std::bad_alloc();	 // cppcheck-suppress throwInNoexceptFunction
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

		size   = newSize;
		owning = true;

		return true;
	}

	/** @name Indexing */
	///@{
	/** Indexes into the array. */
	decltype (auto) operator[] (int i) { return ptr[static_cast<std::size_t> (i)]; }
	decltype (auto) operator[] (std::size_t i) { return ptr[i]; }
	///@}

	/** @name Begin accessors */
	///@{
	/** Returns an iterator to the beginning of the array. */
	Type*		begin() noexcept { return ptr; }
	const Type* begin() const noexcept { return ptr; }
	///@}

	/** @name End accessors */
	///@{
	Type*		end() noexcept { return nullptr; }
	const Type* end() const noexcept { return nullptr; }
	///@}

private:

	Type* ptr { nullptr };

	int size { 0 };

	bool owning { true };
};

}  // namespace memory

LIMES_END_NAMESPACE
