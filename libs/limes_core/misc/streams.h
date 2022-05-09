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

LIMES_BEGIN_NAMESPACE

struct LIMES_EXPORT ScopedStreamFlags final
{
public:

	explicit ScopedStreamFlags (std::ios_base& streamToUse);

	~ScopedStreamFlags();

	LIMES_NON_COPYABLE (ScopedStreamFlags);
	LIMES_NON_MOVABLE (ScopedStreamFlags);

private:

	std::ios_base& stream;

	const std::ios_base::fmtflags flags;
};

LIMES_END_NAMESPACE
