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

#include "./file.h"
#include <iterator>	 // for istreambuf_iterator
#include "../limes_namespace.h"
#include <exception>   // for exception
#include <filesystem>  // for path, operator/
#include <fstream>	   // for string, ifstream, ofstream
#include <string>	   // for char_traits, operator+
#include <string_view>
#include "../hashes/hash.h"					// for hash, md5, sha1, sha224
#include "../misc/Functions.h"				// for try_call
#include "../text/StringUtils.h"			// for joinWithNewlines, splitA...
#include "./directory.h"					// for Directory
#include "./exec_location/exec_location.h"	// for getExecutablePath, getMo...
#include "./FilesystemEntry.h"				// for Path
#include "../memory/RawData.h"				// for RawData
#include <cstdio>
#include <atomic>
#include "../system/limes_assert.h"
#include "../system/compiler_warnings.h"

LIMES_BEGIN_NAMESPACE

namespace files
{

#pragma mark File

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

	return func::try_call ([data, numBytes, overwrite, p = getAbsolutePath()]
						   {
			const auto mode = overwrite ? std::ios::trunc : std::ios::app;

			std::ofstream stream { p.c_str(), mode };

			stream.write (data, static_cast<std::streamsize> (numBytes)); });
}

bool File::overwrite (const char* const data, std::size_t numBytes) const noexcept
{
	return write_data (data, numBytes, true);
}

bool File::overwrite (const memory::RawData& data) const noexcept
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

bool File::append (const memory::RawData& data) const noexcept
{
	return write_data (data.getData(), data.getSize(), false);
}

bool File::append (const std::string_view& text) const noexcept
{
	return write_data (text.data(), text.size(), false);
}

bool File::prepend (const memory::RawData& data) const noexcept
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

File File::duplicate() const noexcept
{
	const auto dir = getDirectory();

	const auto newFilename = [filename = getFilename (false), extension = getFileExtension(), &dir]() -> std::string
	{
		auto newName = filename + "_copy." + extension;

		if (! dir.contains (newName))
			return newName;

		for (auto copyNum = 2; copyNum < 999; ++copyNum)
		{
			newName = filename + "_copy" + std::to_string (copyNum) + "." + extension;

			if (! dir.contains (newName))
				return newName;
		}

		return {};
	}();

	if (newFilename.empty())
		return {};

	File newFile { dir.getAbsolutePath() / newFilename };

	LIMES_ASSERT (! newFile.exists());

	if (! newFile.createIfDoesntExist())
		return {};

	if (! newFile.overwrite (loadAsData()))
	{
		newFile.deleteIfExists();
		return {};
	}

	return newFile;
}

memory::RawData File::loadAsData() const noexcept
{
	try
	{
		std::ifstream stream { getAbsolutePath().c_str() };

		return memory::RawData { stream };
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
	return text::splitAtNewlines (loadAsString());
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

CFile File::getCfile (CFile::Mode mode) const noexcept
{
	if (! exists())
		return {};

	try
	{
		return CFile { getAbsolutePath(), mode };
	}
	catch (std::exception&)
	{
		return {};
	}
}

/*-------------------------------------------------------------------------------------------------------------------------*/

#pragma mark TempFile

static inline Path createTmpFilepath (const Path& inputPath)
{
	if (inputPath.is_absolute())
		return inputPath;

	return Directory::getTempFileDirectory().getAbsolutePath() / inputPath;
}

TempFile::TempFile (const Path& filepath, bool destroyOnDelete)
	: File (createTmpFilepath (filepath)), shouldDelete (destroyOnDelete)
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

LIMES_DISABLE_ALL_COMPILER_WARNINGS
TempFile TempFile::getNextFile()
{
	return TempFile { Path { std::tmpnam (nullptr) }, true };
}
LIMES_REENABLE_ALL_COMPILER_WARNINGS

}  // namespace files

LIMES_END_NAMESPACE

namespace std
{
size_t hash<limes::files::File>::operator() (const limes::files::File& f) const noexcept
{
	return hash<string> {}(f.loadAsString());
}
}  // namespace std
