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
#include <limes_platform.h>
#include <cstdlib>
#include <exception>
#include <memory>
#include "../misc/preprocessor.h"

#if LIMES_MSVC || LIMES_WINDOWS
#	include <malloc.h>
#endif


LIMES_BEGIN_NAMESPACE

namespace memory
{

template <typename T, typename... Args>
LIMES_EXPORT [[nodiscard]] T* allocate_aligned (std::size_t count = 1, std::size_t alignment = 32, Args&&... args) noexcept
{
	try
	{
		const auto sizeBytes = count * sizeof (T);

#if LIMES_MSVC || LIMES_WINDOWS
		auto* const ptr = _aligned_malloc (sizeBytes, alignment);
#else
		auto* const ptr = std::aligned_alloc (alignment, sizeBytes);
#endif

		if (ptr == nullptr)
			return nullptr;

		auto* const typed_ptr = static_cast<T*> (ptr);

		for (size_t i = 0; i < count; ++i)
			std::construct_at (typed_ptr + i, std::forward<Args> (args)...);

		return typed_ptr;
	}
	catch (const std::exception&)
	{
		return nullptr;
	}
}

template <typename T>
LIMES_EXPORT bool deallocate_aligned (T* ptr) noexcept
{
	try
	{
		if (ptr == nullptr)
			return true;

#if LIMES_MSVC || LIMES_WINDOWS
		_aligned_free (ptr);
#else
		std::free (ptr);
#endif

		return true;
	}
	catch (const std::exception&)
	{
		return false;
	}
}


template <typename T>
class LIMES_EXPORT aligned_pointer final
{
public:

	using OwnedType = T;

	template <typename... Args>
	explicit aligned_pointer (std::size_t count = 1, std::size_t alignment = 32, Args&&... args)
		: ptr (allocate_aligned<T> (count, alignment, std::forward<Args> (args)...))
	{
		if (ptr == nullptr)
			throw std::runtime_error ("Failed aligned memory allocation");
	}

	~aligned_pointer()
	{
		deallocate_aligned (ptr);
	}

	LIMES_NON_COPYABLE (aligned_pointer);

	LIMES_DEFAULT_MOVABLE (aligned_pointer);

	[[nodiscard]] T* get() const noexcept { return ptr; }

	[[nodiscard]] T* operator()() const noexcept { return ptr; }

	operator T*() const noexcept { return ptr; }

	T* operator->() const noexcept { return ptr; }

	[[nodiscard]] T& operator*() const
	{
		LIMES_ASSERT (ptr != nullptr);
		return *ptr;
	}

	[[nodiscard]] decltype (auto) operator[] (std::size_t idx) { return ptr[idx]; }

private:

	T* ptr;
};

}  // namespace memory

LIMES_END_NAMESPACE
