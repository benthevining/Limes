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
		func();
	}
	else
	{
		if (result == nullptr)
			func();
		else
			*result = func();
	}

	return true;
}


template <class Function>
class LIMES_EXPORT CallDeferred final
{
public:

	explicit CallDeferred (Function&& function)
		: func (std::move (function))
	{
	}

	~CallDeferred()
	{
		func();
	}

private:

	const Function func;
};


template <class Function1, class Function2>
class LIMES_EXPORT RAIICaller final
{
public:

	explicit RAIICaller (Function1&& onConstruct, Function2&& onDestroy)
		: deferredFunc (std::move (onDestroy))
	{
		onConstruct();
	}

	~RAIICaller()
	{
		deferredFunc();
	}

private:

	const Function2 deferredFunc;
};

LIMES_END_NAMESPACE
