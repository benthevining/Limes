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

bool File::overwriteWithData (const char* const data, std::size_t numBytes) const
{
	return write_data (data, numBytes, true);
}

bool File::appendData (const char* const data, std::size_t numBytes) const
{
	return write_data (data, numBytes, false);
}

bool File::write_data (const char* const data, std::size_t numBytes, bool overwrite) const
{
	if (numBytes == 0)
		return false;

	try
	{
		const auto mode = overwrite ? std::ios::trunc : std::ios::app;

		std::basic_ofstream<char> stream { getAbsolutePath().c_str(), mode };

		stream.write (data, static_cast<std::streamsize> (numBytes));
	}
	catch (const std::exception&)
	{
		return false;
	}

	return true;
}

bool File::overwriteWithText (const std::string& text) const
{
	return overwriteWithData (text.data(), text.size());
}

bool File::appendText (const std::string& text) const
{
	return appendData (text.data(), text.size());
}

[[nodiscard]] static inline std::string linesToSingleString (const std::vector<std::string>& lines)
{
	std::string combined;

	for (const auto& line : lines)
	{
		combined += line;
		combined += '\n';
	}

	return combined;
}

bool File::overwriteWithText (const std::vector<std::string>& text) const
{
	overwriteWithText (linesToSingleString (text));
}

bool File::appendText (const std::vector<std::string>& text) const
{
	appendText (linesToSingleString (text));
}

std::string File::loadAsString() const
{
	try
	{
		using DataType = char;

		std::basic_ifstream<DataType> stream { getAbsolutePath().c_str() };

		using Iterator = std::istreambuf_iterator<DataType>;

		std::basic_string<DataType> content { Iterator (stream), Iterator() };

		return { content };
	}
	catch (const std::exception&)
	{
		return {};
	}
}

std::vector<std::string> File::loadAsLines() const
{
	std::vector<std::string> lines;

	auto ss = std::stringstream { loadAsString() };

	for (std::string line; std::getline (ss, line, '\n');)
		lines.push_back (line);

	return lines;
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
