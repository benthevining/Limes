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

#include "CFile.h"
#include <limes_namespace.h>
#include "../system/limes_assert.h"
#include <string_view>
#include <limes_platform.h>
#include <exception>

#if LIMES_WINDOWS
#	include <locale>  // for wstring_convert
#	include <codecvt>
#endif

LIMES_BEGIN_NAMESPACE

namespace files
{

CFile::CFile (const Path& filepath, Mode mode) noexcept
{
	open (filepath, mode);
}

CFile::~CFile() noexcept
{
	close();
}

CFile::CFile (CFile&& other) noexcept
	: ptr (other.ptr)
{
	other.ptr = nullptr;
}

CFile& CFile::operator= (CFile&& other) noexcept  // cppcheck-suppress operatorEqVarError
{
	close();

	ptr		  = other.ptr;
	other.ptr = nullptr;
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

inline std::string_view modeToString (CFile::Mode mode) noexcept
{
	switch (mode)
	{
		case (CFile::Mode::Read) : return "r";
		case (CFile::Mode::Write) : return "w";
		case (CFile::Mode::Append) : return "a";
		case (CFile::Mode::ReadExtended) : return "r+";
		case (CFile::Mode::WriteExtended) : return "w+";
		case (CFile::Mode::AppendExtended) : return "a+";
		default : LIMES_UNREACHABLE;
	}
}

inline auto pathToString (const Path& path)
{
	auto pathCopy { path };

	const auto pathStr = pathCopy.make_preferred();

#if LIMES_WINDOWS
	return std::wstring_convert<std::codecvt_utf8<wchar_t>> {}.to_bytes (pathStr);
#else
	return pathStr;
#endif
}

bool CFile::open (const Path& filepath, Mode mode) noexcept
{
	close();

	try
	{
		ptr = std::fopen (pathToString (filepath).c_str(), modeToString (mode).begin());
		return true;
	}
	catch (std::exception&)
	{
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

}  // namespace files

LIMES_END_NAMESPACE
