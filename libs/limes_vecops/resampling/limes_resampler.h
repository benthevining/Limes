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
#include "../limes_vecops.h"

/** @defgroup resampling Resampling
	Audio resampling utilities.

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

/** @ingroup fft
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

/** @} */

}  // namespace resampling

/// @cond internals
/** An implementation class for the resampler.
	User code needs no knowledge of this class.
	@ingroup resampling
 */
template <Scalar SampleType>
class ResamplerImpl;
/// @endcond


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

	enum class Dynamism
	{
		RatioOftenChanging,
		RatioMostlyFixed
	};

	enum class RatioChange
	{
		SmoothRatioChange,
		SuddenRatioChange
	};

	/** The quality of the filters used. */
	Quality quality { Quality::FastTolerable };

	/** Performance hint indicating whether the ratio is expected
		to change regularly or not. If not, more work may happen on
		ratio changes to reduce work when ratio is unchanged.
	 */
	Dynamism dynamism { Dynamism::RatioMostlyFixed };

	/** Hint indicating whether to smooth transitions, via filter
		interpolation or some such method, at ratio change
		boundaries, or whether to make a precise switch to the new
		ratio without regard to audible artifacts. The actual
		effect of this depends on the implementation in use.
	 */
	RatioChange ratioChange { RatioChange::SmoothRatioChange };

	/** Rate of expected input prior to resampling: may be used to
		determine the filter bandwidth for the quality setting. If
		you don't know what this will be, you can provide an
		arbitrary rate (such as the default) and the resampler will
		work fine, but quality may not be as designed.
	 */
	double initialSamplerate { 44100. };

	/** Bound on the maximum incount size that may be passed to the
		resample function before the resampler needs to reallocate
		its internal buffers. If this is 0, the first time the
		resampling function is called, internal allocation will occur.
	 */
	int maxBuffersize { 1024 };
};


#pragma mark Resampler class

/** A class that can resample a stream of audio in realtime.
	@ingroup resampling

	@todo fallback impl
	@todo IPP impl
	@todo libsamplerate impl
	@todo speex impl
 */
template <Scalar SampleType>
class LIMES_EXPORT Resampler final
{
public:
	Resampler (const ResamplingParameters& params,
			   int						   numChannels = 1);

	~Resampler() = default;

	LIMES_NON_COPYABLE (Resampler)
	LIMES_DEFAULT_MOVABLE (Resampler)

	/** @name Resampling */
	///@{
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

	int resampleInterleaved (SampleType* const		 out,
							 int					 outspace,
							 const SampleType* const in,
							 int					 incount,
							 double					 ratio) noexcept;
	///@}

	/** Returns the ratio that will be actually used when the given
		ratio is requested. For example, if the resampler internally
		uses a rational approximation of the given ratio, this will
		return the closest double to that approximation. Not all
		implementations support this; an implementation that does not
		will just return the given ratio.
	 */
	[[nodiscard]] double getEffectiveRatio (double inputRatio) const noexcept;

	/** Resets the internal state of the resampler. */
	void reset();

private:
	std::unique_ptr<ResamplerImpl<SampleType>> pimpl;
};

}  // namespace vecops

LIMES_END_NAMESPACE