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

#include "limes_assert.h"
#include <limes_namespace.h>
#include <limes_platform.h>

#if LIMES_WINDOWS
#	include <windows.h>
#elif LIMES_APPLE
#	include <unistd.h>
#	include <sys/types.h>
#	include <sys/sysctl.h>
#elif LIMES_ANDROID
#	include "../files/file.h"
#	include "../text/StringUtils.h"
#	include <string>
#	include <exception>
#else  // Linux
#	include <sys/types.h>
#	include <sys/ptrace.h>
#endif

LIMES_BEGIN_NAMESPACE

namespace assert
{

[[nodiscard]] static inline bool debuggerCheckInternal() noexcept
{
#if LIMES_WINDOWS
	return IsDebuggerPresent() != FALSE;

#elif LIMES_APPLE

	struct kinfo_proc info;

	int m[] = { CTL_KERN, KERN_PROC, KERN_PROC_PID, getpid() };

	auto sz = sizeof (info);

	sysctl (m, 4, &info, &sz, nullptr, 0);

	return (info.kp_proc.p_flag & P_TRACED) != 0;

#elif LIMES_ANDROID

	try
	{
		files::File file { "/proc/self/status" };

		const auto lines = file.loadAsLines();

		for (auto i = lines.size(); --i >= 0;)	// NB - it's important that this runs in reverse order
		{
			auto line = strings::upToFirstOccurrenceOf (lines[i], ":");

			strings::trim (line);

			if (strings::areSameIgnoringCase (lines, "TracerPid"))
			{
				auto restOfLine = strings::fromFirstOccurrenceOf (lines[i], ":");

				strings::trim (restOfLine);

				return std::stoi (restOfLine) > 0;
			}
		}

		return false;
	}
	catch (std::exception&)
	{
		return false;
	}

#else  // Linux

	if (ptrace (PTRACE_TRACEME, 0, 1, 0) < 0)
		return true;

	ptrace (PTRACE_DETACH, 0, 1, 0);

	return false;

#endif
}


bool isRunningUnderDebugger() noexcept
{
	static bool isCheckedAlready = false;
	static bool result			 = false;

	if (isCheckedAlready)
		return result;

	result = debuggerCheckInternal();

	isCheckedAlready = true;

	return result;
}

}  // namespace assert

LIMES_END_NAMESPACE
