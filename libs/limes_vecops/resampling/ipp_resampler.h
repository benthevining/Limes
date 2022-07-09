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
#include <ipps.h>
#include <algorithm>
#include <limes_core.h>
#include <vector>

/** @file
	This file contains a resampler implementation using the IPP library.
	@ingroup resampling
 */

LIMES_BEGIN_NAMESPACE

namespace vecops::resampling
{

/// @cond internals

template <Scalar SampleType>
class LIMES_NO_EXPORT IPPResampler final : public ResamplerImpl<SampleType>
{
public:
	explicit IPPResampler (const ResamplingParameters& params);

	~IPPResampler() final;

	LIMES_NON_COPYABLE (IPPResampler)
	LIMES_DEFAULT_MOVABLE (IPPResampler)

private:
	void prepare (double initialSamplerate, int numChannels, int channelSize) final;

	int resample (SampleType* const * const		  out,
				  int							  outspace,
				  const SampleType* const * const in,
				  int							  incount,
				  double						  ratio) noexcept final;

	void reset() final;

	// to m_outbuf
	int doResample (int outcount, double ratio) noexcept;


	static constexpr auto WINDOW_SIZE = 32;
	static constexpr auto STEP_SIZE	  = 64;

	IppHintAlgorithm algHint;

	int m_specSize { 0 };

	memory::md_array_pointer<float> m_inbuf;
	memory::md_array_pointer<float> m_outbuf;

	int m_bufsize { 0 };
	int m_channels { 2 };

	std::vector<IppsResamplingPolyphase_32f*> m_state;

	float m_factor { 8.f };
	int	  m_history { math::round (static_cast<float> (WINDOW_SIZE) * 0.5f * std::max (1.f, 1.f / m_factor)) + 1 };

	memory::array_pointer<int>	  m_lastread;
	memory::array_pointer<double> m_time;
};

/// @endcond

}  // namespace vecops::resampling

LIMES_END_NAMESPACE
