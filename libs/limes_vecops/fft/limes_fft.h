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

#include <memory>		   // for unique_ptr
#include <limes_export.h>  // for LIMES_EXPORT, LIMES_NO_EXPORT
#include <string_view>	   // for string_view
#include <limes_namespace.h>
#include <limes_core.h>
#include "../vecops/vecops.h"
#include "../vecops/vecops_macros.h"

/** @defgroup fft FFT
	Fourier transform utilities.

	The FFT class can use the following backends:
	- FFTW
	- Intel IPP
	- Apple vDSP
	- Fallback

	@ingroup limes_vecops

	@dependency \b FFTW
	<a href="https://www.fftw.org/">The FFTW Fourier transform library</a> can be used as the backend of the @ref limes_vecops "limes_vecops" FFT class.
	FFTW can be installed to your system with a simple @verbatim cmake --install @endverbatim of their git repository.
	The usage of FFTW is determined by the \c LIMES_USE_FFTW @ref cmakeopt "CMake variable".

	@cmakeopt \b LIMES_USE_FFTW
	By default, <a href="https://www.fftw.org/">FFTW</a> will be searched for in the system, and if found,
	will be used for the @ref limes_vecops "limes_vecops" FFT class's backend. However, you can set this option to \c OFF to ignore FFTW.
	vDSP and IPP also provide FFT implementations.

	@cmakeprop \b LIMES_FFT_IMPLEMENTATION
	String name of the FFT backend being used for the @ref lib_limes_vecops "limes_vecops" library.
 */

/** @defgroup fftw FFTW
	Classes and functions related to integrating the FFTW library.
	@ingroup fft
 */

/** @dir libs/limes_vecops/fft
	This directory contains the Limes FFT implementation.
	@ingroup fft
 */

/** @file
	This file defines the interface for the Limes FFT.
	@ingroup fft
 */

#ifdef DOXYGEN
/** @def LIMES_VECOPS_USE_FFTW
	1 if the FFTW library is being used, otherwise 0.
	@ingroup fftw
 */
#	define LIMES_VECOPS_USE_FFTW 0
#endif

/// @cond
#ifndef LIMES_VECOPS_USE_FFTW
#	if LIMES_HAS_INCLUDE(<fftw3.h>)
#		define LIMES_VECOPS_USE_FFTW 1	 // NOLINT
#	else
#		define LIMES_VECOPS_USE_FFTW 0	 // NOLINT
#	endif
#endif
/// @endcond

LIMES_BEGIN_NAMESPACE

namespace vecops
{

/** This namespace contains Fourier transform utilities.
	@ingroup fft
 */
namespace fft
{

#pragma mark Implementation kind checking

/** @ingroup fft
	@{
 */

/** Returns true if the FFTW FFT implementation is being used. */
[[nodiscard]] LIMES_PURE_FUNCTION consteval bool isUsingFFTW() noexcept
{
#if LIMES_VECOPS_USE_FFTW
	return true;
#else
	return false;
#endif
}

/** Returns true if the Apple vDSP FFT implementation is being used. */
[[nodiscard]] LIMES_PURE_FUNCTION consteval bool isUsingVDSP() noexcept
{
	return vecops::isUsingVDSP() && ! isUsingFFTW();  // cppcheck-suppress knownConditionTrueFalse
}

/** Returns true if the Intel IPP FFT implementation is being used. */
[[nodiscard]] LIMES_PURE_FUNCTION consteval bool isUsingIPP() noexcept
{
	return vecops::isUsingIPP() && ! isUsingFFTW();	 // cppcheck-suppress knownConditionTrueFalse
}

/** Returns true if the NE10 implementation is being used. */
[[nodiscard]] LIMES_PURE_FUNCTION consteval bool isUsingNE10() noexcept
{
	return vecops::isUsingNE10() && ! isUsingFFTW();  // cppcheck-suppress knownConditionTrueFalse
}

/** Returns true if the fallback FFT implementation is being used. */
[[nodiscard]] LIMES_PURE_FUNCTION consteval bool isUsingFallback() noexcept
{
	return ! (isUsingFFTW() || isUsingVDSP() || isUsingIPP() || isUsingNE10());	 // cppcheck-suppress knownConditionTrueFalse
}

static_assert (isUsingFFTW() || isUsingVDSP() || isUsingIPP() || isUsingNE10() || isUsingFallback());

/** Returns a string literal with the name of the FFT implementation being used. */
[[nodiscard]] LIMES_PURE_FUNCTION static consteval const char* getImplementationName() noexcept
{
	if constexpr (isUsingFFTW())
		return "FFTW";
	else if constexpr (isUsingVDSP())
		return "Apple vDSP";
	else if constexpr (isUsingIPP())
		return "Intel IPP";
	else if constexpr (isUsingNE10())
		return "NE10";
	else
		return "Fallback";
}

/// @cond internals
template <Scalar SampleType>
class FFTImpl;
/// @endcond

/** @} */

}  // namespace fft

#pragma mark FFT class

/** A class that performs an FFT.

	Only power-of-two FFT lengths are supported.
	On the time domain side, only real signals are handled.

	@section fft_backend The FFT backend

	Several different FFT libraries can be used as the backend for this class, and there is also a built-in "plain C++" backend available.

	If FFTW is available, it will be used as the backend for this class.
	Otherwise, the same backend being used for the limes_vecops functions will be used -- vDSP, IPP, or the fallback.

	FFTW can be explicitly enabled or disabled using the \c LIMES_VECOPS_USE_FFTW preprocessor macro.
	You can also set either \c FFTW_SINGLE_ONLY or \c FFTW_DOUBLE_ONLY to 1 if only one precision of the FFTW library is available.
	In this case, FFTs being performed with the other datatype will be converted before being processed.
	You can also define the \c FFTW_HEADER_NAME macro to the name of the header that should be included (including the surrounding \c " or \c \< characters).
	It defaults to \c \<fftw3.h> .

	@ingroup fft

	@todo prepare()/releaseResources() ?
	@todo pffft backend
 */
template <Scalar SampleType>
class LIMES_EXPORT FFT final
{
public:
	/** Creates an FFT engine with a specified FFT size.
		Only power-of-two FFT lengths are supported by all backends.

		@throws std::runtime_error An exception will be thrown if you construct an FFT object with a size that is not a power of 2.
	 */
	explicit FFT (int size);

