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

// IWYU pragma: private
// IWYU pragma: friend array_pointer.h

#pragma once

#include "../limes_namespace.h"
#include <memory>
#include <new>
#include <utility>
#include <cstring>

/** @file
	This file contains implementation details for the array_pointer class.
	@see array_pointer.h
	@ingroup memory
 */

LIMES_BEGIN_NAMESPACE

namespace memory
{

template <typename Type, bool UseExceptions>
array_pointer<Type, UseExceptions>::array_pointer (std::size_t arraySize) noexcept (! UseExceptions)
	: ptr (static_cast<Type*> (std::malloc (arraySize * sizeof (Type)))), size (arraySize), owning (true)
{
	throwOrAssertIfAllocationFailed();
}

template <typename Type, bool UseExceptions>
array_pointer<Type, UseExceptions>::array_pointer (Type* memoryToReference, std::size_t arraySize) noexcept
	: ptr (memoryToReference), size (arraySize), owning (false)
{
}

template <typename Type, bool UseExceptions>
array_pointer<Type, UseExceptions>::array_pointer (const array_pointer& other) noexcept (! UseExceptions)
{
	copyFrom (other, true);
}

template <typename Type, bool UseExceptions>
array_pointer<Type, UseExceptions>::~array_pointer()
{
	free();
}

template <typename Type, bool UseExceptions>
array_pointer<Type, UseExceptions>& array_pointer<Type, UseExceptions>::operator= (const array_pointer& other) noexcept (! UseExceptions)  // cppcheck-suppress operatorEqVarError
{
	copyFrom (other, true);
}

template <typename Type, bool UseExceptions>
void array_pointer<Type, UseExceptions>::copyFrom (const array_pointer& other, bool allowAllocation) noexcept (! UseExceptions)
{
	if (allowAllocation)
		reallocate (std::max (getSize(), other.getSize()));

	std::memcpy (ptr, other.ptr, std::min (getSizeBytes(), other.getSizeBytes()));
}

template <typename Type, bool UseExceptions>
void array_pointer<Type, UseExceptions>::copyTo (array_pointer& other, bool allowAllocation) const noexcept (! UseExceptions)
{
	if (allowAllocation)
		other.reallocate (std::max (getSize(), other.getSize()));

	std::memcpy (other.ptr, ptr, std::min (getSizeBytes(), other.getSizeBytes()));
}

template <typename Type, bool UseExceptions>
template <typename OtherType, bool OtherExceptions>
bool array_pointer<Type, UseExceptions>::operator== (const array_pointer<OtherType, OtherExceptions>& other) const noexcept
{
	return (*this == other.ptr);
}

template <typename Type, bool UseExceptions>
template <typename OtherType, bool OtherExceptions>
bool array_pointer<Type, UseExceptions>::operator!= (const array_pointer<OtherType, OtherExceptions>& other) const noexcept
{
	return (*this != other.ptr);
}

template <typename Type, bool UseExceptions>
template <typename OtherType>
bool array_pointer<Type, UseExceptions>::operator== (OtherType* pointer) const noexcept
{
	return static_cast<void*> (std::to_address (ptr)) == static_cast<void*> (std::to_address (pointer));
}

template <typename Type, bool UseExceptions>
template <typename OtherType>
bool array_pointer<Type, UseExceptions>::operator!= (OtherType* pointer) const noexcept
{
	return ! (*this == pointer);
}

template <typename Type, bool UseExceptions>
bool array_pointer<Type, UseExceptions>::operator== (decltype (nullptr)) const noexcept
{
	return ptr == nullptr;
}

template <typename Type, bool UseExceptions>
bool array_pointer<Type, UseExceptions>::operator!= (decltype (nullptr)) const noexcept
{
	return ptr != nullptr;
}

template <typename Type, bool UseExceptions>
void array_pointer<Type, UseExceptions>::fill (Type valueToFillWith)
{
	std::memset (ptr, static_cast<int> (valueToFillWith), size);
}

template <typename Type, bool UseExceptions>
void array_pointer<Type, UseExceptions>::clear()
{
	fill (Type (0));
}

template <typename Type, bool UseExceptions>
array_pointer<Type, UseExceptions>& array_pointer<Type, UseExceptions>::referenceOtherMemory (Type* memoryToReference, std::size_t arraySize) noexcept (! UseExceptions)  // cppcheck-suppress operatorEqRetRefThis
{
	free();

	ptr	   = memoryToReference;
	size   = arraySize;
	owning = false;

	return *this;
}

template <typename Type, bool UseExceptions>
template <typename T>
decltype (auto) array_pointer<Type, UseExceptions>::operator[] (T i)
{
	return ptr[static_cast<std::size_t> (i)];
}

template <typename Type, bool UseExceptions>
void array_pointer<Type, UseExceptions>::free() noexcept (! UseExceptions)
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
		catch (...)
		{
		}
	}

	size   = 0;
	ptr	   = nullptr;
	owning = false;
}

template <typename Type, bool UseExceptions>
bool array_pointer<Type, UseExceptions>::reallocate (std::size_t newSize) noexcept (! UseExceptions)
{
	if (newSize == size)
		return true;

	if (newSize == 0)
	{
		free();
		return true;
	}

	auto alloc_ptr = [this, newSize]() -> Type*
	{
		if (owning)
			return static_cast<Type*> (std::realloc (ptr, newSize * sizeof (Type)));

		owning = true;

		return static_cast<Type*> (std::malloc (newSize * sizeof (Type)));
	};

	if constexpr (UseExceptions)
	{
		ptr = alloc_ptr();
	}
	else
	{
		try
		{
			ptr = alloc_ptr();
		}
		catch (...)
		{
			throwOrAssertIfAllocationFailed();

			return false;
		}
	}

	throwOrAssertIfAllocationFailed();

	if (ptr == nullptr)
		return false;

	size   = newSize;
	owning = true;

	return true;
}

template <typename Type, bool UseExceptions>
void array_pointer<Type, UseExceptions>::throwOrAssertIfAllocationFailed() const noexcept (! UseExceptions)
{
	if (ptr != nullptr)
		return;

	if constexpr (UseExceptions)
		throw std::bad_alloc {};
	else
		LIMES_ASSERT_FALSE;
}

}  // namespace memory

LIMES_END_NAMESPACE
