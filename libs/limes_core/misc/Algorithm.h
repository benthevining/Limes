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
#include <limes_export.h>

namespace limes::alg
{

template <typename T>
concept Container = requires (T a)
{
	{ a.begin() };
	{ a.end() };
};

template <Container ContainerType>
using ElementType = std::remove_reference_t<decltype (*std::begin (std::declval<ContainerType&>()))>;

template <Container ContainerType>
LIMES_EXPORT constexpr void fill (ContainerType& container, const ElementType<ContainerType>& toFillWith)
{
	std::fill (container.begin(), container.end(), toFillWith);
}

template <Container ContainerType>
LIMES_EXPORT [[nodiscard]] constexpr bool contains (const ContainerType& container, const ElementType<ContainerType>& value)
{
	return std::find (container.begin(), container.end(), value) != container.end();
}

template <Container ContainerType, class UnaryPredicate>
LIMES_EXPORT [[nodiscard]] constexpr bool contains_if (const ContainerType& container, UnaryPredicate&& p)
{
	return std::find_if (container.begin(), container.end(), std::forward<UnaryPredicate> (p)) != container.end();
}

template <Container ContainerType, class UnaryPredicate>
LIMES_EXPORT [[nodiscard]] constexpr ElementType<ContainerType>& contains_or (const ContainerType& container, ElementType<ContainerType>& defaultValue, UnaryPredicate&& p)
{
	const auto res = std::find_if (container.begin(), container.end(), std::forward<UnaryPredicate> (p));

	if (res == container.end())
		return defaultValue;

	return *res;
}

template <Container ContainerType, class UnaryPredicate>
LIMES_EXPORT [[nodiscard]] constexpr const ElementType<ContainerType>& contains_or (const ContainerType& container, const ElementType<ContainerType>& defaultValue, UnaryPredicate&& p)
{
	const auto res = std::find_if (container.begin(), container.end(), std::forward<UnaryPredicate> (p));

	if (res == container.end())
		return defaultValue;

	return *res;
}

template <Container ContainerType, class UnaryPredicate>
LIMES_EXPORT [[nodiscard]] constexpr auto contains_or_default (const ContainerType& container, UnaryPredicate&& p)
{
	using T = ElementType<ContainerType>;

	static_assert (std::is_default_constructible_v<T>,
				   "Element type must be default constructible!");

	const auto res = std::find_if (container.begin(), container.end(), std::forward<UnaryPredicate> (p));

	if (res == container.end())
		return T {};

	return *res;
}

template <Container ContainerType, class UnaryPredicate>
LIMES_EXPORT [[nodiscard]] constexpr auto* contains_or_null (const ContainerType& container, UnaryPredicate&& p)
{
	const auto res = std::find_if (container.begin(), container.end(), std::forward<UnaryPredicate> (p));

	if (res == container.end())
	{
		using T = typename std::decay<decltype (*container.begin())>::type;
		return (T) nullptr;
	}

	return *res;
}

template <Container ContainerType, class UnaryPredicate>
LIMES_EXPORT [[nodiscard]] constexpr auto* find_if (const ContainerType& container, UnaryPredicate&& p)
{
	const auto res = std::find_if (container.begin(), container.end(), std::forward<UnaryPredicate> (p));

	if (res == container.end())
	{
		using T = typename std::decay<decltype (*container.begin())>::type;
		return (T) nullptr;
	}

	return *res;
}

template <Container ContainerType, class UnaryPredicate>
LIMES_EXPORT [[nodiscard]] constexpr int num_of (const ContainerType& container, UnaryPredicate&& p)
{
	return static_cast<int> (std::count_if (container.begin(), container.end(), std::forward<UnaryPredicate> (p)));
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
	container.erase (std::remove (container.begin(), container.end(), object));
}

template <Container ContainerType>
LIMES_EXPORT constexpr void removeDuplicates (ContainerType& container)
{
	container.erase (std::unique (container.begin(), container.end()), container.end());
}

template <Container ContainerType>
LIMES_EXPORT constexpr void reverse (ContainerType& container)
{
	std::reverse (container.begin(), container.end());
}

template <Container ContainerType>
LIMES_EXPORT constexpr void sort (ContainerType& container, bool forward = true)
{
	std::sort (container.begin(), container.end());

	if (! forward)
		reverse (container);
}

template <Container ContainerType, class Comparison>
LIMES_EXPORT constexpr void sort (ContainerType& container, Comparison&& predicate, bool forward = true)
{
	std::sort (container.begin(), container.end(), std::forward<Comparison> (predicate));

	if (! forward)
		reverse (container);
}

template <Container Container1, Container Container2, class Callable>
LIMES_EXPORT constexpr void call_both (const Container1& container1, const Container2& container2, Callable&& callable)
{
	for (auto first = container1.begin(), second = container2.begin();
		 first != container1.end() && second != container2.end();
		 ++first, ++second)
	{
		callable (*first, *second);
	}
}

template <Container ContainerType>
LIMES_EXPORT constexpr auto max_value (const ContainerType& container)
{
	return *std::max_element (container.begin(), container.end());
}

template <Container ContainerType>
LIMES_EXPORT constexpr auto min_value (const ContainerType& container)
{
	return *std::min_element (container.begin(), container.end());
}

template <Container InputContainer, Container OutputContainer, class UnaryOp>
LIMES_EXPORT constexpr void transform (const InputContainer& input, OutputContainer& output, UnaryOp&& func)
{
	std::transform (input.begin(), input.end(), output.begin(), std::forward<UnaryOp> (func));
}

template <Container OutputContainerType, Container InputContainer, class UnaryOp>
LIMES_EXPORT constexpr OutputContainerType createFromTransform (const InputContainer& input, UnaryOp&& func)
{
	OutputContainerType output;

	transform (input, output, std::forward<UnaryOp> (func));

	return output;
}

}  // namespace limes::alg
