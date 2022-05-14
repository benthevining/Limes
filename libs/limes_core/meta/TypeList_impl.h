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

#include <limes_export.h>
#include <limes_namespace.h>
#include <type_traits>
#include <cstddef>
#include "TypeTraits.h"

#ifdef DOXYGEN

LIMES_BEGIN_NAMESPACE

/** This namespace contains implementation details for the TypeList class. */
namespace typelist
{

/** A placeholder class representing a type that is null, invalid, or nonexistent. */
struct LIMES_EXPORT NullType final
{
};

/** Returns true if \c T is a \c NullType .
	@tparam T Type to test
	@see NullType
 */
template <typename T>
LIMES_EXPORT static constexpr const bool is_null_type = std::is_same_v<T, NullType>;

}  // namespace typelist

LIMES_END_NAMESPACE

#else

/// @cond

LIMES_BEGIN_NAMESPACE

namespace typelist
{

using std::size_t;

template <size_t Value>
using SizeConstant = std::integral_constant<size_t, Value>;

LIMES_EXPORT static constinit const size_t invalid_index = static_cast<size_t> (-1);

struct LIMES_EXPORT NullType final
{
};

template <typename T>
LIMES_EXPORT static constexpr const bool is_null_type = std::is_same_v<T, NullType>;

template <typename... Types>
struct LIMES_NO_EXPORT InternalTypeList final
{
	template <template <typename...> class TypelistType>
	using makeTypelist = TypelistType<Types...>;
};

template <template <typename> class Condition>
struct LIMES_NO_EXPORT TypeErasedUnaryPredicate final
{
	template <typename Type>
	static constexpr const bool met = Condition<Type>();
};

/*----------------------------------------------------------------------------------------------------------------------*/

template <class LHS, class RHS>
struct LIMES_EXPORT are_same final : std::false_type
{
};

template <template <typename...> class LHS, template <typename...> class RHS, typename... Args>
struct LIMES_EXPORT are_same<LHS<Args...>, RHS<Args...>> final : std::true_type
{
};

template <class LHS, class RHS>
LIMES_EXPORT static constexpr const bool are_same_v = are_same<LHS, RHS>::value;

/*----------------------------------------------------------------------------------------------------------------------*/

template <class Typelist, typename... TypesToFind>
struct LIMES_EXPORT contains final
{
private:

	template <class TypeList, typename TypeToFind>
	struct contains_one;

	template <template <typename...> class TypeList, typename TypeToFind, typename... Args>
	struct contains_one<TypeList<Args...>, TypeToFind> final : std::bool_constant<(std::is_same_v<TypeToFind, Args> || ...)>
	{
	};

public:

	static constexpr const bool value = (contains_one<Typelist, TypesToFind>::value && ...);
};

template <class Typelist, typename... TypesToFind>
LIMES_EXPORT static constexpr const bool contains_v = contains<Typelist, TypesToFind...>::value;

template <class Typelist, typename... TypesToFind>
struct LIMES_EXPORT contains_or final : std::bool_constant<(contains_v<Typelist, TypesToFind> || ...)>
{
};

template <class Typelist, typename... TypesToFind>
LIMES_EXPORT static constexpr const bool contains_or_v = contains_or<Typelist, TypesToFind...>::value;

/*----------------------------------------------------------------------------------------------------------------------*/

template <class Typelist>
struct LIMES_EXPORT size final : SizeConstant<0>
{
};

template <template <typename...> class Typelist, typename... Args>
struct LIMES_EXPORT size<Typelist<Args...>> final : SizeConstant<sizeof...(Args)>
{
};

template <class Typelist>
LIMES_EXPORT static constexpr const size_t size_v = size<Typelist>::value;

template <class Typelist>
LIMES_EXPORT static constexpr const bool is_empty = (size_v<Typelist> == 0);

/*----------------------------------------------------------------------------------------------------------------------*/

template <class Typelist, typename... TypesToAdd>
struct LIMES_EXPORT add;

template <template <typename...> class Typelist, typename... TypesToAdd, typename... Args>
struct LIMES_EXPORT add<Typelist<Args...>, TypesToAdd...> final
{
	using type = Typelist<Args..., TypesToAdd...>;
};

template <class Typelist, typename... TypesToAdd>
LIMES_EXPORT using add_t = typename add<Typelist, TypesToAdd...>::type;

/*----------------------------------------------------------------------------------------------------------------------*/

template <class List1, class List2>
struct LIMES_EXPORT add_from;

template <template <typename...> class List1, template <typename...> class List2, typename... List1Args, typename... List2Args>
struct LIMES_EXPORT add_from<List1<List1Args...>, List2<List2Args...>> final
{
	using type = List1<List1Args..., List2Args...>;
};

template <class List1, class List2>
LIMES_EXPORT using add_from_t = typename add_from<List1, List2>::type;

/*----------------------------------------------------------------------------------------------------------------------*/

template <class Typelist, typename... TypesToAdd>
struct LIMES_EXPORT addIfAbsent;

template <template <typename...> class Typelist, typename... TypesToAdd, typename... Args>
struct LIMES_EXPORT addIfAbsent<Typelist<Args...>, TypesToAdd...> final
{
private:

