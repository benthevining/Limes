
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

#include <limes_export.h>	  // for LIMES_EXPORT
#include <limes_namespace.h>  // for LIMES_BEGIN_NAMESPACE, LIMES_END_NAMESPACE
#include <string>			  // for string
#include "preprocessor.h"	  // for LIMES_CONSTEXPR_COPYABLE, LIMES_CONSTEX...
#include <functional>		  // for std::hash

/** @defgroup misc Miscellaneous
	Miscellaneous things.
	@ingroup limes_core
 */

/** @dir libs/limes_core/misc
	This directory contains miscellaneous utilities.
	@ingroup misc
 */

/** @file
	This file defines the Version class.
	@ingroup misc
 */

LIMES_BEGIN_NAMESPACE

/** This namespace contains miscellaneous things.
	@ingroup misc
 */
namespace misc
{

/** A class that represents a version of something, with major, minor, and patch numbers.
	This class is meant to model the semantic versioning standard.
	@ingroup misc
 */
struct LIMES_EXPORT Version final
{
	/** @name Constructors */
	///@{
	/** Creates a default Version object with the value 0.0.1. */
	constexpr explicit Version() noexcept = default;

	/** Creates a Version object with a specified value. */
	constexpr explicit Version (int maj, int min, int p) noexcept
		: major (maj), minor (min), patch (p)
	{
	}
	///@}

	LIMES_CONSTEXPR_MOVABLE (Version);
	LIMES_CONSTEXPR_COPYABLE (Version);

	/** @name Equality comparisons */
	///@{
	[[nodiscard]] constexpr bool operator== (const Version& other) const noexcept
	{
		return major == other.major && minor == other.minor && patch == other.patch;
	}

	[[nodiscard]] constexpr bool operator!= (const Version& other) const noexcept
	{
		return ! (*this == other);
	}
	///@}

	/** @name Greater/less than comparisons */
	///@{
	/** Returns true if this version is newer than the other one. */
	[[nodiscard]] constexpr bool operator> (const Version& other) const noexcept
	{
		if (major < other.major) return false;

		if (minor < other.minor) return false;

		return patch > other.patch;
	}

	/** Returns true if this version is older than the other one. */
	[[nodiscard]] constexpr bool operator<(const Version& other) const noexcept
	{
		if (major > other.major) return false;

		if (minor > other.minor) return false;

		return patch < other.patch;
	}

	/** Returns true if this version is newer than the other one. */
	[[nodiscard]] constexpr bool isNewerThan (const Version& other) const noexcept
	{
		return *this > other;
	}

	/** Returns true if this version is older than the other one. */
	[[nodiscard]] constexpr bool isOlderThan (const Version& other) const noexcept
	{
		return *this < other;
	}
	///@}

	/** Returns true if this version has the same major release number as the other one. */
	[[nodiscard]] constexpr bool hasSameMajorVersion (const Version& other) const noexcept
	{
		return major == other.major;
	}

	/** Increments the major value and sets minor and patch to 0. */
	constexpr void bumpMajor() noexcept
	{
		++major;
		minor = 0;
		patch = 0;
	}

	/** Increments the minor value and sets patch to 0. */
	constexpr void bumpMinor() noexcept
	{
		++minor;
		patch = 0;
	}

	/** Increments the patch value. */
	constexpr void bumpPatch() noexcept
	{
		++patch;
	}

	/** Returns the major version number. */
	[[nodiscard]] constexpr int getMajor() const noexcept
	{
		return major;
	}

	/** Returns the minor version number. */
	[[nodiscard]] constexpr int getMinor() const noexcept
	{
		return minor;
	}

	/** Returns the patch version number. */
	[[nodiscard]] constexpr int getPatch() const noexcept
	{
		return patch;
	}

	/** Returns a new Version object with bumpMajor() applied.
		@see bumpMajor()
	*/
	[[nodiscard]] constexpr Version withMajorBump() const
	{
		return Version { major + 1,
						 0,
						 0 };
	}

	/** Returns a new Version object with bumpMinor() applied.
		@see bumpMinor()
	*/
	[[nodiscard]] constexpr Version withMinorBump() const
	{
		return Version { major,
						 minor + 1,
						 0 };
	}

	/** Returns a new Version object with bumpPatch() applied.
		@see bumpPatch()
	*/
	[[nodiscard]] constexpr Version withPatchBump() const
	{
		return Version { major,
						 minor,
						 patch + 1 };
	}

	/** Returns a string representation of this Version. */
	[[nodiscard]] std::string toString (char separator = '.') const;

private:

	int major { 0 }, minor { 0 }, patch { 1 };
};

}  // namespace misc

LIMES_END_NAMESPACE

namespace std
{

/** A specialization of \c std::hash for Version objects.
	@ingroup misc
 */
template <>
struct LIMES_EXPORT hash<limes::misc::Version>
{
	hash() = default;

	LIMES_DEFAULT_COPYABLE (hash);
	LIMES_DEFAULT_MOVABLE (hash);

	size_t operator() (const limes::misc::Version& v) const noexcept;
};
}  // namespace std
