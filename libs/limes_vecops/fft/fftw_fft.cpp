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

#include "fftw_fft.h"

namespace limes::vecops
{

template <Scalar SampleType, typename ComplexType>
LIMES_FORCE_INLINE void fftw_pack (const SampleType* re, const SampleType* im,
								   ComplexType* m_packed, int fft_size)
{
	const auto hs = fft_size / 2;

	for (auto i = 0; i <= hs; ++i)
		m_packed[i][0] = re[i];

	if (im == nullptr)
		for (auto i = 0; i <= hs; ++i)
			m_packed[i][1] = SampleType (0);
	else
		for (auto i = 0; i <= hs; ++i)
			m_packed[i][1] = im[i];
}

template <Scalar SampleType, typename ComplexType>
LIMES_FORCE_INLINE void fftw_unpack (SampleType* re, SampleType* im,
									 ComplexType* m_packed, int fft_size)
{
	const auto hs = fft_size / 2;

	for (auto i = 0; i <= hs; ++i)
		re[i] = m_packed[i][0];

	if (im != nullptr)
		for (auto i = 0; i <= hs; ++i)
			im[i] = m_packed[i][1];
}

/*---------------------------------------------------------------------------------------------------------------------------*/

// this is the public interface for wisdom
namespace fftw
{
static std::string widom_file_dir;	// NOLINT
static std::mutex  wisdom_lock;
static bool		   useWisdom { true };

void setWisdomFileDir (std::string_view dirAbsPath)
{
	const std::lock_guard g { wisdom_lock };

	widom_file_dir = dirAbsPath;
}

std::string getWisdomFileDir()
{
	const std::lock_guard g { wisdom_lock };

	if (widom_file_dir.empty())
	{
		const auto* homeDir = std::getenv ("HOME");

		if (homeDir != nullptr)
			widom_file_dir = homeDir;
	}

	return widom_file_dir;
}

void enableWisdom (bool shouldUseWisdom)
{
	useWisdom = shouldUseWisdom;
}

bool isUsingWisdom()
{
	if (getWisdomFileDir().empty())
		return false;

	return useWisdom;
}
}  // namespace fftw

[[nodiscard]] inline FILE* fftw_get_wisdom_file (bool isDouble, bool save)
{
#if FFTW_SINGLE_ONLY
	if (isDouble)
		return nullptr;
#elif FFTW_DOUBLE_ONLY
	if (! isDouble)
		return nullptr;
#endif

	if (! fftw::useWisdom)
		return nullptr;

	const auto fileDir = fftw::getWisdomFileDir();

	if (fileDir.empty())
		return nullptr;

	const auto typeChar = isDouble ? 'd' : 'f';

	char fn[256];
	std::snprintf (fn, sizeof (fn), "%s/%s.%c", fileDir.c_str(), ".fftw_wisdom", typeChar);

	return std::fopen (fn, save ? "wb" : "rb");
}

inline void fftw_load_wisdom (bool isDouble)
{
	if (auto* wisdomFile = fftw_get_wisdom_file (isDouble, false))
	{
		if (isDouble)
			fftw_import_wisdom_from_file (wisdomFile);
		else
			fftwf_import_wisdom_from_file (wisdomFile);

		std::fclose (wisdomFile);
	}
}

inline void fftw_save_wisdom (bool isDouble)
{
	if (auto* wisdomFile = fftw_get_wisdom_file (isDouble, true))
	{
		if (isDouble)
			fftw_export_wisdom_to_file (wisdomFile);
		else
			fftwf_export_wisdom_to_file (wisdomFile);

		std::fclose (wisdomFile);
	}
}

/*---------------------------------------------------------------------------------------------------------------------------*/

FFTW_FloatFFT::FFTW_FloatFFT (int size)
	: FFTImpl<float> (size), m_fbuf (reinterpret_cast<fft_float_type*> (fftw_malloc (fft_size * sizeof (fft_float_type)))),
	  m_fpacked (reinterpret_cast<fftwf_complex*> (fftw_malloc ((fft_size / 2 + 1) * sizeof (fftwf_complex)))),
	  m_fplanf (fftwf_plan_dft_r2c_1d (fft_size, m_fbuf, m_fpacked, FFTW_ESTIMATE)),
	  m_fplani (fftwf_plan_dft_c2r_1d (fft_size, m_fpacked, m_fbuf, FFTW_ESTIMATE))
{
	static_assert (FFT<float>::isUsingFFTW());

	if (m_extantf == 0)
		fftw_load_wisdom (false);

	++m_extantf;
}

FFTW_FloatFFT::~FFTW_FloatFFT()
{
	if (m_fplanf)
	{
		fftwf_destroy_plan (m_fplanf);
		fftwf_destroy_plan (m_fplani);
		fftwf_free (m_fbuf);
		fftwf_free (m_fpacked);
	}

	--m_extantf;

	if (m_extantf <= 0)
	{
		fftw_save_wisdom (false);
		fftwf_cleanup();
	}
}

void FFTW_FloatFFT::forward (const float* realIn, float* realOut, float* imagOut)
{
#if ! FFTW_DOUBLE_ONLY
	if (realIn != m_fbuf)
#endif
		vecops::copy (m_fbuf, realIn, fft_size);

	fftwf_execute (m_fplanf);

	fftw_unpack (realOut, imagOut, m_fpacked, fft_size);
}

void FFTW_FloatFFT::forwardInterleaved (const float* realIn, float* complexOut)
{
#if ! FFTW_DOUBLE_ONLY
	if (realIn != m_fbuf)
#endif
		vecops::copy (m_fbuf, realIn, fft_size);

	fftwf_execute (m_fplanf);
	// v_convert(complexOut, (const fft_float_type *)m_fpacked, fft_size + 2);
}

void FFTW_FloatFFT::forwardPolar (const float* realIn, float* magOut, float* phaseOut)
{
#if ! FFTW_DOUBLE_ONLY
	if (realIn != m_fbuf)
#endif
		vecops::copy (m_fbuf, realIn, fft_size);

	fftwf_execute (m_fplanf);

	vecops::catesianInterleavedToPolar (magOut, phaseOut, (const fft_float_type*) m_fpacked, fft_size / 2 + 1);
}

void FFTW_FloatFFT::forwardMagnitude (const float* realIn, float* magOut)
{
#if ! FFTW_DOUBLE_ONLY
	if (realIn != m_fbuf)
#endif
		vecops::copy (m_fbuf, realIn, fft_size);

	fftwf_execute (m_fplanf);

	vecops::cartesianInterleavedToMagnitudes (magOut, (const fft_float_type*) m_fpacked, fft_size / 2 + 1);
}

void FFTW_FloatFFT::inverse (const float* realIn, const float* imagIn, float* realOut)
{
	fftw_pack (realIn, imagIn, m_fpacked, fft_size);

	fftwf_execute (m_fplani);

#if ! FFTW_DOUBLE_ONLY
	if (realOut != m_fbuf)
#endif
		vecops::copy (realOut, m_fbuf, fft_size);
}

void FFTW_FloatFFT::inverseInterleaved (const float* complexIn, float* realOut)
{
	// v_convert ((fft_float_type*) m_fpacked, complexIn, fft_size + 2);
	fftwf_execute (m_fplani);

#if ! FFTW_DOUBLE_ONLY
	if (realOut != m_fbuf)
#endif
		vecops::copy (realOut, m_fbuf, fft_size);
}

void FFTW_FloatFFT::inversePolar (const float* magIn, const float* phaseIn, float* realOut)
{
	vecops::polarToCartesianInterleaved ((fft_float_type*) m_fpacked, magIn, phaseIn, fft_size / 2 + 1);  // NOLINT

	fftwf_execute (m_fplani);

#if ! FFTW_DOUBLE_ONLY
	if (realOut != m_fbuf)
#endif
		vecops::copy (realOut, m_fbuf, fft_size);
}

void FFTW_FloatFFT::inverseCepstral (const float* magIn, float* cepOut)
{
	for (auto i = 0; i <= fft_size / 2; ++i)
	{
		m_fpacked[i][0] = std::logf (magIn[i] + shiftAmount<fft_float_type>);
		m_fpacked[i][1] = 0.f;
	}

	fftwf_execute (m_fplani);

#if ! FFTW_DOUBLE_ONLY
	if (cepOut != m_fbuf)
#endif
		vecops::copy (cepOut, m_fbuf, fft_size);
}

int FFTW_FloatFFT::m_extantf = 0;

/*---------------------------------------------------------------------------------------------------------------------------*/

FFTW_DoubleFFT::FFTW_DoubleFFT (int size)
	: FFTImpl<double> (size),
	  m_dbuf (reinterpret_cast<fft_double_type*> (fftw_malloc (fft_size * sizeof (fft_double_type)))),
	  m_dpacked (reinterpret_cast<fftw_complex*> (fftw_malloc ((fft_size / 2 + 1) * sizeof (fftw_complex)))),
	  m_dplanf (fftw_plan_dft_r2c_1d (fft_size, m_dbuf, m_dpacked, FFTW_ESTIMATE)),
	  m_dplani (fftw_plan_dft_c2r_1d (fft_size, m_dpacked, m_dbuf, FFTW_ESTIMATE))
{
	static_assert (FFT<double>::isUsingFFTW());

	if (m_extantd == 0)
		fftw_load_wisdom (true);

	++m_extantd;
}

FFTW_DoubleFFT::~FFTW_DoubleFFT()
{
	fftw_destroy_plan (m_dplanf);
	fftw_destroy_plan (m_dplani);
	fftw_free (m_dbuf);
	fftw_free (m_dpacked);

	--m_extantd;

	if (m_extantd <= 0)
	{
		fftw_save_wisdom (true);
		fftw_cleanup();
	}
}

void FFTW_DoubleFFT::forward (const double* realIn, double* realOut, double* imagOut)
{
#if ! FFTW_SINGLE_ONLY
	if (realIn != m_dbuf)
#endif
		vecops::copy (m_dbuf, realIn, fft_size);

	fftw_execute (m_dplanf);

	fftw_unpack (realOut, imagOut, m_dpacked, fft_size);
}

void FFTW_DoubleFFT::forwardInterleaved (const double* realIn, double* complexOut)
{
#if ! FFTW_SINGLE_ONLY
	if (realIn != m_dbuf)
#endif
		vecops::copy (m_dbuf, realIn, fft_size);

	fftw_execute (m_dplanf);
	// v_convert(complexOut, (const fft_double_type *)m_dpacked, fft_size + 2);
}

void FFTW_DoubleFFT::forwardPolar (const double* realIn, double* magOut, double* phaseOut)
{
#if ! FFTW_SINGLE_ONLY
	if (realIn != m_dbuf)
#endif
		vecops::copy (m_dbuf, realIn, fft_size);

	fftw_execute (m_dplanf);

	vecops::catesianInterleavedToPolar (magOut, phaseOut, (const fft_double_type*) m_dpacked, fft_size / 2 + 1);
}

void FFTW_DoubleFFT::forwardMagnitude (const double* realIn, double* magOut)
{
#if ! FFTW_SINGLE_ONLY
	if (realIn != m_dbuf)
#endif
		vecops::copy (m_dbuf, realIn, fft_size);

	fftw_execute (m_dplanf);

	vecops::cartesianInterleavedToMagnitudes (magOut, (const fft_double_type*) m_dpacked, fft_size / 2 + 1);
}

void FFTW_DoubleFFT::inverse (const double* realIn, const double* imagIn, double* realOut)
{
	fftw_pack (realIn, imagIn, m_dpacked, fft_size);

	fftw_execute (m_dplani);

#if ! FFTW_SINGLE_ONLY
	if (realOut != m_dbuf)
#endif
		vecops::copy (realOut, m_dbuf, fft_size);
}

void FFTW_DoubleFFT::inverseInterleaved (const double* complexIn, double* realOut)
{
	// v_convert ((fft_double_type*) m_dpacked, complexIn, fft_size + 2);
	fftw_execute (m_dplani);

#if ! FFTW_SINGLE_ONLY
	if (realOut != m_dbuf)
#endif
		vecops::copy (realOut, m_dbuf, fft_size);
}

void FFTW_DoubleFFT::inversePolar (const double* magIn, const double* phaseIn, double* realOut)
{
	vecops::polarToCartesianInterleaved ((fft_double_type*) m_dpacked, magIn, phaseIn, fft_size / 2 + 1);  // NOLINT

	fftw_execute (m_dplani);

#if ! FFTW_SINGLE_ONLY
	if (realOut != m_dbuf)
#endif
		vecops::copy (realOut, m_dbuf, fft_size);
}

void FFTW_DoubleFFT::inverseCepstral (const double* magIn, double* cepOut)
{
	for (auto i = 0; i <= fft_size / 2; ++i)
	{
		m_dpacked[i][0] = std::log (magIn[i] + shiftAmount<fft_double_type>);
		m_dpacked[i][1] = 0.;
	}

	fftw_execute (m_dplani);

#if ! FFTW_SINGLE_ONLY
	if (cepOut != m_dbuf)
#endif
		vecops::copy (cepOut, m_dbuf, fft_size);
}

int FFTW_DoubleFFT::m_extantd = 0;

}  // namespace limes::vecops


#undef fft_float_type
#undef fft_double_type

#if FFTW_DOUBLE_ONLY
#	undef fftwf_complex
#	undef fftwf_plan
#	undef fftwf_plan_dft_r2c_1d
#	undef fftwf_plan_dft_c2r_1d
#	undef fftwf_destroy_plan
#	undef fftwf_malloc
#	undef fftwf_free
#	undef fftwf_execute
#	undef atan2f
#	undef sqrtf
#	undef cosf
#	undef sinf
#endif /* FFTW_DOUBLE_ONLY */

#if FFTW_SINGLE_ONLY
#	undef fftw_complex
#	undef fftw_plan
#	undef fftw_plan_dft_r2c_1d
#	undef fftw_plan_dft_c2r_1d
#	undef fftw_destroy_plan
#	undef fftw_malloc
#	undef fftw_free
#	undef fftw_execute
#	undef atan2
#	undef sqrt
#	undef cos
#	undef sin
#endif /* FFTW_SINGLE_ONLY */
