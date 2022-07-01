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

#include "./EmbeddedBinaries.h"
#include "../limes_namespace.h"
#include "../memory/RawData.h"
#include <string>
#include <string_view>
#include <exception>

LIMES_BEGIN_NAMESPACE

namespace binaries
{

std::string FileInfo::fullName() const
{
	if (! hasFileExtension())
		return std::string { name() };

	return std::string { name() } + '.' + std::string { xtn() };
}

std::string FileInfo::string() const
{
	return std::string { fileData, fileDataSize };
}

memory::RawData FileInfo::data() const
{
	return memory::RawData { string_view() };
}

memory::RawData FileInfoList::loadFile (const std::string_view& fileName) const noexcept
{
	try
	{
		if (const auto* file = findFile (fileName))
			return file->data();

		return {};
	}
	catch (std::exception&)
	{
		return {};
	}
}

}  // namespace binaries

LIMES_END_NAMESPACE
