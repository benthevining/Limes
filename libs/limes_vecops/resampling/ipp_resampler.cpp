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

LIMES_BEGIN_NAMESPACE

namespace vecops
{

static_assert (resampling::isUsingIPP());

template <Scalar SampleType>
int IPPResampler<SampleType>::resample (SampleType* const * const		out,
										int								outspace,
										const SampleType* const * const in,
										int								incount,
										double							ratio) noexcept
{
}

template <Scalar SampleType>
int IPPResampler<SampleType>::resampleInterleaved (SampleType* const	   out,
												   int					   outspace,
												   const SampleType* const in,
												   int					   incount,
												   double				   ratio) noexcept
{
}

template <Scalar SampleType>
double IPPResampler<SampleType>::getEffectiveRatio (double inputRatio) const noexcept
{
}

template <Scalar SampleType>
void IPPResampler<SampleType>::reset()
{
}

template class IPPResampler<float>;
template class IPPResampler<double>;

}  // namespace vecops

LIMES_END_NAMESPACE
