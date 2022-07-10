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

#include <limes_core.h>

LIMES_DISABLE_ALL_COMPILER_WARNINGS
#include <samplerate.h>
LIMES_REENABLE_ALL_COMPILER_WARNINGS

#include "./lsr_resampler.h"
#include <limes_namespace.h>
#include "../limes_resampler.h"
#include <cmath>
#include <stdexcept>

LIMES_BEGIN_NAMESPACE

namespace vecops::resampling
{

static_assert (isUsingLibSamplerate());

static inline int get_quality_value (ResamplingParameters::Quality q) noexcept
{
	switch (q)
	{
		case (ResamplingParameters::Quality::Best) : return SRC_SINC_BEST_QUALITY;
		case (ResamplingParameters::Quality::Fastest) : return SRC_SINC_FASTEST;
		default : return SRC_SINC_MEDIUM_QUALITY;
	}
}

template <Scalar SampleType>
LibsamplerateResampler<SampleType>::LibsamplerateResampler (const ResamplingParameters& params)
	: m_quality (get_quality_value (params.quality)),
	  m_smoothRatios (params.ratioChange == ResamplingParameters::RatioChange::SmoothRatioChange)
{
}

template <Scalar SampleType>
void LibsamplerateResampler<SampleType>::prepare (double /*initialSamplerate*/, int numChannels, int channelSize)
{
	m_iin.reallocate (static_cast<std::size_t> (channelSize * numChannels));
	m_iout.reallocate (static_cast<std::size_t> (channelSize * numChannels * 2));

	if (m_src.get() == nullptr || numChannels != m_channels)
	{
		m_channels = numChannels;

		int err = 0;

		m_src.reset (src_new (m_quality, numChannels, &err));

		if (err || m_src == nullptr)
			throw std::runtime_error { "Libsamplerate resampler: failed to initialize!" };
	}

	reset();
}

template <>
int LibsamplerateResampler<float>::resample (float* const * const		out,
											 int						outspace,
											 const float* const * const in,
											 int						incount,
											 double						ratio) noexcept
{
	LIMES_ASSERT (m_src.get() != nullptr);

	if (m_channels == 1)
		return resampleInterleaved (*out, outspace, *in, incount, ratio);

	LIMES_ASSERT (incount * m_channels <= static_cast<int> (m_iin.getSize()));
	LIMES_ASSERT (outspace * m_channels <= static_cast<int> (m_iout.getSize()));

	vecops::interleave (m_iin.get(), in, m_channels, incount);

	const auto n = resampleInterleaved (m_iout, outspace, m_iin, incount, ratio);

	vecops::deinterleave (out, m_iout.get(), m_channels, n);

	return n;
}

template <>
int LibsamplerateResampler<double>::resample (double* const * const		  out,
											  int						  outspace,
											  const double* const * const in,
											  int						  incount,
											  double					  ratio) noexcept
{
	LIMES_ASSERT (m_src.get() != nullptr);
	LIMES_ASSERT (incount * m_channels <= static_cast<int> (m_iin.getSize()));
	LIMES_ASSERT (outspace * m_channels <= static_cast<int> (m_iout.getSize()));

	// interleave & convert to floats
	{
		int idx = 0;

		for (auto s = 0; s < incount; ++s)
			for (auto c = 0; c < m_channels; ++c)
				m_iin[idx++] = static_cast<float> (in[c][s]);
	}

	const auto n = resampleInterleaved (m_iout, outspace, m_iin, incount, ratio);

	// deinterleave & convert back to doubles
	{
		int idx = 0;

		for (auto s = 0; s < n; ++s)
			for (auto c = 0; c < m_channels; ++c)
				out[c][s] = static_cast<double> (m_iout[idx++]);
	}

	return n;
}

template <Scalar SampleType>
int LibsamplerateResampler<SampleType>::resampleInterleaved (float* const		out,
															 int				outcount,
															 const float* const in,
															 int				incount,
															 double				ratio) noexcept
{
	SRC_DATA data;

	// libsamplerate smooths the filter change over the duration of
	// the processing block to avoid artifacts due to sudden changes,
	// and it uses outcount to determine how long to smooth the change
	// over. This is a good thing, but it does mean (a) we should
	// never pass outcount significantly longer than the actual
	// expected output, and (b) when the ratio has just changed, we
	// should aim to supply a shortish block next

	outcount = std::min (outcount, math::round (std::ceil (static_cast<double> (incount) * ratio) + 5.));

	if (m_ratioUnset || ! m_smoothRatios)
	{
		// The first time we set a ratio, we want to do it directly
		src_set_ratio (m_src.get(), ratio);

		m_ratioUnset = false;
		m_prevRatio	 = ratio;
	}
	else if (ratio != m_prevRatio)
	{
		// If we are processing a block of appreciable length, turn it
		// into two recursive calls, one for the short smoothing block
		// and the other for the rest. Update m_prevRatio before doing
		// this so that the calls don't themselves recurse!
		m_prevRatio = ratio;

		static constexpr auto shortBlock = 200;

		if (outcount > shortBlock * 2)
		{
			const auto shortIn = math::round (std::floor (static_cast<double> (shortBlock) / ratio));

			if (shortIn >= 10)
			{
				const auto shortOut = resampleInterleaved (out, shortBlock,
														   in, shortIn,
														   ratio);

				int remainingOut = 0;

				if (shortOut < outcount)
				{
					remainingOut = resampleInterleaved (out + shortOut * m_channels,
														outcount - shortOut,
														in + shortIn * m_channels,
														incount - shortIn,
														ratio);
				}

				return shortOut + remainingOut;
			}
		}
	}

	data.data_in  = const_cast<float*> (in);
	data.data_out = out;

	data.input_frames  = incount;
	data.output_frames = outcount;
	data.src_ratio	   = ratio;

	bool final = false;

	data.end_of_input = (final ? 1 : 0);

	src_process (m_src.get(), &data);

	return math::round (data.output_frames_gen);
}

template <Scalar SampleType>
void LibsamplerateResampler<SampleType>::reset()
{
	if (auto* s = m_src.get())
		src_reset (s);

	m_ratioUnset = true;
}

template class LibsamplerateResampler<float>;
template class LibsamplerateResampler<double>;

}  // namespace vecops::resampling

LIMES_END_NAMESPACE
