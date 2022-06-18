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
#include "TypeList_impl.h"	// IWYU pragma: export
#include "../misc/Functions.h"
#include "TypeTraits.h"
#include <type_traits>
#include <utility>
#include <memory>

/** @file
	This file defines the TypeList class.
	@ingroup meta
 */

LIMES_BEGIN_NAMESPACE

namespace meta
{

#pragma mark TypeList

/**
	A compile-time list of types that can be manipulated.

	Example usage:
	@code
	using MyTypeList = TypeList<int, float, double>;

	static_assert (MyTypeList::size == 3);
	static_assert (MyTypeList::num_of<int> == 1);

	using FloatingPointTypes = MyTypeList::remove_if_not<std::is_floating_point>;

	static_assert (FloatingPointTypes::equal<TypeList<float, double>>);
	@endcode

	Most operations on TypeLists are done with using statements, as illustrated above. Once declared, a %TypeList is immutable; it can only create permutations of itself as new specializations of %TypeList.

	@tparam Types The list of types for the %TypeList to hold.
	@ingroup meta
	@test This class's API is extensively tested with static assertions at compile-time.
 */
template <typename... Types>
class LIMES_EXPORT TypeList final
{
public:

	/** You can't actually create an instance of one, %TypeLists are only static objects. */
	TypeList() = delete;

	/** You can get the fully specialized type of this %TypeList using this alias.

		For example:
		@code
		using MyTypeList = TypeList<int>;

		static_assert (std::is_same_v<MyTypeList::TypeID, TypeList<int>>);
		@endcode
	 */
	using TypeID = TypeList<Types...>;

	/** Creates an empty %TypeList. */
	using clear = TypeList<>;

	/** The total size of the %TypeList. */
	static constexpr const size_t size = typelist::size_v<TypeID>;

	/** True if the %TypeList is empty; false otherwise. */
	static constexpr const bool empty = typelist::is_empty<TypeID>;

	/** True if the other %TypeList holds the same types in the same order as this one.
		@tparam Other Another TypeList to compare with.
	 */
	template <class Other>
	static constexpr const bool equal = typelist::are_same_v<TypeID, Other>;

	/** True if the other %TypeList holds the same types as this one, regardless of their order.
		@tparam Other Another TypeList to compare with.
	 */
	template <class Other>
	static constexpr const bool equal_ignore_order = typelist::same_ignoring_order_v<TypeID, Other>;

	/** True if all the passed types are contained in this %TypeList.
		@tparam TypesToFind List of types to check if this %TypeList contains.
	 */
	template <typename... TypesToFind>
	static constexpr const bool contains = typelist::contains_v<TypeID, TypesToFind...>;

	/** True if this %TypeList contains at least one of the passed types.
		@tparam TypesToFind List of types to check if this %TypeList contains.
	 */
	template <typename... TypesToFind>
	static constexpr const bool contains_or = typelist::contains_or_v<TypeID, TypesToFind...>;

	/** The number of times the given type appears in this %TypeList.
		@tparam Type The type to count occurrences of in this %TypeList.
	 */
	template <typename Type>
	static constexpr const size_t num_of = typelist::count_v<TypeID, Type>;

	/** The number of types in the %TypeList that satisfy the predicate.
		@tparam UnaryPredicate A template class that will be instantiated for each type in the %TypeList and must provide a compile-time call operator that will determine if the predicate is met for the type it was instantiated with. Examples of unary predicates are \c std::is_floating_point or \c std::is_enum .
	 */
	template <template <typename> class UnaryPredicate>
	static constexpr const size_t count_if = typelist::count_if_v<TypeID, UnaryPredicate>;

	/** The number of types in the %TypeList that do not satisfy the predicate.
		@tparam UnaryPredicate A template class that will be instantiated for each type in the %TypeList and must provide a compile-time call operator that will determine if the predicate is met for the type it was instantiated with. Examples of unary predicates are \c std::is_floating_point or \c std::is_enum .
	 */
	template <template <typename> class UnaryPredicate>
	static constexpr const size_t count_if_not = typelist::count_if_not_v<TypeID, UnaryPredicate>;

	/** The type of a new %TypeList with the given types appended to the end.
		@tparam TypesToAdd Types to add to the end of this %TypeList.
	 */
	template <typename... TypesToAdd>
	using add = typelist::add_t<TypeID, TypesToAdd...>;

	/** The type of a new %TypeList with types added from another %TypeList.
		@tparam Other Another %TypeList to add types from.
	 */
	template <class Other>
	using add_from = typelist::add_from_t<TypeID, Other>;

	/** For each type in the passed list of types, adds it to the %TypeList if it was not already in the list.
		@tparam TypesToAdd List of types to add to the %TypeList
	 */
	template <typename... TypesToAdd>
	using addIfAbsent = typelist::addIfAbsent_t<TypeID, TypesToAdd...>;

