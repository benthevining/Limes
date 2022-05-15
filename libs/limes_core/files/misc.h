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
#include <limes_platform.h>
#include <cstdint>
#include <filesystem>
#include "../system/compiler_defs.h"

LIMES_BEGIN_NAMESPACE

namespace files
{

/** Returns the available space on the filesystem, in bytes. */
LIMES_EXPORT [[nodiscard]] std::uintmax_t getAvailableSpaceOnFilesystem();

/** Returns the total size of the filesystem, in bytes. */
LIMES_EXPORT [[nodiscard]] std::uintmax_t getTotalSizeOfFilesystem();

/** Returns the platform's preferred separator: \c \ on Windows, and \c / everywhere else. */
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION constexpr char dirSeparator() noexcept
{
	return static_cast<char> (std::filesystem::path::preferred_separator);
}

/** Returns the platform's separator char for the \c PATH  environment variable: \c ; on Windows, and \c : everywhere else. */
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION consteval char PATHseparator() noexcept
{
#if LIMES_WINDOWS
	return ';';
#else
	return ':';
#endif
}

/** Returns true if the current platform's filesystem is case-sensitive. True on Linux, false everywhere else. */
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION consteval bool filesystemIsCaseSensitive() noexcept
{
#if LIMES_LINUX
	return true;
#else
	return false;
#endif
}

}  // namespace files

LIMES_END_NAMESPACE
