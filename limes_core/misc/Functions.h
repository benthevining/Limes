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

namespace lemons
{

template <class Function>
bool call_once (Function&& func, std::invoke_result_t<Function>* result = nullptr)
{
	static bool called = false;

	if (called)
		return false;

	called = true;

	if constexpr (std::is_void_v<std::invoke_result_t<Function>>)
		func();
	else
	{
		if (result == nullptr)
			func();
		else
			*result = func();
	}

	return true;
}

}  // namespace lemons
