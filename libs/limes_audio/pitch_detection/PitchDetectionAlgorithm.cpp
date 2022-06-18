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

#include "PitchDetectionAlgorithm.h"
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace dsp::pitch
{

template <Sample SampleType>
PitchData<SampleType> PitchDetectionAlgorithm<SampleType>::detectPeriod (const Buffer& inputAudio) noexcept
{
	return detectPeriod (inputAudio.getReadPointer (0), inputAudio.getNumSamples());
}

template class PitchDetectionAlgorithm<float>;
template class PitchDetectionAlgorithm<double>;

}  // namespace dsp::pitch

LIMES_END_NAMESPACE
