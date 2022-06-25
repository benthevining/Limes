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

// IWYU pragma: private
// IWYU pragma: friend file.cpp

#pragma once

#include <string>
#include <limes_export.h>
#include <limes_namespace.h>

/** @dir libs/limes_core/files/exec_location
	This directory contains implementation details for retrieving the current executable's location.
	@ingroup files
 */

/** @file
	This file contains implementation details for retrieving the executable's location (used by the File class).
	@ingroup files
	@see File
 */

LIMES_BEGIN_NAMESPACE

namespace files
{

/// @cond internals

/** Returns the path of the current executable, as a string.
	@ingroup files
 */
LIMES_NO_EXPORT [[nodiscard]] std::string getExecutablePath();

/** Returns the path of the current module, as a string.
	@ingroup files
 */
LIMES_NO_EXPORT [[nodiscard]] std::string getModulePath();

/// @endcond

}  // namespace files

LIMES_END_NAMESPACE
