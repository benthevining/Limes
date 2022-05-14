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
#include <limes_platform.h>
#include "ReferenceCountedObject.h"
#include <type_traits>
#include <limes_namespace.h>
#include "../misc/preprocessor.h"

LIMES_BEGIN_NAMESPACE

namespace memory
{

template <typename SuperType>
struct LIMES_EXPORT SharedObject : public ReferenceCountedObject
{
	SharedObject()	= default;
	~SharedObject() = default;

	using Ptr = ReferenceCountedObjectPtr<SuperType>;

	[[nodiscard]] virtual Ptr clone() const = 0;
};


template <typename OwnedObjectType>
struct LIMES_EXPORT SharedObjectOwner
{
	static_assert (std::is_base_of_v<SharedObject<OwnedObjectType>, OwnedObjectType>);

public:

	using SharedObjectType = OwnedObjectType;

	template <typename... Args>
	explicit SharedObjectOwner (Args&&... args)
		: data (new OwnedObjectType (std::forward<Args> (args)...))
	{
	}

	LIMES_DEFAULT_COPYABLE (SharedObjectOwner);
	LIMES_DEFAULT_MOVABLE (SharedObjectOwner);

	[[nodiscard]] int getReferenceCount() const noexcept
	{
		if (data == nullptr)
			return 0;

		return data->getRefCount();
	}

	bool duplicateIfShared()
	{
		if (getReferenceCount() > 1)
		{
			LIMES_ASSERT (data != nullptr);
			data = data->clone();
			return true;
		}

		return false;
	}

protected:

	[[nodiscard]] bool ownedObjectExists() const noexcept
	{
		return data != nullptr;
	}

	[[nodiscard]] OwnedObjectType& getOwnedObject() const
	{
		LIMES_ASSERT (ownedObjectExists());
		return *data;
	}

	typename OwnedObjectType::Ptr data;
};

}  // namespace memory

LIMES_END_NAMESPACE
