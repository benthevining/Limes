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

LIMES_BEGIN_NAMESPACE

namespace vecops::resampling
{

static_assert (isUsingFallback());

template <Scalar SampleType>
void FallbackResampler<SampleType>::prepare (double initialSamplerate, int numChannels, int channelSize)
{
}

template <Scalar SampleType>
int FallbackResampler<SampleType>::resample (SampleType* const * const		 out,
											 int							 outspace,
											 const SampleType* const * const in,
											 int							 incount,
											 double							 ratio) noexcept
{
	return 0;
}

template <Scalar SampleType>
double FallbackResampler<SampleType>::getEffectiveRatio (double inputRatio) const noexcept
{
	return inputRatio;
}

template <Scalar SampleType>
void FallbackResampler<SampleType>::reset()
{
}

template class FallbackResampler<float>;
template class FallbackResampler<double>;

}  // namespace vecops::resampling

LIMES_END_NAMESPACE
