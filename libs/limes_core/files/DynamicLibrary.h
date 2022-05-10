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

#include <limes_export.h>
#include <limes_namespace.h>
#include <string_view>
#include "../misc/preprocessor.h"

LIMES_BEGIN_NAMESPACE

class LIMES_EXPORT DynamicLibrary final
{
public:

	DynamicLibrary() = default;

	explicit DynamicLibrary (const std::string_view& nameOrPath) noexcept;

	~DynamicLibrary();

	LIMES_DEFAULT_COPYABLE (DynamicLibrary);
	LIMES_DEFAULT_MOVABLE (DynamicLibrary);

	[[nodiscard]] bool isOpen() const noexcept;

	[[nodiscard]] void* getHandle() const noexcept;

	void* findFunction (const std::string_view& functionName) noexcept;

	bool open (const std::string_view& nameOrPath) noexcept;

	void close();

private:

	void* handle { nullptr };
};

LIMES_END_NAMESPACE
