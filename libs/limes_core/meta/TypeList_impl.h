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

LIMES_BEGIN_NAMESPACE

namespace typelist
{

using std::size_t;

LIMES_EXPORT static constinit const auto invalid_index = static_cast<size_t> (-1);

struct LIMES_EXPORT NoType final
{
};

template <typename T>
LIMES_EXPORT static constinit const bool is_null_type = std::is_same_v<T, NoType>;

template <typename... Types>
struct LIMES_NO_EXPORT InternalTypeList final
{
	template <template <typename...> class TypelistType>
	using makeTypelist = TypelistType<Types...>;
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
LIMES_EXPORT constinit const auto are_same_v = are_same<LHS, RHS>::value;

/*----------------------------------------------------------------------------------------------------------------------*/

template <class Typelist, typename TypeToFind>
struct LIMES_EXPORT contains;

template <template <typename...> class Typelist, typename TypeToFind, typename... Args>
struct LIMES_EXPORT contains<Typelist<Args...>, TypeToFind> final : std::bool_constant<(std::is_same_v<TypeToFind, Args> || ...)>
{
};

template <class Typelist, typename TypeToFind>
LIMES_EXPORT constinit const auto contains_v = contains<Typelist, TypeToFind>::value;

/*----------------------------------------------------------------------------------------------------------------------*/

template <class Typelist>
struct LIMES_EXPORT size final : std::integral_constant<size_t, 0>
{
};

template <template <typename...> class Typelist, typename... Args>
struct LIMES_EXPORT size<Typelist<Args...>> final : std::integral_constant<size_t, sizeof...(Args)>
{
};

template <class Typelist>
LIMES_EXPORT constinit const auto size_v = size<Typelist>::value;

template <class Typelist>
LIMES_EXPORT constinit const bool is_empty = (size_v<Typelist> == 0);

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

template <class Typelist, typename... TypesToAdd>
struct LIMES_EXPORT addIfAbsent;

template <template <typename...> class Typelist, typename... TypesToAdd, typename... Args>
struct LIMES_EXPORT addIfAbsent<Typelist<Args...>, TypesToAdd...> final
{
private:

	template <typename Type>
	static constinit const auto isTypeAbsent = ! contains_v<Typelist<Args...>, Type>;

	template <class ListType, typename... ListArgs>
	struct adder;

	template <class ListType, typename First, typename... Others>
	struct adder<ListType, First, Others...> final
	{
		using type = typename adder<
			std::conditional_t<
				isTypeAbsent<First>,
				add_t<ListType, First>,
				ListType>,
			Others...>::type;
	};

	template <class ListType, typename First>
	struct adder<ListType, First> final
	{
		using type = std::conditional_t<
			isTypeAbsent<First>,
			add_t<ListType, First>,
			ListType>;
	};

public:

	using type = typename adder<Typelist<Args...>, TypesToAdd...>::type;
};

template <class Typelist, typename... TypesToAdd>
LIMES_EXPORT using addIfAbsent_t = typename addIfAbsent<Typelist, TypesToAdd...>::type;

/*----------------------------------------------------------------------------------------------------------------------*/

template <class Typelist, size_t Index>
struct LIMES_EXPORT get;

template <template <typename...> class Typelist, size_t Index, typename... Args>
struct LIMES_EXPORT get<Typelist<Args...>, Index> final
{
private:

	static_assert (Index <= size_v<Typelist<Args...>>, "Search index in type list is out of bounds!");

	template <size_t SearchIndex, typename First, typename... Others>
	struct finder final
	{
		using type = std::conditional_t<
			SearchIndex == 0,
			First,
			typename finder<SearchIndex - 1, Others...>::type>;
	};

	template <size_t SearchIndex, typename First>
	struct finder<SearchIndex, First> final
	{
		using type = std::conditional_t<
			SearchIndex == 0,
			First,
			NoType>;
	};

public:

	using type = typename finder<Index, Args...>::type;
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
	struct finder<Index, First, Others...> final
	{
		static constinit const auto value = std::conditional_t<
			std::is_same_v<First, TypeToFind>,
			std::integral_constant<size_t, Index>,
			finder<Index + 1, Others...>>::value;
	};

