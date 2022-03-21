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

#include "RealtimeSafeObject/RealtimeMutatableObject.h"
#include "RealtimeSafeObject/NonrealtimeMutatableObject.h"
#include "RealtimeSafeObject/AtomicObjectHolder.h"
#include <limes_export.h>

namespace lemons::threads
{

namespace detail
{

template <typename ObjectType, bool RealtimeMutatable>
using RealtimeSafeObjectBase = std::conditional_t<RealtimeMutatable,
												  RealtimeMutatableObject<ObjectType>, NonRealtimeMutatableObject<ObjectType>>;

}


template <typename ObjectType, bool RealtimeMutatable>
class LIMES_EXPORT RealtimeSafeObject final : public std::conditional_t<std::atomic<ObjectType>::is_always_lock_free,
																		detail::AtomicObjectBase<ObjectType, RealtimeMutatable>,
																		detail::RealtimeSafeObjectBase<ObjectType, RealtimeMutatable>>
{
public:

	static_assert (std::is_base_of_v<detail::ObjectBase<ObjectType>, RealtimeSafeObject<ObjectType, RealtimeMutatable>>, "");


	[[nodiscard]] ThreadedObjectWriter<ObjectType> write();

	[[nodiscard]] ThreadedObjectReader<ObjectType> realtime_read();

	[[nodiscard]] ThreadedObjectReader<ObjectType> nonRealtime_read();

	[[nodiscard]] ThreadedObjectReader<ObjectType> read (bool isRealtimeThread);
};


template <typename ObjectType, size_t NumThreads = 2, bool RealtimeSafe = true, bool RealtimeMutatable = false>
using ThreadedObject = std::conditional_t<(NumThreads > 2),
										  ThreadSafeObject<ObjectType, NumThreads>,
										  std::conditional_t<RealtimeSafe, RealtimeSafeObject<ObjectType, RealtimeMutatable>,
															 ThreadSafeObject<ObjectType, 2>>>;

}  // namespace lemons::threads
