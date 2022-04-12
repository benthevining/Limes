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

#pragma once

#include "fft_common.h"
#include <limes_export.h>


namespace limes::vecops
{

class LIMES_NO_EXPORT vDSP_FloatFFT final : public FFTImpl<float>
{
public:

	explicit vDSP_FloatFFT (int size);

	~vDSP_FloatFFT() final;

	vDSP_FloatFFT (const vDSP_FloatFFT&) = delete;

	vDSP_FloatFFT& operator= (const vDSP_FloatFFT&) = delete;

	vDSP_FloatFFT (vDSP_FloatFFT&&) = delete;

	vDSP_FloatFFT& operator= (vDSP_FloatFFT&&) = delete;

private:

	void forward (const float* realIn, float* realOut, float* imagOut) final;

	void forwardInterleaved (const float* realIn, float* complexOut) final;

	void forwardPolar (const float* realIn, float* magOut, float* phaseOut) final;

	void forwardMagnitude (const float* realIn, float* magOut) final;

	void inverse (const float* realIn, const float* imagIn, float* realOut) final;

	void inverseInterleaved (const float* complexIn, float* realOut) final;

	void inversePolar (const float* magIn, const float* phaseIn, float* realOut) final;

	void inverseCepstral (const float* magIn, float* cepOut) final;

	LIMES_FORCE_INLINE void packReal (const float* const re);

	LIMES_FORCE_INLINE void unpackReal (float* const re);

	LIMES_FORCE_INLINE void unpackComplex (float* const re, float* const im);

	FFTSetup		m_spec { vDSP_create_fftsetup (m_order, FFT_RADIX2) };
	float*			m_spare;
	float*			m_spare2;
	DSPSplitComplex m_buf, m_packed;
};


class LIMES_NO_EXPORT vDSP_DoubleFFT final : public FFTImpl<double>
{
public:

	explicit vDSP_DoubleFFT (int size);

	~vDSP_DoubleFFT() final;

	vDSP_DoubleFFT (const vDSP_DoubleFFT&) = delete;

	vDSP_DoubleFFT& operator= (const vDSP_DoubleFFT&) = delete;

	vDSP_DoubleFFT (vDSP_DoubleFFT&&) = delete;

	vDSP_DoubleFFT& operator= (vDSP_DoubleFFT&&) = delete;

private:

	void forward (const double* realIn, double* realOut, double* imagOut) final;

	void forwardInterleaved (const double* realIn, double* complexOut) final;

	void forwardPolar (const double* realIn, double* magOut, double* phaseOut) final;

	void forwardMagnitude (const double* realIn, double* magOut) final;

	void inverse (const double* realIn, const double* imagIn, double* realOut) final;

	void inverseInterleaved (const double* complexIn, double* realOut) final;

	void inversePolar (const double* magIn, const double* phaseIn, double* realOut) final;

	void inverseCepstral (const double* magIn, double* cepOut) final;

	LIMES_FORCE_INLINE void packReal (const double* const re);

	LIMES_FORCE_INLINE void unpackReal (double* const re);

	LIMES_FORCE_INLINE void unpackComplex (double* const re, double* const im);

	FFTSetupD			  m_spec { vDSP_create_fftsetupD (m_order, FFT_RADIX2) };
	double*				  m_spare;
	double*				  m_spare2;
	DSPDoubleSplitComplex m_buf, m_packed;
};

}  // namespace limes::vecops
