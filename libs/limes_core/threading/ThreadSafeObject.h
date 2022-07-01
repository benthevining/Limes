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

#include <atomic>
#include <array>
#include <type_traits>
#include <limes_export.h>
#include "../limes_namespace.h"
#include "../misc/preprocessor.h"

/** @file
	This file defines the ThreadSafeObject class.
	@ingroup threads
 */

LIMES_BEGIN_NAMESPACE

namespace threads
{

/** An object that holds a non-const reference to another object.
	@ingroup threads
 */
template <typename ObjectType>
struct LIMES_EXPORT ThreadedObjectWriter
{
	virtual ~ThreadedObjectWriter();

	[[nodiscard]] ObjectType& get() const noexcept { return object; }

	ObjectType* operator->() const noexcept { return &object; }

	ObjectType& object;

	LIMES_NON_COPYABLE (ThreadedObjectWriter)
	LIMES_NON_MOVABLE (ThreadedObjectWriter)

protected:

	explicit ThreadedObjectWriter (ObjectType& obj)
		: object (obj)
	{
	}
};

/** An object that holds a const reference to another object.
	@ingroup threads
 */
template <typename ObjectType>
struct LIMES_EXPORT ThreadedObjectReader
{
	explicit ThreadedObjectReader (const ObjectType& objectToReference)
		: object (objectToReference)
	{
	}

	virtual ~ThreadedObjectReader() = default;

	[[nodiscard]] const ObjectType& get() const noexcept { return object; }

	const ObjectType* operator->() const noexcept { return &object; }

	const ObjectType& object;

	LIMES_NON_COPYABLE (ThreadedObjectReader)
	LIMES_NON_MOVABLE (ThreadedObjectReader)
};

/** A wrapper class that makes an object thread safe.
	Internally, this class holds multiple copies of the owned object, one for each thread, and updates the other threads' copies any time changes are made.
	Although this class guarantees thread safety, this class does not guarantee realtime safety.
	@tparam ObjectType The type of object to be owned
	@tparam totalNumThreads The total number of threads that will be accessing the object. This is the number of copies of the object that will be maintained internally, and must be greater than 0.
	@ingroup threads
 */
template <typename ObjectType, size_t totalNumThreads = 2>
class LIMES_EXPORT ThreadSafeObject final
{
public:

	/** The type of a reader accessor. */
	using Reader = ThreadedObjectReader<ObjectType>;

	/** The type of a writer accessor. */
	using Writer = ThreadedObjectWriter<ObjectType>;

	static_assert (totalNumThreads > 0, "totalNumThreads must be greater than 0");

	static_assert (std::is_default_constructible_v<ObjectType>, "ObjectType must be default constructable");

	/** Constructor. */
	ThreadSafeObject() = default;

	LIMES_NON_COPYABLE (ThreadSafeObject)
	LIMES_NON_MOVABLE (ThreadSafeObject)

	/** Acquires a read-only copy of the owned object. */
	[[nodiscard]] Reader read (size_t threadIdx)
	{
		return Reader { getObjectCopyForThread (threadIdx) };
	}

	/** Acquires a writable copy of the owned object. */
	[[nodiscard]] Writer write (size_t threadIdx)
	{
		return Writer_impl { *this, threadIdx };
	}

	/** Returns true if acquiring read and write accessors for the owned object is lock-free. */
	[[nodiscard]] bool isLockFree() const noexcept
	{
		return state.is_lock_free();
	}

	/** Returns true if acquiring read and write accessors for the owned object is always lock-free. */
	[[nodiscard]] static constexpr bool isAlwaysLockFree() noexcept
	{
		return std::atomic<ObjectType>::is_always_lock_free;
	}

private:

	struct Writer_impl final : public Writer
	{
		explicit Writer_impl (ThreadSafeObject& tso, size_t threadIdx)
			: Writer (tso.getObjectCopyForThread (threadIdx)), parent (tso)
		{
		}

		~Writer_impl() final
		{
			parent.state.store (this->object);
		}

	private:

		ThreadSafeObject& parent;
	};

	// friend struct Writer_impl;

	[[nodiscard]] inline ObjectType& getObjectCopyForThread (size_t threadIdx) const noexcept
	{
		LIMES_ASSERT (threadIdx < totalNumThreads);

		copies[threadIdx] = state.load();

		return copies[threadIdx];
	}

	std::atomic<ObjectType> state;

	std::array<ObjectType, totalNumThreads> copies;
};

}  // namespace threads

LIMES_END_NAMESPACE
