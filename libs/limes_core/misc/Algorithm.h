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

/** @defgroup alg Algorithms
	Algorithms for manipulating generic container types.
	@ingroup limes_core
 */

/** @file
	This file defines various utility algorithms.
	@ingroup alg
 */

LIMES_BEGIN_NAMESPACE

/** This namespace defines various utility algorithms.
	@ingroup alg
 */
namespace alg
{

/** @ingroup alg
	@{
 */

/** @concept Container
	This concept requires that a type has \c begin() and \c end() functions that return iterators.
 */
template <typename T>
concept Container = requires (T a)
{
	{ std::begin (a) };
	{ std::end (a) };
};

/** Handy typedef for deducing the type of the objects in a Container. */
template <Container ContainerType>
using ElementType = std::remove_reference_t<decltype (*std::begin (std::declval<ContainerType&>()))>;

/** Typedef for the type returned by calling \c begin() or \c end() on a Container. */
template <Container ContainerType>
using IteratorType = decltype (std::begin (std::declval<ContainerType>()));

/** Fills the container with the specified value. */
template <Container ContainerType>
LIMES_EXPORT constexpr void fill (ContainerType& container, const ElementType<ContainerType>& toFillWith)
{
	std::fill (std::begin (container), std::end (container), toFillWith);
}

/** Returns true if the container contains the specified value. */
template <Container ContainerType>
LIMES_EXPORT [[nodiscard]] constexpr bool contains (const ContainerType& container, const ElementType<ContainerType>& value)
{
	return std::find (std::begin (container), std::end (container), value) != std::end (container);
}

/** Returns true if the specified predicate is met by at least one object in the container. */
template <Container ContainerType, class UnaryPredicate>
LIMES_EXPORT [[nodiscard]] constexpr bool contains_if (const ContainerType& container, UnaryPredicate&& p)
{
	return std::find_if (std::begin (container), std::end (container), std::forward<UnaryPredicate> (p)) != std::end (container);
}

/** If the specified predicate is met by any object in the container, then the first object to meet the predicate is returned.
	If no objects in the container meet the predicate, then \c defaultValue is returned.
 */
template <Container ContainerType, class UnaryPredicate>
LIMES_EXPORT [[nodiscard]] constexpr ElementType<ContainerType>& contains_or (const ContainerType& container, ElementType<ContainerType>& defaultValue, UnaryPredicate&& p)
{
	const auto res = std::find_if (std::begin (container), std::end (container), std::forward<UnaryPredicate> (p));

	if (res == std::end (container))
		return defaultValue;

	return *res;
}

/** If the specified predicate is met by any object in the container, then the first object to meet the predicate is returned.
	If no objects in the container meet the predicate, then \c defaultValue is returned.
 */
template <Container ContainerType, class UnaryPredicate>
LIMES_EXPORT [[nodiscard]] constexpr const ElementType<ContainerType>& contains_or (const ContainerType& container, const ElementType<ContainerType>& defaultValue, UnaryPredicate&& p)
{
	const auto res = std::find_if (std::begin (container), std::end (container), std::forward<UnaryPredicate> (p));

	if (res == std::end (container))
		return defaultValue;

	return *res;
}

/** If the specified predicate is met by any object in the container, then the first object to meet the predicate is returned.
	If no objects in the container meet the predicate, then a default-constructed object is returned.
	Note that the container's element type must be default constructable!
 */
template <Container ContainerType, class UnaryPredicate>
LIMES_EXPORT [[nodiscard]] constexpr ElementType<ContainerType> contains_or_default (const ContainerType& container, UnaryPredicate&& p)
{
	using T = ElementType<ContainerType>;

	static_assert (std::is_default_constructible_v<T>,
				   "Element type must be default constructible!");

	const auto res = std::find_if (std::begin (container), std::end (container), std::forward<UnaryPredicate> (p));

	if (res == std::end (container))
		return T {};

	return *res;
}

/** If the specified predicate is met by any object in the container, then the first object to meet the predicate is returned.
	If no objects in the container meet the predicate, then a nullptr is returned.
 */
template <Container ContainerType, class UnaryPredicate>
LIMES_EXPORT [[nodiscard]] constexpr ElementType<ContainerType>* contains_or_null (const ContainerType& container, UnaryPredicate&& p)
{
	const auto res = std::find_if (std::begin (container), std::end (container), std::forward<UnaryPredicate> (p));

	if (res == std::end (container))
		return nullptr;

	return res;
}

/** If the specified predicate is met by any object in the container, then the first object to meet the predicate is returned.
	If no objects in the container meet the predicate, then a nullptr is returned.
 */
template <Container ContainerType, class UnaryPredicate>
LIMES_EXPORT [[nodiscard]] constexpr ElementType<ContainerType>* find_if (const ContainerType& container, UnaryPredicate&& p)
{
	const auto res = std::find_if (std::begin (container), std::end (container), std::forward<UnaryPredicate> (p));

	if (res == std::end (container))
		return nullptr;

	return res;
}

/** If the specified predicate is met by any object in the container, then the first object to meet the predicate is returned.
	If no objects in the container meet the predicate, then a nullptr is returned.
 */
template <Container ContainerType, class UnaryPredicate>
LIMES_EXPORT [[nodiscard]] constexpr const ElementType<ContainerType>* find_if (const ContainerType& container, UnaryPredicate&& p)
{
	const auto res = std::find_if (std::begin (container), std::end (container), std::forward<UnaryPredicate> (p));

	if (res == std::end (container))
		return nullptr;

	return res;
}

/** Counts the number of objects in the container that meet the specified predicate. */
template <Container ContainerType, class UnaryPredicate>
LIMES_EXPORT [[nodiscard]] constexpr int num_of (const ContainerType& container, UnaryPredicate&& p)
{
	return static_cast<int> (std::count_if (std::begin (container), std::end (container), std::forward<UnaryPredicate> (p)));
}

/** Counts the number of objects in the container. */
template <Container ContainerType>
LIMES_EXPORT [[nodiscard]] constexpr int size (const ContainerType& container)
{
	return num_of (container, [] (const ElementType<ContainerType>&)
				   { return true; });
}

/** Reverses the objects in the container. */
template <Container ContainerType>
LIMES_EXPORT constexpr void reverse (ContainerType& container)
{
	std::reverse (std::begin (container), std::end (container));
}

/** Sorts the objects in the container.
	@param container The container to sort.
	@param forward If false, the container will be reversed after it is sorted.
 */
template <Container ContainerType>
LIMES_EXPORT constexpr void sort (ContainerType& container, bool forward = true)
{
	std::sort (std::begin (container), std::end (container));

	if (! forward)
		reverse (container);
}

/** Sorts the container using a custom comparator. */
template <Container ContainerType, class Comparison>
LIMES_EXPORT constexpr void sort (ContainerType& container, Comparison&& predicate, bool forward = true)
{
	std::sort (std::begin (container), std::end (container), std::forward<Comparison> (predicate));

	if (! forward)
		reverse (container);
}

/** Randomizes the order of the elements in the container. */
template <Container ContainerType>
LIMES_EXPORT void shuffle (ContainerType& container)
{
	static std::random_device		  device {};
	static std::default_random_engine engine { device() };

	std::shuffle (std::begin (container), std::end (container), engine);
}

/** Calls a function with corresponding pairs of elements from two containers.
	The function's first two arguments must be the element types of \c Container1 and \c Container2 ; if the function accepts a third integer argument, then it will also receive the current index.
	The iteration stops when the end of the smallest container is reached.
	@returns The number of times the function was called
 */
template <Container Container1, Container Container2, class Callable>
LIMES_EXPORT constexpr int call_both (const Container1& container1, const Container2& container2, Callable&& callable)
{
	int count = 0;

	for (auto first = std::begin (container1), second = std::begin (container2);
		 first != std::end (container1) && second != std::end (container2);
		 ++first, ++second, ++count)
	{
		if constexpr (requires { std::invoke (callable, *first, *second, count); })
			std::invoke (callable, *first, *second, count);
		else
			std::invoke (callable, *first, *second);
	}

	return count;
}

/** Returns the maximum value in the container. */
template <Container ContainerType>
LIMES_EXPORT [[nodiscard]] constexpr auto max_value (const ContainerType& container)
{
	return *std::max_element (std::begin (container), std::end (container));
}

/** Returns the maximum value in the container and its index in the container. */
template <Container ContainerType>
LIMES_EXPORT [[nodiscard]] constexpr auto max_value (const ContainerType& container, int& maxIndex)
{
	const auto max_elem = std::max_element (std::begin (container), std::end (container));

	maxIndex = static_cast<int> (std::distance (std::begin (container), max_elem));

	return *max_elem;
}

/** Returns the minimum value in the container. */
template <Container ContainerType>
LIMES_EXPORT [[nodiscard]] constexpr auto min_value (const ContainerType& container)
{
	return *std::min_element (std::begin (container), std::end (container));
}

/** Returns the minimum value in the container and its index in the container. */
template <Container ContainerType>
LIMES_EXPORT [[nodiscard]] constexpr auto min_value (const ContainerType& container, int& minIndex)
{
	const auto min_elem = std::min_element (std::begin (container), std::end (container));

	minIndex = static_cast<int> (std::distance (std::begin (container), min_elem));

	return *min_elem;
}

/** Populates \c output by calling \c func with each element of \c input in sequence.
	@see createFromTransform()
 */
template <Container InputContainer, Container OutputContainer, class UnaryOp>
LIMES_EXPORT constexpr void transform (const InputContainer& input, OutputContainer& output, UnaryOp&& func)
{
	std::transform (std::begin (input), std::end (input), std::begin (output), std::forward<UnaryOp> (func));
}

/** The same as \c transform() , except the output container is default constructed before being populated.
	\c OutputContainerType must be default constructable!
	@see transform()
 */
template <Container OutputContainerType, Container InputContainer, class UnaryOp>
LIMES_EXPORT [[nodiscard]] constexpr OutputContainerType createFromTransform (const InputContainer& input, UnaryOp&& func)
{
	static_assert (std::is_default_constructible_v<OutputContainerType>,
				   "OutputContainerType must be default constructible!");

	OutputContainerType output;

	transform (input, output, std::forward<UnaryOp> (func));

	return output;
}

/** This function allows structured bindings of iterators, similar to Python's \c %enumerate() method.

	Example usage:
	@code
	std::vector<int> v {7,14,21};

	for (const auto [idx, val] : enumerate(v))
		assert ((idx+1)*7 == val);
	@endcode
 */
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

/** Removes all duplicate elements from a \c std::vector . */
template <typename ElementType>
LIMES_EXPORT constexpr void removeDuplicates (std::vector<ElementType>& vector)
{
	vector.erase (std::unique (std::begin (vector), std::end (vector)), std::end (vector));
}

/** @} */

}  // namespace alg

LIMES_END_NAMESPACE
