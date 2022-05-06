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

#include "file.h"
#include "directory.h"
#include <limes_namespace.h>
#include <fstream>
#include <algorithm>
#include <exception>
#include <sstream>
#include "../misc/Functions.h"
#include "../text/StringUtils.h"
#include "../hashes/hash.h"

LIMES_BEGIN_NAMESPACE

namespace files
{

std::string File::getFilename (bool includeExtension) const
{
	if (! includeExtension)
		return getPath().stem().string();

	return getPath().filename().string();
}

std::string File::getFileExtension() const
{
	return getPath().extension().string();
}

bool File::hasFileExtension (const std::string& extension) const
{
	return getFileExtension() == extension;
}

bool File::hasFileExtension() const
{
	return getPath().has_extension();
}

File& File::replaceFileExtension (const std::string& newFileExtension)
{
	assignPath (getPath().replace_extension (newFileExtension));
	return *this;
}

bool File::write_data (const char* const data, std::size_t numBytes, bool overwrite) const noexcept
{
	if (numBytes == 0)
		return false;

	return try_call ([data, numBytes, overwrite, p = getAbsolutePath()]
					 {
			const auto mode = overwrite ? std::ios::trunc : std::ios::app;

			std::ofstream stream { p.c_str(), mode };

			stream.write (data, static_cast<std::streamsize> (numBytes)); });
}

bool File::overwriteWithData (const char* const data, std::size_t numBytes) const noexcept
{
	return write_data (data, numBytes, true);
}

bool File::overwriteWithText (const std::string& text) const noexcept
{
	return overwriteWithData (text.data(), text.size());
}

bool File::overwriteWithText (const std::vector<std::string>& text) const noexcept
{
	return overwriteWithText (strings::joinWithNewlines (text));
}

bool File::appendData (const char* const data, std::size_t numBytes) const noexcept
{
	return write_data (data, numBytes, false);
}

bool File::appendText (const std::string& text) const noexcept
{
	return appendData (text.data(), text.size());
}

bool File::appendText (const std::vector<std::string>& text) const noexcept
{
	return appendText (strings::joinWithNewlines (text));
}

bool File::prependText (const std::string& text) const noexcept
{
	auto data = loadAsString();

	data = text + data;

	return overwriteWithText (data);
}

bool File::prependText (const std::vector<std::string>& text) const noexcept
{
	return prependText (strings::joinWithNewlines (text));
}

RawData File::loadAsData() const noexcept
{
	try
	{
		std::ifstream stream { getAbsolutePath().c_str() };

		return RawData { stream };
	}
	catch (const std::exception&)
	{
		return {};
	}
}

std::string File::loadAsString() const noexcept
{
	try
	{
		std::ifstream stream { getAbsolutePath().c_str() };

		using Iterator = std::istreambuf_iterator<char>;

		return { Iterator (stream), Iterator() };
	}
	catch (const std::exception&)
	{
		return {};
	}
}

std::vector<std::string> File::loadAsLines() const
{
	return strings::splitAtNewlines (loadAsString());
}

std::string File::hash (hash::Type hashType) const
{
	return hash::hash (hashType, loadAsString());
}

std::string File::hash_md5() const
{
	return hash::md5 (loadAsString());
}

std::string File::hash_sha1() const
{
	return hash::sha1 (loadAsString());
}

std::string File::hash_sha224() const
{
	return hash::sha224 (loadAsString());
}

std::string File::hash_sha256() const
{
	return hash::sha256 (loadAsString());
}

std::string File::hash_sha384() const
{
	return hash::sha384 (loadAsString());
}

std::string File::hash_sha512() const
{
	return hash::sha512 (loadAsString());
}

/*-------------------------------------------------------------------------------------------------------------------------*/

TempFile::TempFile (const std::string& filename)
	: File (Directory::getTempFileDirectory().getAbsolutePath() / filename)
{
	createIfDoesntExist();
}

TempFile::~TempFile()
{
	deleteIfExists();
}

}  // namespace files

LIMES_END_NAMESPACE
