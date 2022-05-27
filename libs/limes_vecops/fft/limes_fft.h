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
#include <limes_vecops.h>  // for concept Scalar
#include <limes_namespace.h>
#include <filesystem>
#include <limes_core.h>

/** @defgroup fft FFT
	Fourier transform utilities.
	@ingroup limes_vecops
 */

/** @file
	This file defines the interface for the Limes FFT.
	@ingroup fft
 */

#ifdef DOXYGEN
/** @def LIMES_VECOPS_USE_FFTW
	1 if the FFTW library is being used, otherwise 0.
	@ingroup fft
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

/** Returns true if the fallback FFT implementation is being used. */
[[nodiscard]] LIMES_PURE_FUNCTION consteval bool isUsingFallback() noexcept
{
	return ! (isUsingFFTW() || isUsingVDSP() || isUsingIPP());	// cppcheck-suppress knownConditionTrueFalse
}

static_assert (isUsingFFTW() || isUsingVDSP() || isUsingIPP() || isUsingFallback());

/** Returns a string literal with the name of the FFT implementation being used. */
[[nodiscard]] LIMES_PURE_FUNCTION static consteval const char* getImplementationName() noexcept
{
	if constexpr (isUsingFFTW())
		return "FFTW";
	else if constexpr (isUsingVDSP())
		return "Apple vDSP";
	else if constexpr (isUsingIPP())
		return "Intel IPP";
	else
		return "Fallback";
}

/** @} */

}  // namespace fft

/// @cond internals
/** An implementation class for FFT.
	@ingroup fft
 */
template <Scalar SampleType>
class FFTImpl;
/// @endcond

/** A class that performs an FFT.
	@ingroup fft
 */
template <Scalar SampleType>
class LIMES_EXPORT FFT final
{
public:

	/** Creates an FFT engine with a specified FFT size. */
	explicit FFT (int size);

	LIMES_NON_COPYABLE (FFT);
	LIMES_DEFAULT_MOVABLE (FFT);

	/** Returns the FFT size of this engine. */
	[[nodiscard]] int getSize() const noexcept;

	/** @name Forward
		Perform forward FFTs.
	 */
	///@{

	/** Performs a forward FFT, outputting cartesian data. */
	void forward (const SampleType* realIn, SampleType* realOut, SampleType* imagOut);

	/** Performs a forward FFT, outputting interleaved cartesian data. */
	void forwardInterleaved (const SampleType* realIn, SampleType* complexOut);

	/** Performs a forward FFT, outputting polar data. */
	void forwardPolar (const SampleType* realIn, SampleType* magOut, SampleType* phaseOut);

	/** Performs a forward FFT, outputting polar magnitudes. */
	void forwardMagnitude (const SampleType* realIn, SampleType* magOut);

	///@}

	/** @name Inverse
		Perform inverse FFTs.
	 */
	///@{

	/** Performs an inverse FFT. */
	void inverse (const SampleType* realIn, const SampleType* imagIn, SampleType* realOut);

	/** Performs an inverse FFT, operating on interleaved input data. */
	void inverseInterleaved (const SampleType* complexIn, SampleType* realOut);

	/** Performs an inverse FFT, operating on polar input data. */
	void inversePolar (const SampleType* magIn, const SampleType* phaseIn, SampleType* realOut);

	/** Performs an inverse FFT, outputting cepstral data. */
	void inverseCepstral (const SampleType* magIn, SampleType* cepOut);

	///@}

private:

	std::unique_ptr<FFTImpl<SampleType>> pimpl;
};

/** This namespace contains functions for controlling FFTW wisdom.
	These functions always exist, but simply do nothing when the FFTW backend is not being used.
	@ingroup fft
 */
namespace fftw
{

/** @ingroup fft
	@{
 */

/** Sets the directory that FFTW wisdom files will be read from and saved to.
	The actual filenames will be \c .fftw_wisdom.d (double precision) and \c .fftw_wisdom.f (float precision), within this directory.
	@note To load wisdom files from a specified directory, this function should be called before the FFT object is created.
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
