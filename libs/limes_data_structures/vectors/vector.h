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

#include "basic_vector.h"	// IWYU pragma: export
#include "scalar_vector.h"	// IWYU pragma: export
#include <string>
#include <limes_namespace.h>
#include <limes_core.h>

LIMES_BEGIN_NAMESPACE

template <typename ObjectType, class Allocator = std::allocator<ObjectType>>
using vector = std::conditional_t<std::is_scalar_v<ObjectType> && ! std::is_pointer_v<ObjectType>,
								  scalar_vector<ObjectType, Allocator>,
								  basic_vector<ObjectType, Allocator>>;

using StringVector = vector<std::string>;

template <typename ObjectType>
using Matrix = vector<vector<ObjectType>>;

template <typename ObjectType, size_t N>
using StackVector = vector<ObjectType, StackAllocator<ObjectType, N>>;

template <typename ObjectType, size_t Capacity>
using PoolVector = vector<ObjectType, MemoryPoolAllocator<ObjectType, Capacity>>;

LIMES_END_NAMESPACE
