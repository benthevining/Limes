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
#include <atomic>
#include "compiler_warnings.h"

#if LIMES_WINDOWS
#	include <windows.h>
#elif LIMES_APPLE
#	include <unistd.h>
#	include <sys/sysctl.h>
#elif LIMES_ANDROID
#	include "../files/file.h"
#	include "../text/StringUtils.h"
#	include <string>
#	include <exception>
#else  // Linux
#	include <sys/ptrace.h>
#endif

#if LIMES_APPLE || LIMES_LINUX
#	include <sys/types.h>
#endif

#if LIMES_IOS || LIMES_LINUX
#	include <signal.h>
#endif

LIMES_BEGIN_NAMESPACE

namespace debugger
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


static std::atomic<bool> isCheckedAlready = false;
static std::atomic<bool> result			  = false;

bool isRunningUnderDebugger() noexcept
{
	if (isCheckedAlready.load())
		return result.load();

	const auto res = debuggerCheckInternal();

	result.store (res);

	isCheckedAlready.store (true);

	return res;
}

#if LIMES_IOS || LIMES_LINUX
#	define LIMES_BREAK_IN_DEBUGGER ::kill (0, SIGTRAP);
#elif LIMES_MSVC
#	define LIMES_BREAK_IN_DEBUGGER __debugbreak();
#elif defined(__has_builtin)
#	if __has_builtin(__builtin_trap)
#		define LIMES_BREAK_IN_DEBUGGER __builtin_trap();
#	elif __has_builtin(__builtin_debugtrap)
#		define LIMES_BREAK_IN_DEBUGGER __builtin_debugtrap();
#	endif
#endif

#ifndef LIMES_BREAK_IN_DEBUGGER
#	if LIMES_INTEL && (LIMES_GCC || LIMES_CLANG || LIMES_OSX)
#		define LIMES_BREAK_IN_DEBUGGER asm("int $3");
#	else
#		define LIMES_BREAK_IN_DEBUGGER __asm int 3
#	endif
#endif


LIMES_DISABLE_ALL_COMPILER_WARNINGS

#if LIMES_MSVC
#	ifndef __INTEL_COMPILER
#		pragma intrinsic(__debugbreak)
#	endif
#endif

void breakInDebugger() noexcept {
	LIMES_BREAK_IN_DEBUGGER
}

#undef LIMES_BREAK_IN_DEBUGGER

LIMES_REENABLE_ALL_COMPILER_WARNINGS

}  // namespace debugger

LIMES_END_NAMESPACE
