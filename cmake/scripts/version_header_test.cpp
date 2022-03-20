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

#include <Limes/limes.h>

static constexpr auto testVersionObj = limes::getLimesVersion();

static_assert (false,
	"major: " << testVersionObj.getMajor() << "; minor: " << testVersionObj.getMinor() << "; patch: " << testVersionObj.getPatch())
