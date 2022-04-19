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

LIMES_BEGIN_NAMESPACE

namespace threads
{

template <class Function, typename... Args>
LIMES_EXPORT inline void callInBackground (Function&& function, Args&&... args)
{
	std::thread worker (std::move (function), std::forward<Args> (args)...);
	worker.detach();
}

template <class Function, class Callback>
LIMES_EXPORT inline void callInBackgroundWithCallback (Function&& functionToCall, Callback&& callback)
{
	auto func = [call = std::move (functionToCall), cb = std::move (callback)]
	{
		call();
		cb();
	};

	callInBackground (std::move (func));
}

template <class Function, typename... Args>
LIMES_EXPORT inline void callAndBlock (Function&& function, Args&&... args)
{
	std::thread worker (std::move (function), std::forward<Args> (args)...);
	worker.join();
}

LIMES_EXPORT [[nodiscard]] int maxNumThreads (int defaultVal = 4) noexcept;

}  // namespace threads

LIMES_END_NAMESPACE
