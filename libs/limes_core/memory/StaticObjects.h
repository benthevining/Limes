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

#include <memory>
#include <limes_export.h>
#include <limes_namespace.h>

/** @file
	This file contains utilities for working with static objects.
	@ingroup memory
 */

LIMES_BEGIN_NAMESPACE

namespace memory
{

/** This function forces some data to be evaluated at compile time.
	This can be useful in some scenarios to ensure that your data is available at compile time.
	@ingroup memory
 */
template <auto Data>
LIMES_EXPORT [[nodiscard]] consteval const auto& make_static() noexcept
{
	return Data;
}

/** This function retrieves a static object of a specified type.
	If the object hasn't been initialized before, this function call will construct it with the specified constructor arguments.
	@ingroup memory
 */
template <typename ObjectType, typename... Args>
LIMES_EXPORT [[nodiscard]] ObjectType& getStaticObject (Args&&... args)
{
	static std::unique_ptr<ObjectType> ptr;

	if (ptr.get() == nullptr)
		ptr = std::make_unique<ObjectType> (std::forward<Args> (args)...);

	return *ptr;  // cppcheck-suppress nullPointerRedundantCheck
}

}  // namespace memory

LIMES_END_NAMESPACE
