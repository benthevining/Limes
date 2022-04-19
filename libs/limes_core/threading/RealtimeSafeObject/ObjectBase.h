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

#include <limes_export.h>
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace threads::detail
{

template <typename ObjectType>
struct LIMES_EXPORT ObjectBase
{
	virtual ~ObjectBase() = default;

	[[nodiscard]] virtual bool isUsingAtomic() const noexcept = 0;

	[[nodiscard]] virtual ObjectType& acquire_write() = 0;
	virtual void					  release_write() = 0;

	[[nodiscard]] virtual const ObjectType& acquire_read() = 0;
	virtual void							release_read() = 0;
};

}  // namespace threads::detail

LIMES_END_NAMESPACE
