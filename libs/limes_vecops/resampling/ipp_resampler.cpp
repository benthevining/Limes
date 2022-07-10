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

#include "./ipp_resampler.h"
#include <limes_namespace.h>
#include "./limes_resampler.h"
#include <limes_core.h>
#include <cmath>
#include <stdexcept>

#if LIMES_HAS_INCLUDE(<ippversion.h>)
#	include <ippversion.h>

#	if (IPP_VERSION_MAJOR < 7)
#		error Unsupported IPP version, must be >= 7
#	endif
#endif

/*
	- implement reset()
	- releaseResources()
 */

LIMES_BEGIN_NAMESPACE

namespace vecops::resampling
{

static_assert (isUsingIPP());

static inline IppHintAlgorithm algHintFromQuality (ResamplingParameters::Quality q) noexcept
{
	if (q == ResamplingParameters::Quality::Best)
		return ippAlgHintAccurate;

	return ippAlgHintFast;
}

template <Scalar SampleType>
IPPResampler<SampleType>::IPPResampler (const ResamplingParameters& params)
	: algHint (algHintFromQuality (params.quality))
{
	ippsResamplePolyphaseGetSize_32f (static_cast<float> (WINDOW_SIZE),
									  STEP_SIZE,
									  &m_specSize,
									  algHint);

	if (m_specSize == 0)
		throw std::runtime_error { "IPP resampler: invalid m_specSize of 0!" };
}

template <Scalar SampleType>
IPPResampler<SampleType>::~IPPResampler()
{
	for (auto ptr : m_state)
		ippsFree (ptr);
}

template <Scalar SampleType>
void IPPResampler<SampleType>::prepare (double /*initialSamplerate*/, int numChannels, int channelSize)
{
	const auto realBufsize = [this, channelSize]
	{
		int size = 0;

		for (auto c = 0; c < m_channels; ++c)
			size = std::max (size, m_lastread[c] + channelSize + m_history);

		return std::max (size, m_bufsize);
	}();

	m_bufsize  = realBufsize;
	m_channels = numChannels;

	m_lastread.reallocate (static_cast<std::size_t> (numChannels));
	m_time.reallocate (static_cast<std::size_t> (numChannels));

	for (auto i = m_channels; i < numChannels; ++i)
	{
		m_state.emplace_back (reinterpret_cast<IppsResamplingPolyphase_32f*> (ippsMalloc_8u (m_specSize)));

		ippsResamplePolyphaseInit_32f (static_cast<float> (WINDOW_SIZE),
									   STEP_SIZE,
									   0.95f,
									   9.f,
									   m_state.back(),
									   algHint);

		m_lastread[i] = m_history;
		m_time[i]	  = m_history;
	}

	for (auto i = m_channels; i > numChannels; --i)
		m_state.pop_back();

	LIMES_ASSERT (static_cast<int> (m_state.size()) == m_channels);

	const auto n1 = realBufsize + m_history + 2;

	const auto n2 = math::round (std::lrintf (std::ceil (static_cast<float> (realBufsize - m_history) * m_factor + 2.f)));

	m_inbuf.reallocate (static_cast<std::size_t> (numChannels), static_cast<std::size_t> (n1));
	m_outbuf.reallocate (static_cast<std::size_t> (numChannels), static_cast<std::size_t> (n2));
}

template <Scalar SampleType>
int IPPResampler<SampleType>::resample (SampleType* const * const		out,
										int								outspace,
										const SampleType* const * const in,
										int								incount,
										double							ratio) noexcept
{
	if (ratio > static_cast<double> (m_factor))
	{
		m_factor  = static_cast<float> (ratio);
		m_history = math::round (static_cast<float> (WINDOW_SIZE) * 0.5f * std::max (1.f, 1.f / m_factor)) + 1;
	}

#if LIMES_DEBUG
	for (auto c = 0; c < m_channels; ++c)
		LIMES_ASSERT ((m_lastread[c] + incount + m_history) <= m_bufsize);
#endif

	for (auto c = 0; c < m_channels; ++c)
	{
		for (auto i = 0; i < incount; ++i)
			m_inbuf[c][m_lastread[c] + i] = static_cast<float> (in[c][i]);

		m_lastread[c] += incount;
	}

	const auto got = doResample (outspace, ratio);

	for (auto c = 0; c < m_channels; ++c)
		for (auto i = 0; i < got; ++i)
			out[c][i] = static_cast<SampleType> (m_outbuf[c][i]);

	return got;
}

template <Scalar SampleType>
int IPPResampler<SampleType>::doResample (int outspace, double ratio) noexcept
{
	auto outcount = 0;

	for (auto c = 0; c < m_channels; ++c)
	{
		auto n = m_lastread[c] - m_history - math::round (m_time[c]);

		if (n <= 0)
			continue;

		// We're committed to not overrunning outspace, so we need to
		// offer the resampler only enough samples to ensure it won't

		const auto limit = math::round (std::floor (static_cast<double> (outspace) / ratio));

		if (n > limit)
			n = limit;

		using ST = std::vector<IppsResamplingPolyphase_32f*>::size_type;

		ippsResamplePolyphase_32f (m_inbuf[c],
								   n,
								   m_outbuf[c],
								   ratio,
								   1.f,
								   &m_time[c],
								   &outcount,
								   m_state[static_cast<ST> (c)]);

		auto moveFrom = math::round (std::floor (m_time[c])) - m_history;

		if (moveFrom >= m_lastread[c])
			moveFrom = m_lastread[c];
		else
			std::memmove (m_inbuf[c], m_inbuf[c] + moveFrom,
						  static_cast<std::size_t> (m_lastread[c] - moveFrom) * sizeof (SampleType));

		m_lastread[c] -= moveFrom;
		m_time[c] -= moveFrom;

		// bool final = true;

		// if (final && n < limit)
		// {
		// 	// Looks like this actually produces too many samples
		// 	// (additionalcount is a few samples too large).

		// 	// Also, we aren't likely to have enough space in the
		// 	// output buffer as the caller won't have allowed for
		// 	// all the samples we're retrieving here.

		// 	// What to do?

		// 	auto additionalcount = 0;

		// 	for (auto i = 0; i < m_history; ++i)
		// 		m_inbuf[c][m_lastread[c] + i] = 0.f;

		// 	auto nAdditional = m_lastread[c] - math::round (m_time[c]);

		// 	if (n + nAdditional > limit)
		// 		nAdditional = limit - n;

		// 	ippsResamplePolyphase_32f (m_inbuf[c],
		// 							   nAdditional,
		// 							   m_outbuf[c],
		// 							   ratio,
		// 							   1.f,
		// 							   &m_time[c],
		// 							   &additionalcount,
		// 							   m_state[static_cast<ST> (c)]);

		// 	if (c == 0)
		// 		outcount += additionalcount;
		// }
	}

	return outcount;
}

template <Scalar SampleType>
void IPPResampler<SampleType>::reset()
{
	// TODO
}

template class IPPResampler<float>;
template class IPPResampler<double>;

}  // namespace vecops::resampling

LIMES_END_NAMESPACE
