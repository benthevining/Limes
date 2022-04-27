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

#include <thread>
#include <limes_export.h>
#include <limes_namespace.h>
#include "../misc/preprocessor.h"

LIMES_BEGIN_NAMESPACE

namespace threads
{

template <class Function, typename... Args>
LIMES_EXPORT inline void callInBackground (Function&& function, Args&&... args)
{
	std::thread worker (std::forward<Function> (function), std::forward<Args> (args)...);
	worker.detach();
}

template <class Function, typename... Args>
LIMES_EXPORT inline void callAndBlock (Function&& function, Args&&... args)
{
	std::thread worker (std::forward<Function> (function), std::forward<Args> (args)...);
	worker.join();
}

LIMES_EXPORT [[nodiscard]] int maxNumThreads (int defaultVal = 4) noexcept;


template <class Function>
class LIMES_EXPORT ScopedLoopingThread final
{
public:

	explicit ScopedLoopingThread (Function&& function)
	{
		auto callback = [f = std::move (function), this]
		{
			while (! exitSignal.load())
				f();
		};

		callInBackground (std::move (callback));
	}

	~ScopedLoopingThread()
	{
		exitSignal.store (true);
	}

	LIMES_NON_COPYABLE (ScopedLoopingThread);
	LIMES_NON_MOVABLE (ScopedLoopingThread);

private:

	std::atomic<bool> exitSignal { false };
};

}  // namespace threads

LIMES_END_NAMESPACE
