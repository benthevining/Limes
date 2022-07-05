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

#include "../limes_namespace.h"
#include "./CFile.h"
#include "./file.h"
#include "../system/limes_assert.h"
#include <string_view>
#include <limes_platform.h>
#include <exception>
#include <cstdio>

#if LIMES_WINDOWS
#	include <windows.h>
#endif

LIMES_BEGIN_NAMESPACE

namespace files
{

CFile::CFile (const Path& filepath, Mode mode) noexcept
	: path (filepath)
{
	open (filepath, mode);
}

CFile::CFile (std::FILE* fileHandle) noexcept
	: ptr (fileHandle)
{
}

CFile::~CFile() noexcept
{
	close();
}

CFile::CFile (CFile&& other) noexcept
	: ptr (other.ptr), path (other.path)
{
	other.ptr = nullptr;
	other.path.clear();
}

CFile& CFile::operator= (CFile&& other) noexcept  // cppcheck-suppress operatorEqVarError
{
	close();

	ptr		  = other.ptr;
	other.ptr = nullptr;

	path = other.path;
	other.path.clear();

	return *this;
}

std::FILE* CFile::get() const noexcept
{
	return ptr;
}

std::FILE* CFile::operator->() const noexcept
{
	return ptr;
}

std::FILE& CFile::operator*() const
{
	LIMES_ASSERT (ptr != nullptr);
	return *ptr;
}

static inline std::string_view modeToString (CFile::Mode mode) noexcept
{
	switch (mode)
	{
		case (CFile::Mode::Read) : return "r";
		case (CFile::Mode::Write) : return "w";
		case (CFile::Mode::Append) : return "a";
		case (CFile::Mode::ReadExtended) : return "r+";
		case (CFile::Mode::WriteExtended) : return "w+";
		case (CFile::Mode::AppendExtended) : return "a+";
		default : LIMES_UNREACHABLE; return "";
	}
}

static inline std::string pathToString (const Path& path)
{
	auto pathCopy { path };

	const auto pathStr = pathCopy.make_preferred();

#if LIMES_WINDOWS
	const auto sizeNeeded = WideCharToMultiByte (CP_UTF8, 0, &pathStr[0], static_cast<int> (pathStr.size()), NULL, 0, NULL, NULL);

	std::string strTo { size_needed, 0 };

	WideCharToMultiByte (CP_UTF8, 0, &wstr[0], static_cast<int> (wstr.size()), &strTo[0], size_needed, NULL, NULL);

	return strTo;
#else
	return pathStr;
#endif
}

bool CFile::open (const Path& filepath, Mode mode) noexcept
{
	close();

	try
	{
		ptr = std::fopen (pathToString (filepath).c_str(), modeToString (mode).data());

		if (ptr == nullptr)
			return false;

		path = filepath;

		return true;
	}
	catch (std::exception&)
	{
		ptr = nullptr;
		path.clear();

		return false;
	}
}

void CFile::close() noexcept
{
	if (ptr != nullptr)
	{
		try
		{
			std::fclose (ptr);
		}
		catch (std::exception&)
		{
		}

		path.clear();
	}
}

bool CFile::isOpen() const noexcept
{
	return ptr != nullptr;
}

CFile::operator bool() const noexcept
{
	return isOpen();
}

Path CFile::getPath() const noexcept
{
	return path;
}

File CFile::getFile() const
{
	return File { path };
}

CFile CFile::createTempFile()
{
	return CFile { std::tmpfile() };
}

}  // namespace files

LIMES_END_NAMESPACE
