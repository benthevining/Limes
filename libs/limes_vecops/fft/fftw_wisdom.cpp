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
#include "./limes_fft.h"  // IWYU pragma: associated
#include <limes_core.h>
#include <filesystem>
#include <cstdlib>
#include <sstream>
#include <cstdio>
#include <exception>

#if LIMES_VECOPS_USE_FFTW
#	include <mutex>  // for lock_guard, mutex
#	include <atomic>
#endif

LIMES_BEGIN_NAMESPACE

namespace vecops
{

namespace fftw
{

#if LIMES_VECOPS_USE_FFTW
static files::Directory	 widom_file_dir;	  // NOLINT
static std::mutex		 wisdom_lock;		  // NOLINT
static std::atomic<bool> useWisdom { true };  // NOLINT
#endif

bool setWisdomFileDir (const files::Directory& directory)
{
#if LIMES_VECOPS_USE_FFTW
	if (! directory.isValid())
		return false;

	const std::lock_guard g { wisdom_lock };

	if (widom_file_dir == directory)
		return false;

	widom_file_dir = directory;
	return true;
#else
	return false;
#endif
}

files::Directory getWisdomFileDir()
{
#if LIMES_VECOPS_USE_FFTW
	const std::lock_guard g { wisdom_lock };

	if (! widom_file_dir.isValid())
	{
		if (const auto* dir = std::getenv ("FFTW_WISDOM_FILE_DIR"))
			widom_file_dir = files::Path { dir };
		else if (const auto* homeDir = std::getenv ("HOME"))
			widom_file_dir = files::Path { homeDir };
	}

	return widom_file_dir;
#else
	return {};
#endif
}

void enableWisdom (bool shouldUseWisdom)
{
#if LIMES_VECOPS_USE_FFTW
	useWisdom.store (shouldUseWisdom);
#endif
}

bool isUsingWisdom()
{
#if LIMES_VECOPS_USE_FFTW
	if (! getWisdomFileDir().isValid())
		return false;

	return useWisdom.load();
#else
	return false;
#endif
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

[[nodiscard]] static inline files::CFile fftw_get_wisdom_file (bool isDouble, bool save) noexcept
{
#	if FFTW_SINGLE_ONLY
	if (isDouble)
		return nullptr;
#	elif FFTW_DOUBLE_ONLY
	if (! isDouble)
		return nullptr;
#	endif

	if (! fftw::useWisdom.load())
		return {};

	try
	{
		const auto fileDir = fftw::getWisdomFileDir();

		if (! fileDir.isValid())
			return {};

		std::string filename { ".fftw_wisdom." };

		const auto typeChar = isDouble ? 'd' : 'f';

		filename.push_back (typeChar);

		const auto mode = save ? files::CFile::Mode::Write : files::CFile::Mode::Read;

		return fileDir.getChildFile (filename).getCfile (mode);
	}
	catch (const std::exception&)
	{
		return {};
	}
}


template <bool IsDouble>
void fftw_save_wisdom()
{
	if (auto wisdomFile = fftw_get_wisdom_file (IsDouble, false))
	{
		if constexpr (IsDouble)
			fftw_import_wisdom_from_file (wisdomFile.get());
		else
			fftwf_import_wisdom_from_file (wisdomFile.get());
	}
}

template void fftw_save_wisdom<true>();
template void fftw_save_wisdom<false>();


template <bool IsDouble>
void fftw_load_wisdom()
{
	if (auto wisdomFile = fftw_get_wisdom_file (IsDouble, true))
	{
		if constexpr (IsDouble)
			fftw_export_wisdom_to_file (wisdomFile.get());
		else
			fftwf_export_wisdom_to_file (wisdomFile.get());
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
