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

namespace vecops
{

static_assert (resampling::isUsingFallback());

template <Scalar SampleType>
int FallbackResampler<SampleType>::resample (SampleType* const * const		 out,
											 int							 outspace,
											 const SampleType* const * const in,
											 int							 incount,
											 double							 ratio) noexcept
{
}

template <Scalar SampleType>
int FallbackResampler<SampleType>::resampleInterleaved (SampleType* const		out,
														int						outspace,
														const SampleType* const in,
														int						incount,
														double					ratio) noexcept
{
}

template <Scalar SampleType>
double FallbackResampler<SampleType>::getEffectiveRatio (double inputRatio) const noexcept
{
}

template <Scalar SampleType>
void FallbackResampler<SampleType>::reset()
{
}

template class FallbackResampler<float>;
template class FallbackResampler<double>;

}  // namespace vecops

LIMES_END_NAMESPACE
