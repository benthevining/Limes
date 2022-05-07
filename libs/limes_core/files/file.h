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

#include <limes_export.h>		// for LIMES_EXPORT
#include <limes_namespace.h>	// for LIMES_BEGIN_NAMESPACE, LIMES_END_NAME...
#include <cstddef>				// for size_t
#include <string>				// for string
#include <vector>				// for vector
#include "../hashes/hash.h"		// for Type
#include "../memory/RawData.h"	// for RawData
#include "FilesystemEntry.h"	// for FilesystemEntry, Path


LIMES_BEGIN_NAMESPACE

namespace files
{

class LIMES_EXPORT File : public FilesystemEntry
{
public:

	using FilesystemEntry::FilesystemEntry;

	[[nodiscard]] std::string getFilename (bool includeExtension = false) const;

	[[nodiscard]] std::string getFileExtension() const;

	[[nodiscard]] bool hasFileExtension (const std::string& extension) const;

	[[nodiscard]] bool hasFileExtension() const;

	File& replaceFileExtension (const std::string& newFileExtension);

	bool overwriteWithData (const char* const data, std::size_t numBytes) const noexcept;

	bool overwriteWithText (const std::string& text) const noexcept;

	bool overwriteWithText (const std::vector<std::string>& text) const noexcept;

	bool appendData (const char* const data, std::size_t numBytes) const noexcept;

	bool appendText (const std::string& text) const noexcept;

	bool appendText (const std::vector<std::string>& text) const noexcept;

	bool prependText (const std::string& text) const noexcept;

	bool prependText (const std::vector<std::string>& text) const noexcept;

	[[nodiscard]] RawData loadAsData() const noexcept;

	[[nodiscard]] std::string loadAsString() const noexcept;

	[[nodiscard]] std::vector<std::string> loadAsLines() const;

	[[nodiscard]] std::string hash (hash::Type hashType) const;

	[[nodiscard]] std::string hash_md5() const;

	[[nodiscard]] std::string hash_sha1() const;

	[[nodiscard]] std::string hash_sha224() const;

	[[nodiscard]] std::string hash_sha256() const;

	[[nodiscard]] std::string hash_sha384() const;

	[[nodiscard]] std::string hash_sha512() const;

	[[nodiscard]] static File getCurrentExecutable();

	[[nodiscard]] static File getCurrentModule();

private:

	[[nodiscard]] bool write_data (const char* const data, std::size_t numBytes, bool overwrite) const noexcept;
};

/*-------------------------------------------------------------------------------------------------------------------------*/

class LIMES_EXPORT TempFile final : public File
{
public:

	explicit TempFile (const std::string& filename);

	~TempFile() final;
};

}  // namespace files

LIMES_END_NAMESPACE
