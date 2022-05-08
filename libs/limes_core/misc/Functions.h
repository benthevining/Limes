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

template <class Function>
LIMES_EXPORT bool call_once (Function&& func, std::invoke_result_t<Function>* result = nullptr)
{
	static std::atomic<bool> called = false;

	if (called)
		return false;

	called = true;

	if constexpr (std::is_void_v<std::invoke_result_t<Function>>)
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


template <class Function>
class LIMES_EXPORT CallDeferred final
{
public:

	constexpr explicit CallDeferred (Function&& function)
		: func (std::move (function))
	{
	}

	~CallDeferred()
	{
		std::invoke (func);
	}

	LIMES_NON_COPYABLE (CallDeferred);
	LIMES_NON_MOVABLE (CallDeferred);

private:

	const Function func;
};


template <class Function1, class Function2>
class LIMES_EXPORT RAIICaller final
{
public:

	constexpr explicit RAIICaller (Function1&& onConstruct, Function2&& onDestroy)
		: deferredFunc (std::move (onDestroy))
	{
		std::invoke (onConstruct);
	}

	~RAIICaller()
	{
		std::invoke (deferredFunc);
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
	else {
		return [f, ps...] (auto... qs)
		{ return curry (f, ps..., qs...); };
	}
}


template <typename... Param>
LIMES_EXPORT consteval decltype (auto) consteval_invoke (Param&&... param)
{
	return std::invoke (std::forward<Param> (param)...);
}


template <typename Function, typename... Args>
LIMES_EXPORT inline bool try_call (Function&& func, Args&&... args) noexcept
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

LIMES_END_NAMESPACE
