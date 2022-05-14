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
#include "preprocessor.h"

LIMES_BEGIN_NAMESPACE

/** This namespace contains some utilities for working with higher-level functions and functions as objects. */
namespace func
{

// clang-format off
template <typename T>
concept Function = requires (T f)
{
	{ f() };
};
// clang-format on

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


template <Function Func>
class LIMES_EXPORT CallDeferred final
{
public:

	constexpr explicit CallDeferred (Func&& function) noexcept
		: func (std::move (function))
	{
	}

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

	LIMES_NON_COPYABLE (CallDeferred);
	LIMES_NON_MOVABLE (CallDeferred);

private:

	const Func func;
};


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

	LIMES_NON_COPYABLE (RAIICaller);
	LIMES_NON_MOVABLE (RAIICaller);

private:

	const Function2 deferredFunc;
};


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
			func (std::forward<Args> (args)...);
			return true;
		}
		catch (const std::exception&)
		{
			return false;
		}
	}
}

}  // namespace func

LIMES_END_NAMESPACE
