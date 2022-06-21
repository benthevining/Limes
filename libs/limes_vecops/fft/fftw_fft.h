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

#if defined(DOXYGEN) || ! defined(FFTW_HEADER_NAME)
/** @def FFTW_HEADER_NAME
	The name of the header that should be included for the FFTW library.
	This may differ depending on if only one precision is available.
	It defaults to \c \<fftw3.h> .
	@ingroup fftw
 */
#	define FFTW_HEADER_NAME <fftw3.h>
#endif

#include FFTW_HEADER_NAME

#if defined(DOXYGEN) || ! defined(FFTW_SINGLE_ONLY)
/** @def FFTW_SINGLE_ONLY
	Define this to 1 if only the float version of the FFTW library is available.
	To perform FFTs on double data, the data will first be converted to floats before calling the FFTW float library's functions.
	@see FFTW_DOUBLE_ONLY
	@ingroup fftw
 */
#	define FFTW_SINGLE_ONLY 0
#endif

#if defined(DOXYGEN) || ! defined(FFTW_DOUBLE_ONLY)
/** @def FFTW_DOUBLE_ONLY
	Define this to 1 if only the double version of the FFTW library is available.
	To perform FFTs on float data, the data will first be converted to doubles before calling the FFTW double library's functions.
	@see FFTW_SINGLE_ONLY
	@ingroup fftw
 */
#	define FFTW_DOUBLE_ONLY 0
#endif

#if (FFTW_SINGLE_ONLY && FFTW_DOUBLE_ONLY)
#	error FFTW_SINGLE_ONLY and FFTW_DOUBLE_ONLY cannot both be defined to 1!
#endif

LIMES_BEGIN_NAMESPACE

namespace vecops
{

/// @cond internals

/** Saves the current wisdom data to a file.
	The directory in which the files are saved can be set using the function \c fftw::setWisdomFileDir() or the environment variable \c FFTW_WISDOM_FILE_DIR .
	If neither of these options have been specified, this function will attempt to look up the \c HOME environment variable, and if that is set, will use that directory.
	Within the chosen directory, the files will be named \c .fftw_wisdom.d (for the double version) and \c .fftw_wisdom.f (for the float version).
	@see fftw_load_wisdom(), fftw::getWisdomFileDir()
	@ingroup fftw
 */
template <bool IsDouble>
LIMES_NO_EXPORT void fftw_save_wisdom();

/** Loads a previously saved wisdom file.
	The directory in which the files are saved can be set using the function \c fftw::setWisdomFileDir() or the environment variable \c FFTW_WISDOM_FILE_DIR .
	If neither of these options have been specified, this function will attempt to look up the \c HOME environment variable, and if that is set, will use that directory.
	Within the chosen directory, the files will be named \c .fftw_wisdom.d (for the double version) and \c .fftw_wisdom.f (for the float version).
	@see fftw_save_wisdom(), fftw::getWisdomFileDir()
	@ingroup fftw
 */
template <bool IsDouble>
LIMES_NO_EXPORT void fftw_load_wisdom();

/** This class implements an FFT using the FFTW library.

	The behavior of this class can be altered in several ways:

	@par Preprocessor macros
	\c FFTW_SINGLE_ONLY or \c FFTW_DOUBLE_ONLY can be defined if only one precision of the FFTW library is available.
	In this case, the data will first be converted, if necessary, before performing the FFT. This is not desirable for performance. \n
	\c FFTW_HEADER_NAME can be defined to specify the exact name of the header that should be included.
	This may need to be changed if only one precision of the library is available.
	It defaults to \c \<fftw3.h> .

	@par Wisdom files
	This library provides a way to control the usage of FFTW Wisdom files from outside this class.
	The interface to wisdom is a set of free functions that always exist, but simply do nothing if the FFTW backend isn't being used.
	See the fftw namespace for more information.

	@ingroup fftw
 */
template <Scalar SampleType>
class LIMES_NO_EXPORT FFTW_FFT final : public FFTImpl<SampleType>
{
public:

	explicit FFTW_FFT (int size);

	~FFTW_FFT() final;

	LIMES_NON_COPYABLE (FFTW_FFT)
	LIMES_DEFAULT_MOVABLE (FFTW_FFT)

private:

	void forward (const SampleType* realIn, SampleType* realOut, SampleType* imagOut) noexcept final;

	void forwardInterleaved (const SampleType* realIn, SampleType* complexOut) noexcept final;

	void forwardPolar (const SampleType* realIn, SampleType* magOut, SampleType* phaseOut) noexcept final;

	void forwardMagnitude (const SampleType* realIn, SampleType* magOut) noexcept final;

	void inverse (const SampleType* realIn, const SampleType* imagIn, SampleType* realOut) noexcept final;

	void inverseInterleaved (const SampleType* complexIn, SampleType* realOut) noexcept final;

	void inversePolar (const SampleType* magIn, const SampleType* phaseIn, SampleType* realOut) noexcept final;

	void inverseCepstral (const SampleType* magIn, SampleType* cepOut) noexcept final;

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
