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
#include "TypeList_impl.h"
#include "../misc/Functions.h"
#include "TypeTraits.h"
#include <type_traits>
#include <utility>
#include <memory>
#include <vector>
#include <string>

LIMES_BEGIN_NAMESPACE

namespace typelist
{

/*
 - compare typelists - get another typelist with items in common / not
 - equality comparison ignoring order
 */

template <typename... Types>
class LIMES_EXPORT TypeList final
{
public:

	TypeList() = delete;

	using size_type = size_t;

	using TypeID = TypeList<Types...>;

	using clear = TypeList<>;

	static constexpr const size_type size = size_v<TypeID>;

	static constexpr const bool empty = is_empty<TypeID>;

	template <class Other>
	static constexpr const bool equal = are_same_v<TypeID, Other>;

	template <typename... TypesToFind>
	static constexpr const bool contains = contains_v<TypeID, TypesToFind...>;

	template <typename... TypesToFind>
	static constexpr const bool contains_or = contains_or_v<TypeID, TypesToFind...>;

	template <typename Type>
	static constexpr const size_type num_of = count_v<TypeID, Type>;

	template <template <typename> class UnaryPredicate>
	static constexpr const size_type count_if = count_if_v<TypeID, UnaryPredicate>;

	template <template <typename> class UnaryPredicate>
	static constexpr const size_type count_if_not = count_if_not_v<TypeID, UnaryPredicate>;

	template <typename... TypesToAdd>
	using add = add_t<TypeID, TypesToAdd...>;

	template <typename... TypesToAdd>
	using addIfAbsent = addIfAbsent_t<TypeID, TypesToAdd...>;

	template <size_type Index, typename ToInsert>
	using insert_at = insert_at_t<TypeID, Index, ToInsert>;

	template <typename ToPrepend>
	using prepend = prepend_t<TypeID, ToPrepend>;

	template <typename ToAppend>
	using append = append_t<TypeID, ToAppend>;

	template <size_type Index1, size_type Index2>
	using swap_at = swap_at_t<TypeID, Index1, Index2>;

	template <typename Type1, typename Type2>
	using swap = swap_t<TypeID, Type1, Type2>;

	template <size_type Index>
	using at = get_t<TypeID, Index>;

	using front = get_first_t<TypeID>;
	using back	= get_last_t<TypeID>;

	using reverse = reverse_t<TypeID>;

	template <size_type Index, typename... Args>
	static constexpr at<Index> construct (Args&&... args)
	{
		return at<Index> (std::forward<Args> (args)...);
	}

	template <size_type Index, typename... Args>
	static std::unique_ptr<at<Index>> make_unique (Args&&... args)
	{
		return std::make_unique<at<Index>> (std::forward<Args> (args)...);
	}

	template <typename Type>
	static constexpr const size_type index_of = find_v<TypeID, Type>;

	template <typename... TypesToRemove>
	using remove = remove_t<TypeID, TypesToRemove...>;

	template <size_type Index>
	using remove_at = remove_at_t<TypeID, Index>;

	using remove_first = remove_at<0>;
	using remove_last  = remove_at<size - 1>;

	using remove_null_types = remove_null_types_t<TypeID>;

	template <template <typename> class UnaryPredicate>
	using remove_if = remove_if_t<TypeID, UnaryPredicate>;

	template <template <typename> class UnaryPredicate>
	using remove_if_not = remove_if_not_t<TypeID, UnaryPredicate>;

	template <typename Replace, typename With>
	using replace = replace_t<TypeID, Replace, With>;

	template <size_type Index, typename ReplaceWith>
	using replace_at = replace_at_t<TypeID, Index, ReplaceWith>;

	static constexpr const bool contains_duplicates = contains_duplicates_v<TypeID>;

	using remove_duplicates = remove_duplicates_t<TypeID>;

	template <template <typename...> class T>
	using apply_to = T<Types...>;