	template <class ListType, typename Type>
	using add_one = std::conditional_t<
		(! contains_v<Typelist<Args...>, Type>),
		add_t<ListType, Type>,
		ListType>;

	template <class ListType, typename... ListArgs>
	struct adder;

	template <class ListType, typename First, typename... Others>
	struct adder<ListType, First, Others...> final
	{
		using type = typename adder<
			add_one<ListType, First>,
			Others...>::type;
	};

	template <class ListType, typename First>
	struct adder<ListType, First> final
	{
		using type = add_one<ListType, First>;
	};

public:

	using type = typename adder<Typelist<Args...>, TypesToAdd...>::type;
};

template <class Typelist, typename... TypesToAdd>
LIMES_EXPORT using addIfAbsent_t = typename addIfAbsent<Typelist, TypesToAdd...>::type;

/*----------------------------------------------------------------------------------------------------------------------*/

template <class List1, class List2>
struct LIMES_EXPORT common_with;

template <template <typename...> class List1, template <typename...> class List2, typename... List1Types, typename... List2Types>
struct LIMES_EXPORT common_with<List1<List1Types...>, List2<List2Types...>> final
{
private:

	template <class NewList, typename Type>
	using conditional_add = std::conditional_t<
		contains_v<List2<List2Types...>, Type>,
		add_t<NewList, Type>,
		NewList>;

	template <class NewList, typename... Types>
	struct builder;

	template <class NewList, typename First, typename... Others>
	struct builder<NewList, First, Others...> final
	{
		using type = typename builder<
			conditional_add<NewList, First>,
			Others...>::type;
	};

	template <class NewList, typename First>
	struct builder<NewList, First> final
	{
		using type = conditional_add<NewList, First>;
	};

public:

	using type = typename builder<InternalTypeList<>, List1Types...>::type::template makeTypelist<List1>;
};

template <class List1, class List2>
LIMES_EXPORT using common_t = typename common_with<List1, List2>::type;

/*----------------------------------------------------------------------------------------------------------------------*/

template <class List1, class List2>
struct LIMES_EXPORT not_in;

template <template <typename...> class List1, template <typename...> class List2, typename... List1Types, typename... List2Types>
struct LIMES_EXPORT not_in<List1<List1Types...>, List2<List2Types...>> final
{
private:

	template <class NewList, typename Type>
	using conditional_add = std::conditional_t<
		contains_v<List2<List2Types...>, Type>,
		NewList,
		add_t<NewList, Type>>;

	template <class NewList, typename... Types>
	struct builder;

	template <class NewList, typename First, typename... Others>
	struct builder<NewList, First, Others...> final
	{
		using type = typename builder<
			conditional_add<NewList, First>,
			Others...>::type;
	};

	template <class NewList, typename First>
	struct builder<NewList, First> final
	{
		using type = conditional_add<NewList, First>;
	};

public:

	using type = typename builder<InternalTypeList<>, List1Types...>::type::template makeTypelist<List1>;
};

template <class List1, class List2>
LIMES_EXPORT using not_in_t = typename not_in<List1, List2>::type;

template <class List1, class List2>
LIMES_EXPORT static constexpr const bool same_ignoring_order_v = is_empty<not_in_t<List1, List2>>&& is_empty<not_in_t<List2, List1>>;

/*----------------------------------------------------------------------------------------------------------------------*/

template <class Typelist, size_t Index, typename ToInsert>
struct LIMES_EXPORT insert_at;

template <template <typename...> class Typelist, size_t Index, typename ToInsert, typename... Args>
struct LIMES_EXPORT insert_at<Typelist<Args...>, Index, ToInsert> final
{
private:

