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

#include <limes_namespace.h>
#include <limes_export.h>
#include "FilesystemEntry.h"

LIMES_BEGIN_NAMESPACE

namespace files
{

class LIMES_EXPORT SymLink final : public FilesystemEntry
{
public:

	using FilesystemEntry::FilesystemEntry;

	SymLink (const Path& symLinkPath, const FilesystemEntry& linkTarget);

	[[nodiscard]] FilesystemEntry follow (bool recurse = true) const;

	[[nodiscard]] bool references (const FilesystemEntry& entry) const;

	[[nodiscard]] bool referencesSameLocationAs (const SymLink& other) const;

	static bool create (const Path& linkPath, const FilesystemEntry& target);
};

}  // namespace files

LIMES_END_NAMESPACE
