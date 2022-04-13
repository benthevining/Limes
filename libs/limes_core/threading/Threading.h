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

namespace limes::threads
{

template <class Function, typename... Args>
LIMES_EXPORT inline void callInBackground (Function&& function, Args&&... args)
{
	std::thread worker (std::move (function), std::forward<Args> (args)...);
	worker.detach();
}

template <class Function, typename... Args>
LIMES_EXPORT inline void callAndBlock (Function&& function, Args&&... args)
{
	std::thread worker (std::move (function), std::forward<Args> (args)...);
	worker.join();
}

LIMES_EXPORT [[nodiscard]] inline int maxNumThreads (int defaultVal = 4)
{
	const auto ret = std::thread::hardware_concurrency();

	if (ret > 0)
		return static_cast<int> (ret);

	return defaultVal;
}

}  // namespace limes::threads