	static_assert (Index <= size_v<Typelist<Args...>>, "Insertion index in type list is out of bounds!");

	template <size_t CurrentIndex, class ListType, typename Type>
	using inserter = std::conditional_t<
		CurrentIndex == Index,
		add_t<add_t<ListType, ToInsert>, Type>,
		add_t<ListType, Type>>;

	template <size_t CurrentIndex, class ListType, typename... ListArgs>
	struct rebuilder;

	template <size_t CurrentIndex, class ListType, typename First, typename... Others>
	struct rebuilder<CurrentIndex, ListType, First, Others...> final
	{
		using type = typename rebuilder<CurrentIndex + 1, inserter<CurrentIndex, ListType, First>, Others...>::type;
	};

	template <size_t CurrentIndex, class ListType, typename First>
	struct rebuilder<CurrentIndex, ListType, First> final
	{
	public:

		using type = inserter<CurrentIndex, ListType, First>;
	};

public:

	using type = typename rebuilder<0, InternalTypeList<>, Args...>::type::template makeTypelist<Typelist>;
};

template <class Typelist, size_t Index, typename ToInsert>
LIMES_EXPORT using insert_at_t = typename insert_at<Typelist, Index, ToInsert>::type;

template <class Typelist, typename ToPrepend>
LIMES_EXPORT using prepend_t = insert_at_t<Typelist, 0, ToPrepend>;

template <class Typelist, typename ToAppend>
LIMES_EXPORT using append_t = add_t<Typelist, ToAppend>;

/*----------------------------------------------------------------------------------------------------------------------*/

template <class Typelist, size_t Index>
struct LIMES_EXPORT get;

template <template <typename...> class Typelist, size_t Index, typename... Args>
struct LIMES_EXPORT get<Typelist<Args...>, Index> final
{
private:

	static_assert (Index <= size_v<Typelist<Args...>>, "Search index in type list is out of bounds!");

	template <size_t SearchIndex, typename First, typename... Others>
	struct getter final
	{
		using type = std::conditional_t<
			SearchIndex == 0,
			First,
			typename getter<SearchIndex - 1, Others...>::type>;
	};

	template <size_t SearchIndex, typename First>
	struct getter<SearchIndex, First> final
	{
		using type = std::conditional_t<
			SearchIndex == 0,
			First,
			NullType>;
	};

public:

	using type = typename getter<Index, Args...>::type;
};

template <class Typelist, size_t Index>
LIMES_EXPORT using get_t = typename get<Typelist, Index>::type;

template <class Typelist>
LIMES_EXPORT using get_first_t = get_t<Typelist, 0>;

template <class Typelist>
LIMES_EXPORT using get_last_t = get_t<Typelist, size_v<Typelist> - 1>;

/*----------------------------------------------------------------------------------------------------------------------*/

template <class Typelist, typename TypeToFind>
struct LIMES_EXPORT find;

template <template <typename...> class Typelist, typename TypeToFind, typename... Args>
struct LIMES_EXPORT find<Typelist<Args...>, TypeToFind> final
{
private:

	static_assert (contains_v<Typelist<Args...>, TypeToFind>,
				   "TypeToFind is not in the Typelist!");

	template <size_t Index, typename... Types>
	struct finder;

	template <size_t Index, typename First, typename... Others>
	struct finder<Index, First, Others...> final : SizeConstant<std::conditional_t<std::is_same_v<First, TypeToFind>,
																				   std::integral_constant<size_t, Index>,
																				   finder<Index + 1, Others...>>::value>
	{
	};

	template <size_t Index, typename First>
	struct finder<Index, First> final : SizeConstant<Index>
	{
	};

public:

	static constexpr const size_t value = finder<0, Args...>::value;
};

template <class Typelist, typename TypeToFind>
LIMES_EXPORT static constexpr const size_t find_v = find<Typelist, TypeToFind>::value;

/*----------------------------------------------------------------------------------------------------------------------*/

template <class Typelist, typename... TypesToRemove>
struct LIMES_EXPORT remove;

template <template <typename...> class Typelist, typename... TypesToRemove, typename... Args>
struct LIMES_EXPORT remove<Typelist<Args...>, TypesToRemove...>
{
private:

