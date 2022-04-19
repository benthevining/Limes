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

LIMES_BEGIN_NAMESPACE

template <auto Data>
LIMES_EXPORT [[nodiscard]] constexpr const auto& make_static() noexcept
{
	return Data;
}

template <typename ObjectType, typename... Args>
LIMES_EXPORT [[nodiscard]] ObjectType& getStaticObject (Args&&... args)
{
	static std::unique_ptr<ObjectType> ptr;

	if (ptr.get() == nullptr)
		ptr = std::make_unique<ObjectType> (std::forward<Args> (args)...);

	return *ptr;  // cppcheck-suppress nullPointerRedundantCheck
}

LIMES_END_NAMESPACE
