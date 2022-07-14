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
#include <limes_core.h>
#include "./vecops_macros.h"
#include <string_view>

/** @defgroup vec_impl Implementation kind checking
	Implementation kind checking functions.
	@ingroup limes_vecops
 */

/** @file
	This file contains functions for checking the implementation being used for the limes_vecops functions.
	@ingroup vec_impl
 */

LIMES_BEGIN_NAMESPACE

namespace vecops
{

/** @ingroup vec_impl
	@{
 */

/** Returns true if the implementation being used is Apple vDSP. */
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION consteval bool isUsingVDSP() noexcept
{
#if LIMES_VECOPS_USE_VDSP
	return true;
#else
	return false;
#endif
}

/** Returns true if the implementation being used is Intel IPP. */
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION consteval bool isUsingIPP() noexcept
{
#if LIMES_VECOPS_USE_IPP
	return true;
#else
	return false;
#endif
}

/** Returns true if the implementation being used is MIPP. */
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION consteval bool isUsingMIPP() noexcept
{
#if LIMES_VECOPS_USE_MIPP
	return true;
#else
	return false;
#endif
}

/** Returns true if the implementation being used is NE10. */
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION consteval bool isUsingNE10() noexcept
{
#if LIMES_VECOPS_USE_NE10
	return true;
#else
	return false;
#endif
}

/** Returns true if the fallback implementation is being used. */
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION consteval bool isUsingFallback() noexcept
{
	return ! (isUsingVDSP() || isUsingIPP() || isUsingMIPP() || isUsingNE10());	 // cppcheck-suppress knownConditionTrueFalse
}

static_assert (isUsingVDSP() || isUsingIPP() || isUsingMIPP() || isUsingNE10() || isUsingFallback());

/** Returns a string literal with the name of the implementation being used. */
LIMES_EXPORT [[nodiscard]] LIMES_PURE_FUNCTION static consteval std::string_view getImplementationName() noexcept
{
	if constexpr (isUsingVDSP())
		return "Apple vDSP";
	else if constexpr (isUsingIPP())
		return "Intel IPP";
	else if constexpr (isUsingMIPP())
		return "MIPP";
	else if constexpr (isUsingNE10())
		return "NE10";
	else
		return "Fallback";
}

}  // namespace vecops

LIMES_END_NAMESPACE