	template <typename Type>
	static constinit const bool isTypeAbsent = (! std::is_same_v<TypesToRemove, Type> && ...);

	template <class ListType, typename Type>
	using remove_one = std::conditional_t<isTypeAbsent<Type>, add_t<ListType, Type>, ListType>;

	template <class ListType, typename... ListArgs>
	struct remover;

	template <class ListType, typename First, typename... Others>
	struct remover<ListType, First, Others...> final
	{
		using type = typename remover<
			remove_one<ListType, First>,
			Others...>::type;
	};

	template <class ListType, typename First>
	struct remover<ListType, First>
	{
		using type = remove_one<ListType, First>;
	};

public:

	using type = typename remover<InternalTypeList<>, Args...>::type::template makeTypelist<Typelist>;
};

template <class Typelist, typename... TypesToRemove>
LIMES_EXPORT using remove_t = typename remove<Typelist, TypesToRemove...>::type;

template <class Typelist>
LIMES_EXPORT using remove_null_types_t = remove_t<Typelist, NullType>;

/*----------------------------------------------------------------------------------------------------------------------*/

template <class Typelist, size_t Index>
struct LIMES_EXPORT remove_at;

template <template <typename...> class Typelist, size_t Index, typename... Args>
struct LIMES_EXPORT remove_at<Typelist<Args...>, Index> final
{
private:

	static_assert (Index <= size_v<Typelist<Args...>>, "Cannot remove item out of range of TypeList!");

	template <size_t CurrentIndex, class ListType, typename Type>
	using do_remove = std::conditional_t<CurrentIndex == Index, ListType, add_t<ListType, Type>>;

	template <size_t CurrentIndex, class ListType, typename... ListArgs>
	struct remover;

	template <size_t CurrentIndex, class ListType, typename First, typename... Others>
	struct remover<CurrentIndex, ListType, First, Others...> final
	{
		using type = typename remover<
			CurrentIndex + 1,
			do_remove<CurrentIndex, ListType, First>,
			Others...>::type;
	};

	template <size_t CurrentIndex, class ListType, typename First>
	struct remover<CurrentIndex, ListType, First>
	{
		using type = do_remove<CurrentIndex, ListType, First>;
	};

public:

	using type = typename remover<0, InternalTypeList<>, Args...>::type::template makeTypelist<Typelist>;
};

template <class Typelist, size_t Index>
LIMES_EXPORT using remove_at_t = typename remove_at<Typelist, Index>::type;

/*----------------------------------------------------------------------------------------------------------------------*/

template <class Typelist, template <typename> class UnaryPredicate>
struct LIMES_EXPORT remove_if;

template <template <typename...> class Typelist, template <typename> class UnaryPredicate, typename... Args>
struct LIMES_EXPORT remove_if<Typelist<Args...>, UnaryPredicate> final
{
private:

	using Condition = TypeErasedUnaryPredicate<UnaryPredicate>;

	template <class TypeList, typename Type>
	using conditional_add = std::conditional_t<Condition::template met<Type>,
											   TypeList,
											   add_t<TypeList, Type>>;

	template <class TypeList, typename...>
	struct rebuilder;

	template <class TypeList, typename First, typename... Others>
	struct rebuilder<TypeList, First, Others...> final
	{
		using type = typename rebuilder<
			conditional_add<TypeList, First>,
			Others...>::type;
	};

	template <class TypeList, typename First>
	struct rebuilder<TypeList, First> final
	{
		using type = conditional_add<TypeList, First>;
	};

public:

	using type = typename rebuilder<InternalTypeList<>, Args...>::type::template makeTypelist<Typelist>;
};

template <class Typelist, template <typename> class UnaryPredicate>
LIMES_EXPORT using remove_if_t = typename remove_if<Typelist, UnaryPredicate>::type;

/*----------------------------------------------------------------------------------------------------------------------*/

template <class Typelist, template <typename> class UnaryPredicate>
struct LIMES_EXPORT remove_if_not;

template <template <typename...> class Typelist, template <typename> class UnaryPredicate, typename... Args>
struct LIMES_EXPORT remove_if_not<Typelist<Args...>, UnaryPredicate> final
{
private:

	using Condition = TypeErasedUnaryPredicate<UnaryPredicate>;

