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

LIMES_BEGIN_NAMESPACE

namespace files
{

LIMES_EXPORT [[nodiscard]] std::uintmax_t getAvailableSpaceOnFilesystem();

LIMES_EXPORT [[nodiscard]] constexpr char dirSeparator() noexcept
{
	return static_cast<char> (std::filesystem::path::preferred_separator);
}

LIMES_EXPORT [[nodiscard]] consteval char PATHseparator() noexcept
{
#if LIMES_WINDOWS
	return ';';
#else
	return ':';
#endif
}

}  // namespace files

LIMES_END_NAMESPACE
