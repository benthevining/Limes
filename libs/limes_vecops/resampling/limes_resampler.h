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

#include <limes_export.h>
#include <limes_namespace.h>
#include <memory>
#include <limes_core.h>
#include "../vecops/vecops.h"

/** @defgroup resampling Resampling
	Audio resampling utilities.

	The resampler class can use the following backends:
	- libsamplerate
	- Intel IPP
	- Apple vDSP
	- Fallback

	@ingroup limes_vecops

	@dependency \b libsamplerate
	<a href="http://www.mega-nerd.com/SRC/">The libsamplerate library</a> can be used as the backend for the @ref limes_vecops "limes_vecops" Resampler class.
	The usage of libsamplerate is determined by the \c LIMES_USE_LIBSAMPLERATE @ref cmakeopt "CMake variable".

	@cmakeopt \b LIMES_USE_LIBSAMPLERATE
	By default, <a href="http://www.mega-nerd.com/SRC/">libsamplerate</a> will be searched for in the system, and if found,
	will be used for the @ref limes_vecops "limes_vecops" Resampler class's backend. However, you can set this option to \c OFF to ignore libsamplerate.
	IPP also provide a resampler implementation.

	@cmakeprop \b LIMES_RESAMPLER_IMPLEMENTATION
	String name of the resampling backend being used for the @ref lib_limes_vecops "limes_vecops" library.
 */

/** @dir libs/limes_vecops/resampling
	This directory contains audio resampling utilities.
	@ingroup resampling
 */

/** @file
	This file defines the interface for the Limes audio resampler.
	@ingroup resampling
 */

#ifdef DOXYGEN
/** @def LIMES_VECOPS_USE_LIBSAMPLERATE
	1 if the libsamplerate library is being used for resampling, otherwise 0.
	@ingroup resampling
 */
#	define LIMES_VECOPS_USE_LIBSAMPLERATE 0
#endif

/// @cond
#ifndef LIMES_VECOPS_USE_LIBSAMPLERATE
#	if LIMES_HAS_INCLUDE(<samplerate.h>)
#		define LIMES_VECOPS_USE_LIBSAMPLERATE 1  // NOLINT
#	else
#		define LIMES_VECOPS_USE_LIBSAMPLERATE 0  // NOLINT
#	endif
#endif
/// @endcond

LIMES_BEGIN_NAMESPACE

namespace vecops
{

/** This namespace contains utilities related to the Resampler class.
	@ingroup resampling
 */
namespace resampling
{

#pragma mark Implementation kind checking

/** @ingroup resampling
	@{
 */

/** Returns true if the libsamplerate implementation is being used. */
[[nodiscard]] LIMES_PURE_FUNCTION consteval bool isUsingLibSamplerate() noexcept
{
#if LIMES_VECOPS_USE_LIBSAMPLERATE
	return true;
#else
	return false;
#endif
}

/** Returns true if the Intel IPP implementation is being used. */
[[nodiscard]] LIMES_PURE_FUNCTION consteval bool isUsingIPP() noexcept
{
	return vecops::isUsingIPP() && ! isUsingLibSamplerate();  // cppcheck-suppress knownConditionTrueFalse
}

/** Returns true if the fallback implementation is being used. */
[[nodiscard]] LIMES_PURE_FUNCTION consteval bool isUsingFallback() noexcept
{
	return ! (isUsingLibSamplerate() || isUsingIPP());	// cppcheck-suppress knownConditionTrueFalse
}

static_assert (isUsingLibSamplerate() || isUsingIPP() || isUsingFallback());

/// @cond internals
template <Scalar SampleType>
class ResamplerImpl;
/// @endcond

/** @} */

}  // namespace resampling


#pragma mark Parameters

/** This class defines parameters for a Resampler.
	@ingroup resampling
 */
class LIMES_EXPORT ResamplingParameters final
{
public:
	enum class Quality
	{
		Best,
		FastTolerable,
		Fastest
	};

	enum class RatioChange
	{
		SmoothRatioChange,
		SuddenRatioChange
	};

	/** The quality of the filters used. */
	Quality quality { Quality::FastTolerable };

	/** Hint indicating whether to smooth transitions, via filter
		interpolation or some such method, at ratio change
		boundaries, or whether to make a precise switch to the new
		ratio without regard to audible artifacts. The actual
		effect of this depends on the implementation in use.
	 */
	RatioChange ratioChange { RatioChange::SmoothRatioChange };
};


#pragma mark Resampler class

/** A class that can resample a stream of audio in realtime.
	@ingroup resampling
 */
template <Scalar SampleType>
class LIMES_EXPORT Resampler final
{
public:
	/** Creates a resampler with the specified parameters. */
	Resampler (const ResamplingParameters& params);

	/** Destructor. */
	~Resampler() = default;

	LIMES_NON_COPYABLE (Resampler)
	LIMES_DEFAULT_MOVABLE (Resampler)

	/** Prepares the resampler for processing.
		This may allocate memory.
	 */
	void prepare (double initialSamplerate, int numChannels, int channelSize);

	/** Resample the given multi-channel buffer, where \c incount is the
		number of frames in the input buffer and \c outspace is the space
		available in the output buffer. Generally you want \c outspace to
		be at least \c ceil(incount*ratio) .

		@return The number of frames actually written to the output
		buffer. This may be smaller than \c outspace even where the ratio
		suggests otherwise, particularly at the start of processing
		where there may be a filter tail to allow for.
	 */
	int resample (SampleType* const * const		  out,
				  int							  outspace,
				  const SampleType* const * const in,
				  int							  incount,
				  double						  ratio) noexcept;

	/** Resets the internal state of the resampler. */
	void reset();

private:
	std::unique_ptr<resampling::ResamplerImpl<SampleType>> pimpl;
};

}  // namespace vecops

LIMES_END_NAMESPACE
