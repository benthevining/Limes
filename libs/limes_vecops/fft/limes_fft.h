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

#include <memory>		// for unique_ptr
#include <string_view>	// for std::string, only needed if FFTW is used
#include <limes_export.h>
#include <limes_vecops.h>

#ifndef LIMES_VECOPS_USE_FFTW
#	if __has_include(<fftw3.h>)
#		define LIMES_VECOPS_USE_FFTW 1	 // NOLINT
#	else
#		define LIMES_VECOPS_USE_FFTW 0	 // NOLINT
#	endif
#endif

namespace limes::vecops
{

template <Scalar SampleType>
class LIMES_NO_EXPORT FFTImpl;

template <Scalar SampleType>
class LIMES_EXPORT FFT final
{
public:

	explicit FFT (int size);

	[[nodiscard]] int getSize() const noexcept;

	void forward (const SampleType* realIn, SampleType* realOut, SampleType* imagOut);

	void forwardInterleaved (const SampleType* realIn, SampleType* complexOut);

	void forwardPolar (const SampleType* realIn, SampleType* magOut, SampleType* phaseOut);

	void forwardMagnitude (const SampleType* realIn, SampleType* magOut);

	void inverse (const SampleType* realIn, const SampleType* imagIn, SampleType* realOut);

	void inverseInterleaved (const SampleType* complexIn, SampleType* realOut);

	void inversePolar (const SampleType* magIn, const SampleType* phaseIn, SampleType* realOut);

	void inverseCepstral (const SampleType* magIn, SampleType* cepOut);

	[[nodiscard]] static constexpr bool isUsingFFTW() noexcept
	{
#if LIMES_VECOPS_USE_FFTW
		return true;
#else
		return false;
#endif
	}

	[[nodiscard]] static constexpr bool isUsingVDSP() noexcept
	{
		return vecops::isUsingVDSP() && ! isUsingFFTW();  // cppcheck-suppress knownConditionTrueFalse
	}

	[[nodiscard]] static constexpr bool isUsingIPP() noexcept
	{
		return vecops::isUsingIPP() && ! isUsingFFTW();	 // cppcheck-suppress knownConditionTrueFalse
	}

	[[nodiscard]] static constexpr bool isUsingFallback() noexcept
	{
		return ! (isUsingFFTW() || isUsingVDSP() || isUsingIPP());	// cppcheck-suppress knownConditionTrueFalse
	}

	static_assert (isUsingFFTW() || isUsingVDSP() || isUsingIPP() || isUsingFallback());

	[[nodiscard]] static constexpr const char* const getImplementationName() noexcept
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

private:

	std::unique_ptr<FFTImpl<SampleType>> pimpl;
};

#if LIMES_VECOPS_USE_FFTW
namespace fftw
{
LIMES_EXPORT void setWisdomFileDir (const std::string_view& dirAbsPath);

LIMES_EXPORT [[nodiscard]] std::string getWisdomFileDir();

LIMES_EXPORT void enableWisdom (bool shouldUseWisdom);

LIMES_EXPORT [[nodiscard]] bool isUsingWisdom();
}  // namespace fftw
#endif

}  // namespace limes::vecops
