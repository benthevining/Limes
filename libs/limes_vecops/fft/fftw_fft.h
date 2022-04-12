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

#include "fft_common.h"
#include <limes_export.h>


#ifndef FFTW_HEADER_NAME
#	define FFTW_HEADER_NAME <fftw3.h>
#endif

#include FFTW_HEADER_NAME

#ifndef FFTW_SINGLE_ONLY
#	define FFTW_SINGLE_ONLY 0
#endif

#ifndef FFTW_DOUBLE_ONLY
#	define FFTW_DOUBLE_ONLY 0
#endif

#if (FFTW_SINGLE_ONLY && FFTW_DOUBLE_ONLY)
#	error FFTW_SINGLE_ONLY and FFTW_DOUBLE_ONLY cannot both be defined to 1!
#endif

/* All these symbols are undefined at the end of the fftw_fft.cpp file */
#if FFTW_DOUBLE_ONLY
#	define fft_float_type		  double
#	define fftwf_complex		  fftw_complex
#	define fftwf_plan			  fftw_plan
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
#else
#	define fft_float_type float
#endif /* FFTW_DOUBLE_ONLY */

#if FFTW_SINGLE_ONLY
#	define fft_double_type		 float
#	define fftw_complex		 fftwf_complex
#	define fftw_plan			 fftwf_plan
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
#else
#	define fft_double_type double
#endif /* FFTW_SINGLE_ONLY */


namespace limes::vecops
{

class LIMES_NO_EXPORT FFTW_FloatFFT final : public FFTImpl<float>
{
public:

	explicit FFTW_FloatFFT (int size);

	~FFTW_FloatFFT() final;

	FFTW_FloatFFT (const FFTW_FloatFFT&) = delete;

	FFTW_FloatFFT& operator= (const FFTW_FloatFFT&) = delete;

	FFTW_FloatFFT (FFTW_FloatFFT&&) = delete;

	FFTW_FloatFFT& operator= (FFTW_FloatFFT&&) = delete;

private:

	void forward (const float* realIn, float* realOut, float* imagOut) final;

	void forwardInterleaved (const float* realIn, float* complexOut) final;

	void forwardPolar (const float* realIn, float* magOut, float* phaseOut) final;

	void forwardMagnitude (const float* realIn, float* magOut) final;

	void inverse (const float* realIn, const float* imagIn, float* realOut) final;

	void inverseInterleaved (const float* complexIn, float* realOut) final;

	void inversePolar (const float* magIn, const float* phaseIn, float* realOut) final;

	void inverseCepstral (const float* magIn, float* cepOut) final;

	fftwf_plan m_fplanf;
	fftwf_plan m_fplani;

	fft_float_type* m_fbuf;

	fftwf_complex* m_fpacked;

	static int m_extantf;
};


class LIMES_NO_EXPORT FFTW_DoubleFFT final : public FFTImpl<double>
{
public:

	explicit FFTW_DoubleFFT (int size);

	~FFTW_DoubleFFT() final;

	FFTW_DoubleFFT (const FFTW_DoubleFFT&) = delete;

	FFTW_DoubleFFT& operator= (const FFTW_DoubleFFT&) = delete;

	FFTW_DoubleFFT (FFTW_DoubleFFT&&) = delete;

	FFTW_DoubleFFT& operator= (FFTW_DoubleFFT&&) = delete;

private:

	void forward (const double* realIn, double* realOut, double* imagOut) final;

	void forwardInterleaved (const double* realIn, double* complexOut) final;

	void forwardPolar (const double* realIn, double* magOut, double* phaseOut) final;

	void forwardMagnitude (const double* realIn, double* magOut) final;

	void inverse (const double* realIn, const double* imagIn, double* realOut) final;

	void inverseInterleaved (const double* complexIn, double* realOut) final;

	void inversePolar (const double* magIn, const double* phaseIn, double* realOut) final;

	void inverseCepstral (const double* magIn, double* cepOut) final;

	fftw_plan m_dplanf;
	fftw_plan m_dplani;

	fft_double_type* m_dbuf;

	fftw_complex* m_dpacked;

	static int m_extantd;
};

}  // namespace limes::vecops
