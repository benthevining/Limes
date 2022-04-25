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
#include <limes_namespace.h>
#include <limes_core.h>

LIMES_BEGIN_NAMESPACE

namespace vecops
{

template <Scalar SampleType>
class LIMES_NO_EXPORT FallbackFFT final : public FFTImpl<SampleType>
{
public:

	explicit FallbackFFT (int size);

	FallbackFFT (const FallbackFFT&) = delete;

	FallbackFFT& operator= (const FallbackFFT&) = delete;

	FallbackFFT (FallbackFFT&&) = delete;

	FallbackFFT& operator= (FallbackFFT&&) = delete;

private:

	void forward (const SampleType* realIn, SampleType* realOut, SampleType* imagOut) final;

	void forwardInterleaved (const SampleType* realIn, SampleType* complexOut) final;

	void forwardPolar (const SampleType* realIn, SampleType* magOut, SampleType* phaseOut) final;

	void forwardMagnitude (const SampleType* realIn, SampleType* magOut) final;

	void inverse (const SampleType* realIn, const SampleType* imagIn, SampleType* realOut) final;

	void inverseInterleaved (const SampleType* complexIn, SampleType* realOut) final;

	void inversePolar (const SampleType* magIn, const SampleType* phaseIn, SampleType* realOut) final;

	void inverseCepstral (const SampleType* magIn, SampleType* cepOut) final;

	// Uses m_a and m_b internally; does not touch m_c or m_d
	LIMES_FORCE_INLINE void transformF (const SampleType* ri,
										double* ro, double* io);

	// Uses m_c and m_d internally; does not touch m_a or m_b
	LIMES_FORCE_INLINE void transformI (const double* ri, const double* ii,
										SampleType* ro);

	LIMES_FORCE_INLINE void transformComplex (const double* ri, const double* ii,
											  double* ro, double* io,
											  bool inverse);

	static constexpr auto m_blockTableSize = 16;
	static constexpr auto m_maxTabledBlock = 1 << m_blockTableSize;

	const int m_half { this->fft_size / 2 };

	aligned_pointer<int> m_table;

	aligned_pointer<double> m_sincos, m_sincos_r, m_vr, m_vi, m_a, m_b, m_c, m_d;

	double* m_a_and_b[2] { m_a, m_b };
	double* m_c_and_d[2] { m_c, m_d };
};

}  // namespace vecops

LIMES_END_NAMESPACE
