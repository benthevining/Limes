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
#include "../misc/preprocessor.h"
#include <cstdlib>

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

	@see md_array_pointer
	@ingroup memory

	@todo write unit tests
	@todo need to create an underlying shared memory class, to RAIIze the 'non-owning' pointers??
	@todo bool template param IsAligned?
	@todo ability to inject custom allocators/deallocators?
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

	/** Allocates an array with the specified number of elements.
		@throws std::bad_alloc An exception will be thrown if the UseExceptions template parameter is true and allocation fails.
	 */
	explicit array_pointer (std::size_t arraySize) noexcept (! UseExceptions);

	/** Creates a non-owning array pointer that refers to the passed memory. */
	explicit array_pointer (Type* memoryToReference, std::size_t arraySize) noexcept;
	///@}

	/** Destructor. */
	~array_pointer();

	/** @name Copying */
	///@{
	/** Copies the \c other pointer's memory into a new pointer. */
	array_pointer (const array_pointer& other) noexcept (! UseExceptions);

	/** Copies the \c other pointer's memory into this one.
		@see copyFrom()
	 */
	array_pointer& operator= (const array_pointer& other) noexcept (! UseExceptions);

	/** Copies the \c other pointer's memory into this one.
		@see operator=, copyTo()
	 */
	void copyFrom (const array_pointer& other, bool allowAllocation = false) noexcept (! UseExceptions);

	/**	Copies this pointer's memory into the \c other one.
		@see copyFrom()
	 */
	void copyTo (array_pointer& other, bool allowAllocation = false) const noexcept (! UseExceptions);
	///@}

	LIMES_DEFAULT_MOVABLE (array_pointer)

	/** @name Equality comparisons */
	///@{
	/** Returns true if the two array pointers reference the same base memory location. */
	template <typename OtherType, bool OtherExceptions>
	bool operator== (const array_pointer<OtherType, OtherExceptions>& other) const noexcept;

	/** Returns true if the two array pointers do not reference the same base memory location. */
	template <typename OtherType, bool OtherExceptions>
	bool operator!= (const array_pointer<OtherType, OtherExceptions>& other) const noexcept;

	/** Returns true if the passed pointer is this pointer's root memory location. */
	template <typename OtherType>
	bool operator== (OtherType* pointer) const noexcept;

	/** Returns true if the passed pointer is not this pointer's root memory location. */
	template <typename OtherType>
	bool operator!= (OtherType* pointer) const noexcept;

	/** Equality comparison specialization for nullptr. */
	bool operator== (decltype (nullptr)) const noexcept;

	/** Equality comparison specialization for nullptr. */
	bool operator!= (decltype (nullptr)) const noexcept;
	///@}

	/** Fills the array with the specified value.
		@see clear()
	 */
	void fill (Type valueToFillWith);

	/** Fills the array with zeroes.
		@see fill()
	 */
	void clear();

	/** Deallocates this pointer's memory, and assigns it to refer to the passed pointer.
		This object will not take ownership of the underlying memory.
	 */
	array_pointer& referenceOtherMemory (Type* memoryToReference, std::size_t arraySize) noexcept (! UseExceptions);

	/** @name Accessors
	 */
	///@{
	/** Returns a pointer to the first object in the owned array. */
	[[nodiscard]] Type* get() const noexcept { return ptr; }
	[[nodiscard]] Type* operator()() const noexcept { return ptr; }
						operator Type*() const noexcept { return ptr; }
	Type*				operator->() const noexcept { return ptr; }
	///@}

	/** Returns the size of the pointed-to array, in number of objects. */
	std::size_t getSize() const noexcept { return size; }

	/** Returns the size of the pointed-to array, in bytes. */
	std::size_t getSizeBytes() const noexcept { return size * sizeof (Type); }

	/** Frees the allocated memory.
		If this is a non-owning array pointer that simply references some other memory, then calling this function does nothing.
	 */
	void free() noexcept (! UseExceptions);

	/** Reallocates the array's memory.
		@throws std::bad_alloc An exception will be thrown if the UseExceptions template parameter is true and allocation fails.
	 */
	bool reallocate (std::size_t newSize) noexcept (! UseExceptions);

	/** Indexes into the array. */
	template <typename T>
	decltype (auto) operator[] (T i);

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
	void throwOrAssertIfAllocationFailed() const noexcept (! UseExceptions);

	Type* ptr { nullptr };

	std::size_t size { 0 };

	bool owning { true };
};

}  // namespace memory

LIMES_END_NAMESPACE

#include "./array_pointer_impl.h"  // IWYU pragma: export