	template <class TypeList, typename Type>
	using conditional_add = std::conditional_t<! Condition::template met<Type>,
											   TypeList,
											   add_t<TypeList, Type>>;

	template <class TypeList, typename...>
	struct rebuilder;

	template <class TypeList, typename First, typename... Others>
	struct rebuilder<TypeList, First, Others...> final
	{
		using type = typename rebuilder<
			conditional_add<TypeList, First>,
			Others...>::type;
	};

	template <class TypeList, typename First>
	struct rebuilder<TypeList, First> final
	{
		using type = conditional_add<TypeList, First>;
	};

public:

	using type = typename rebuilder<InternalTypeList<>, Args...>::type::template makeTypelist<Typelist>;
};

template <class Typelist, template <typename> class UnaryPredicate>
LIMES_EXPORT using remove_if_not_t = typename remove_if_not<Typelist, UnaryPredicate>::type;

/*----------------------------------------------------------------------------------------------------------------------*/

template <class Typelist, typename ToReplace, typename ReplaceWith>
struct LIMES_EXPORT replace;

template <template <typename...> class Typelist, typename ToReplace, typename ReplaceWith, typename... Args>
struct LIMES_EXPORT replace<Typelist<Args...>, ToReplace, ReplaceWith> final
{
private:

	static_assert (contains_v<Typelist<Args...>, ToReplace>, "Cannot replace type not in Typelist!");

	template <class TypeList, typename First, typename InPlaceType>
	using do_replace = add_t<TypeList, std::conditional_t<std::is_same_v<First, ToReplace>, InPlaceType, First>>;

	template <class TypeList, typename InPlaceType, typename...>
	struct replacer;

	template <class TypeList, typename InPlaceType, typename First>
	struct replacer<TypeList, InPlaceType, First> final
	{
		using type = do_replace<TypeList, First, InPlaceType>;
	};

	template <class TypeList, typename InPlaceType, typename First, typename... Others>
	struct replacer<TypeList, InPlaceType, First, Others...> final
	{
		using type = typename replacer<
			do_replace<TypeList, First, InPlaceType>,
			InPlaceType,
			Others...>::type;
	};

public:

	using type = typename replacer<InternalTypeList<>, ReplaceWith, Args...>::type::template makeTypelist<Typelist>;
};

template <class Typelist, typename ToReplace, typename ReplaceWith>
LIMES_EXPORT using replace_t = typename replace<Typelist, ToReplace, ReplaceWith>::type;

/*----------------------------------------------------------------------------------------------------------------------*/

template <class Typelist, size_t Index, typename ReplaceWith>
struct LIMES_EXPORT replace_at;

template <template <typename...> class Typelist, size_t Index, typename ReplaceWith, typename... Args>
struct LIMES_EXPORT replace_at<Typelist<Args...>, Index, ReplaceWith> final
{
private:

	static_assert (Index <= size_v<Typelist<Args...>>, "Cannot replace item out of range of TypeList!");

	template <size_t CurrentIndex, class ListType, typename First>
	using do_replace = std::conditional_t<CurrentIndex == Index, add_t<ListType, ReplaceWith>, add_t<ListType, First>>;

	template <size_t CurrentIndex, class ListType, typename... ListArgs>
	struct rebuilder;

	template <size_t CurrentIndex, class ListType, typename First, typename... Others>
	struct rebuilder<CurrentIndex, ListType, First, Others...> final
	{
		using type = typename rebuilder<
			CurrentIndex + 1,
			do_replace<CurrentIndex, ListType, First>,
			Others...>::type;
	};

	template <size_t CurrentIndex, class ListType, typename First>
	struct rebuilder<CurrentIndex, ListType, First>
	{
		using type = do_replace<CurrentIndex, ListType, First>;
	};

public:

	using type = typename rebuilder<0, InternalTypeList<>, Args...>::type::template makeTypelist<Typelist>;
};

template <class Typelist, size_t Index, typename ReplaceWith>
LIMES_EXPORT using replace_at_t = typename replace_at<Typelist, Index, ReplaceWith>::type;

/*----------------------------------------------------------------------------------------------------------------------*/

template <class Typelist, size_t Index1, size_t Index2>
struct LIMES_EXPORT swap_at;

template <template <typename...> class Typelist, size_t Index1, size_t Index2, typename... Args>
struct LIMES_EXPORT swap_at<Typelist<Args...>, Index1, Index2> final
{
private:

