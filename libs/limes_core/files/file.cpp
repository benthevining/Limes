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
#include <iterator>			  // for istreambuf_iterator
#include <limes_namespace.h>  // for LIMES_BEGIN_NAMESPACE
#include <exception>		  // for exception
#include <filesystem>		  // for path, operator/
#include <fstream>			  // for string, ifstream, ofstream
#include <string>			  // for char_traits, operator+
#include <string_view>
#include "../hashes/hash.h"				  // for hash, md5, sha1, sha224
#include "../misc/Functions.h"			  // for try_call
#include "../text/StringUtils.h"		  // for joinWithNewlines, splitA...
#include "directory.h"					  // for Directory
#include "exec_location/exec_location.h"  // for getExecutablePath, getMo...
#include "FilesystemEntry.h"			  // for Path
#include "../memory/RawData.h"			  // for RawData
#include <cstdio>
#include <atomic>

#if LIMES_WINDOWS
#	include <locale>  // for wstring_convert
#	include <codecvt>
#endif

LIMES_BEGIN_NAMESPACE

namespace files
{

File& File::operator= (const Path& newPath)
{
	assignPath (newPath);
	return *this;
}

File& File::operator= (const std::string_view& newPath)
{
	assignPath (newPath);
	return *this;
}

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

bool File::hasFileExtension (const std::string_view& extension) const
{
	return getFileExtension() == extension;
}

bool File::hasFileExtension() const
{
	return getPath().has_extension();
}

File& File::replaceFileExtension (const std::string_view& newFileExtension,
								  bool					  renameOnDisk)
{
	if (renameOnDisk)
		rename (getAbsolutePath().replace_extension (newFileExtension));
	else
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

bool File::overwrite (const char* const data, std::size_t numBytes) const noexcept
{
	return write_data (data, numBytes, true);
}

bool File::overwrite (const RawData& data) const noexcept
{
	return write_data (data.getData(), data.getSize(), true);
}

bool File::overwrite (const std::string_view& text) const noexcept
{
	return overwrite (text.data(), text.size());
}

bool File::append (const char* const data, std::size_t numBytes) const noexcept
{
	return write_data (data, numBytes, false);
}

bool File::append (const RawData& data) const noexcept
{
	return write_data (data.getData(), data.getSize(), false);
}

bool File::append (const std::string_view& text) const noexcept
{
	return write_data (text.data(), text.size(), false);
}

bool File::prepend (const RawData& data) const noexcept
{
	return prepend (data.getData(), data.getSize());
}

bool File::prepend (const char* const data, std::size_t numBytes) const noexcept
{
	auto dataObj = loadAsData();

	dataObj.append (data, numBytes);

	return overwrite (dataObj);
}

bool File::prepend (const std::string_view& text) const noexcept
{
	auto data = loadAsString();

	data = std::string { text } + data;

	return overwrite (data);
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

File File::getCurrentExecutable()
{
	return File { getExecutablePath() };
}

File File::getCurrentModule()
{
	return File { getModulePath() };
}

std::FILE* File::getCfile (char mode) const noexcept
{
	if (! exists())
		return nullptr;

	try
	{
		const auto pathStr = getAbsolutePath().make_preferred();

#if LIMES_WINDOWS
		const auto str = std::wstring_convert<std::codecvt_utf8<wchar_t>> {}.to_bytes (pathStr);
		return std::fopen (str.c_str(), &mode);
#else
		return std::fopen (pathStr.c_str(), &mode);
#endif
	}
	catch (std::exception&)
	{
		return nullptr;
	}
}

/*-------------------------------------------------------------------------------------------------------------------------*/

TempFile::TempFile (const std::string_view& filename, bool destroyOnDelete)
	: File (Directory::getTempFileDirectory().getAbsolutePath() / filename), shouldDelete (destroyOnDelete)
{
	createIfDoesntExist();
}

TempFile::~TempFile()
{
	if (shouldDelete)
	{
		try
		{
			deleteIfExists();
		}
		catch (std::exception&)
		{
		}
	}
}

TempFile::TempFile (TempFile&& other) noexcept
	: shouldDelete (other.shouldDelete)
{
	assignPath (other.getAbsolutePath());

	other.shouldDelete = false;
}

TempFile& TempFile::operator= (TempFile&& other) noexcept
{
	assignPath (other.getAbsolutePath());

	shouldDelete = other.shouldDelete;

	other.shouldDelete = false;

	return *this;
}

static std::atomic<int> tempFileCounter = 0;

TempFile TempFile::getNextFile()
{
	const auto name = std::to_string (tempFileCounter++) + ".tmp";

	return TempFile { name, true };
}

}  // namespace files

LIMES_END_NAMESPACE

namespace std
{
size_t hash<limes::files::File>::operator() (const limes::files::File& f) const noexcept
{
	return hash<string> {}(f.loadAsString());
}
}  // namespace std
