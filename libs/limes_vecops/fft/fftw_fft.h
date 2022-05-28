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
#include <limes_core.h>

/** @file
	This file defines the FFTW FFT implementation.
	@ingroup fft
	@see limes_fft.h
 */

/// @cond internals
#if defined(DOXYGEN) || ! defined(FFTW_HEADER_NAME)
#	define FFTW_HEADER_NAME <fftw3.h>
#endif
/// @endcond

/// @cond

#include FFTW_HEADER_NAME

#if defined(DOXYGEN) || ! defined(FFTW_SINGLE_ONLY)
#	define FFTW_SINGLE_ONLY 0
#endif

#if defined(DOXYGEN) || ! defined(FFTW_DOUBLE_ONLY)
#	define FFTW_DOUBLE_ONLY 0
#endif

#if (FFTW_SINGLE_ONLY && FFTW_DOUBLE_ONLY)
#	error FFTW_SINGLE_ONLY and FFTW_DOUBLE_ONLY cannot both be defined to 1!
#endif

/// @endcond

LIMES_BEGIN_NAMESPACE

namespace vecops
{

/// @cond internals

template <bool IsDouble>
LIMES_NO_EXPORT void fftw_save_wisdom();

template <bool IsDouble>
LIMES_NO_EXPORT void fftw_load_wisdom();


template <Scalar SampleType>
class LIMES_NO_EXPORT FFTW_FFT final : public FFTImpl<SampleType>
{
public:

	explicit FFTW_FFT (int size);

	~FFTW_FFT() final;

	LIMES_NON_COPYABLE (FFTW_FFT);
	LIMES_DEFAULT_MOVABLE (FFTW_FFT);

private:

	void forward (const SampleType* realIn, SampleType* realOut, SampleType* imagOut) final;

	void forwardInterleaved (const SampleType* realIn, SampleType* complexOut) final;

	void forwardPolar (const SampleType* realIn, SampleType* magOut, SampleType* phaseOut) final;

	void forwardMagnitude (const SampleType* realIn, SampleType* magOut) final;

	void inverse (const SampleType* realIn, const SampleType* imagIn, SampleType* realOut) final;

	void inverseInterleaved (const SampleType* complexIn, SampleType* realOut) final;

	void inversePolar (const SampleType* magIn, const SampleType* phaseIn, SampleType* realOut) final;

	void inverseCepstral (const SampleType* magIn, SampleType* cepOut) final;

	void init();

	using fft_float_type		  = std::conditional_t<FFTW_DOUBLE_ONLY, double, float>;
	using fftw_float_plan		  = std::conditional_t<FFTW_DOUBLE_ONLY, fftw_plan, fftwf_plan>;
	using fftw_float_complex_type = std::conditional_t<FFTW_DOUBLE_ONLY, fftw_complex, fftwf_complex>;

	using fft_double_type		   = std::conditional_t<FFTW_SINGLE_ONLY, float, double>;
	using fftw_double_plan		   = std::conditional_t<FFTW_SINGLE_ONLY, fftwf_plan, fftw_plan>;
	using fftw_double_complex_type = std::conditional_t<FFTW_SINGLE_ONLY, fftwf_complex, fftw_complex>;

	using plan_type = std::conditional_t<std::is_same_v<SampleType, float>,
										 fftw_float_plan,
										 fftw_double_plan>;

	using float_type = std::conditional_t<std::is_same_v<SampleType, float>,
										  fft_float_type,
										  fft_double_type>;

	using complex_type = std::conditional_t<std::is_same_v<SampleType, float>,
											fftw_float_complex_type,
											fftw_double_complex_type>;

	plan_type m_planf, m_plani;

	float_type* m_buf;

	complex_type* m_packed;

	static int m_extant;
};

/// @endcond

}  // namespace vecops

LIMES_END_NAMESPACE
