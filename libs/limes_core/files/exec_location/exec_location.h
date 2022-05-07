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

#include <string>
#include <limes_export.h>
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace files
{

LIMES_NO_EXPORT [[nodiscard]] std::string getExecutablePath();

LIMES_NO_EXPORT [[nodiscard]] std::string getModulePath();

}  // namespace files

LIMES_END_NAMESPACE
