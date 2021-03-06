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

#include "../limes_namespace.h"
#include <limes_export.h>
#include "./FilesystemEntry.h"
#include <functional>  // for std::hash

/** @file
	This file defines the SymLink class.
	@ingroup files
 */

LIMES_BEGIN_NAMESPACE

namespace files
{

class Directory;

/** This class represents a symbolic link on the filesystem.
	@ingroup files
 */
class LIMES_EXPORT SymLink final : public FilesystemEntry
{
public:
	using FilesystemEntry::FilesystemEntry;

	LIMES_DEFAULT_COPYABLE (SymLink)
	LIMES_DEFAULT_MOVABLE (SymLink)

	/** Creates a symbolic link object (the link itself will also be created on the filesystem). */
	explicit SymLink (const Path& symLinkPath, const FilesystemEntry& linkTarget);

	/** Follows the symbolic link to find its target.
		If the \c recurse option is true, and the immediate target of this symlink is another symlink, then this function will follow *that* symlink, and so on, until a non-symbolic-link filesystem object is reached.
		The recursion is limited to a depth of 50 recursive calls.
	 */
	[[nodiscard]] FilesystemEntry follow (bool recurse = true) const noexcept;

	/** Returns true if this symbolic link references the given FilesystemEntry, either as its immediate target or as part of the chain of recursive symbolic links (if one exists). */
	[[nodiscard]] bool references (const FilesystemEntry& entry) const;

	/** Returns true if the other symbolic link references the same target as this one, either as its immediate target or as part of the chain of recursive symbolic links (if one exists). */
	[[nodiscard]] bool referencesSameLocationAs (const SymLink& other) const;

	/** @name Symbolic link creation */
	///@{
	/** Creates a symbolic link on the filesystem.
		@param linkPath The path to the symbolic link to be created
		@param target The filesystem object that the new link will reference
		@returns True if link creation was successful
	 */
	static bool create (const Path& linkPath, const Path& target) noexcept;
	static bool create (const Path& linkPath, const FilesystemEntry& target) noexcept;

	/** Creates a symbolic link to the specified object in the new directory.
		The created symlink will be at \c <newDirectory>/<origFilename> .
		@param newDirectory The new directory to create the link in.
		@param target The filesystem object that the new link will reference
		@returns True if link creation was successful
	 */
	static bool create (const Directory& newDirectory, const Path& target) noexcept;
	static bool create (const Directory& newDirectory, const FilesystemEntry& target) noexcept;
	///@}

private:
	[[nodiscard]] FilesystemEntry follow_recurse (int counter) const;
};

}  // namespace files

LIMES_END_NAMESPACE

namespace std
{

/** A specialization of \c std::hash for SymLink objects.
	The hash value is computed using both the link's path and the path of the target object.
	@ingroup files
 */
template <>
struct LIMES_EXPORT hash<limes::files::SymLink> final
{
	hash() = default;

	LIMES_DEFAULT_COPYABLE (hash)
	LIMES_DEFAULT_MOVABLE (hash)

	size_t operator() (const limes::files::SymLink& l) const noexcept;
};

}  // namespace std
