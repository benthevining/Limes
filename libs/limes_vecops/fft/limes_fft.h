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

#ifndef LIMES_VECOPS_USE_FFTW
#	if __has_include(<fftw3.h>)
#		define LIMES_VECOPS_USE_FFTW 1	 // NOLINT
#	else
#		define LIMES_VECOPS_USE_FFTW 0	 // NOLINT
#	endif
#endif

LIMES_BEGIN_NAMESPACE

namespace vecops
{

namespace fft
{
[[nodiscard]] consteval bool isUsingFFTW() noexcept
{
#if LIMES_VECOPS_USE_FFTW
	return true;
#else
	return false;
#endif
}

[[nodiscard]] consteval bool isUsingVDSP() noexcept
{
	return vecops::isUsingVDSP() && ! isUsingFFTW();  // cppcheck-suppress knownConditionTrueFalse
}

[[nodiscard]] consteval bool isUsingIPP() noexcept
{
	return vecops::isUsingIPP() && ! isUsingFFTW();	 // cppcheck-suppress knownConditionTrueFalse
}

[[nodiscard]] consteval bool isUsingFallback() noexcept
{
	return ! (isUsingFFTW() || isUsingVDSP() || isUsingIPP());	// cppcheck-suppress knownConditionTrueFalse
}

static_assert (isUsingFFTW() || isUsingVDSP() || isUsingIPP() || isUsingFallback());

[[nodiscard]] static consteval const char* getImplementationName() noexcept
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
}  // namespace fft

template <Scalar SampleType>
class FFTImpl;

template <Scalar SampleType>
class LIMES_EXPORT FFT final
{
public:

	explicit FFT (int size);

	LIMES_NON_COPYABLE (FFT);
	LIMES_DEFAULT_MOVABLE (FFT);

	[[nodiscard]] int getSize() const noexcept;

	void forward (const SampleType* realIn, SampleType* realOut, SampleType* imagOut);

	void forwardInterleaved (const SampleType* realIn, SampleType* complexOut);

	void forwardPolar (const SampleType* realIn, SampleType* magOut, SampleType* phaseOut);

	void forwardMagnitude (const SampleType* realIn, SampleType* magOut);

	void inverse (const SampleType* realIn, const SampleType* imagIn, SampleType* realOut);

	void inverseInterleaved (const SampleType* complexIn, SampleType* realOut);

	void inversePolar (const SampleType* magIn, const SampleType* phaseIn, SampleType* realOut);

	void inverseCepstral (const SampleType* magIn, SampleType* cepOut);

private:

	std::unique_ptr<FFTImpl<SampleType>> pimpl;
};

/* These functions always exist, but simply do nothing when the FFTW backend is not being used. */
namespace fftw
{
LIMES_EXPORT bool setWisdomFileDir (const files::Directory& directory);

LIMES_EXPORT [[nodiscard]] files::Directory getWisdomFileDir();

LIMES_EXPORT void enableWisdom (bool shouldUseWisdom);

LIMES_EXPORT [[nodiscard]] bool isUsingWisdom();
}  // namespace fftw

}  // namespace vecops

LIMES_END_NAMESPACE
