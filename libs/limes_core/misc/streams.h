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

#include <limes_namespace.h>
#include <limes_export.h>
#include <ios>
#include "preprocessor.h"

/** @file
	This file defines the class ScopedStreamFlags.
	@ingroup misc
 */

LIMES_BEGIN_NAMESPACE

namespace misc
{

/** A simple RAII class to save and restore the state of a standard stream's flags.
	@ingroup misc
 */
struct LIMES_EXPORT ScopedStreamFlags final
{
public:

	explicit ScopedStreamFlags (std::ios_base& streamToUse);

	~ScopedStreamFlags();

	LIMES_NON_COPYABLE (ScopedStreamFlags)
	LIMES_NON_MOVABLE (ScopedStreamFlags)

private:

	std::ios_base& stream;

	const std::ios_base::fmtflags flags;
};

}  // namespace misc

LIMES_END_NAMESPACE