	template <size_t Index, typename First>
	struct finder<Index, First> final
	{
		static constinit const auto value = Index;
	};

public:

	static constinit const auto value = finder<0, Args...>::value;
};

template <class Typelist, typename TypeToFind>
LIMES_EXPORT constinit const auto find_v = find<Typelist, TypeToFind>::value;

/*----------------------------------------------------------------------------------------------------------------------*/

template <class Typelist, typename... TypesToRemove>
struct LIMES_EXPORT remove;

template <template <typename...> class Typelist, typename... TypesToRemove, typename... Args>
struct LIMES_EXPORT remove<Typelist<Args...>, TypesToRemove...>
{
private:

	static_assert ((contains_v<Typelist<Args...>, TypesToRemove> && ...),
				   "TypesToRemove contains types that are not in the Typelist");

	template <typename Type>
	static constinit const auto isTypeAbsent = (! std::is_same_v<TypesToRemove, Type> && ...);

	template <class ListType, typename... ListArgs>
	struct remover;

	template <class ListType, typename First, typename... Others>
	struct remover<ListType, First, Others...> final
	{
		using type = typename remover<
			std::conditional_t<
				isTypeAbsent<First>,
				add_t<ListType, First>,
				ListType>,
			Others...>::type;
	};

	template <class ListType, typename First>
	struct remover<ListType, First>
	{
		using type = std::conditional_t<
			isTypeAbsent<First>,
			add_t<ListType, First>,
			ListType>;
	};

public:

	using type = typename remover<InternalTypeList<>, Args...>::type::template makeTypelist<Typelist>;
};

template <class Typelist, typename... TypesToRemove>
LIMES_EXPORT using remove_t = typename remove<Typelist, TypesToRemove...>::type;

/*----------------------------------------------------------------------------------------------------------------------*/

template <class Typelist, size_t Index>
struct LIMES_EXPORT remove_at;

template <template <typename...> class Typelist, size_t Index, typename... Args>
struct LIMES_EXPORT remove_at<Typelist<Args...>, Index> final
{
private:

	static_assert (Index <= size_v<Typelist<Args...>>, "Cannot remove item out of range of TypeList!");

	template <size_t CurrentIndex, class ListType, typename... ListArgs>
	struct remover;

	template <size_t CurrentIndex, class ListType, typename First, typename... Others>
	struct remover<CurrentIndex, ListType, First, Others...> final
	{
		using type = typename remover<
			CurrentIndex + 1,
			std::conditional_t<CurrentIndex == Index, ListType, add_t<ListType, First>>,
			Others...>::type;
	};

	template <size_t CurrentIndex, class ListType, typename First>
	struct remover<CurrentIndex, ListType, First>
	{
		using type = std::conditional_t<CurrentIndex == Index,
										ListType,
										add_t<ListType, First>>;
	};

public:

	using type = typename remover<0, InternalTypeList<>, Args...>::type::template makeTypelist<Typelist>;
};

template <class Typelist, size_t Index>
LIMES_EXPORT using remove_at_t = typename remove_at<Typelist, Index>::type;

/*----------------------------------------------------------------------------------------------------------------------*/

template <class Typelist, typename ToReplace, typename ReplaceWith>
struct LIMES_EXPORT replace;

template <template <typename...> class Typelist, typename ToReplace, typename ReplaceWith, typename... Args>
struct LIMES_EXPORT replace<Typelist<Args...>, ToReplace, ReplaceWith> final
{
private:

	static_assert (contains_v<Typelist<Args...>, ToReplace>, "Cannot replace type not in Typelist!");

	template <class TypeList, typename InPlaceType, typename...>
	struct replacer;

	template <class TypeList, typename InPlaceType, typename First>
	struct replacer<TypeList, InPlaceType, First> final
	{
		using type = add_t<TypeList, std::conditional_t<std::is_same_v<First, ToReplace>, InPlaceType, First>>;
	};

	template <class TypeList, typename InPlaceType, typename First, typename... Others>
	struct replacer<TypeList, InPlaceType, First, Others...> final
	{
		using type = typename replacer<
			add_t<TypeList, std::conditional_t<std::is_same_v<First, ToReplace>, InPlaceType, First>>,
			InPlaceType,
			Others...>::type;
	};

public:

