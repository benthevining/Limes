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

#include "basic_vector.h"
#include "scalar_vector.h"
#include <string>

namespace lemons
{

template <typename ObjectType>
using vector = std::conditional_t<std::is_scalar_v<ObjectType> && ! std::is_pointer_v<ObjectType>, scalar_vector<ObjectType>, basic_vector<ObjectType>>;

using StringVector = vector<std::string>;

}  // namespace lemons
