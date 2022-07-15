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

// IWYU pragma: private
// IWYU pragma: friend limes_fft.h

#pragma once

#include <limes_namespace.h>
#include <limes_export.h>
#include <limes_core.h>
#include <string_view>

/** @file
	This file defines implementation-selecting macros and
	functions for retrieving information about which implementation
	is being used for the limes_vecops FFT.

	@ingroup fft
 */

#ifdef DOXYGEN
/** @def LIMES_VECOPS_USE_FFTW
	1 if the FFTW library is being used, otherwise 0.
	@ingroup fft
 */
#	define LIMES_VECOPS_USE_FFTW 0

/** @def LIMES_VECOPS_USE_PFFFT
	1 if the pffft library is being used, otherwise 0.
	@ingroup fft
 */
#	define LIMES_VECOPS_USE_PFFFT 0
#endif

/// @cond
// clang-format off

#pragma mark LIMES_VECOPS_USE_FFTW

#ifdef LIMES_VECOPS_USE_FFTW

#	ifdef LIMES_VECOPS_USE_PFFFT
#		if (LIMES_VECOPS_USE_FFTW && LIMES_VECOPS_USE_PFFFT)
			LIMES_COMPILER_WARNING ("LIMES_VECOPS_USE_PFFFT and LIMES_VECOPS_USE_FFTW are both 1!")
			// pffft and FFTW are both on - prefer FFTW
#			undef LIMES_VECOPS_USE_PFFFT
#			define LIMES_VECOPS_USE_PFFFT 0
#		endif
#	endif

#else /* ifdef LIMES_VECOPS_USE_FFTW */

#	ifdef LIMES_VECOPS_USE_PFFFT
#		if LIMES_VECOPS_USE_PFFFT
#			define LIMES_VECOPS_USE_FFTW 0
#		endif
#	endif

#	ifndef LIMES_VECOPS_USE_FFTW
#		if LIMES_HAS_INCLUDE(<fftw3.h>)
#			define LIMES_VECOPS_USE_FFTW 1	 // NOLINT
#		else
#			define LIMES_VECOPS_USE_FFTW 0	 // NOLINT
#		endif
#	endif

#endif /* ifdef LIMES_VECOPS_USE_FFTW */

#ifndef LIMES_VECOPS_USE_FFTW
#	error Internal error - LIMES_VECOPS_USE_FFTW not defined!
#endif


#pragma mark LIMES_VECOPS_USE_PFFFT

#ifdef LIMES_VECOPS_USE_PFFFT

#	if (LIMES_VECOPS_USE_PFFFT && LIMES_VECOPS_USE_FFTW)
		LIMES_COMPILER_WARNING ("LIMES_VECOPS_USE_PFFFT and LIMES_VECOPS_USE_FFTW are both 1!")
		// pffft and FFTW are both on - prefer FFTW
#		undef LIMES_VECOPS_USE_PFFFT
#		define LIMES_VECOPS_USE_PFFFT 0
#	endif

#else /* ifdef LIMES_VECOPS_USE_PFFFT */

#	if (LIMES_VECOPS_USE_FFTW || LIMES_VECOPS_USE_VDSP || LIMES_VECOPS_USE_IPP)
#		define LIMES_VECOPS_USE_PFFFT 0
#	elif LIMES_HAS_INCLUDE(<pffft.h>)
#		define LIMES_VECOPS_USE_PFFFT 1
#	else
#		define LIMES_VECOPS_USE_PFFFT 0
#	endif

#endif /* ifdef LIMES_VECOPS_USE_PFFFT */

#ifndef LIMES_VECOPS_USE_PFFFT
#	error Internal error - LIMES_VECOPS_USE_PFFFT not defined!
#endif

/// @endcond
// clang-format on


#pragma mark -
#pragma mark Implementation kind checking

LIMES_BEGIN_NAMESPACE

namespace vecops::fft
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

/** Returns true if the pffft FFT implementation is being used. */
[[nodiscard]] LIMES_PURE_FUNCTION consteval bool isUsingpffft() noexcept
{
#if LIMES_VECOPS_USE_PFFFT
	return true;
#else
	return false;
#endif
}

/** Returns true if the Apple vDSP FFT implementation is being used. */
[[nodiscard]] LIMES_PURE_FUNCTION consteval bool isUsingVDSP() noexcept
{
	return vecops::isUsingVDSP() && ! (isUsingFFTW() || isUsingpffft());  // cppcheck-suppress knownConditionTrueFalse
}

/** Returns true if the Intel IPP FFT implementation is being used. */
[[nodiscard]] LIMES_PURE_FUNCTION consteval bool isUsingIPP() noexcept
{
	return vecops::isUsingIPP() && ! (isUsingFFTW() || isUsingpffft());	 // cppcheck-suppress knownConditionTrueFalse
}

/** Returns true if the NE10 implementation is being used. */
[[nodiscard]] LIMES_PURE_FUNCTION consteval bool isUsingNE10() noexcept
{
	return vecops::isUsingNE10() && ! (isUsingFFTW() || isUsingpffft());  // cppcheck-suppress knownConditionTrueFalse
}

/** Returns true if the fallback FFT implementation is being used. */
[[nodiscard]] LIMES_PURE_FUNCTION consteval bool isUsingFallback() noexcept
{
	return ! (isUsingFFTW() || isUsingpffft() || isUsingVDSP() || isUsingIPP() || isUsingNE10());  // cppcheck-suppress knownConditionTrueFalse
}

static_assert (isUsingFFTW() || isUsingpffft() || isUsingVDSP() || isUsingIPP() || isUsingNE10() || isUsingFallback());

/** Returns a string literal with the name of the FFT implementation being used. */
[[nodiscard]] LIMES_PURE_FUNCTION static consteval std::string_view getImplementationName() noexcept
{
	if constexpr (isUsingFFTW())
		return "FFTW";
	else if constexpr (isUsingpffft())
		return "pffft";
	else if constexpr (isUsingVDSP())
		return "Apple vDSP";
	else if constexpr (isUsingIPP())
		return "Intel IPP";
	else if constexpr (isUsingNE10())
		return "NE10";
	else
		return "Fallback";
}

/** @} */

}  // namespace vecops::fft

LIMES_END_NAMESPACE
