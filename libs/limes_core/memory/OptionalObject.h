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
#include <cstddef>
#include <memory>
#include <cstring>	// for memcpy

LIMES_BEGIN_NAMESPACE

namespace memory
{

template <typename T>
class LIMES_EXPORT OptionalObject final
{
public:

	using Type = T;

	OptionalObject()  = default;
	~OptionalObject() = default;

	template <typename... Args>
	explicit OptionalObject (Args&&... args);

	explicit OptionalObject (const OptionalObject& other);

	explicit OptionalObject (OptionalObject&& other) noexcept;

	OptionalObject& operator= (const OptionalObject& other);

	OptionalObject& operator= (OptionalObject&& other) noexcept;

	template <typename... Args>
	T& create (Args&&... args);

	void destroy();

	[[nodiscard]] T* get() const noexcept;

	T* operator->() const noexcept;

	T& operator*() const;

	[[nodiscard]] bool exists() const noexcept;

	[[nodiscard]] static consteval size_t getSize() noexcept;

private:

	void duplicate (const OptionalObject& other);

	alignas (T) std::byte memory[sizeof (T)] {};

	T* object { nullptr };
};


template <typename T>
template <typename... Args>
OptionalObject<T>::OptionalObject (Args&&... args)
{
	create (std::forward<Args> (args)...);
}

template <typename T>
T* OptionalObject<T>::get() const noexcept
{
	return object;
}

template <typename T>
T* OptionalObject<T>::operator->() const noexcept
{
	return object;
}

template <typename T>
T& OptionalObject<T>::operator*() const
{
	LIMES_ASSERT (object != nullptr);
	return *object;
}

template <typename T>
OptionalObject<T>::OptionalObject (const OptionalObject& other)
{
	duplicate (other);
}

template <typename T>
OptionalObject<T>::OptionalObject (OptionalObject&& other) noexcept
	: memory (std::move (other.memory)), object (other.object)
{
}

template <typename T>
OptionalObject<T>& OptionalObject<T>::operator= (const OptionalObject& other)
{
	duplicate (other);

	return *this;
}

template <typename T>
void OptionalObject<T>::duplicate (const OptionalObject& other)
{
	std::memcpy (static_cast<void*> (memory),
				 static_cast<void*> (other.memory),
				 sizeof (T));

	if (other.object == nullptr)
		object = nullptr;
	else
		object = static_cast<T*> (memory);
}

template <typename T>
OptionalObject<T>& OptionalObject<T>::operator= (OptionalObject&& other) noexcept
{
	object = std::move (other.object);
	memory = std::move (other.memory);

	return *this;
}

template <typename T>
template <typename... Args>
T& OptionalObject<T>::create (Args&&... args)
{
	destroy();

	object = static_cast<T*> (memory);

	std::construct_at (object, std::forward<Args> (args)...);

	return *object;
}

template <typename T>
void OptionalObject<T>::destroy()
{
	if (object != nullptr)
	{
		std::destroy_at (object);
		object = nullptr;
	}
}

template <typename T>
bool OptionalObject<T>::exists() const noexcept
{
	return object != nullptr;
}

template <typename T>
consteval size_t OptionalObject<T>::getSize() noexcept
{
	return sizeof (T);
}

}  // namespace memory

LIMES_END_NAMESPACE
