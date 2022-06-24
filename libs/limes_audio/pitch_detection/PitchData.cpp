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

#include "./PitchData.h"
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace dsp::pitch
{

template <Sample SampleType>
void PitchData<SampleType>::setPeriod (SampleType period, double samplerate) noexcept
{
	freqHz = static_cast<SampleType> (samplerate) / period;
}

template <Sample SampleType>
SampleType PitchData<SampleType>::getPeriod (double samplerate) const noexcept
{
	return static_cast<SampleType> (samplerate) / freqHz;
}

template <Sample SampleType>
void PitchData<SampleType>::reset() noexcept
{
	freqHz	   = SampleType (0);
	confidence = SampleType (0);
}

template <Sample SampleType>
bool PitchData<SampleType>::isPitched() const noexcept
{
	return freqHz > SampleType (0) && confidence > SampleType (0);
}

template <Sample SampleType>
bool PitchData<SampleType>::isUnpitched() const noexcept
{
	return ! isPitched();
}

template struct PitchData<float>;
template struct PitchData<double>;

}  // namespace dsp::pitch

LIMES_END_NAMESPACE
