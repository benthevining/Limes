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

#include <limes_export.h>

namespace limes::threads::detail
{

template <typename ObjectType, bool RealtimeMutatable>
struct LIMES_EXPORT AtomicObjectBase : public ObjectBase<ObjectType>
{
	static_assert (std::atomic<ObjectType>::is_always_lock_free, "Atomic must be lock-free!");
	static_assert (is_realtime_copy_safe<ObjectType>::value, "ObjectType must be realtime copy safe!");

	[[nodiscard]] static constexpr bool typeUsesAtomic() noexcept { return true; }

	[[nodiscard]] bool isUsingAtomic() const noexcept final { return true; }

	[[nodiscard]] const ObjectType& acquire_read() final
	{
		return storage.acquire_read (idxForThread (! RealtimeMutatable));
	}

	void release_read() final
	{
	}

	[[nodiscard]] ObjectType& acquire_write() final
	{
		return storage.acquire_write (idxForThread (RealtimeMutatable));
	}

	void release_write() final
	{
		storage.release_write (idxForThread (RealtimeMutatable));
	}

private:

	[[nodiscard]] static constexpr size_t idxForThread (bool realtimeThread)
	{
		if (realtimeThread)
			return 0;

		return 1;
	}

	ThreadSafeObject<ObjectType, 2> storage;
};

}  // namespace limes::threads::detail
