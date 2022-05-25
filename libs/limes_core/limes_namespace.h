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

/** @file
	This file contains macros defining the Limes namespace.
	@ingroup limes_core
 */

/** @def LIMES_BEGIN_NAMESPACE
	This macro defines the Limes namespace; all Limes code is sandwiched between this macro and LIMES_END_NAMESPACE.
	An inline namespace is used to protect the ABI from accidental breakage between versions.
	This macro is intended for internal usage and should not be used by projects consuming Limes.
	@see LIMES_END_NAMESPACE
	@ingroup limes_core
 */
#define LIMES_BEGIN_NAMESPACE \
	namespace limes           \
	{                         \
	inline namespace v4_8_1   \
	{

/** @def LIMES_END_NAMESPACE
	This macro defines the end of the Limes namespace.
	This macro is intended for internal usage and should not be used by projects consuming Limes.
	@see LIMES_BEGIN_NAMESPACE
	@ingroup limes_core
 */
#define LIMES_END_NAMESPACE \
	}                       \
	}
