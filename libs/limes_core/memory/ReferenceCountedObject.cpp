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

#include "./ReferenceCountedObject.h"
#include "../system/limes_assert.h"
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace memory
{

ReferenceCountedObject::~ReferenceCountedObject()
{
	// it's dangerous to delete an object that's still referenced by something else!
	LIMES_ASSERT (getRefCount() == 0);
}

ReferenceCountedObject::ReferenceCountedObject (const ReferenceCountedObject&) noexcept { }

/** Copying from another object does not affect this one's reference-count. */
ReferenceCountedObject::ReferenceCountedObject (ReferenceCountedObject&&) noexcept { }

/** Copying from another object does not affect this one's reference-count. */
ReferenceCountedObject& ReferenceCountedObject::operator= (const ReferenceCountedObject&) noexcept { return *this; }

/** Copying from another object does not affect this one's reference-count. */
ReferenceCountedObject& ReferenceCountedObject::operator= (ReferenceCountedObject&&) noexcept { return *this; }

int ReferenceCountedObject::incRefCount() noexcept
{
	return ++refCount;
}

bool ReferenceCountedObject::decRefCount() noexcept
{
	LIMES_ASSERT (getRefCount() > 0);

	if (--refCount > 0)
		return false;

	delete this;
	return true;
}

bool ReferenceCountedObject::decRefCountWithoutDeleting() noexcept
{
	LIMES_ASSERT (getRefCount() > 0);
	return --refCount == 0;
}

int ReferenceCountedObject::getRefCount() const noexcept
{
	return refCount.load();
}

}  // namespace memory

LIMES_END_NAMESPACE
