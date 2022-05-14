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

LIMES_BEGIN_NAMESPACE

/** This namespace contains functions related to the debugger. */
namespace debugger
{

/** Returns true if the calling process is running under a debugger. */
LIMES_EXPORT [[nodiscard]] bool isRunningUnderDebugger() noexcept;

/** Attempts to break into the debugger. If one is not attached, this function may or may not crash, depending on the platform.
 */
LIMES_EXPORT [[noreturn]] void breakInDebugger() noexcept;

}  // namespace debugger

LIMES_END_NAMESPACE