	using type = std::conditional_t<std::is_same_v<ToReplace, ReplaceWith>,
									Typelist<Args...>,
									typename replacer<InternalTypeList<>, ReplaceWith, Args...>::type::template makeTypelist<Typelist>>;
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

	template <size_t CurrentIndex, class ListType, typename... ListArgs>
	struct rebuilder;

	template <size_t CurrentIndex, class ListType, typename First, typename... Others>
	struct rebuilder<CurrentIndex, ListType, First, Others...> final
	{
		using type = typename rebuilder<
			CurrentIndex + 1,
			std::conditional_t<CurrentIndex == Index, add_t<ListType, ReplaceWith>, add_t<ListType, First>>,
			Others...>::type;
	};

	template <size_t CurrentIndex, class ListType, typename First>
	struct rebuilder<CurrentIndex, ListType, First>
	{
		using type = std::conditional_t<CurrentIndex == Index,
										add_t<ListType, ReplaceWith>,
										add_t<ListType, First>>;
	};

public:

	using type = typename rebuilder<0, InternalTypeList<>, Args...>::type::template makeTypelist<Typelist>;
};

template <class Typelist, size_t Index, typename ReplaceWith>
LIMES_EXPORT using replace_at_t = typename replace_at<Typelist, Index, ReplaceWith>::type;

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

		static constinit const auto thisValue = std::conditional_t<
			std::is_same_v<ToCount, get_t<Typelist<Args...>, CurrentIndex>>,
			std::integral_constant<size_t, Count + 1>,
			std::integral_constant<size_t, Count>>::value;

	public:

		static constinit const auto value = std::conditional_t<
			CurrentIndex == size_v<Typelist<Args...>>,
			std::integral_constant<size_t, thisValue>,
			counter<thisValue, CurrentIndex + 1>>::value;
	};

public:

	static constinit const auto value = std::conditional_t<contains_v<Typelist<Args...>, ToCount>,
														   std::integral_constant<size_t, counter<0, 0>::value>,
														   std::integral_constant<size_t, 0>>::value;
};

template <class Typelist, typename ToCount>
LIMES_EXPORT static constinit const auto count_v = count<Typelist, ToCount>::value;

/*----------------------------------------------------------------------------------------------------------------------*/

template <class Typelist>
struct LIMES_EXPORT remove_duplicates;

template <template <typename...> class Typelist, typename... Args>
struct LIMES_EXPORT remove_duplicates<Typelist<Args...>> final
{
private:

	template <size_t CurrentIndex>
	using CurrentType = get_t<Typelist<Args...>, CurrentIndex>;

	template <class ListType, size_t CurrentIndex>
	static constinit const bool containsCurrentType = contains_v<ListType, CurrentType<CurrentIndex>>;

	template <size_t CurrentIndex, class ListType, typename...>
	struct rebuilder;

	template <size_t CurrentIndex, class ListType, typename First, typename... Others>
	struct rebuilder<CurrentIndex, ListType, First, Others...> final
	{
		using type = typename rebuilder<
			CurrentIndex + 1,
			std::conditional_t<containsCurrentType<ListType, CurrentIndex>,
							   ListType,
							   add_t<ListType, CurrentType<CurrentIndex>>>,
			Others...>::type;
	};

	template <size_t CurrentIndex, class ListType, typename First>
	struct rebuilder<CurrentIndex, ListType, First> final
	{
		using type = std::conditional_t<containsCurrentType<ListType, CurrentIndex>,
										ListType,
										add_t<ListType, CurrentType<CurrentIndex>>>;
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
struct LIMES_EXPORT contains_duplicates<Typelist<Args...>> final
{
private:

	using NoDuplicates = remove_duplicates_t<Typelist<Args...>>;

	static constinit const auto origSize = size_v<Typelist<Args...>>;

	static constinit const auto prunedSize = size_v<NoDuplicates>;

public:

	static constinit const auto value = origSize != prunedSize;
};

template <class Typelist>
LIMES_EXPORT static constinit const auto contains_duplicates_v = contains_duplicates<Typelist>::value;

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