	/** Inserts a type in the %TypeList at the given \c Index . Types after the given index in the list will be pushed back by 1.
		@tparam Index The index in the %TypeList to insert the new type at. Indices start from 0.
		@tparam ToInsert The type to insert in the %TypeList.
	 */
	template <size_t Index, typename ToInsert>
	using insert_at = typelist::insert_at_t<TypeID, Index, ToInsert>;

	/** Prepends a type to the beginning of the %TypeList.
		@tparam ToPrepend Type to prepend to the list.
	 */
	template <typename ToPrepend>
	using prepend = typelist::prepend_t<TypeID, ToPrepend>;

	/** Appends a type to the end of the %TypeList.
		This has the same semantics as using add with one passed type, but is provided so that you can be more explicit.
		@tparam ToAppend Type to append to the list.
	 */
	template <typename ToAppend>
	using append = typelist::append_t<TypeID, ToAppend>;

	/** Swaps the types at two indices in the list.
		A compile-time error will be raised if \c Index1 and \c Index2 are the same.
		@tparam Index1 The first index to be swapped.
		@tparam Index2 The second index to be swapped.
	 */
	template <size_t Index1, size_t Index2>
	using swap_at = typelist::swap_at_t<TypeID, Index1, Index2>;

	/** Swaps two types in the list.
		This finds the first instances of each type in the list, and uses \c swap_at to swap their indices.
		A compile-time error will be raised if \c Type1 and \c Type2 are the same type.
		@tparam Type1 The first type to be swapped.
		@tparam Type2 The second type to be swapped.
	 */
	template <typename Type1, typename Type2>
	using swap = typelist::swap_t<TypeID, Type1, Type2>;

	/** Gets the type of an element of the list at the specified index.
		@tparam Index The index in the list to retrieve. A compile-time error will be raised if this index is out of bounds for this %TypeList, unless the list is empty -- if the list is empty, this will always evaluate to NullType.
	 */
	template <size_t Index>
	using at = typelist::get_t<TypeID, Index>;

	/** The type of the first element in the %TypeList.
		This is semantically the same as \c at<0> .
	 */
	using front = typelist::get_first_t<TypeID>;

	/** The type of the last element in the %TypeList.
		This is semantically the same as \c at<size-1> .
	 */
	using back = typelist::get_last_t<TypeID>;

	/** The type of a new %TypeList with the same types as this one, but in reverse order. */
	using reverse = typelist::reverse_t<TypeID>;

	/** The index of the first occurrence of the specified type in the %TypeList.
		A compile-time error will be raised if the specified type is not in the %TypeList, unless the list is empty -- if the list is empty, this will always evaluate to \c static_cast<size_t>(-1) , regardless of what type was specified.
		@tparam Type The type to find in the %TypeList.
	 */
	template <typename Type>
	static constexpr const size_t index_of = typelist::find_v<TypeID, Type>;

	/** The type of a new %TypeList with all occurrences of the passed types removed.
		@tparam TypesToRemove List of types to remove from the list.
	 */
	template <typename... TypesToRemove>
	using remove = typelist::remove_t<TypeID, TypesToRemove...>;

	/** If any \c NullType elements are present in this %TypeList, removes them.
		This is semantically the same as \c remove<NullType> .
	 */
	using remove_null_types = typelist::remove_null_types_t<TypeID>;

	/** The type of a new %TypeList with the type at the specified index removed.
		All types after \c Index in the list will be moved forward by 1.
		A compile-time error will be raised if the \c Index is out of range for this %TypeList.
		@tparam Index Index to remove from the list.
	 */
	template <size_t Index>
	using remove_at = typelist::remove_at_t<TypeID, Index>;

	/** Removes the first element of the %TypeList.
		This is semantically the same as \c remove_at<0> .
	 */
	using remove_first = remove_at<0>;

	/** Removes the last element of the %TypeList.
		This is semantically the same as \c remove_at<size-1> .
	 */
	using remove_last = remove_at<size - 1>;

	/** Evaluates the predicate for each type in the list, and removes it if the predicate is met.
		@tparam UnaryPredicate A template class that will be instantiated for each type in the %TypeList and must provide a compile-time call operator that will determine if the predicate is met for the type it was instantiated with. Examples of unary predicates are \c std::is_floating_point or \c std::is_enum .
	 */
	template <template <typename> class UnaryPredicate>
	using remove_if = typelist::remove_if_t<TypeID, UnaryPredicate>;

	/** Evaluates the predicate for each type in the list, and removes it if the predicate is not met.
		@tparam UnaryPredicate A template class that will be instantiated for each type in the %TypeList and must provide a compile-time call operator that will determine if the predicate is met for the type it was instantiated with. Examples of unary predicates are \c std::is_floating_point or \c std::is_enum .
	 */
	template <template <typename> class UnaryPredicate>
	using remove_if_not = typelist::remove_if_not_t<TypeID, UnaryPredicate>;

