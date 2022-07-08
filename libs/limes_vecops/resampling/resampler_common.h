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
// IWYU pragma: friend *_resampler.h

#pragma once

#include <limes_export.h>
#include <limes_namespace.h>
#include "../limes_vecops.h"

/** @file
	This file contains implementation details common to multiple resampler implementations.
	@ingroup resampling
 */

LIMES_BEGIN_NAMESPACE

namespace vecops::resampling
{

/// @cond internals

/** This class defines the interface for any implementation of a resampling backend.
	@see Resampler
	@ingroup resampling
 */
template <Scalar SampleType>
class LIMES_NO_EXPORT ResamplerImpl
{
public:
	virtual ~ResamplerImpl() = default;

	virtual void prepare (double initialSamplerate, int numChannels, int channelSize) = 0;

	virtual int resample (SampleType* const * const		  out,
						  int							  outspace,
						  const SampleType* const * const in,
						  int							  incount,
						  double						  ratio) noexcept = 0;

	virtual double getEffectiveRatio (double inputRatio) const noexcept = 0;

	virtual void reset() = 0;

private:
};

/// @endcond

}  // namespace vecops::resampling

LIMES_END_NAMESPACE
