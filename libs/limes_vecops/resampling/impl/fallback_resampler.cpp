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

#include "./fallback_resampler.h"
#include <limes_namespace.h>
#include "../limes_resampler.h"
#include <stdexcept>
#include "./speex.h"

LIMES_BEGIN_NAMESPACE

namespace vecops::resampling
{

static_assert (isUsingFallback());

static inline int getSpeexQuality (ResamplingParameters::Quality q) noexcept
{
	switch (q)
	{
		case (ResamplingParameters::Quality::Best) : return 10;
		case (ResamplingParameters::Quality::Fastest) : return 0;
		default : return 4;
	}
}

template <Scalar SampleType>
FallbackResampler<SampleType>::FallbackResampler (const ResamplingParameters& params)
	: speex_quality (getSpeexQuality (params.quality))
{
}

template <Scalar SampleType>
void FallbackResampler<SampleType>::prepare (double initialSamplerate, int numChannels, int channelSize)
{
	if (resampler.get() == nullptr
		|| resampler->get_num_channels() != static_cast<std::uint32_t> (numChannels))
	{
		const auto rrate = static_cast<std::uint32_t> (math::round (initialSamplerate));

		resampler.reset (new speex::Resampler (static_cast<std::uint32_t> (numChannels),
											   std::uint32_t (1), std::uint32_t (1),
											   rrate, rrate, speex_quality));

		if (resampler.get() == nullptr)
			throw std::runtime_error { "Fallback resampler: failed to initialize Speex object" };
	}

	if (channelSize > 0 && numChannels > 1)
	{
		const auto inSize = static_cast<std::size_t> (channelSize * numChannels);

		m_iin.reallocate (inSize);
		m_iout.reallocate (inSize * 2);
	}
}

template <>
int FallbackResampler<float>::resample (float* const * const	   out,
										int						   outspace,
										const float* const * const in,
										int						   incount,
										double					   ratio) noexcept
{
	LIMES_ASSERT (resampler.get() != nullptr);

	if (ratio != m_lastratio)
		setRatio (ratio);

	auto uincount  = static_cast<std::uint32_t> (incount);
	auto uoutcount = static_cast<std::uint32_t> (outspace);

	const float* data_in;
	float*		 data_out;

	if (m_channels == 1)
	{
		data_in	 = *in;
		data_out = *out;
	}
	else
	{
		LIMES_ASSERT (incount * m_channels > static_cast<int> (m_iin.getSize()));
		LIMES_ASSERT (outspace * m_channels > static_cast<int> (m_iout.getSize()));

		vecops::interleave (m_iin.get(), in, m_channels, incount);

		data_in	 = m_iin.get();
		data_out = m_iout.get();
	}

	resampler->process_interleaved (data_in, uincount,
									data_out, uoutcount);

	if (m_channels > 1)
		vecops::deinterleave (out, m_iout.get(), m_channels, uoutcount);

	return static_cast<int> (uoutcount);
}

template <>
int FallbackResampler<double>::resample (double* const * const		 out,
										 int						 outspace,
										 const double* const * const in,
										 int						 incount,
										 double						 ratio) noexcept
{
	LIMES_ASSERT (resampler.get() != nullptr);

	if (ratio != m_lastratio)
		setRatio (ratio);

	auto uincount  = static_cast<std::uint32_t> (incount);
	auto uoutcount = static_cast<std::uint32_t> (outspace);

	LIMES_ASSERT (incount * m_channels > static_cast<int> (m_iin.getSize()));
	LIMES_ASSERT (outspace * m_channels > static_cast<int> (m_iout.getSize()));

	// interleave & convert to float
	{
		int idx = 0;

		for (auto s = 0; s < incount; ++s)
			for (auto c = 0; c < m_channels; ++c)
				m_iin[idx++] = static_cast<float> (in[c][s]);
	}

	resampler->process_interleaved (m_iin.get(), uincount,
									m_iout.get(), uoutcount);

	// deinterleave & convert back to doubles
	{
		int idx = 0;

		for (unsigned s = 0; s < uoutcount; ++s)
			for (auto c = 0; c < m_channels; ++c)
				out[c][s] = static_cast<double> (m_iout[idx++]);
	}

	return static_cast<int> (uoutcount);
}

template <Scalar SampleType>
void FallbackResampler<SampleType>::setRatio (double ratio)
{
	LIMES_ASSERT (resampler.get() != nullptr);

	// Speex wants a ratio of two unsigned integers, not a single float

	static constexpr auto big = 272408136U;

	std::uint32_t denom = 1U, num = 1U;

	if (ratio < 1.)
	{
		denom = big;
		num	  = static_cast<unsigned> (math::round (static_cast<double> (big) * ratio));
	}
	else
	{
		num	  = big;
		denom = static_cast<unsigned> (math::round (static_cast<double> (big) * ratio));
	}

	resampler->set_rate_frac (denom, num,
							  static_cast<std::uint32_t> (math::round (m_initialSampleRate)),
							  static_cast<std::uint32_t> (math::round (m_initialSampleRate * ratio)));

	m_lastratio = ratio;

	if (m_initial)
	{
		resampler->skip_zeroes();
		m_initial = false;
	}
}

template <Scalar SampleType>
void FallbackResampler<SampleType>::reset()
{
	m_lastratio = -1.0;	 // force reset of ratio
	m_initial	= true;

	if (auto* r = resampler.get())
		r->reset_mem();
}

template class FallbackResampler<float>;
template class FallbackResampler<double>;

}  // namespace vecops::resampling

LIMES_END_NAMESPACE
