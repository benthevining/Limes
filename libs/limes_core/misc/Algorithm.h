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

#include <algorithm>
#include <random>
#include <limes_export.h>
#include <vector>  // for std::begin, std::end
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace alg
{

template <typename T>
concept Container = requires (T a)
{
	{ std::begin (a) };
	{ std::end (a) };
};

template <Container ContainerType>
using ElementType = std::remove_reference_t<decltype (*std::begin (std::declval<ContainerType&>()))>;

template <Container ContainerType>
using IteratorType = decltype (std::begin (std::declval<ContainerType>()));

template <Container ContainerType>
LIMES_EXPORT constexpr void fill (ContainerType& container, const ElementType<ContainerType>& toFillWith)
{
	std::fill (std::begin (container), std::end (container), toFillWith);
}

template <Container ContainerType>
LIMES_EXPORT [[nodiscard]] constexpr bool contains (const ContainerType& container, const ElementType<ContainerType>& value)
{
	return std::find (std::begin (container), std::end (container), value) != std::end (container);
}

template <Container ContainerType, class UnaryPredicate>
LIMES_EXPORT [[nodiscard]] constexpr bool contains_if (const ContainerType& container, UnaryPredicate&& p)
{
	return std::find_if (std::begin (container), std::end (container), std::forward<UnaryPredicate> (p)) != std::end (container);
}

template <Container ContainerType, class UnaryPredicate>
LIMES_EXPORT [[nodiscard]] constexpr ElementType<ContainerType>& contains_or (const ContainerType& container, ElementType<ContainerType>& defaultValue, UnaryPredicate&& p)
{
	const auto res = std::find_if (std::begin (container), std::end (container), std::forward<UnaryPredicate> (p));

	if (res == std::end (container))
		return defaultValue;

	return *res;
}

template <Container ContainerType, class UnaryPredicate>
LIMES_EXPORT [[nodiscard]] constexpr const ElementType<ContainerType>& contains_or (const ContainerType& container, const ElementType<ContainerType>& defaultValue, UnaryPredicate&& p)
{
	const auto res = std::find_if (std::begin (container), std::end (container), std::forward<UnaryPredicate> (p));

	if (res == std::end (container))
		return defaultValue;

	return *res;
}

template <Container ContainerType, class UnaryPredicate>
LIMES_EXPORT [[nodiscard]] constexpr auto contains_or_default (const ContainerType& container, UnaryPredicate&& p)
{
	using T = ElementType<ContainerType>;

	static_assert (std::is_default_constructible_v<T>,
				   "Element type must be default constructible!");

	const auto res = std::find_if (std::begin (container), std::end (container), std::forward<UnaryPredicate> (p));

	if (res == std::end (container))
		return T {};

	return *res;
}

template <Container ContainerType, class UnaryPredicate>
LIMES_EXPORT [[nodiscard]] constexpr auto* contains_or_null (const ContainerType& container, UnaryPredicate&& p)
{
	const auto res = std::find_if (std::begin (container), std::end (container), std::forward<UnaryPredicate> (p));

	if (res == std::end (container))
	{
		using T = typename std::decay<decltype (*container.begin())>::type;
		return (T) nullptr;
	}

	return *res;
}

template <Container ContainerType, class UnaryPredicate>
LIMES_EXPORT [[nodiscard]] constexpr auto* find_if (const ContainerType& container, UnaryPredicate&& p)
{
	const auto res = std::find_if (std::begin (container), std::end (container), std::forward<UnaryPredicate> (p));

	if (res == std::end (container))
	{
		using T = typename std::decay<decltype (*container.begin())>::type;
		return (T) nullptr;
	}

	return *res;
}

template <Container ContainerType, class UnaryPredicate>
LIMES_EXPORT [[nodiscard]] constexpr int num_of (const ContainerType& container, UnaryPredicate&& p)
{
	return static_cast<int> (std::count_if (std::begin (container), std::end (container), std::forward<UnaryPredicate> (p)));
}

template <Container ContainerType>
LIMES_EXPORT [[nodiscard]] constexpr int size (const ContainerType& container)
{
	using T = ElementType<ContainerType>;

	return num_of (container, [] (const T&)
				   { return true; });
}

template <Container ContainerType>
LIMES_EXPORT constexpr void remove (ContainerType& container, const ElementType<ContainerType>& object)
{
	container.erase (std::remove (std::begin (container), std::end (container), object));
}

template <Container ContainerType>
LIMES_EXPORT constexpr void removeDuplicates (ContainerType& container)
{
	container.erase (std::unique (std::begin (container), std::end (container)), std::end (container));
}

template <Container ContainerType>
LIMES_EXPORT constexpr void reverse (ContainerType& container)
{
	std::reverse (std::begin (container), std::end (container));
}

template <Container ContainerType>
LIMES_EXPORT constexpr void sort (ContainerType& container, bool forward = true)
{
	std::sort (std::begin (container), std::end (container));

	if (! forward)
		reverse (container);
}

template <Container ContainerType, class Comparison>
LIMES_EXPORT constexpr void sort (ContainerType& container, Comparison&& predicate, bool forward = true)
{
	std::sort (std::begin (container), std::end (container), std::forward<Comparison> (predicate));

	if (! forward)
		reverse (container);
}

LIMES_NO_EXPORT static inline auto& get_rand_engine()
{
	struct RandomEngine final
	{
		std::random_device		   device {};
		std::default_random_engine engine { device() };
	};

	static RandomEngine engine;

	return engine.engine;  // cppcheck-suppress returnReference
}

template <Container ContainerType>
LIMES_EXPORT void shuffle (ContainerType& container)
{
	std::shuffle (std::begin (container), std::end (container), get_rand_engine());
}

template <Container Container1, Container Container2, class Callable>
LIMES_EXPORT constexpr int call_both (const Container1& container1, const Container2& container2, Callable&& callable)
{
	int count = 0;

	for (auto first = std::begin (container1), second = std::begin (container2);
		 first != std::end (container1) && second != std::end (container2);
		 ++first, ++second, ++count)
	{
		callable (*first, *second);
	}

	return count;
}

template <Container ContainerType>
LIMES_EXPORT [[nodiscard]] constexpr auto max_value (const ContainerType& container)
{
	return *std::max_element (std::begin (container), std::end (container));
}

template <Container ContainerType>
LIMES_EXPORT [[nodiscard]] constexpr auto max_value (const ContainerType& container, int& maxIndex)
{
	const auto max_elem = std::max_element (std::begin (container), std::end (container));

	maxIndex = static_cast<int> (std::distance (std::begin (container), max_elem));

	return *max_elem;
}

template <Container ContainerType>
LIMES_EXPORT [[nodiscard]] constexpr auto min_value (const ContainerType& container)
{
	return *std::min_element (std::begin (container), std::end (container));
}

template <Container ContainerType>
LIMES_EXPORT [[nodiscard]] constexpr auto min_value (const ContainerType& container, int& minIndex)
{
	const auto min_elem = std::min_element (std::begin (container), std::end (container));

	minIndex = static_cast<int> (std::distance (std::begin (container), min_elem));

	return *min_elem;
}

template <Container InputContainer, Container OutputContainer, class UnaryOp>
LIMES_EXPORT constexpr void transform (const InputContainer& input, OutputContainer& output, UnaryOp&& func)
{
	std::transform (std::begin (input), std::end (input), std::begin (output), std::forward<UnaryOp> (func));
}

template <Container OutputContainerType, Container InputContainer, class UnaryOp>
LIMES_EXPORT [[nodiscard]] constexpr OutputContainerType createFromTransform (const InputContainer& input, UnaryOp&& func)
{
	static_assert (std::is_default_constructible_v<OutputContainerType>,
				   "OutputContainerType must be default constructible!");

	OutputContainerType output;

	transform (input, output, std::forward<UnaryOp> (func));

	return output;
}


template <Container ContainerType>
LIMES_EXPORT [[nodiscard]] constexpr auto enumerate (ContainerType&& iterable)
{
	struct iterator final
	{
		size_t						i;
		IteratorType<ContainerType> iter;

		bool operator!= (const iterator& other) const { return iter != other.iter; }

		void operator++()
		{
			++i;
			++iter;
		}

		auto operator*() const { return std::tie (i, *iter); }
	};

	struct iterable_wrapper final
	{
		ContainerType iterable;

		auto begin() { return iterator { 0, std::begin (iterable) }; }
		auto end() { return iterator { 0, std::end (iterable) }; }
	};

	return iterable_wrapper { std::forward<ContainerType> (iterable) };
}

}  // namespace alg

LIMES_END_NAMESPACE
