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

#include "./fftw_fft.h"
#include <cmath>			 // for log, logf
#include "./fft_common.h"	 // for FFTImpl, shiftAmount
#include "./limes_fft.h"	 // for FFT, enableWisdom, getWisdomFileDir, isU...
#include <limes_vecops.h>	 // for copy, cartesianInterleavedToMagnitudes
#include <limes_platform.h>	 // for LIMES_FORCE_INLINE
#include <limes_namespace.h>
#include <limes_core.h>

LIMES_BEGIN_NAMESPACE

namespace vecops
{

#if FFTW_DOUBLE_ONLY
#	define fftwf_plan_dft_r2c_1d fftw_plan_dft_r2c_1d
#	define fftwf_plan_dft_c2r_1d fftw_plan_dft_c2r_1d
#	define fftwf_destroy_plan	  fftw_destroy_plan
#	define fftwf_malloc		  fftw_malloc
#	define fftwf_free			  fftw_free
#	define fftwf_execute		  fftw_execute
#	define fftwf_cleanup		  fftw_cleanup
#	define atan2f				  atan2
#	define sqrtf				  sqrt
#	define cosf				  cos
#	define sinf				  sin
#elif FFTW_SINGLE_ONLY
#	define fftw_plan_dft_r2c_1d fftwf_plan_dft_r2c_1d
#	define fftw_plan_dft_c2r_1d fftwf_plan_dft_c2r_1d
#	define fftw_destroy_plan	 fftwf_destroy_plan
#	define fftw_malloc			 fftwf_malloc
#	define fftw_free			 fftwf_free
#	define fftw_execute		 fftwf_execute
#	define fftw_cleanup		 fftwf_cleanup
#	define atan2				 atan2f
#	define sqrt				 sqrtf
#	define cos					 cosf
#	define sin					 sinf
#endif


template <Scalar SampleType, typename ComplexType>
LIMES_FORCE_INLINE static void fftw_pack (const SampleType* re, const SampleType* im,
										  ComplexType* m_packed, int fft_size) noexcept
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
LIMES_FORCE_INLINE static void fftw_unpack (SampleType* re, SampleType* im,
											ComplexType* m_packed, int fft_size) noexcept
{
	const auto hs = fft_size / 2;

	for (auto i = 0; i <= hs; ++i)
		re[i] = m_packed[i][0];

	if (im != nullptr)
		for (auto i = 0; i <= hs; ++i)
			im[i] = m_packed[i][1];
}


/*---------------------------------------------------------------------------------------------------------------------------*/

LIMES_DISABLE_ALL_COMPILER_WARNINGS

template <>
FFTW_FFT<float>::FFTW_FFT (int size)
	: FFTImpl<float> (size),
	  m_planf (fftwf_plan_dft_r2c_1d (fft_size, m_buf, m_packed, FFTW_ESTIMATE)),
	  m_plani (fftwf_plan_dft_c2r_1d (fft_size, m_packed, m_buf, FFTW_ESTIMATE)),
	  m_buf (reinterpret_cast<fft_float_type*> (fftw_malloc (static_cast<size_t> (fft_size) * sizeof (fft_float_type)))),
	  m_packed (reinterpret_cast<fftw_float_complex_type*> (fftw_malloc ((static_cast<size_t> (fft_size) / 2 + 1) * sizeof (fftw_float_complex_type))))
{
	init();
}

template <>
FFTW_FFT<double>::FFTW_FFT (int size)
	: FFTImpl<double> (size),
	  m_planf (fftw_plan_dft_r2c_1d (fft_size, m_buf, m_packed, FFTW_ESTIMATE)),
	  m_plani (fftw_plan_dft_c2r_1d (fft_size, m_packed, m_buf, FFTW_ESTIMATE)),
	  m_buf (reinterpret_cast<fft_double_type*> (fftw_malloc (static_cast<size_t> (fft_size) * sizeof (fft_double_type)))),
	  m_packed (reinterpret_cast<fftw_double_complex_type*> (fftw_malloc ((static_cast<size_t> (fft_size) / 2 + 1) * sizeof (fftw_double_complex_type))))
{
	init();
}

LIMES_REENABLE_ALL_COMPILER_WARNINGS

template <Scalar SampleType>
void FFTW_FFT<SampleType>::init()
{
	static_assert (fft::isUsingFFTW());

	if (m_extant == 0)
		fftw_load_wisdom<std::is_same_v<SampleType, double>>();

	++m_extant;
}

template <Scalar SampleType>
FFTW_FFT<SampleType>::~FFTW_FFT()
{
	if constexpr (std::is_same_v<SampleType, float>)
	{
		fftwf_destroy_plan (m_planf);
		fftwf_destroy_plan (m_plani);
		fftwf_free (m_buf);
		fftwf_free (m_packed);
	}
	else
	{
		fftw_destroy_plan (m_planf);
		fftw_destroy_plan (m_plani);
		fftw_free (m_buf);
		fftw_free (m_packed);
	}

	--m_extant;

	if (m_extant <= 0)
	{
		fftw_save_wisdom<std::is_same_v<SampleType, double>>();

		if constexpr (std::is_same_v<SampleType, float>)
			fftwf_cleanup();
		else
			fftw_cleanup();

		m_extant = 0;
	}
}

template <Scalar SampleType>
void FFTW_FFT<SampleType>::forward (const SampleType* realIn, SampleType* realOut, SampleType* imagOut) noexcept
{
	if (realIn != m_buf)
		vecops::copy (m_buf, realIn, this->fft_size);

	if constexpr (std::is_same_v<SampleType, float>)
		fftwf_execute (m_planf);
	else
		fftw_execute (m_planf);

	fftw_unpack (realOut, imagOut, m_packed, this->fft_size);
}

template <Scalar SampleType>
void FFTW_FFT<SampleType>::forwardInterleaved (const SampleType* realIn, SampleType* complexOut) noexcept
{
	if (realIn != m_buf)
		vecops::copy (m_buf, realIn, this->fft_size);

	if constexpr (std::is_same_v<SampleType, float>)
		fftwf_execute (m_planf);
	else
		fftw_execute (m_planf);

	vecops::copy (complexOut, reinterpret_cast<const SampleType*> (m_packed), this->fft_size + 2);
}

template <Scalar SampleType>
void FFTW_FFT<SampleType>::forwardPolar (const SampleType* realIn, SampleType* magOut, SampleType* phaseOut) noexcept
{
	if (realIn != m_buf)
		vecops::copy (m_buf, realIn, this->fft_size);

	if constexpr (std::is_same_v<SampleType, float>)
		fftwf_execute (m_planf);
	else
		fftw_execute (m_planf);

	vecops::catesianInterleavedToPolar (magOut, phaseOut, (const float_type*) m_packed, this->fft_size / 2 + 1);
}

template <Scalar SampleType>
void FFTW_FFT<SampleType>::forwardMagnitude (const SampleType* realIn, SampleType* magOut) noexcept
{
	if (realIn != m_buf)
		vecops::copy (m_buf, realIn, this->fft_size);

	if constexpr (std::is_same_v<SampleType, float>)
		fftwf_execute (m_planf);
	else
		fftw_execute (m_planf);

	vecops::cartesianInterleavedToMagnitudes (magOut, (const float_type*) m_packed, this->fft_size / 2 + 1);
}

template <Scalar SampleType>
void FFTW_FFT<SampleType>::inverse (const SampleType* realIn, const SampleType* imagIn, SampleType* realOut) noexcept
{
	fftw_pack (realIn, imagIn, m_packed, this->fft_size);

	if constexpr (std::is_same_v<SampleType, float>)
		fftwf_execute (m_plani);
	else
		fftw_execute (m_plani);

	if (realOut != m_buf)
		vecops::copy (realOut, m_buf, this->fft_size);
}

template <Scalar SampleType>
void FFTW_FFT<SampleType>::inverseInterleaved (const SampleType* complexIn, SampleType* realOut) noexcept
{
	vecops::copy (reinterpret_cast<SampleType*> (m_packed), complexIn, this->fft_size + 2);

	if constexpr (std::is_same_v<SampleType, float>)
		fftwf_execute (m_plani);
	else
		fftw_execute (m_plani);

	if (realOut != m_buf)
		vecops::copy (realOut, m_buf, this->fft_size);
}

template <Scalar SampleType>
void FFTW_FFT<SampleType>::inversePolar (const SampleType* magIn, const SampleType* phaseIn, SampleType* realOut) noexcept
{
	vecops::polarToCartesianInterleaved ((float_type*) m_packed, magIn, phaseIn, this->fft_size / 2 + 1);  // NOLINT

	if constexpr (std::is_same_v<SampleType, float>)
		fftwf_execute (m_plani);
	else
		fftw_execute (m_plani);

	if (realOut != m_buf)
		vecops::copy (realOut, m_buf, this->fft_size);
}

template <Scalar SampleType>
void FFTW_FFT<SampleType>::inverseCepstral (const SampleType* magIn, SampleType* cepOut) noexcept
{
	for (auto i = 0; i <= this->fft_size / 2; ++i)
	{
		m_packed[i][0] = std::log (magIn[i] + shiftAmount<float_type>);
		m_packed[i][1] = SampleType (0.);
	}

	if constexpr (std::is_same_v<SampleType, float>)
		fftwf_execute (m_plani);
	else
		fftw_execute (m_plani);

	if (cepOut != m_buf)
		vecops::copy (cepOut, m_buf, this->fft_size);
}

template <>
int FFTW_FFT<float>::m_extant = 0;	// NOLINT

template <>
int FFTW_FFT<double>::m_extant = 0;	 // NOLINT

template class FFTW_FFT<float>;
template class FFTW_FFT<double>;

}  // namespace vecops

LIMES_END_NAMESPACE

#if FFTW_DOUBLE_ONLY
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
#elif FFTW_SINGLE_ONLY
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
#endif
