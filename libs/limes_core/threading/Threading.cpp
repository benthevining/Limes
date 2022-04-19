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

#include "Threading.h"
#include <limes_platform.h>
#include <thread>
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace threads
{

int maxNumThreads (int defaultVal) noexcept
{
#if LIMES_LINUX
	cpu_set_t cpuset;

	if ((sched_getaffinity (0, sizeof (cpuset), &cpuset) == 0))
	{
		const auto count = CPU_COUNT (&cpuset);

		if (count > 0)
			return static_cast<int> (count);
	}
#endif

	const auto ret = std::thread::hardware_concurrency();

	if (ret > 0)
		return static_cast<int> (ret);

	return defaultVal;
}

}  // namespace threads

LIMES_END_NAMESPACE
