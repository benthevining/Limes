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

#include <limes_export.h>  // for LIMES_NO_EXPORT
#include <type_traits>	   // for conditional_t
#include "fft_common.h"	   // for FFTImpl
#include <limes_namespace.h>

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

LIMES_BEGIN_NAMESPACE

namespace vecops
{

using fft_float_type		  = std::conditional_t<FFTW_DOUBLE_ONLY, double, float>;
using fftw_float_plan		  = std::conditional_t<FFTW_DOUBLE_ONLY, fftw_plan, fftwf_plan>;
using fftw_float_complex_type = std::conditional_t<FFTW_DOUBLE_ONLY, fftw_complex, fftwf_complex>;

using fft_double_type		   = std::conditional_t<FFTW_SINGLE_ONLY, float, double>;
using fftw_double_plan		   = std::conditional_t<FFTW_SINGLE_ONLY, fftwf_plan, fftw_plan>;
using fftw_double_complex_type = std::conditional_t<FFTW_SINGLE_ONLY, fftwf_complex, fftw_complex>;


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

	fftw_float_plan m_fplanf;
	fftw_float_plan m_fplani;

	fft_float_type* m_fbuf;

	fftw_float_complex_type* m_fpacked;

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

	fftw_double_plan m_dplanf;
	fftw_double_plan m_dplani;

	fft_double_type* m_dbuf;

	fftw_double_complex_type* m_dpacked;

	static int m_extantd;
};

}  // namespace vecops

LIMES_END_NAMESPACE
