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

#include <type_traits>
#include <typeinfo>
#include <string>
#include <limes_export.h>
#include <limes_platform.h>
#include <limes_namespace.h>
#include <exception>

#if ! LIMES_WINDOWS
#	include <cxxabi.h>
#endif


/** @defgroup lemons_core lemons_core
	@ingroup Common
	Core building blocks.
 */


LIMES_BEGIN_NAMESPACE

template <typename From, typename To>
concept convertible_to = std::is_convertible_v<From, To>;

template <typename A, typename B>
concept same_as = std::is_same_v<A, B>;

template <class ClassToTest, typename RequiredBase>
concept inherits_from = std::is_base_of_v<RequiredBase, ClassToTest>;

/** @ingroup lemons_core
	Utility struct that evaluates to std::true_type if the given class specializes the given template, and false otherwise.
	For example:
	@code
	is_specialization< std::vector<int>, std::vector >()
	@endcode
	evaluates to a compile-time true value, and
	@code
	is_specialization< std::vector<int>, std::list >()
	@endcode
	evaluates to a compile-time false value.
 */
template <class T, template <class...> class Template>
struct LIMES_EXPORT is_specialization final : std::false_type
{
};

/** @ingroup lemons_core
 Utility struct that evaluates to std::true_type if the given class specializes the given template, and false otherwise.
 For example:
 @code
 is_specialization< std::vector<int>, std::vector >()
 @endcode
 evaluates to a compile-time true value, and
 @code
 is_specialization< std::vector<int>, std::list >()
 @endcode
 evaluates to a compile-time false value.
 */
template <template <class...> class Template, class... Args>
struct LIMES_EXPORT is_specialization<Template<Args...>, Template> final : std::true_type
{
};

template <class ClassToTest, template <class...> class Template>
concept specializes = is_specialization<ClassToTest, Template>::value;


template <class Derived, class Base>
struct LIMES_EXPORT covariance_check final : std::is_base_of<Base, Derived>
{
};

template <template <class...> class T, class... Ds, class... Bs>
struct LIMES_EXPORT covariance_check<T<Ds...>, T<Bs...>> final : std::conjunction<covariance_check<Ds, Bs>...>
{
};

template <class Derived, class Base>
concept covariant_subtype_of = covariance_check<Derived, Base>::value;


template <typename Test, typename... Types>
struct LIMES_EXPORT is_one_of final : std::disjunction<std::is_same<Test, Types>...>
{
};

template <typename Test, typename... Types>
struct LIMES_EXPORT is_none_of final : std::negation<is_one_of<Test, Types...>>
{
};


LIMES_EXPORT [[nodiscard]] std::string getDemangledTypeName (const auto& object) noexcept
{
	try
	{
#if LIMES_WINDOWS
		const auto res = std::string (typeid (object).name());

		if (res.startsWith ("class "))
			return res.substring (6);

		if (res.startsWith ("struct "))
			return res.substring (7);

		return res;
#else
		int status = 0;

		if (auto* demangled = abi::__cxa_demangle (typeid (object).name(), nullptr, nullptr, &status))
		{
			const auto res = std::string (demangled);
			free (demangled);

			return res;
		}

		return {};
#endif
	}
	catch (std::exception&)
	{
		return {};
	}
}

LIMES_EXPORT [[nodiscard]] std::string getDemangledTypeName (const auto* c) noexcept
{
	if (c != nullptr)
		return getDemangledName (*c) + " pointer";

	return "nullptr";
}

LIMES_END_NAMESPACE
