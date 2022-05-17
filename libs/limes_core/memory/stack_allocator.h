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
#include <cassert>
#include <cstddef>
#include <cstdint>

LIMES_BEGIN_NAMESPACE

namespace memory
{

template <std::size_t N, std::size_t alignment = alignof (std::max_align_t)>
class LIMES_EXPORT stack_buffer final
{
public:

	~stack_buffer() = default;

	stack_buffer (const stack_buffer&) = delete;
	stack_buffer (stack_buffer&&)	   = delete;
	stack_buffer& operator= (const stack_buffer&) = delete;
	stack_buffer& operator= (stack_buffer&&) = delete;

	template <std::size_t ReqAlign>
	[[nodiscard]] std::byte* allocate (std::size_t n);

	void deallocate (std::byte* p, std::size_t n) noexcept;

	[[nodiscard]] static consteval std::size_t size() noexcept;

	[[nodiscard]] std::size_t used() const noexcept;

	void reset() noexcept;

private:

	[[nodiscard]] static std::size_t align_up (std::size_t n) noexcept;

	[[nodiscard]] bool contains (std::byte* p) noexcept;

	alignas (alignment) std::byte buf_[N];
	std::byte* ptr_ { buf_ };
};

/*-----------------------------------------------------------------------------------------------------------------*/

template <class T, std::size_t N, std::size_t Align = alignof (std::max_align_t)>
class LIMES_EXPORT StackAllocator final
{
public:

	using pointer		  = T*;
	using const_pointer	  = const T*;
	using reference		  = T&;
	using const_reference = const T&;
	using value_type	  = T;
	using size_type		  = std::size_t;
	using difference_type = ptrdiff_t;

	StackAllocator (const StackAllocator&) = default;
	StackAllocator& operator= (const StackAllocator&) = delete;

	StackAllocator() noexcept
	{
		static_assert (N % Align == 0,
					   "size N needs to be a multiple of alignment Align");
	}

	template <class U>
	StackAllocator (const StackAllocator<U, N, Align>& a) noexcept
		: a_ (a.a_)
	{
	}

	template <class _Up>
	struct LIMES_EXPORT rebind
	{
		using other = StackAllocator<_Up, N, Align>;
	};

	[[nodiscard]] T* allocate (std::size_t n);

	void deallocate (T* p, std::size_t n) noexcept;

	template <class T1, std::size_t N1, std::size_t A1,
			  class U, std::size_t M, std::size_t A2>
	friend bool operator== (const StackAllocator<T1, N1, A1>& x, const StackAllocator<U, M, A2>& y) noexcept;

	template <class U, std::size_t M, std::size_t A>
	friend class StackAllocator;

private:

	stack_buffer<N, Align> a_;
};

template <class T, std::size_t N, std::size_t A1, class U, std::size_t M, std::size_t A2>
inline bool operator== (const StackAllocator<T, N, A1>& x, const StackAllocator<U, M, A2>& y) noexcept
{
	return N == M && A1 == A2 && &x.a_ == &y.a_;
}

template <class T, std::size_t N, std::size_t A1, class U, std::size_t M, std::size_t A2>
inline bool operator!= (const StackAllocator<T, N, A1>& x, const StackAllocator<U, M, A2>& y) noexcept
{
	return ! (x == y);
}

/*-----------------------------------------------------------------------------------------------------------------*/

template <class T, std::size_t N, std::size_t Align>
T* StackAllocator<T, N, Align>::allocate (std::size_t n)
{
	return reinterpret_cast<T*> (a_.template allocate<alignof (T)> (n * sizeof (T)));
}

template <class T, std::size_t N, std::size_t Align>
void StackAllocator<T, N, Align>::deallocate (T* p, std::size_t n) noexcept
{
	a_.deallocate (reinterpret_cast<char*> (p), n * sizeof (T));
}

/*-----------------------------------------------------------------------------------------------------------------*/

template <std::size_t N, std::size_t alignment>
consteval std::size_t stack_buffer<N, alignment>::size() noexcept
{
	return N;
}

template <std::size_t N, std::size_t alignment>
std::size_t stack_buffer<N, alignment>::used() const noexcept
{
	return static_cast<std::size_t> (ptr_ - buf_);
}

template <std::size_t N, std::size_t alignment>
void stack_buffer<N, alignment>::reset() noexcept
{
	ptr_ = buf_;
}

template <std::size_t N, std::size_t alignment>
template <std::size_t ReqAlign>
std::byte* stack_buffer<N, alignment>::allocate (std::size_t n)
{
	static_assert (ReqAlign <= alignment, "alignment is too small for this arena");

	LIMES_ASSERT (contains (ptr_) && "short_alloc has outlived arena");

	const auto aligned_n = align_up (n);

	if (static_cast<decltype (aligned_n)> (buf_ + N - ptr_) >= aligned_n)
	{
		auto* r = ptr_;
		ptr_ += aligned_n;
		return r;
	}

	static_assert (alignment <= alignof (std::max_align_t), "you've chosen an "
															"alignment that is larger than alignof(std::max_align_t), and "
															"cannot be guaranteed by normal operator new");

	return static_cast<std::byte*> (::operator new (n));
}

template <std::size_t N, std::size_t alignment>
void stack_buffer<N, alignment>::deallocate (std::byte* p, std::size_t n) noexcept
{
	LIMES_ASSERT (contains (ptr_) && "short_alloc has outlived arena");

	if (contains (p))
	{
		n = align_up (n);

		if (p + n == ptr_)
			ptr_ = p;
	}
	else
		::operator delete (p);
}

template <std::size_t N, std::size_t alignment>
std::size_t stack_buffer<N, alignment>::align_up (std::size_t n) noexcept
{
	return (n + (alignment - 1)) & ~(alignment - 1);
}

template <std::size_t N, std::size_t alignment>
bool stack_buffer<N, alignment>::contains (std::byte* p) noexcept
{
	return std::uintptr_t (buf_) <= std::uintptr_t (p) && std::uintptr_t (p) <= std::uintptr_t (buf_) + N;
}

}  // namespace memory

LIMES_END_NAMESPACE