	/** Replaces all occurrences of \c Replace in the list with \c With.
		@tparam Replace The type to be replaced in the list. A compile-time error will be raised if the list does not contain this type.
		@tparam With The type to replace \c Replace with.
	 */
	template <typename Replace, typename With>
	using replace = typelist::replace_t<TypeID, Replace, With>;

	/** Replaces the type at the specified \c Index with the new type \c ReplaceWith.
		A compile-time error will be raised if the \c Index is out of range for this %TypeList.
		@tparam Index The index of the element to be replaced in the list.
		@tparam ReplaceWith The new type to put at index \c Index in the list.
	 */
	template <size_t Index, typename ReplaceWith>
	using replace_at = typelist::replace_at_t<TypeID, Index, ReplaceWith>;

	/** True if any type appears in the list more than once. */
	static constexpr const bool contains_duplicates = typelist::contains_duplicates_v<TypeID>;

	/** The type of a new %TypeList with all duplicates removed -- if a type appeared in the original list multiple times, it will appear in the new list exactly once. */
	using remove_duplicates = typelist::remove_duplicates_t<TypeID>;

	/** The type of a new %TypeList that contains only types that were present in the original list and the passed list \c Other .

		For example:
		@code
		using TypeListA = TypeList<int, float, double>;

		using TypeListB = TypeList<int, double, size_t>;

		static_assert (TypeListA::common_with<TypeListB>::equal<TypeList<int, double>>);
		@endcode

		@tparam Other Another %TypeList to compare this one with.
	 */
	template <class Other>
	using common_with = typelist::common_t<TypeID, Other>;

	/** The type of a new %TypeList that contains only types that were present in the original list and not in the passed list \c Other .

		For example:
		@code
		using TypeListA = TypeList<int, float, double>;

		using TypeListB = TypeList<int, double, size_t>;

		static_assert (TypeListA::not_in<TypeListB>::equal<TypeList<float>>);
		@endcode

		@tparam Other  Another %TypeList to compare this one with.
	 */
	template <class Other>
	using not_in = typelist::not_in_t<TypeID, Other>;

	/** Applies the types in this %TypeList to a class template, as its template arguments.

		For example:
		@code
		using TypeListA = TypeList<int, size_t, double, float>;

		using ExpectedResult = std::variant<int, size_t, double, float>;

		static_assert (std::is_same_v <TypeListA::apply_to<std::variant>, ExpectedResult>);
		@endcode

		@tparam T Template class to specialize using the types in this list.
	 */
	template <template <typename...> class T>
	using apply_to = T<Types...>;

	/** Constructs an object of the type at the given index in the list.
		If the TypeList is empty, this will return a \c NullType object.
		@tparam Index The index in the list of the type to be constructed. A compile-time error will be raised if this index is out of range for the list.
		@tparam Args Constructor arguments for the new object.
	 */
	template <size_t Index, typename... Args>
	static constexpr at<Index> construct (Args&&... args) noexcept (noexcept (at<Index> (std::forward<Args> (args)...)))
	{
		return at<Index> (std::forward<Args> (args)...);
	}

	/** The same as \c construct() , except it returns a \c unique_ptr to the new object.
		@tparam Index The index in the list of the type to be constructed. A compile-time error will be raised if this index is out of range for the list.
		@tparam Args Constructor arguments for the new object.
	 */
	template <size_t Index, typename... Args>
	static std::unique_ptr<at<Index>> make_unique (Args&&... args) noexcept (noexcept (at<Index> (std::forward<Args> (args)...)))
	{
		return std::make_unique<at<Index>> (std::forward<Args> (args)...);
	}

	/** Calls a templated function for each type in the list.
		If the list is empty, this function does nothing.
		@tparam Func Templated function to call.
		@tparam Args Arguments to forward to the function.
	 */
	template <func::Function Func, typename... Args>
	static constexpr void for_each (Func&& f, Args&&... args) noexcept (noexcept ((std::forward<Func> (f).template operator()<Types> (std::forward<Args> (args)...), ...)))
	{
		(std::forward<Func> (f).template operator()<Types> (std::forward<Args> (args)...), ...);
	}

	/** Calls a templated function once, with the template arguments being the types contained in the list.
		If the list is empty, the function is called with an empty template parameter list.
		@tparam Func Templated function to call.
		@tparam Args Arguments to forward to the function.
		@return The return type of the function.
	 */
	template <func::Function Func, typename... Args>
	static constexpr auto for_all (Func&& f, Args&&... args) noexcept (noexcept (f().template operator()<Types...> (std::forward<Args> (args)...)))
		-> func::result_type<Func, Args...>
	{
		return f().template operator()<Types...> (std::forward<Args> (args)...);
	}
};

/*----------------------------------------------------------------------------------------------------------------------*/

/// @cond internals

#pragma mark Empty TypeList specialization

/** The empty specialization of TypeList requires some special implementation. */
template <>
class LIMES_EXPORT TypeList<> final
{
public:

