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

#include "./limes_resampler.h"
#include <limes_namespace.h>

#if LIMES_VECOPS_USE_LIBSAMPLERATE
#	include "./lsr_resampler.h"
#elif LIMES_VECOPS_USE_IPP
#	include "./ipp_resampler.h"
#else
#	include "./fallback_resampler.h"
#endif

LIMES_BEGIN_NAMESPACE

namespace vecops
{

template <Scalar SampleType>
Resampler<SampleType>::Resampler (const ResamplingParameters& params,
								  int						  numChannels)
	:
#if LIMES_VECOPS_USE_LIBSAMPLERATE
	  pimpl (std::make_unique<LibsamplerateResampler<SampleType>>())
#elif LIMES_VECOPS_USE_IPP
	  pimpl (std::make_unique<IPPResampler<SampleType>>())
#else
	  pimpl (std::make_unique<FallbackResampler<SampleType>>())
#endif
{
}

template <Scalar SampleType>
int Resampler<SampleType>::resample (SampleType* const * const		 out,
									 int							 outspace,
									 const SampleType* const * const in,
									 int							 incount,
									 double							 ratio) noexcept
{
	return pimpl->resample (out, outspace, in, incount, ratio);
}

template <Scalar SampleType>
int Resampler<SampleType>::resampleInterleaved (SampleType* const		out,
												int						outspace,
												const SampleType* const in,
												int						incount,
												double					ratio) noexcept
{
	return pimpl->resampleInterleaved (out, outspace, in, incount, ratio);
}

template <Scalar SampleType>
double Resampler<SampleType>::getEffectiveRatio (double inputRatio) const noexcept
{
	return pimpl->getEffectiveRatio (inputRatio);
}

template <Scalar SampleType>
void Resampler<SampleType>::reset()
{
	pimpl->reset();
}

template class Resampler<float>;
template class Resampler<double>;

}  // namespace vecops

LIMES_END_NAMESPACE
