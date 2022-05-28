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

#include <future>
#include <utility>
#include <limes_export.h>
#include <limes_namespace.h>
#include "../misc/preprocessor.h"
#include "../misc/Functions.h"

/** @defgroup threads Threading
	Threading utilities.
	@ingroup limes_core
 */

/** @file
	This file contains general threading utilities.
	@ingroup threads
 */

LIMES_BEGIN_NAMESPACE

/** This namespace contains threading utilities.
	@ingroup threads
 */
namespace threads
{

/** Launches a function on a background thread and returns immediately.
	@returns A \c std::future that will eventually hold the result of the called function.
	@ingroup threads
 */
template <func::Function Func, typename... Args>
LIMES_EXPORT inline auto callInBackground (Func&& function, Args&&... args)
{
	return std::async (std::move (function), std::forward<Args> (args)...);
}

/** Calls a function on a background thread, blocks until the function finishes executing, and returns the function result.
	@returns Result of calling the function
	@ingroup threads
 */
template <func::Function Func, typename... Args>
LIMES_EXPORT inline func::result_type<Func, Args...> callAndBlock (Func&& function, Args&&... args)
{
	auto future = std::async (std::move (function), std::forward<Args> (args)...);

	future.wait();

	return future.get();
}

/** Tries to detect the number of concurrent threads supported by the current hardware and implementation.
	If the detection fails, \c defaultVal will be returned.
	@ingroup threads
 */
LIMES_EXPORT [[nodiscard]] int maxNumThreads (int defaultVal = 4) noexcept;

}  // namespace threads

LIMES_END_NAMESPACE
