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

#include <limes_namespace.h>
#include "limes_fft.h"
#include <mutex>  // for lock_guard, mutex
#include <filesystem>
#include <cstdlib>
#include <sstream>
#include <cstdio>
#include <exception>

LIMES_BEGIN_NAMESPACE

namespace vecops
{

namespace fftw
{
static std::filesystem::path widom_file_dir { std::getenv ("HOME") };  // NOLINT
static std::mutex			 wisdom_lock;							   // NOLINT
static bool					 useWisdom { true };					   // NOLINT

bool setWisdomFileDir (const std::filesystem::path& dirAbsPath)
{
	if constexpr (! fft::isUsingFFTW())
		return false;

	const std::lock_guard g { wisdom_lock };

	if (dirAbsPath.empty() || ! dirAbsPath.is_absolute())
		return false;

	if (dirAbsPath == widom_file_dir)
		return false;

	widom_file_dir = dirAbsPath;
	return true;
}

std::filesystem::path getWisdomFileDir()
{
	if constexpr (! fft::isUsingFFTW())
		return {};

	const std::lock_guard g { wisdom_lock };

	if (widom_file_dir.empty())
		if (const auto* homeDir = std::getenv ("HOME"))
			widom_file_dir = homeDir;

	return widom_file_dir;
}

void enableWisdom (bool shouldUseWisdom)
{
	useWisdom = shouldUseWisdom;
}

bool isUsingWisdom()
{
	if constexpr (! fft::isUsingFFTW())
		return false;

	if (getWisdomFileDir().empty())
		return false;

	return useWisdom;
}
}  // namespace fftw


#if LIMES_VECOPS_USE_FFTW

#	include "fftw_fft.h"

#	if FFTW_DOUBLE_ONLY
#		define fftwf_import_wisdom_from_file fftw_import_wisdom_from_file
#		define fftwf_export_wisdom_to_file	  fftw_export_wisdom_to_file
#	elif FFTW_SINGLE_ONLY
#		define fftw_import_wisdom_from_file fftwf_import_wisdom_from_file
#		define fftw_export_wisdom_to_file	 fftwf_export_wisdom_to_file
#	endif

[[nodiscard]] inline FILE* fftw_get_wisdom_file (bool isDouble, bool save)
{
#	if FFTW_SINGLE_ONLY
	if (isDouble)
		return nullptr;

#	elif FFTW_DOUBLE_ONLY
	if (! isDouble)
		return nullptr;
#	endif

	if (! fftw::useWisdom)
		return nullptr;

	try
	{
		const auto fileDir = fftw::getWisdomFileDir();

		if (fileDir.empty())
			return nullptr;

		const auto typeChar = isDouble ? 'd' : 'f';

		std::stringstream file_name;

		file_name << fileDir << "/.fftw_wisdom." << typeChar;

		const auto mode_char = save ? "wb" : "rb";

		return std::fopen (file_name.str().c_str(), mode_char);
	}
	catch (const std::exception&)
	{
		return nullptr;
	}
}


template <bool IsDouble>
void fftw_save_wisdom()
{
	if (auto* wisdomFile = fftw_get_wisdom_file (IsDouble, false))
	{
		if constexpr (IsDouble)
			fftw_import_wisdom_from_file (wisdomFile);
		else
			fftwf_import_wisdom_from_file (wisdomFile);

		std::fclose (wisdomFile);
	}
}

template void fftw_save_wisdom<true>();
template void fftw_save_wisdom<false>();


template <bool IsDouble>
void fftw_load_wisdom()
{
	if (auto* wisdomFile = fftw_get_wisdom_file (IsDouble, true))
	{
		if constexpr (IsDouble)
			fftw_export_wisdom_to_file (wisdomFile);
		else
			fftwf_export_wisdom_to_file (wisdomFile);

		std::fclose (wisdomFile);
	}
}

template void fftw_load_wisdom<true>();
template void fftw_load_wisdom<false>();

#	if FFTW_DOUBLE_ONLY
#		undef fftwf_import_wisdom_from_file
#		undef fftwf_export_wisdom_to_file
#	elif FFTW_SINGLE_ONLY
#		undef fftw_import_wisdom_from_file
#		undef fftw_export_wisdom_to_file
#	endif

#endif /* LIMES_VECOPS_USE_FFTW */

}  // namespace vecops

LIMES_END_NAMESPACE
