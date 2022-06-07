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

#include "serializing.h"
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace serializing
{

Node parseXML (const std::string_view& xmlText)
{
	return Node { ObjectType::Null };
}

}  // namespace serializing

LIMES_END_NAMESPACE