	static_assert (Index1 != Index2, "Cannot swap elements at the same index!");

	using OriginalList = Typelist<Args...>;

	static_assert (Index1 <= size_v<OriginalList>, "Swap index out of range of TypeList!");
	static_assert (Index2 <= size_v<OriginalList>, "Swap index out of range of TypeList!");

	template <size_t CurrentIndex, class ListType, typename First>
	using do_swap = std::conditional_t<CurrentIndex == Index1,
									   add_t<ListType, get_t<OriginalList, Index2>>,
									   std::conditional_t<CurrentIndex == Index2,
														  add_t<ListType, get_t<OriginalList, Index1>>,
														  add_t<ListType, First>>>;

	template <size_t CurrentIndex, class ListType, typename...>
	struct rebuilder;

	template <size_t CurrentIndex, class ListType, typename First, typename... Others>
	struct rebuilder<CurrentIndex, ListType, First, Others...> final
	{
		using type = typename rebuilder<
			CurrentIndex + 1,
			do_swap<CurrentIndex, ListType, First>,
			Others...>::type;
	};

	template <size_t CurrentIndex, class ListType, typename First>
	struct rebuilder<CurrentIndex, ListType, First> final
	{
		using type = do_swap<CurrentIndex, ListType, First>;
	};

public:

	using type = typename rebuilder<0, InternalTypeList<>, Args...>::type::template makeTypelist<Typelist>;
};

template <class Typelist, size_t Index1, size_t Index2>
LIMES_EXPORT using swap_at_t = typename swap_at<Typelist, Index1, Index2>::type;

/*----------------------------------------------------------------------------------------------------------------------*/

template <class Typelist, typename Type1, typename Type2>
struct LIMES_EXPORT swap;

template <template <typename...> class Typelist, typename Type1, typename Type2, typename... Args>
struct LIMES_EXPORT swap<Typelist<Args...>, Type1, Type2> final
{
private:

	static_assert (! std::is_same_v<Type1, Type2>, "Type1 and Type2 need to be different types");

	using OriginalList = Typelist<Args...>;

	static_assert (contains_v<OriginalList, Type1>, "Cannot swap types not in Typelist!");
	static_assert (contains_v<OriginalList, Type2>, "Cannot swap types not in Typelist!");

public:

	using type = swap_at_t<OriginalList,
						   find_v<OriginalList, Type1>,
						   find_v<OriginalList, Type2>>;
};

template <class Typelist, typename Type1, typename Type2>
LIMES_EXPORT using swap_t = typename swap<Typelist, Type1, Type2>::type;

/*----------------------------------------------------------------------------------------------------------------------*/

template <class Typelist, typename ToCount>
struct LIMES_EXPORT count;

template <template <typename...> class Typelist, typename ToCount, typename... Args>
struct LIMES_EXPORT count<Typelist<Args...>, ToCount> final
{
private:

	template <size_t Count, size_t CurrentIndex>
	struct counter final
	{
	private:

		static constexpr const size_t thisValue = std::conditional_t<
			std::is_same_v<ToCount, get_t<Typelist<Args...>, CurrentIndex>>,
			SizeConstant<Count + 1>,
			SizeConstant<Count>>::value;

	public:

		static constexpr const size_t value = std::conditional_t<
			CurrentIndex == size_v<Typelist<Args...>>,
			SizeConstant<thisValue>,
			counter<thisValue, CurrentIndex + 1>>::value;
	};

public:

	static constexpr const size_t value = counter<0, 0>::value;
};

template <class Typelist, typename ToCount>
LIMES_EXPORT static constexpr const size_t count_v = count<Typelist, ToCount>::value;

/*----------------------------------------------------------------------------------------------------------------------*/

template <class Typelist, template <typename> class UnaryPredicate>
struct LIMES_EXPORT count_if;

template <template <typename...> class Typelist, template <typename> class UnaryPredicate, typename... Args>
struct LIMES_EXPORT count_if<Typelist<Args...>, UnaryPredicate> final
{
private:

	using Condition = TypeErasedUnaryPredicate<UnaryPredicate>;

	template <size_t Count, size_t CurrentIndex>
	struct counter final
	{
	private:

		using thisValue = std::conditional_t<
			Condition::template met<get_t<Typelist<Args...>, CurrentIndex>>,
			SizeConstant<Count + 1>,
			SizeConstant<Count>>;

	public:

		static constexpr const size_t value = std::conditional_t<
			CurrentIndex == size_v<Typelist<Args...>>,
			thisValue,
			counter<thisValue::value, CurrentIndex + 1>>::value;
	};

public:

	static constexpr const size_t value = counter<0, 0>::value;
};

template <class Typelist, template <typename> class UnaryPredicate>
LIMES_EXPORT static constexpr const size_t count_if_v = count_if<Typelist, UnaryPredicate>::value;

/*----------------------------------------------------------------------------------------------------------------------*/

template <class Typelist, template <typename> class UnaryPredicate>
struct LIMES_EXPORT count_if_not : SizeConstant<(size_v<Typelist> - count_if_v<Typelist, UnaryPredicate>)>
{
};

template <class Typelist, template <typename> class UnaryPredicate>
LIMES_EXPORT static constexpr const size_t count_if_not_v = count_if_not<Typelist, UnaryPredicate>::value;

/*----------------------------------------------------------------------------------------------------------------------*/

template <class Typelist>
struct LIMES_EXPORT remove_duplicates;

template <template <typename...> class Typelist, typename... Args>
struct LIMES_EXPORT remove_duplicates<Typelist<Args...>> final
{
private:

	template <size_t CurrentIndex>
	using CurrentType = get_t<Typelist<Args...>, CurrentIndex>;

	template <size_t CurrentIndex, class ListType>
	using remover = std::conditional_t<contains_v<ListType, CurrentType<CurrentIndex>>,
									   ListType,
									   add_t<ListType, CurrentType<CurrentIndex>>>;

	template <size_t CurrentIndex, class ListType, typename...>
	struct rebuilder;

	template <size_t CurrentIndex, class ListType, typename First, typename... Others>
	struct rebuilder<CurrentIndex, ListType, First, Others...> final
	{
		using type = typename rebuilder<
			CurrentIndex + 1,
			remover<CurrentIndex, ListType>,
			Others...>::type;
	};

	template <size_t CurrentIndex, class ListType, typename First>
	struct rebuilder<CurrentIndex, ListType, First> final
	{
		using type = remover<CurrentIndex, ListType>;
	};

public:

	using type = typename rebuilder<0, InternalTypeList<>, Args...>::type::template makeTypelist<Typelist>;
};

template <class Typelist>
LIMES_EXPORT using remove_duplicates_t = typename remove_duplicates<Typelist>::type;

/*----------------------------------------------------------------------------------------------------------------------*/

template <class Typelist>
struct LIMES_EXPORT contains_duplicates;

template <template <typename...> class Typelist, typename... Args>
struct LIMES_EXPORT contains_duplicates<Typelist<Args...>> final : std::bool_constant<
																	   (size_v<Typelist<Args...>> != size_v<remove_duplicates_t<Typelist<Args...>>>)>
{
};

template <class Typelist>
LIMES_EXPORT static constexpr const bool contains_duplicates_v = contains_duplicates<Typelist>::value;

/*----------------------------------------------------------------------------------------------------------------------*/

template <class Typelist>
struct LIMES_EXPORT reverse;

template <template <typename...> class Typelist, typename... Args>
struct LIMES_EXPORT reverse<Typelist<Args...>> final
{
private:

	template <typename>
	struct base;

	template <template <typename...> class T, typename... Types>
	struct base<T<Types...>> final
	{
		using type = T<>;
	};

	template <typename T, typename = typename base<T>::type>
	struct reverse_impl;

	template <template <typename...> class T, typename... Types>
	struct reverse_impl<typename base<T<Types...>>::type, T<Types...>> final
	{
		using type = T<Types...>;
	};

	template <template <typename...> class T, typename First, typename... Rest, typename... done>
	struct reverse_impl<T<First, Rest...>, T<done...>> final
	{
		using type = typename reverse_impl<T<Rest...>, T<First, done...>>::type;
	};

public:

	using type = typename reverse_impl<InternalTypeList<Args...>>::type::template makeTypelist<Typelist>;
};

template <class Typelist>
LIMES_EXPORT using reverse_t = typename reverse<Typelist>::type;

}  // namespace typelist

LIMES_END_NAMESPACE

/// @endcond

#endif
