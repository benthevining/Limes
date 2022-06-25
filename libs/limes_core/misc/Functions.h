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

#include <atomic>
#include <type_traits>
#include <limes_export.h>
#include <limes_namespace.h>
#include <functional>
#include <exception>
#include "./preprocessor.h"

/** @defgroup func Functional
	Utilities for working with higher-level functions and functions as objects.
	@ingroup limes_core
 */

/** @file
	This file defines utilities for working with higher-level functions and functions as objects.
	@ingroup func
 */

LIMES_BEGIN_NAMESPACE

/** This namespace contains some utilities for working with higher-level functions and functions as objects. */
namespace func
{

/** @concept Function
	This concept simply requires that an object is invocable.
	@ingroup func
 */
// clang-format off
template <typename T>
concept Function = requires (T f)
{
	{ std::invoke (f) };
};
// clang-format on

/** This typedef evaluates to the result type returned by calling the given function with the given arguments.
	@ingroup func
 */
template <Function Func, typename... Args>
LIMES_EXPORT using result_type = std::invoke_result_t<std::decay_t<Func>, std::decay_t<Args>...>;

/** Calls the function object exactly once throughout the entire life of the program.
	Note that the call counter is specific to this process.
	@param func Function to call
	@param result If the result type of \c func is not \c void and \c result is not \c nullptr , then \c result will be populated with \c func 's return value.
	@ingroup func
 */
template <Function Func>
LIMES_EXPORT bool call_once (Func&& func, std::invoke_result_t<Func>* result = nullptr) noexcept (noexcept (func))
{
	static std::atomic<bool> called = false;

	if (called.load())
		return false;

	called.store (true);

	if constexpr (std::is_void_v<std::invoke_result_t<Func>>)
	{
		std::invoke (func);
	}
	else
	{
		if (result == nullptr)
			std::invoke (func);
		else
			*result = std::invoke (func);
	}

	return true;
}


/** This class calls a given function when its destructor is executed.
	@ingroup func
 */
template <Function Func>
class LIMES_EXPORT CallDeferred final
{
public:

	/** The constructor simply stores the function object, it does not execute it. */
	constexpr explicit CallDeferred (Func&& function) noexcept
		: func (std::move (function))
	{
	}  // cppcheck-suppress missingReturn

	/** The destructor executes the stored function. */
	~CallDeferred()
	{
		try
		{
			std::invoke (func);
		}
		catch (std::exception&)
		{
		}
	}

	LIMES_NON_COPYABLE (CallDeferred)
	LIMES_NON_MOVABLE (CallDeferred)

private:

	const Func func;
};


/** This generic RAII object allows you to specify a function to be called when it is created and when it is destroyed.
	@ingroup func
 */
template <Function Function1, Function Function2>
class LIMES_EXPORT RAIICaller final
{
public:

	constexpr explicit RAIICaller (Function1&& onConstruct, Function2&& onDestroy) noexcept (noexcept (onConstruct))
		: deferredFunc (std::move (onDestroy))
	{
		std::invoke (onConstruct);
	}

	~RAIICaller()
	{
		try
		{
			std::invoke (deferredFunc);
		}
		catch (std::exception&)
		{
		}
	}

	LIMES_NON_COPYABLE (RAIICaller)
	LIMES_NON_MOVABLE (RAIICaller)

private:

	const Function2 deferredFunc;
};


/** Breaks down a function that takes multiple arguments into a series of functions that each take one argument.
	Additionally, if the original function is callable with one argument, then this function returns the result of calling that function.
	@ingroup func
 */
constexpr decltype (auto) curry (auto f, auto... ps)
{
	if constexpr (requires { std::invoke (f, ps...); })
	{
		return std::invoke (f, ps...);
	}
	else
	{
		return [f, ps...] (auto... qs)
		{ return curry (f, ps..., qs...); };
	}
}


/** Forces a given function to be executed at compile time.
	@ingroup func
 */
template <typename... Param>
LIMES_EXPORT consteval decltype (auto) consteval_invoke (Param&&... param) noexcept
{
	try
	{
		return std::invoke (std::forward<Param> (param)...);
	}
	catch (std::exception&)
	{
	}
}


/** Executes a potentially-throwing function within a try-catch block, and returns false if an exception is thrown and true if no exceptions are thrown.
	This is basically a simple Lippincott function.
	If the return type of the function is bool, then the value it returns will be returned by \c %try_call if the function does not throw an exception.
	@ingroup func
 */
template <Function Func, typename... Args>
LIMES_EXPORT constexpr bool try_call (Func&& func, Args&&... args) noexcept
{
	if constexpr (noexcept (func))
	{
		func (std::forward<Args> (args)...);
		return true;
	}
	else
	{
		try
		{
			if constexpr (std::is_same_v<bool, result_type<Func, Args...>>)
				return func (std::forward<Args> (args)...);
			else
			{
				func (std::forward<Args> (args)...);
				return true;
			}
		}
		catch (const std::exception&)
		{
			return false;
		}
	}
}

}  // namespace func

LIMES_END_NAMESPACE
