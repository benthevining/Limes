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

#include "streams.h"
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace misc
{

ScopedStreamFlags::ScopedStreamFlags (std::ios_base& streamToUse)
	: stream (streamToUse), flags (stream.flags())
{
}

ScopedStreamFlags::~ScopedStreamFlags()
{
	stream.flags (flags);
}

}  // namespace misc

LIMES_END_NAMESPACE
