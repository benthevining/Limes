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
// IWYU pragma: friend limes_resampler.cpp

#pragma once

#include "./resampler_common.h"
#include <limes_export.h>
#include <limes_namespace.h>

/** @file
	This file contains a fallback resampler implementation.
	@ingroup resampling
 */

LIMES_BEGIN_NAMESPACE

namespace vecops::resampling
{

/// @cond internals

template <Scalar SampleType>
class LIMES_NO_EXPORT FallbackResampler final : public ResamplerImpl<SampleType>
{
public:
private:
	void prepare (double initialSamplerate, int numChannels, int channelSize) final;

	int resample (SampleType* const * const		  out,
				  int							  outspace,
				  const SampleType* const * const in,
				  int							  incount,
				  double						  ratio) noexcept final;

	double getEffectiveRatio (double inputRatio) const noexcept final;

	void reset() final;
};

/// @endcond

}  // namespace vecops::resampling

LIMES_END_NAMESPACE
