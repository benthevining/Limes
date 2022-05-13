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


LIMES_BEGIN_NAMESPACE

/** Exactly the same as std::convertible_to, only implemented here because some versions of Xcode seem to be missing it. */
template <class From, class To>
concept convertible_to =
	std::is_convertible_v<From, To> && requires
{
	static_cast<To> (std::declval<From>());
};

template <typename A, typename B>
concept same_as = std::is_same_v<A, B>;

template <class ClassToTest, typename RequiredBase>
concept inherits_from = std::is_base_of_v<RequiredBase, ClassToTest>;

#ifndef DOXYGEN

template <class T, template <class...> class Template>
struct LIMES_EXPORT is_specialization final : std::false_type
{
};

template <template <class...> class Template, class... Args>
struct LIMES_EXPORT is_specialization<Template<Args...>, Template> final : std::true_type
{
};

#endif

/** Evaluates to true if the given type is a specialization of the given template class; false otherwise.
	@tparam ClassToTest Fully-specialized type to test.
	@tparam Template Template class to test if ClassToTest specializes.
 */
template <class ClassToTest, template <class...> class Template>
static constexpr const bool is_specialization_v = is_specialization<ClassToTest, Template>::value;

template <class ClassToTest, template <class...> class Template>
concept specializes = requires
{
	is_specialization_v<ClassToTest, Template>;
};

#ifndef DOXYGEN

template <class Derived, class Base>
struct LIMES_EXPORT covariance_check final : std::is_base_of<Base, Derived>
{
};

template <template <class...> class T, class... Ds, class... Bs>
struct LIMES_EXPORT covariance_check<T<Ds...>, T<Bs...>> final : std::conjunction<covariance_check<Ds, Bs>...>
{
};

#endif

/** Evaluates to true if Base and Derived are covariant types; false otherwise.
	@tparam Derived Derived type that may be covariant with Base.
	@tparam Base Type that Derived may be covariant with.
 */
template <class Derived, class Base>
static constexpr const bool is_covariant_v = covariance_check<Derived, Base>::value;

template <class Derived, class Base>
concept covariant_subtype_of = requires
{
	is_covariant_v<Derived, Base>;
};

#ifndef DOXYGEN

template <typename Test, typename... Types>
struct LIMES_EXPORT is_one_of final : std::disjunction<std::is_same<Test, Types>...>
{
};

#endif

/** Evaluates to true if Test is the same as any of the types in Types.
	@tparam Test Type to search for in the list of types.
	@tparam Types List of types to check.
 */
template <typename Test, typename... Types>
static constexpr const bool is_one_of_v = is_one_of<Test, Types...>::value;

#ifndef DOXYGEN

template <typename Test, typename... Types>
struct LIMES_EXPORT is_none_of final : std::negation<is_one_of<Test, Types...>>
{
};

#endif

/** Evalutes to true if Test is not in the list of Types.
	@tparam Test Type to search for in the list of types.
	@tparam Types List of types to check.
 */
template <typename Test, typename... Types>
static constexpr const bool is_none_of_v = is_none_of<Test, Types...>::value;

/** Returns the demangled name of the type of the passed object. */
LIMES_EXPORT [[nodiscard]] std::string getDemangledTypeName (const auto& object) noexcept
{
	try
	{
#if LIMES_WINDOWS
		const auto res = std::string (typeid (object).name());

		if (res.starts_with ("class "))
			return res.substr (6, res.length());

		if (res.starts_with ("struct "))
			return res.substr (7, res.length());

		return res;
#else
		int status = 0;

		if (auto* demangled = abi::__cxa_demangle (typeid (object).name(), nullptr, nullptr, &status))
		{
			const auto res = std::string (demangled);
			std::free (demangled);

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

/** Returns the demangled name of the type of the passed object, ending with " pointer". */
LIMES_EXPORT [[nodiscard]] std::string getDemangledTypeName (const auto* c) noexcept
{
	if (c == nullptr)
		return "nullptr";

	return getDemangledName (*c) + " pointer";
}

LIMES_END_NAMESPACE
