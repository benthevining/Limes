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

#include "./lsr_resampler.h"
#include <limes_namespace.h>
#include "./limes_resampler.h"

LIMES_BEGIN_NAMESPACE

namespace vecops
{

static_assert (resampling::isUsingLibSamplerate());

template <Scalar SampleType>
int LibsamplerateResampler<SampleType>::resample (SampleType* const * const		  out,
												  int							  outspace,
												  const SampleType* const * const in,
												  int							  incount,
												  double						  ratio) noexcept
{
	return 0;
}

template <Scalar SampleType>
int LibsamplerateResampler<SampleType>::resampleInterleaved (SampleType* const		 out,
															 int					 outspace,
															 const SampleType* const in,
															 int					 incount,
															 double					 ratio) noexcept
{
	return 0;
}

template <Scalar SampleType>
double LibsamplerateResampler<SampleType>::getEffectiveRatio (double inputRatio) const noexcept
{
	return inputRatio;
}

template <Scalar SampleType>
void LibsamplerateResampler<SampleType>::reset()
{
}

template class LibsamplerateResampler<float>;
template class LibsamplerateResampler<double>;

}  // namespace vecops

LIMES_END_NAMESPACE
