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
#include <limes_core.h>
#include <memory>

LIMES_DISABLE_ALL_COMPILER_WARNINGS
#include <samplerate.h>
LIMES_REENABLE_ALL_COMPILER_WARNINGS

/** @file
	This file contains a resampler implementation using libsamplerate.
	@ingroup resampling
 */

LIMES_BEGIN_NAMESPACE

namespace vecops::resampling
{

/// @cond internals

template <Scalar SampleType>
class LIMES_NO_EXPORT LibsamplerateResampler final : public ResamplerImpl<SampleType>
{
public:
	explicit LibsamplerateResampler (const ResamplingParameters& params);

	LIMES_NON_COPYABLE (LibsamplerateResampler)
	LIMES_DEFAULT_MOVABLE (LibsamplerateResampler)

private:
	void prepare (double initialSamplerate, int numChannels, int channelSize) final;

	int resample (SampleType* const * const		  out,
				  int							  outspace,
				  const SampleType* const * const in,
				  int							  incount,
				  double						  ratio) noexcept final;

	int resampleInterleaved (float* const		out,
							 int				outcount,
							 const float* const in,
							 int				incount,
							 double				ratio) noexcept;

	void reset() final;

	std::unique_ptr<SRC_STATE, decltype ([] (SRC_STATE* s)
										 { src_delete (s); })>
		m_src;

	int m_quality;

	memory::array_pointer<float> m_iin, m_iout;

	int	   m_channels { 0 };
	double m_prevRatio { 1. };
	bool   m_ratioUnset { true };
	bool   m_smoothRatios { true };
};

/// @endcond

}  // namespace vecops::resampling

LIMES_END_NAMESPACE
