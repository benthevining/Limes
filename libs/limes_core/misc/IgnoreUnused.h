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
#include "../limes_namespace.h"
#include <limes_platform.h>

/** @defgroup misc Miscellaneous
	Miscellaneous things.
	This module contains classes and functions that don't seem to fit into any other module within limes_core.
	@ingroup limes_core
 */

/** @dir libs/limes_core/misc
	This directory contains miscellaneous utilities.
	@ingroup misc
 */

/** @file
	This file defines the function ignore_unused().
	@ingroup misc
 */

LIMES_BEGIN_NAMESPACE

namespace misc
{

/** A utility function to silence unused variable warnings.
	@ingroup misc
 */
#if LIMES_INTEL_COMPILER
#	pragma warning disable 592
#endif
template <typename... Args>
LIMES_EXPORT constexpr void ignore_unused (Args&&...) noexcept
{
}

}  // namespace misc

LIMES_END_NAMESPACE
