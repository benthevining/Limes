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
#include <limes_export.h>
#include <limes_vecops.h>

namespace limes::threads
{


template <typename ObjectType>
struct LIMES_EXPORT ThreadedObjectWriter
{
	virtual ~ThreadedObjectWriter();

	[[nodiscard]] ObjectType& get() const noexcept { return object; }

	ObjectType* operator->() const noexcept { return &object; }

	ObjectType& object;

protected:

	explicit ThreadedObjectWriter (ObjectType& obj)
		: object (obj)
	{
	}
};

template <typename ObjectType>
struct LIMES_EXPORT ThreadedObjectReader
{
	virtual ~ThreadedObjectReader() = default;

	[[nodiscard]] const ObjectType& get() const noexcept { return object; }

	const ObjectType* operator->() const noexcept { return &object; }

	const ObjectType& object;

protected:

	explicit ThreadedObjectReader (const ObjectType& objectToReference)
		: object (objectToReference)
	{
	}
};


template <typename ObjectType, size_t totalNumThreads = 2>
class LIMES_EXPORT ThreadSafeObject final
{
public:

	static_assert (totalNumThreads > 0, "");

	[[nodiscard]] const ObjectType& acquire_read (size_t threadIdx)
	{
		auto& obj = getObjectCopyForThread (threadIdx);
		obj		  = state.load();
		return obj;
	}

	[[nodiscard]] ThreadedObjectReader<ObjectType> read (size_t threadIdx)
	{
		return ThreadedObjectReader<ObjectType> (acquire_read (threadIdx));
	}

	[[nodiscard]] ObjectType& acquire_write (size_t threadIdx)
	{
		auto& obj = getObjectCopyForThread (threadIdx);
		obj		  = state.load();
		return obj;
	}

	void release_write (size_t threadIdx)
	{
		state.store (getObjectCopyForThread (threadIdx));
	}

	[[nodiscard]] ThreadedObjectWriter<ObjectType> write (size_t threadIdx)
	{
		struct Writer final : public ThreadedObjectWriter<ObjectType>
		{
			explicit Writer (ThreadSafeObject& tso, size_t threadIdx)
				: ThreadedObjectWriter<ObjectType> (tso.acquire_write (threadIdx)), threadNum (threadIdx), parent (tso)
			{
			}

			~Writer()
			{
				parent.release_write (threadNum);
			}

		private:

			const size_t threadNum;

			ThreadSafeObject& parent;
		};

		return Writer { *this, threadIdx };
	}

	[[nodiscard]] bool isLockFree() noexcept
	{
		return state.is_lock_free();
	}

private:

	[[nodiscard]] constexpr ObjectType& getObjectCopyForThread (size_t threadIdx) const noexcept
	{
		LIMES_ASSERT (threadIdx < totalNumThreads);
		return copies[threadIdx];
	}

	std::atomic<ObjectType> state;

	std::array<ObjectType, totalNumThreads> copies;
};

}  // namespace limes::threads
