/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

#pragma once
#include "../../misc/Platform.h"
#include <cassert>
#include <memory>

namespace lemons::threads::detail
{

template <typename ObjectType>
class NonRealtimeMutatableObject : public ObjectBase<ObjectType>
{
public:

	~NonRealtimeMutatableObject() override
	{
		LIMES_ASSERT (pointer.load() != nullptr);  // never delete this object while the realtime thread is holding the lock

		// Spin!
		while (pointer.load() == nullptr)
			;

		[[maybe_unused]] const auto acquired = nonRealtimeLock.try_lock();

		LIMES_ASSERT (acquired);  // <- you didn't call release on one of the non-realtime threads before deleting this object

		nonRealtimeLock.unlock();
	}

	[[nodiscard]] static constexpr bool typeUsesAtomic() noexcept { return false; }

	[[nodiscard]] bool isUsingAtomic() const noexcept final { return false; }


	// --------------- REALTIME THREAD

	[[nodiscard]] const ObjectType& acquire_read() final
	{
		currentObj = pointer.exchange (nullptr);

		LIMES_ASSERT (currentObj != nullptr);  // You didn't balance your acquire and release calls!

		return *currentObj;
	}

	void release_read() final
	{
		currentObj = pointer.exchange (nullptr);

		LIMES_ASSERT (currentObj == nullptr);  // You didn't balance your acquire and release calls!
	}


	// --------------- NON-REALTIME THREAD

	[[nodiscard]] ObjectType& acquire_write() final
	{
		nonRealtimeLock.lock();
		copy.reset (new ObjectType (*storage));

		return *copy.get();
	}

	void release_write() final
	{
		// block until realtime thread is done using the object
		for (auto* ptr = storage.get();
			 ! pointer.compare_exchange_weak (ptr, copy.get());
			 ptr = storage.get())
			;

		storage = std::move (copy);
		nonRealtimeLock.unlock();
	}

private:

	std::unique_ptr<ObjectType> storage, copy;
	std::atomic<ObjectType*>	pointer { nullptr };
	ObjectType*					currentObj { nullptr };

	std::mutex nonRealtimeLock;
};

}  // namespace lemons::threads::detail