	template <Function Func, typename... Args>
	static constexpr void for_each (Func&& f, Args&&... args) noexcept (noexcept ((std::forward<Func> (f).template operator()<Types> (std::forward<Args> (args)...), ...)))
	{
		(std::forward<Func> (f).template operator()<Types> (std::forward<Args> (args)...), ...);
	}

	template <Function Func, typename... Args>
	static constexpr auto for_all (Func&& f, Args&&... args) noexcept (noexcept (f().template operator()<Types...> (std::forward<Args> (args)...)))
		-> std::invoke_result_t<Func, Args...>
	{
		return f().template operator()<Types...> (std::forward<Args> (args)...);
	}
};

/*----------------------------------------------------------------------------------------------------------------------*/

template <>
class LIMES_EXPORT TypeList<> final
{
public:

	TypeList() = delete;

	using size_type = size_t;

	using TypeID = TypeList<>;
	using clear	 = TypeID;

	static constinit const size_type size = 0;

	static constinit const bool empty = true;

	template <class Other>
	static constexpr const bool equal = is_empty<Other>;

	template <typename...>
	static constinit const bool contains = false;

	template <typename...>
	static constinit const bool contains_or = false;

	template <typename>
	static constinit const size_type num_of = 0;

	template <template <typename> class>
	static constinit const size_type count_if = 0;

	template <template <typename> class UnaryPredicate>
	static constinit const size_type count_if_not = 0;

	template <typename... TypesToAdd>
	using add = TypeList<TypesToAdd...>;

	template <typename... TypesToAdd>
	using addIfAbsent = addIfAbsent_t<TypeID, TypesToAdd...>;

	template <size_type, typename ToInsert>
	using insert_at = TypeList<ToInsert>;

	template <typename ToPrepend>
	using prepend = TypeList<ToPrepend>;

	template <typename ToAppend>
	using append = TypeList<ToAppend>;

	template <size_type, size_type>
	using swap_at = TypeID;

	template <typename, typename>
	using swap = TypeID;

	template <size_type>
	using at = NullType;

	using front = NullType;
	using back	= NullType;

	using reverse = TypeID;

	template <size_type, typename... Args>
	static constexpr NullType construct (Args&&...)
	{
		return {};
	}

	template <size_type, typename... Args>
	static std::unique_ptr<NullType> make_unique (Args&&...)
	{
		return std::make_unique<NullType>();
	}

	template <typename>
	static constinit const size_type index_of = invalid_index;

	template <typename...>
	using remove = TypeID;

	template <size_type>
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

	template <size_type, typename>
	using replace_at = TypeID;

	static constinit const bool contains_duplicates = false;

	using remove_duplicates = TypeID;

	template <template <typename...> class T>
	using apply_to = T<>;

	template <Function Func, typename... Args>
	static constexpr void for_each (Func&&, Args&&...) noexcept
	{
	}

	template <Function Func, typename... Args>
	static constexpr auto for_all (Func&& f, Args&&... args) noexcept (noexcept (f().template operator()<> (std::forward<Args> (args)...)))
		-> std::invoke_result_t<Func, Args...>
	{
		return f().template operator()<> (std::forward<Args> (args)...);
	}
};

LIMES_EXPORT using Empty = TypeList<>;

/*----------------------------------------------------------------------------------------------------------------------*/

template <typename>
struct LIMES_EXPORT make_type_list_from;

template <template <typename...> class T, typename... Args>
struct LIMES_EXPORT make_type_list_from<T<Args...>> final
{
	using type = TypeList<Args...>;
};

template <typename... Args>
LIMES_EXPORT using make_type_list_from_t = typename make_type_list_from<Args...>::type;

/*----------------------------------------------------------------------------------------------------------------------*/

template <class T>
LIMES_EXPORT static constinit const auto is_typelist = is_specialization<T, TypeList>::value;

template <typename T>
concept type_list = is_typelist<T>;

}  // namespace typelist

LIMES_END_NAMESPACE
