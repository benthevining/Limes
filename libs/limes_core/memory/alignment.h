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

#if LIMES_MSVC || LIMES_WINDOWS
#	include <malloc.h>
#endif


LIMES_BEGIN_NAMESPACE

template <typename T, typename... Args>
LIMES_EXPORT [[nodiscard]] T* allocate_aligned (std::size_t count = 1, std::size_t alignment = 32, Args&&... args)
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
		new (typed_ptr + i) T (std::forward<Args> (args)...);

	return typed_ptr;
}

template <typename T>
LIMES_EXPORT void deallocate_aligned (T* ptr)
{
	if (ptr == nullptr)
		return;

#if LIMES_MSVC || LIMES_WINDOWS
	_aligned_free (ptr);
#else
	free (ptr);
#endif
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
		LIMES_ASSERT (ptr != nullptr);
	}

	~aligned_pointer()
	{
		deallocate_aligned (ptr);
	}

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

LIMES_END_NAMESPACE
