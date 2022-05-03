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

namespace cstring
{

LIMES_EXPORT [[nodiscard]] bool areSame (const char* string1, const char* string2);

LIMES_EXPORT [[nodiscard]] int length (const char* string);

}  // namespace cstring

LIMES_EXPORT void printLimesASCII();

LIMES_END_NAMESPACE
