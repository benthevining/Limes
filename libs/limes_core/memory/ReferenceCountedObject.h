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

#include <limes_export.h>  // for LIMES_EXPORT
#include "../limes_namespace.h"
#include <atomic>				 // for atomic
#include <limes_platform.h>		 // for LIMES_ASSERT
#include "../meta/TypeTraits.h"	 // for concept inherits_from

/** @file
	This file defines the ReferenceCountedObject class.
	@ingroup memory
 */

LIMES_BEGIN_NAMESPACE

namespace memory
{

#pragma mark ReferenceCountedObject

/** An object that is reference-counted, and garbage collected once the reference count reaches 0.
	@ingroup memory
	@see ReferenceCountedObjectPtr
	@todo document this class
 */
class LIMES_EXPORT ReferenceCountedObject
{
public:
	/** Increments the object's reference count.
		@returns The new reference count, after the increment.
	 */
	int incRefCount() noexcept;

	/** Decrements the object's reference count.
		If the reference count reaches 0, the object will be deleted.
		@returns True if the object was deleted
		@see decRefCountWithoutDeleting
	 */
	bool decRefCount() noexcept;

	/** Decrements the object's reference count, but doesn't delete the object if the ref count reaches 0.
		@returns True if the reference count reaches 0.
		@see decRefCount
	 */
	bool decRefCountWithoutDeleting() noexcept;

	/** Returns the object's current reference count. */
	[[nodiscard]] int getRefCount() const noexcept;

protected:
	/** Creates the reference-counted object (with an initial ref count of zero). */
	ReferenceCountedObject() = default;

	/** Copying from another object does not affect this one's reference-count. */
	ReferenceCountedObject (const ReferenceCountedObject&) noexcept;

	/** Copying from another object does not affect this one's reference-count. */
	ReferenceCountedObject (ReferenceCountedObject&&) noexcept;

	/** Copying from another object does not affect this one's reference-count. */
	ReferenceCountedObject& operator= (const ReferenceCountedObject&) noexcept;

	/** Copying from another object does not affect this one's reference-count. */
	ReferenceCountedObject& operator= (ReferenceCountedObject&&) noexcept;

	/** Destructor. */
	virtual ~ReferenceCountedObject();

private:
	std::atomic<int> refCount { 0 };
};

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark ReferenceCountedObjectPtr

template <meta::inherits_from<ReferenceCountedObject> ObjectType>
class LIMES_EXPORT ReferenceCountedObjectPtr final
{
public:
	using ReferencedType = ObjectType;

	/** Creates a pointer to a null object. */
	ReferenceCountedObjectPtr() = default;

	/** Creates a pointer to an object.
		This will increment the object's reference-count.
	*/
	explicit ReferenceCountedObjectPtr (ReferencedType* refCountedObject) noexcept
		: referencedObject (refCountedObject)
	{
		incIfNotNull();
	}

	/** Creates a pointer to an object.
		This will increment the object's reference-count.
	*/
	explicit ReferenceCountedObjectPtr (ReferencedType& refCountedObject) noexcept
		: referencedObject (&refCountedObject)
	{
		refCountedObject.incRefCount();
	}

	/** Copies another pointer.
		This will increment the object's reference-count.
	*/
	explicit ReferenceCountedObjectPtr (const ReferenceCountedObjectPtr& other) noexcept
		: referencedObject (other.referencedObject)
	{
		incIfNotNull();
	}

	/** Takes-over the object from another pointer. */
	explicit ReferenceCountedObjectPtr (ReferenceCountedObjectPtr&& other) noexcept
		: referencedObject (other.referencedObject)
	{
		other.referencedObject = nullptr;
	}

	/** Copies another pointer.
		This will increment the object's reference-count (if it is non-null).
	*/
	template <typename Convertible>
	explicit ReferenceCountedObjectPtr (const ReferenceCountedObjectPtr<Convertible>& other) noexcept
		: referencedObject (other.get())
	{
		incIfNotNull();
	}

	/** Changes this pointer to point at a different object.
		The reference count of the old object is decremented, and it might be
		deleted if it hits zero. The new object's count is incremented.
	*/
	ReferenceCountedObjectPtr& operator= (const ReferenceCountedObjectPtr& other)
	{
		return operator= (other.referencedObject);
	}

	/** Changes this pointer to point at a different object.
		The reference count of the old object is decremented, and it might be
		deleted if it hits zero. The new object's count is incremented.
	*/
	template <typename Convertible>
	ReferenceCountedObjectPtr& operator= (const ReferenceCountedObjectPtr<Convertible>& other)
	{
		return operator= (other.get());
	}

