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

#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <limes_export.h>
#include <limes_namespace.h>

/** @defgroup rt_safety Realtime safety
	Metaprogramming utilities for detecting the realtime safety of certain operations on certain types.
	@ingroup threads
 */

/** @file
	This file contains metaprogramming utilities for detecting the realtime safety of certain operations on certain types.
	@ingroup rt_safety
 */

LIMES_BEGIN_NAMESPACE

namespace threads
{

/** This namespace contains implementation details for the realtime safety metaprogramming utilities.
	@ingroup rt_safety
 */
namespace detail
{

#pragma mark Detail

/** @ingroup rt_safety
	@{
 */

/** A type tag representing a copy operation. */
struct LIMES_EXPORT copy_tag;

/** A type tag representing a move operation. */
struct LIMES_EXPORT move_tag;

/** A type tag representing a type construction operation. */
struct LIMES_EXPORT constructible_tag;

/** A type tag representing an assignment operation. */
struct LIMES_EXPORT assignable_tag;

/** Represents a query for realtime safety of the operation represented by \c Tag1 and \c Tag2 on the type \c T .
	This class template can be specialized to specify realtime safety of certain operations on containers and individual types.
 */
template <typename T, typename Tag1, typename Tag2>
struct LIMES_EXPORT is_rt_safe final : std::false_type
{
};

/** Specialization of the \c %is_rt_safe class template for copy assignment operations. */
template <typename T>
struct LIMES_EXPORT is_rt_safe<T, copy_tag, assignable_tag> final : std::bool_constant<std::is_trivially_copy_assignable<T>::value>
{
};

/** Specialization of the \c %is_rt_safe class template for copy construction operations. */
template <typename T>
struct LIMES_EXPORT is_rt_safe<T, copy_tag, constructible_tag> final : std::bool_constant<std::is_trivially_copy_constructible<T>::value>
{
};

/** Specialization of the \c %is_rt_safe class template for move assignment operations. */
template <typename T>
struct LIMES_EXPORT is_rt_safe<T, move_tag, assignable_tag> final : std::bool_constant<std::is_trivially_move_assignable<T>::value>
{
};

/** Specialization of the \c %is_rt_safe class template for move construction operations. */
template <typename T>
struct LIMES_EXPORT is_rt_safe<T, move_tag, constructible_tag> final : std::bool_constant<std::is_trivially_move_constructible<T>::value>
{
};

/** Specialization of the \c %is_rt_safe class template for move operations within a \c std::vector . */
template <typename T, typename Tag>
struct LIMES_EXPORT is_rt_safe<std::vector<T>, move_tag, Tag> final : is_rt_safe<T, move_tag, Tag>
{
};

/** Specialization of the \c %is_rt_safe class template for move operations within a \c std::set . */
template <typename T, typename Tag>
struct LIMES_EXPORT is_rt_safe<std::set<T>, move_tag, Tag> final : is_rt_safe<T, move_tag, Tag>
{
};

/** Specialization of the \c %is_rt_safe class template for move operations within a \c std::map . */
template <typename T, typename U, typename Tag>
struct LIMES_EXPORT is_rt_safe<std::map<T, U>, move_tag, Tag> final : std::bool_constant<is_rt_safe<T, move_tag, Tag>::value && is_rt_safe<U, move_tag, Tag>::value>
{
};

/** Specialization of the \c %is_rt_safe class template for move operations within a \c std::unordered_map . */
template <typename T, typename U, typename Tag>
struct LIMES_EXPORT is_rt_safe<std::unordered_map<T, U>, move_tag, Tag> final : std::bool_constant<is_rt_safe<T, move_tag, Tag>::value && is_rt_safe<U, move_tag, Tag>::value>
{
};

/** Specialization of the \c %is_rt_safe class template for move operations within a \c std::unordered_set . */
template <typename T, typename Tag>
struct LIMES_EXPORT is_rt_safe<std::unordered_set<T>, move_tag, Tag> final : is_rt_safe<T, move_tag, Tag>
{
};

/** @} */

}  // namespace detail

/** @ingroup rt_safety
	@{
 */

#pragma mark Public API

/** A compile-time boolean constant that evaluates to true if the type \c T is realtime-safe copy assignable. */
template <typename T>
struct LIMES_EXPORT is_realtime_copy_assignable final : detail::is_rt_safe<T, detail::copy_tag, detail::assignable_tag>
{
};

/** A compile-time boolean constant that evaluates to true if the type \c T is realtime-safe copy constructable. */
template <typename T>
struct LIMES_EXPORT is_realtime_copy_constructable final : detail::is_rt_safe<T, detail::copy_tag, detail::constructible_tag>
{
};

/** A compile-time boolean constant that evaluates to true if the type \c T is realtime-safe copy constructable and copy assignable. */
template <typename T>
using is_realtime_copy_safe = std::bool_constant<is_realtime_copy_assignable<T>::value && is_realtime_copy_constructable<T>::value>;

/** @concept RealtimeCopySafe
	This concept requires that a type is realtime-safe copy constructable and copy assignable.
 */
template <typename T>
concept RealtimeCopySafe = is_realtime_copy_safe<T>::value;

/** A compile-time boolean constant that evaluates to true if the type \c T is realtime-safe move assignable. */
template <typename T>
struct LIMES_EXPORT is_realtime_move_assignable final : detail::is_rt_safe<T, detail::move_tag, detail::assignable_tag>
{
};

/** A compile-time boolean constant that evaluates to true if the type \c T is realtime-safe move constructable. */
template <typename T>
struct LIMES_EXPORT is_realtime_move_constructable final : detail::is_rt_safe<T, detail::move_tag, detail::constructible_tag>
{
};

/** A compile-time boolean constant that evaluates to true if the type \c T is realtime-safe move constructable and move assignable. */
template <typename T>
using is_realtime_move_safe = std::bool_constant<is_realtime_move_assignable<T>::value && is_realtime_move_constructable<T>::value>;

/** This concept requires that a type is realtime-safe move constructable and move assignable. */
template <typename T>
concept RealtimeMoveSafe = is_realtime_move_safe<T>::value;

/** This concept requires that a type is realtime-safe copy constructable, copy assignable, move constructable and move assignable. */
template <typename T>
concept RealtimeSafe = RealtimeCopySafe<T> && RealtimeMoveSafe<T>;

/** @} */

}  // namespace threads

LIMES_END_NAMESPACE
