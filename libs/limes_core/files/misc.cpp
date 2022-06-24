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

#include "./misc.h"
#include <limes_namespace.h>
#include <filesystem>
#include "./directory.h"

LIMES_BEGIN_NAMESPACE

namespace files
{

std::uintmax_t getAvailableSpaceOnFilesystem()
{
	return std::filesystem::space (Directory::getCurrentWorkingDirectory().getAbsolutePath()).available;
}

std::uintmax_t getTotalSizeOfFilesystem()
{
	return std::filesystem::space (Directory::getCurrentWorkingDirectory().getAbsolutePath()).capacity;
}

}  // namespace files

LIMES_END_NAMESPACE
