/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

#pragma once

#include <algorithm>

namespace lemons::alg
{

template <typename T>
concept Container = requires (T a)
{
	{ a.begin() };
	{ a.end() };
};


template <Container ContainerType, class T>
constexpr void fill (ContainerType& container, const T& toFillWith)
{
	std::fill (container.begin(), container.end(), toFillWith);
}

template <Container ContainerType, class T>
[[nodiscard]] constexpr bool contains (const ContainerType& container, const T& value)
{
	return std::find (container.begin(), container.end(), value) != container.end();
}

template <Container ContainerType, class UnaryPredicate>
[[nodiscard]] constexpr bool contains_if (const ContainerType& container, UnaryPredicate&& p)
{
	return std::find_if (container.begin(), container.end(), std::move (p)) != container.end();
}

template <Container ContainerType, class T, class UnaryPredicate>
[[nodiscard]] constexpr T& contains_or (const ContainerType& container, T& defaultValue, UnaryPredicate&& p)
{
	const auto res = std::find_if (container.begin(), container.end(), std::move (p));

	if (res == container.end())
		return defaultValue;

	return *res;
}

template <Container ContainerType, class T, class UnaryPredicate>
[[nodiscard]] constexpr const T& contains_or (const ContainerType& container, const T& defaultValue, UnaryPredicate&& p)
{
	const auto res = std::find_if (container.begin(), container.end(), std::move (p));

	if (res == container.end())
		return defaultValue;

	return *res;
}

template <Container ContainerType, class UnaryPredicate>
[[nodiscard]] constexpr auto contains_or_default (const ContainerType& container, UnaryPredicate&& p)
{
	using T = typename std::decay<decltype (*container.begin())>::type;

	static_assert (std::is_default_constructible_v<T>,
				   "T must be default constructible!");

	const auto res = std::find_if (container.begin(), container.end(), std::move (p));

	if (res == container.end())
		return T {};

	return *res;
}

template <Container ContainerType, class UnaryPredicate>
[[nodiscard]] constexpr auto* contains_or_null (const ContainerType& container, UnaryPredicate&& p)
{
	const auto res = std::find_if (container.begin(), container.end(), std::move (p));

	if (res == container.end())
	{
		using T = typename std::decay<decltype (*container.begin())>::type;
		return (T) nullptr;
	}

	return *res;
}

template <Container ContainerType, class UnaryPredicate>
[[nodiscard]] constexpr auto* find_if (const ContainerType& container, UnaryPredicate&& p)
{
	const auto res = std::find_if (container.begin(), container.end(), std::move (p));

	if (res == container.end())
	{
		using T = typename std::decay<decltype (*container.begin())>::type;
		return (T) nullptr;
	}

	return *res;
}

template <Container ContainerType, class UnaryPredicate>
[[nodiscard]] constexpr int num_of (const ContainerType& container, UnaryPredicate&& p)
{
	return static_cast<int> (std::count_if (container.begin(), container.end(), std::move (p)));
}

template <Container ContainerType>
[[nodiscard]] constexpr int size (const ContainerType& container)
{
	using ElementType = decltype (*container.begin());

	return num_of (container, [] (const ElementType&)
				   { return true; });
}

template <Container ContainerType, typename ElementType>
constexpr void remove (ContainerType& container, const ElementType& object)
{
	container.erase (std::remove (container.begin(), container.end(), object));
}

template <Container ContainerType>
constexpr void removeDuplicates (ContainerType& container)
{
	container.erase (std::unique (container.begin(), container.end()), container.end());
}

template <Container ContainerType>
constexpr void reverse (ContainerType& container)
{
	std::reverse (container.begin(), container.end());
}

template <Container ContainerType>
constexpr void sort (ContainerType& container, bool forward = true)
{
	std::sort (container.begin(), container.end());

	if (! forward)
		reverse (container);
}

template <Container ContainerType, class Comparison>
constexpr void sort (ContainerType& container, Comparison&& predicate, bool forward = true)
{
	std::sort (container.begin(), container.end(), std::move (predicate));

	if (! forward)
		reverse (container);
}

template <Container Container1, Container Container2, class Callable>
constexpr void call_both (const Container1& container1, const Container2& container2, Callable&& callable)
{
	for (auto first = container1.begin(), second = container2.begin();
		 first != container1.end() && second != container2.end();
		 ++first, ++second)
	{
		callable (*first, *second);
	}
}

template <Container ContainerType>
constexpr auto max_value (const ContainerType& container)
{
	return *std::max_element (container.begin(), container.end());
}

template <Container ContainerType>
constexpr auto min_value (const ContainerType& container)
{
	return *std::min_element (container.begin(), container.end());
}

template <Container InputContainer, Container OutputContainer, class UnaryOp>
constexpr void transform (const InputContainer& input, OutputContainer& output, UnaryOp&& func)
{
	std::transform (input.begin(), input.end(), output.begin(), std::move (func));
}

template <Container OutputContainerType, Container InputContainer, class UnaryOp>
constexpr OutputContainerType createFromTransform (const InputContainer& input, UnaryOp&& func)
{
	OutputContainerType output;

	transform (input, output, std::move (func));

	return output;
}

}  // namespace lemons::alg
