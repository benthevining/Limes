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

class LIMES_NO_EXPORT IPP_FloatFFT final : public FFTImpl<float>
{
public:

	explicit IPP_FloatFFT (int size);

	~IPP_FloatFFT() final;

	IPP_FloatFFT (const IPP_FloatFFT&) = delete;

	IPP_FloatFFT& operator= (const IPP_FloatFFT&) = delete;

	IPP_FloatFFT (IPP_FloatFFT&&) = delete;

	IPP_FloatFFT& operator= (IPP_FloatFFT&&) = delete;

private:

	void forward (const float* realIn, float* realOut, float* imagOut) final;

	void forwardInterleaved (const float* realIn, float* complexOut) final;

	void forwardPolar (const float* realIn, float* magOut, float* phaseOut) final;

	void forwardMagnitude (const float* realIn, float* magOut) final;

	void inverse (const float* realIn, const float* imagIn, float* realOut) final;

	void inverseInterleaved (const float* complexIn, float* realOut) final;

	void inversePolar (const float* magIn, const float* phaseIn, float* realOut) final;

	void inverseCepstral (const float* magIn, float* cepOut) final;

	IppsFFTSpec_R_32f* m_spec;
	Ipp8u*			   m_specbuf;
	Ipp8u*			   m_buf;
	float*			   m_packed;
	float*			   m_spare;
};


class LIMES_NO_EXPORT IPP_DoubleFFT final : public FFTImpl<double>
{
public:

	explicit IPP_DoubleFFT (int size);

	~IPP_DoubleFFT() final;

	IPP_DoubleFFT (const IPP_DoubleFFT&) = delete;

	IPP_DoubleFFT& operator= (const IPP_DoubleFFT&) = delete;

	IPP_DoubleFFT (IPP_DoubleFFT&&) = delete;

	IPP_DoubleFFT& operator= (IPP_DoubleFFT&&) = delete;

private:

	void forward (const double* realIn, double* realOut, double* imagOut) final;

	void forwardInterleaved (const double* realIn, double* complexOut) final;

	void forwardPolar (const double* realIn, double* magOut, double* phaseOut) final;

	void forwardMagnitude (const double* realIn, double* magOut) final;

	void inverse (const double* realIn, const double* imagIn, double* realOut) final;

	void inverseInterleaved (const double* complexIn, double* realOut) final;

	void inversePolar (const double* magIn, const double* phaseIn, double* realOut) final;

	void inverseCepstral (const double* magIn, double* cepOut) final;

	IppsFFTSpec_R_64f* m_spec;
	Ipp8u*			   m_specbuf;
	Ipp8u*			   m_buf;
	double*			   m_packed;
	double*			   m_spare;
};

}  // namespace limes::vecops
