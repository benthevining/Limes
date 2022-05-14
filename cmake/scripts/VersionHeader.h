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

#include <limes_core.h>

namespace @LIMES_ARG_NAMESPACE@
{
	[[nodiscard]] static consteval ::limes::misc::Version @LIMES_ARG_FUNCTION_NAME@() noexcept
	{
		return ::limes::misc::Version { @LIMES_ARG_MAJOR@, @LIMES_ARG_MINOR@, @LIMES_ARG_PATCH@ };
	}
}
