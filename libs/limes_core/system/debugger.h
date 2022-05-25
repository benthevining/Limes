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

#include <limes_export.h>
#include <limes_namespace.h>
#include "compiler_defs.h"

/** @file
	This file defines functions related to the debugger.
	@ingroup system
 */

LIMES_BEGIN_NAMESPACE

/** This namespace contains functions related to the debugger.
	@ingroup system
 */
namespace debugger
{

/** Returns true if the calling process is running under a debugger.
	@ingroup system
 */
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION bool isRunningUnderDebugger() noexcept;

/** Attempts to break into the debugger. If one is not attached, this function may or may not crash, depending on the platform.
	@ingroup system
 */
LIMES_EXPORT [[noreturn]] void breakInDebugger() noexcept;

}  // namespace debugger

LIMES_END_NAMESPACE