	LIMES_NON_COPYABLE (FFT)
	LIMES_DEFAULT_MOVABLE (FFT)

	~FFT() = default;

	/** Returns the FFT size of this engine. */
	[[nodiscard]] int getSize() const noexcept;

	/** Changes the size of the FFT.
		This will cause memory allocation to occur.

		@throws std::runtime_error An exception will be thrown if you pass a size that is not a power of 2.
	 */
	void changeSize (int newFFTSize);

	/** Resets the internal state of the FFT engine. */
	void reset();

	/** @name Forward
		Perform forward FFTs.
	 */
	///@{

	/** Performs a forward FFT, outputting cartesian data. */
	void forward (const SampleType* realIn, SampleType* realOut, SampleType* imagOut) noexcept;

	/** Performs a forward FFT, outputting interleaved cartesian data. */
	void forwardInterleaved (const SampleType* realIn, SampleType* complexOut) noexcept;

	/** Performs a forward FFT, outputting polar data. */
	void forwardPolar (const SampleType* realIn, SampleType* magOut, SampleType* phaseOut) noexcept;

	/** Performs a forward FFT, outputting polar magnitudes. */
	void forwardMagnitude (const SampleType* realIn, SampleType* magOut) noexcept;

	///@}


	/** @name Inverse
		Perform inverse FFTs.
	 */
	///@{

	/** Performs an inverse FFT. */
	void inverse (const SampleType* realIn, const SampleType* imagIn, SampleType* realOut) noexcept;

	/** Performs an inverse FFT, operating on interleaved input data. */
	void inverseInterleaved (const SampleType* complexIn, SampleType* realOut) noexcept;

	/** Performs an inverse FFT, operating on polar input data. */
	void inversePolar (const SampleType* magIn, const SampleType* phaseIn, SampleType* realOut) noexcept;

	/** Performs an inverse FFT, outputting cepstral data. */
	void inverseCepstral (const SampleType* magIn, SampleType* cepOut) noexcept;

	///@}


	/** Returns the frequency for a given bin number in the output data.
		An assertion will be thrown if the given bin number is larger than the FFT size of this engine.
	 */
	[[nodiscard]] double getFrequencyForBin (int binNumber, double samplerate) const noexcept;

	/** Returns the bin number for the given frequency in the output data.
		An assertion will be thrown if the resulting bin number is larger than the FFT size of this engine.
	 */
	[[nodiscard]] int getBinForFrequency (double frequency, double samplerate) const noexcept;

private:
	std::unique_ptr<fft::FFTImpl<SampleType>> pimpl;
};

#pragma mark FFTW wisdom

/** This namespace contains functions for controlling FFTW wisdom.
	These functions always exist, but simply do nothing when the FFTW backend is not being used.
	@ingroup fftw
 */
namespace fftw
{

/** @ingroup fftw
	@{
 */

/** Sets the directory that FFTW wisdom files will be read from and saved to.
	The actual filenames will be \c .fftw_wisdom.d (double precision) and \c .fftw_wisdom.f (float precision), within this directory.
	@note To load wisdom files from a specified directory, this function should be called before the first FFT object is created.

	@envvar @b FFTW_WISDOM_FILE_DIR This environment variable may be set to the path to a directory where FFTW wisdom files will be saved to and loaded from.
	Note that this variable is only used as a fallback; calling the \c setWisdomFileDir() function will override this variable's value.

	@envvar @b HOME If no FFTW wisdom file directory has been explicitly set by calling the \c setWisdomFileDir() function, and the \c FFTW_WISDOM_FILE_DIR
	environment variable is also not set, then the value of \c HOME will be used as the directory to store and load wisdom files.

	@see getWisdomFileDir
 */
LIMES_EXPORT bool setWisdomFileDir (const files::Directory& directory);

/** Returns the directory being used to save and load FFTW wisdom files.
	The actual filenames will be \c .fftw_wisdom.d (double precision) and \c .fftw_wisdom.f (float precision), within this directory.
	@see setWisdomFileDir
 */
LIMES_EXPORT [[nodiscard]] files::Directory getWisdomFileDir();

/** Enables or disables loading and saving of FFTW wisdom files.
	@note This function should be called before the FFT object is created.
	@see isUsingWisdom
 */
LIMES_EXPORT void enableWisdom (bool shouldUseWisdom);

/** Returns true if loading and saving of FFTW wisdom files is enabled.
	@see enableWisdom
 */
LIMES_EXPORT [[nodiscard]] bool isUsingWisdom();

/** @} */

}  // namespace fftw

}  // namespace vecops

LIMES_END_NAMESPACE