	TypeList() = delete;

	using TypeID = TypeList<>;
	using clear	 = TypeID;

	static constinit const size_t size = 0;

	static constinit const bool empty = true;

	template <class Other>
	static constexpr const bool equal = typelist::is_empty<Other>;

	template <class Other>
	static constexpr const bool equal_ignore_order = typelist::is_empty<Other>;

	template <typename...>
	static constinit const bool contains = false;

	template <typename...>
	static constinit const bool contains_or = false;

	template <typename>
	static constinit const size_t num_of = 0;

	template <template <typename> class>
	static constinit const size_t count_if = 0;

	template <template <typename> class UnaryPredicate>
	static constinit const size_t count_if_not = 0;

	template <typename... TypesToAdd>
	using add = TypeList<TypesToAdd...>;

	template <class Other>
	using add_from = Other;

	template <typename... TypesToAdd>
	using addIfAbsent = typelist::addIfAbsent_t<TypeID, TypesToAdd...>;

	template <size_t, typename ToInsert>
	using insert_at = TypeList<ToInsert>;

	template <typename ToPrepend>
	using prepend = TypeList<ToPrepend>;

	template <typename ToAppend>
	using append = TypeList<ToAppend>;

	template <size_t, size_t>
	using swap_at = TypeID;

	template <typename, typename>
	using swap = TypeID;

	template <size_t>
	using at = NullType;

	using front = NullType;
	using back	= NullType;

	using reverse = TypeID;

	template <typename>
	static constinit const size_t index_of = typelist::invalid_index;

	template <typename...>
	using remove = TypeID;

	template <size_t>
	using remove_at = TypeID;

	using remove_first = TypeID;
	using remove_last  = TypeID;

	using remove_null_types = TypeID;

	template <template <typename> class>
	using remove_if = TypeID;

	template <template <typename> class>
	using remove_if_not = TypeID;

	template <typename, typename>
	using replace = TypeID;

	template <size_t, typename>
	using replace_at = TypeID;

	static constinit const bool contains_duplicates = false;

	using remove_duplicates = TypeID;

	template <class>
	using common_with = TypeID;

	template <class Other>
	using not_in = TypeID;

	template <template <typename...> class T>
	using apply_to = T<>;

	template <size_t, typename... Args>
	static constexpr NullType construct (Args&&...) noexcept
	{
		return {};
	}

	template <size_t, typename... Args>
	static std::unique_ptr<NullType> make_unique (Args&&...) noexcept
	{
		return std::make_unique<NullType>();
	}

	template <func::Function Func, typename... Args>
	static constexpr void for_each (Func&&, Args&&...) noexcept
	{
	}

	template <func::Function Func, typename... Args>
	static constexpr auto for_all (Func&& f, Args&&... args) noexcept (noexcept (f().template operator()<> (std::forward<Args> (args)...)))
		-> func::result_type<Func, Args...>
	{
		return f().template operator()<> (std::forward<Args> (args)...);
	}
};

/// @endcond

#pragma mark Misc utilities

/** A utility typedef for an empty TypeList.
	@ingroup meta
 */
LIMES_EXPORT using Empty = TypeList<>;

/*----------------------------------------------------------------------------------------------------------------------*/

/// @cond internals

template <typename>
struct LIMES_EXPORT make_type_list_from;

template <template <typename...> class T, typename... Args>
struct LIMES_EXPORT make_type_list_from<T<Args...>> final
{
	using type = TypeList<Args...>;
};

/// @endcond

/** Creates a typelist from the template arguments of a template type.
	For example:
	@code
	using OriginalType = std::variant<int, float, double>;

	using TypeList = make_type_list_from_t<OriginalType>;

	static_assert (TypeList::equal<TypeList<int, float, double>>);
	@endcode
	@ingroup meta
 */
template <typename... Args>
LIMES_EXPORT using make_type_list_from_t = typename make_type_list_from<Args...>::type;

/*----------------------------------------------------------------------------------------------------------------------*/

/** True if the specified type is a specialization of TypeList.
	@ingroup meta
 */
template <class T>
LIMES_EXPORT static constinit const bool is_typelist = meta::is_specialization<T, TypeList>::value;

/** @concept type_list
	@see is_typelist
	@ingroup meta
 */
template <typename T>
concept type_list = is_typelist<T>;

}  // namespace meta

LIMES_END_NAMESPACE
