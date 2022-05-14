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
#include "file.h"

LIMES_BEGIN_NAMESPACE

namespace files
{

/** This class represents a dynamically loaded library. */
class LIMES_EXPORT DynamicLibrary final
{
public:

	/** Constructors */
	///@{

	/** Creates an unopened library object. Call open() to actually open a library. */
	DynamicLibrary() = default;

	/** Creates a library object and attempts to open the specified library. */
	explicit DynamicLibrary (const std::string_view& nameOrPath) noexcept;

	///@}

	/** Destructor. If the library is open when this object is destroyed, the destructor will close the library. */
	~DynamicLibrary();

	LIMES_DEFAULT_COPYABLE (DynamicLibrary);
	LIMES_DEFAULT_MOVABLE (DynamicLibrary);

	/** Returns true if this library object refers to the same shared library as the other one. */
	[[nodiscard]] bool operator== (const DynamicLibrary& other) const noexcept;

	/** Returns true if this library object does not refer to the same shared library as the other one. */
	[[nodiscard]] bool operator!= (const DynamicLibrary& other) const noexcept;

	/** Returns true if the library is currently open.
		This can be called after the constructor, or after calling open(), to check if the library opened correctly.
	 */
	[[nodiscard]] bool isOpen() const noexcept;

	/** Returns an opaque platform-specific handle to the currently open shared library (if any). */
	[[nodiscard]] void* getHandle() const noexcept;

	/** Attempts to locate a function within the shared library, and, if found, returns a pointer to the function.
	 */
	[[nodiscard]] void* findFunction (const std::string_view& functionName) noexcept;

	/** Attempts to open a new shared library. If one was previously open, calling this will close the old library.
		@return True if the library was opened successfully
	 */
	bool open (const std::string_view& nameOrPath) noexcept;

	/** Closes the shared library, if one is open. */
	void close();

	/** Attempts to locate the file on disk where the code for the current shared library is actually located. */
	[[nodiscard]] File getFile() const;

private:

	void* handle { nullptr };
};

}  // namespace files

LIMES_END_NAMESPACE

namespace std
{

/** A specialization of \c std::hash for dynamic libraries.
	The hash value is computed based on the path of the file containing the dynamic library code.
 */
template <>
struct LIMES_EXPORT hash<limes::files::DynamicLibrary> final
{
	hash() = default;

	LIMES_DEFAULT_COPYABLE (hash);
	LIMES_DEFAULT_MOVABLE (hash);

	size_t operator() (const limes::files::DynamicLibrary& l) const noexcept;
};

}  // namespace std
