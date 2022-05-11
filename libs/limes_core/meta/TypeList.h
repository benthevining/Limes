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
#include <utility>
#include <memory>
#include <vector>
#include <string>

LIMES_BEGIN_NAMESPACE

namespace typelist
{

/*
 - compare typelists - get another typelist with items in common / not
 - sort?
 */

template <typename... Types>
class LIMES_EXPORT TypeList final
{
public:

	TypeList() = delete;

	using size_type = size_t;

	using TypeID = TypeList<Types...>;

	using clear = TypeList<>;

	static constinit const size_type size = size_v<TypeList>;

	static constinit const bool empty = is_empty<TypeList>;

	template <class Other>
	static constinit const bool equal = are_same_v<TypeList, Other>;

	template <typename Type>
	static constinit const bool contains = contains_v<TypeList, Type>;

	template <typename Type>
	static constinit const size_type num_of = count_v<TypeList, Type>;

	template <typename... TypesToAdd>
	using add = add_t<TypeList, TypesToAdd...>;

	template <typename... TypesToAdd>
	using addIfAbsent = addIfAbsent_t<TypeList, TypesToAdd...>;

	template <size_t Index, typename ToInsert>
	using insert_at = insert_at_t<TypeList, Index, ToInsert>;

	template <typename ToPrepend>
	using prepend = prepend_t<TypeList, ToPrepend>;

	template <typename ToAppend>
	using append = append_t<TypeList, ToAppend>;

	template <size_t Index1, size_t Index2>
	using swap_at = swap_at_t<TypeList, Index1, Index2>;

	template <typename Type1, typename Type2>
	using swap = swap_t<TypeList, Type1, Type2>;

	template <size_type Index>
	using at = get_t<TypeList, Index>;

	using front = get_first_t<TypeList>;
	using back	= get_last_t<TypeList>;

	using reverse = reverse_t<TypeList>;

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
	static constinit const size_type index_of = find_v<TypeList, Type>;

	template <typename... TypesToRemove>
	using remove = remove_t<TypeList, TypesToRemove...>;

	template <size_type Index>
	using remove_at = remove_at_t<TypeList, Index>;

	using remove_first = remove_at<0>;
	using remove_last  = remove_at<size - 1>;

	template <typename Replace, typename With>
	using replace = replace_t<TypeList, Replace, With>;

	template <size_type Index, typename ReplaceWith>
	using replace_at = replace_at_t<TypeList, Index, ReplaceWith>;

	static constinit const bool contains_duplicates = contains_duplicates_v<TypeList>;

	using remove_duplicates = remove_duplicates_t<TypeList>;

	template <template <typename...> class T>
	using apply_to = T<Types...>;

	template <Function Func, typename... Args>
	static constexpr void for_each (Func&& f, Args&&... args) noexcept (noexcept ((std::forward<Func> (f).template operator()<Types> (std::forward<Args> (args)...), ...)))
	{
		(std::forward<Func> (f).template operator()<Types> (std::forward<Args> (args)...), ...);
	}

	template <Function Func, typename... Args>
	static constexpr void for_all (Func&& f, Args&&... args) noexcept (noexcept (f().template operator()<Types...> (std::forward<Args> (args)...)))
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
	static constinit const bool equal = is_empty<Other>;

	template <typename>
	static constinit const bool contains = false;

	template <typename>
	static constinit const size_type num_of = 0;

	template <typename... TypesToAdd>
	using add = TypeList<TypesToAdd...>;

	template <typename... TypesToAdd>
	using addIfAbsent = addIfAbsent_t<TypeList, TypesToAdd...>;

	template <size_t Index, typename ToInsert>
	using insert_at = insert_at_t<TypeList, Index, ToInsert>;

	template <typename ToPrepend>
	using prepend = add_t<TypeList, ToPrepend>;

	template <typename ToAppend>
	using append = append_t<TypeList, ToAppend>;

	template <size_t, size_t>
	using swap_at = TypeID;

	template <typename, typename>
	using swap = TypeID;

	template <size_type>
	using at = NoType;

	using front = NoType;
	using back	= NoType;

	using reverse = TypeID;

	template <size_type, typename... Args>
	static constexpr NoType construct (Args&&...)
	{
		return {};
	}

	template <size_type, typename... Args>
	static std::unique_ptr<NoType> make_unique (Args&&...)
	{
		return std::make_unique<NoType>();
	}

	template <typename>
	static constinit const size_type index_of = invalid_index;

	template <typename...>
	using remove = TypeID;

	template <size_type>
	using remove_at = TypeID;

	using remove_first = TypeID;
	using remove_last  = TypeID;

	template <typename, typename>
	using replace = TypeID;

	template <size_type, typename>
	using replace_at = TypeID;

	static constinit const bool contains_duplicates = false;

	using remove_duplicates = TypeID;

	template <template <typename...> class T>
	using apply_to = T<NoType>;

	template <Function Func, typename... Args>
	static constexpr void for_each (Func&&, Args&&...) noexcept
	{
	}

	template <Function Func, typename... Args>
	static constexpr void for_all (Func&& f, Args&&... args) noexcept (noexcept (f().template operator()<> (std::forward<Args> (args)...)))
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
