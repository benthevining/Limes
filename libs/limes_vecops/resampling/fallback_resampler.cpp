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
#include "./limes_resampler.h"
#include <stdexcept>

LIMES_BEGIN_NAMESPACE

namespace vecops::resampling
{

static_assert (isUsingFallback());

static inline int getSpeexQuality (ResamplingParameters::Quality q) noexcept
{
	if (q == ResamplingParameters::Quality::Best)
		return 10;

	if (q == ResamplingParameters::Quality::Fastest)
		return 0;

	return 4;
}

template <Scalar SampleType>
FallbackResampler<SampleType>::FallbackResampler (const ResamplingParameters& params)
	: speex_quality (getSpeexQuality (params.quality))
{
}

template <Scalar SampleType>
void FallbackResampler<SampleType>::prepare (double initialSamplerate, int numChannels, int channelSize)
{
	const auto rrate = math::round (initialSamplerate);

	resampler.reset (new speex::Resampler (numChannels, 1, 1, rrate, rrate, speex_quality));

	if (resampler.get() == nullptr)
		throw std::runtime_error { "Fallback resampler: failed to initialize Speex object" };

	if (channelSize > 0 && numChannels > 1)
	{
		m_iin.reallocate (channelSize * numChannels);
		m_iout.reallocate (channelSize * numChannels * 2);
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

	unsigned int uincount  = incount;
	unsigned int uoutcount = outspace;

	float *data_in, *data_out;

	if (m_channels == 1)
	{
		data_in	 = const_cast<float*> (*in);
		data_out = *out;
	}
	else
	{
		LIMES_ASSERT (int (incount * m_channels) > m_iin.getSize());
		LIMES_ASSERT (int (outspace * m_channels) > m_iout.getSize());

		vecops::interleave (m_iin.get(), in, m_channels, incount);

		data_in	 = m_iin.get();
		data_out = m_iout.get();
	}

	doResample (data_in, uincount, data_out, uoutcount, ratio);

	if (m_channels > 1)
		vecops::deinterleave (out, m_iout.get(), m_channels, uoutcount);

	return uoutcount;
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

	unsigned int uincount  = incount;
	unsigned int uoutcount = outspace;

	LIMES_ASSERT (int (incount * m_channels) > m_iin.getSize());
	LIMES_ASSERT (int (outspace * m_channels) > m_iout.getSize());

	// interleave & convert to float
	{
		int idx = 0;

		for (auto s = 0; s < incount; ++s)
			for (auto c = 0; c < m_channels; ++c)
				m_iin[idx++] = static_cast<float> (in[c][s]);
	}

	doResample (m_iin.get(), uincount, m_iout.get(), uoutcount, ratio);

	// deinterleave & convert back to doubles
	{
		int idx = 0;

		for (auto s = 0; s < uoutcount; ++s)
			for (auto c = 0; c < m_channels; ++c)
				out[c][s] = static_cast<double> (m_iout[idx++]);
	}

	return uoutcount;
}

template <Scalar SampleType>
void FallbackResampler<SampleType>::setRatio (double ratio)
{
	// Speex wants a ratio of two unsigned integers, not a single float

	static constexpr auto big = 272408136U;

	std::uint32_t denom = 1, num = 1U;

	if (ratio < 1.)
	{
		denom		= big;
		double dnum = double (big) * double (ratio);
		num			= (unsigned) dnum;
	}
	else if (ratio > 1.)
	{
		num			  = big;
		double ddenom = double (big) / double (ratio);
		denom		  = (unsigned int) ddenom;
	}

	resampler->set_rate_frac (denom, num, int (round (m_initialSampleRate)), int (round (m_initialSampleRate * ratio)));

	m_lastratio = ratio;

	if (m_initial)
	{
		resampler->skip_zeroes();
		m_initial = false;
	}
}

template <Scalar SampleType>
void FallbackResampler<SampleType>::doResample (const float* in, unsigned& incount,
												float* out, unsigned& outcount,
												double ratio)
{
	int initial_outcount = int (outcount);

	resampler->process_interleaved (in, &incount,
									out, &outcount);

	bool final = false;

	if (final)
	{
		int actual	 = int (outcount);
		int expected = std::min (initial_outcount, int (round (incount * ratio)));

		if (actual < expected)
		{
			//			unsigned int final_out = expected - actual;
			//			unsigned int final_in  = (unsigned int) (round (final_out / ratio));
			//
			//			if (final_in > 0)
			//			{
			//				float* pad = allocate_and_zero<float> (final_in * m_channels);
			//				err		   = speex_resampler_process_interleaved_float (m_resampler,
			//																		pad, &final_in,
			//																		data_out + actual * m_channels, &final_out);
			//				deallocate (pad);
			//				outcount += final_out;
			//			}
		}
	}
}

template <Scalar SampleType>
void FallbackResampler<SampleType>::reset()
{
	m_lastratio = -1.0;	 // force reset of ratio
	m_initial	= true;

	resampler->reset_mem();
}

template class FallbackResampler<float>;
template class FallbackResampler<double>;

}  // namespace vecops::resampling

LIMES_END_NAMESPACE
