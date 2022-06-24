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

#include "./WhiteNoiseGenerator.h"
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace dsp
{

template <Sample SampleType>
WhiteNoiseGenerator<SampleType>::WhiteNoiseGenerator()
	: SampleStream<SampleType> ([this]() noexcept
								{ return getNextSampleValue(); })
{
}

template <Sample SampleType>
WhiteNoiseGenerator<SampleType>::WhiteNoiseGenerator (int64_t randomSeed)
	: SampleStream<SampleType> ([this]() noexcept
								{ return getNextSampleValue(); }),
	  random (randomSeed)
{
}

template <Sample SampleType>
WhiteNoiseGenerator<SampleType>::WhiteNoiseGenerator (math::Random& randomToFork)
	: SampleStream<SampleType> ([this]() noexcept
								{ return getNextSampleValue(); }),
	  random (randomToFork.fork())
{
}

template <Sample SampleType>
SampleType WhiteNoiseGenerator<SampleType>::getNextSampleValue() noexcept
{
	return random.next<SampleType>() * SampleType (0.25) - SampleType (0.125);
}

template struct WhiteNoiseGenerator<float>;
template struct WhiteNoiseGenerator<double>;

}  // namespace dsp

LIMES_END_NAMESPACE
