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

/** @file
	This file defines the OptionalObject class.
	@ingroup memory
 */

LIMES_BEGIN_NAMESPACE

namespace memory
{

/** An object that retains its allocated memory, but can be dynamically constructed and destroyed (without deallocating its memory).
	@tparam T The type of owned object
	@ingroup memory
 */
template <typename T>
class LIMES_EXPORT OptionalObject final
{
public:

	/** You can access the type of the owned object with this typedef. */
	using Type = T;

	/** @name Constructors */
	///@{
	/** Creates an uninitialized, null object. */
	OptionalObject() = default;

	/** Constructs the object with the given arguments. */
	template <typename... Args>
	explicit OptionalObject (Args&&... args);

	/** Copy constructor. */
	explicit OptionalObject (const OptionalObject& other);

	/** Move constructor. */
	explicit OptionalObject (OptionalObject&& other) noexcept;
	///@}

	/** @name Assignment */
	///@{
	OptionalObject& operator= (const OptionalObject& other);
	OptionalObject& operator= (OptionalObject&& other) noexcept;
	///@}

	/** Constructs the object with the given arguments.
		If the object already existed when this function is called, its destructor will be called.
	 */
	template <typename... Args>
	T& create (Args&&... args);

	/** Calls the object's destructor, but does not deallocate its memory. */
	void destroy();

	/** @name Accessors
		Returns a pointer to the owned object.
	 */
	///@{
	[[nodiscard]] T* get() const noexcept;
	T*				 operator->() const noexcept;
	///@}

	/** Returns a reference to the owned object.
		Be careful, the owned pointer may be null!
	 */
	T& operator*() const;

	/** Returns true if the object currently exists. */
	[[nodiscard]] bool exists() const noexcept;

	/** Returns the size of the owned object (and thus, the preallocated memory), in bytes. */
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
OptionalObject<T>& OptionalObject<T>::operator= (const OptionalObject& other)  // cppcheck-suppress operatorEqVarError
{
	duplicate (other);

	return *this;
}

template <typename T>
void OptionalObject<T>::duplicate (const OptionalObject& other)
{
	if (other.object == nullptr)
	{
		object = nullptr;
		return;
	}

	object = static_cast<T*> (memory);

	*object = *other.object;
}

template <typename T>
OptionalObject<T>& OptionalObject<T>::operator= (OptionalObject&& other) noexcept  // cppcheck-suppress operatorEqVarError
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
