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

#include "ObjectBase.h"
#include "../RealtimeTypeTraits.h"
#include <mutex>
#include <limes_export.h>

namespace limes::threads::detail
{

template <typename ObjectType>
class LIMES_EXPORT RealtimeMutatableObject : public ObjectBase<ObjectType>
{
public:

	static_assert (is_realtime_copy_safe<ObjectType>::value, "ObjectType must be realtime copy safe!");

	~RealtimeMutatableObject() override
	{
		LIMES_ASSERT ((control.load() & BIT::BUSY) == 0);  // <- never delete this object while the realtime thread is still using it

		// Spin!
		while ((control.load() & BIT::BUSY) == 1)
			;

		[[maybe_unused]] const auto acquired = nonRealtimeLock.try_lock();

		LIMES_ASSERT (acquired);  // <- you didn't call release on one of the non-realtime threads before deleting this object

		nonRealtimeLock.unlock();
	}

	RealtimeMutatableObject (const RealtimeMutatableObject&) = delete;

	RealtimeMutatableObject (RealtimeMutatableObject&& other) = delete;

	RealtimeMutatableObject& operator= (const RealtimeMutatableObject&) = delete;

	RealtimeMutatableObject& operator= (RealtimeMutatableObject&&) = delete;

	[[nodiscard]] bool isUsingAtomic() const noexcept final { return false; }


	// --------------- REALTIME THREAD

	[[nodiscard]] ObjectType& acquire_write() final
	{
		return realtimeCopy;
	}

	void release_write() final
	{
		const auto idx = acquireIndex();
		data[idx]	   = realtimeCopy;
		releaseIndex (idx);
	}


	// --------------- NON-REALTIME THREAD

	[[nodiscard]] const ObjectType& acquire_read() final
	{
		nonRealtimeLock.lock();
		auto current = control.load (std::memory_order_acquire);

		// there is new data so flip the indices around atomically ensuring we are not inside realtimeAssign
		if ((current & BIT::NEW_DATA) != 0)
		{
			int newValue;  // NOLINT

			do
			{
				// expect the realtime thread not to be inside the realtime-assign
				current &= ~BIT::BUSY;

				// realtime thread should flip index value and clear the newdata bit
				newValue = (current ^ BIT::INDEX) & BIT::INDEX;
			} while (! control.compare_exchange_weak (current, newValue, std::memory_order_acq_rel));

			current = newValue;
		}

		// flip the index bit as we always use the index that the realtime thread is currently NOT using
		const auto nonRealtimeIndex = (current & BIT::INDEX) ^ 1;

		return data[nonRealtimeIndex];
	}

	void release_read() final
	{
		nonRealtimeLock.unlock();
	}


private:

	enum class BIT : size_t
	{
		INDEX	 = (1 << 0),
		BUSY	 = (1 << 1),
		NEW_DATA = (1 << 2)
	};

	[[nodiscard]] size_t acquireIndex() noexcept
	{
		return control.fetch_or (BIT::BUSY, std::memory_order_acquire) & BIT::INDEX;
	}

	void releaseIndex (size_t idx) noexcept
	{
		control.store ((idx & BIT::INDEX) | BIT::NEW_DATA, std::memory_order_release);
	}

	std::atomic<size_t> control { 0 };

	std::array<ObjectType, 2> data;
	ObjectType				  realtimeCopy;

	std::mutex nonRealtimeLock;
};

}  // namespace limes::threads::detail
