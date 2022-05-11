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

LIMES_BEGIN_NAMESPACE

namespace typelist
{

/*
 - compare typelists - get another typelist with items in common / not
 - reverse
 - sort?
 */

template <typename... Types>
class LIMES_EXPORT TypeList final
{
public:

	using TypeID = TypeList<Types...>;

	using clear = TypeList<>;

	static constinit const auto size = size_v<TypeList>;

	static constinit const auto empty = size == 0;

	template <class Other>
	static constinit const auto equal = are_same_v<TypeList, Other>;

	template <typename Type>
	static constinit const auto contains = contains_v<TypeList, Type>;

	template <typename Type>
	static constinit const auto num_of = count_v<TypeList, Type>;

	template <typename... TypesToAdd>
	using add = add_t<TypeList, TypesToAdd...>;

	template <typename... TypesToAdd>
	using addIfAbsent = addIfAbsent_t<TypeList, TypesToAdd...>;

	template <size_t Index>
	using at = get_t<TypeList, Index>;

	using front = get_first_t<TypeList>;

	using back = get_last_t<TypeList, Types...>;

	//	using reverse = reverse_t<TypeList>;

	template <size_t Index, typename... Args>
	static constexpr at<Index> construct (Args&&... args)
	{
		return at<Index> (std::forward<Args> (args)...);
	}

	template <size_t Index, typename... Args>
	static std::unique_ptr<at<Index>> make_unique (Args&&... args)
	{
		return std::make_unique<at<Index>> (std::forward<Args> (args)...);
	}

	template <typename Type>
	static constinit const auto index_of = find_v<TypeList, Type>;

	template <typename... TypesToRemove>
	using remove = remove_t<TypeList, TypesToRemove...>;

	template <size_t Index>
	using remove_at = remove_at_t<TypeList, Index>;

	using remove_first = remove_at<0>;
	using remove_last  = remove_at<size - 1>;

	template <typename Replace, typename With>
	using replace = replace_t<TypeList, Replace, With>;

	template <size_t Index, typename ReplaceWith>
	using replace_at = replace_at_t<TypeList, Index, ReplaceWith>;

	static constinit const auto contains_duplicates = contains_duplicates_v<TypeList>;

	using remove_duplicates = remove_duplicates_t<TypeList>;

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


template <class T>
static constinit const auto is_typelist = is_specialization<T, TypeList>::value;

template <typename T>
concept type_list = is_typelist<T>;

}  // namespace typelist

LIMES_END_NAMESPACE