	/** Changes this pointer to point at a different object.
		The reference count of the old object is decremented, and it might be
		deleted if it hits zero. The new object's count is incremented.
	*/
	ReferenceCountedObjectPtr& operator= (ReferencedType* newObject)
	{
		if (newObject != nullptr)
			return operator= (*newObject);

		reset();
		return *this;
	}

	/** Changes this pointer to point at a different object.
		The reference count of the old object is decremented, and it might be
		deleted if it hits zero. The new object's count is incremented.
	*/
	ReferenceCountedObjectPtr& operator= (ReferencedType& newObject)
	{
		if (referencedObject != &newObject)
		{
			newObject.incRefCount();
			auto* oldObject	 = referencedObject;
			referencedObject = &newObject;
			decIfNotNull (oldObject);
		}

		return *this;
	}

	/** Resets this pointer to a null pointer. */
	ReferenceCountedObjectPtr& operator= (decltype (nullptr))
	{
		reset();
		return *this;
	}

	/** Takes-over the object from another pointer. */
	ReferenceCountedObjectPtr& operator= (ReferenceCountedObjectPtr&& other) noexcept
	{
		std::swap (referencedObject, other.referencedObject);
		return *this;
	}

	/** Destructor.
		This will decrement the object's reference-count, which will cause the
		object to be deleted when the ref-count hits zero.
	*/
	~ReferenceCountedObjectPtr()
	{
		decIfNotNull (referencedObject);
	}

	//==============================================================================
	/** Returns the object that this pointer references.
		The pointer returned may be null, of course.
	*/
	[[nodiscard]] ReferencedType* get() const noexcept { return referencedObject; }

	[[nodiscard]] bool exists() const noexcept { return referencedObject != nullptr; }

	/** Resets this object to a null pointer. */
	void reset() noexcept
	{
		auto* oldObject	 = referencedObject;  // need to null the pointer before deleting the object
		referencedObject = nullptr;			  // in case this ptr is itself deleted as a side-effect
		decIfNotNull (oldObject);			  // of the destructor
	}

	// the -> operator is called on the referenced object
	ReferencedType* operator->() const noexcept
	{
		LIMES_ASSERT (referencedObject != nullptr);	 // null pointer method call!
		return referencedObject;
	}

	/** Dereferences the object that this pointer references.
		The pointer returned may be null, of course.
	*/
	ReferencedType& operator*() const noexcept
	{
		LIMES_ASSERT (referencedObject != nullptr);
		return *referencedObject;
	}

	/** Checks whether this pointer is null */
	bool operator== (decltype (nullptr)) const noexcept { return referencedObject == nullptr; }
	/** Checks whether this pointer is null */
	bool operator!= (decltype (nullptr)) const noexcept { return referencedObject != nullptr; }

	/** Compares two ReferenceCountedObjectPtrs. */
	bool operator== (const ObjectType* other) const noexcept { return referencedObject == other; }
	/** Compares two ReferenceCountedObjectPtrs. */
	bool operator== (const ReferenceCountedObjectPtr& other) const noexcept { return referencedObject == other.get(); }
	/** Compares two ReferenceCountedObjectPtrs. */
	bool operator!= (const ObjectType* other) const noexcept { return referencedObject != other; }
	/** Compares two ReferenceCountedObjectPtrs. */
	bool operator!= (const ReferenceCountedObjectPtr& other) const noexcept { return referencedObject != other.get(); }

	/** Checks whether this pointer is null */
	explicit operator bool() const noexcept { return referencedObject != nullptr; }

private:
	ReferencedType* referencedObject = nullptr;

	void incIfNotNull() noexcept
	{
		if (referencedObject != nullptr)
			referencedObject->incRefCount();
	}

	static void decIfNotNull (ReferencedType* o) noexcept
	{
		if (o != nullptr)
			o->decRefCount();
	}
};

#pragma mark Free functions

template <typename Type>
LIMES_EXPORT bool operator== (const Type* object1, const memory::ReferenceCountedObjectPtr<Type>& object2) noexcept
{
	return object1 == object2.get();
}

template <typename Type>
LIMES_EXPORT bool operator!= (const Type* object1, const memory::ReferenceCountedObjectPtr<Type>& object2) noexcept
{
	return object1 != object2.get();
}

}  // namespace memory

LIMES_END_NAMESPACE
