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

#include "SampleProcessor.h"
#include <limes_namespace.h>
#include <utility>	// for move

LIMES_BEGIN_NAMESPACE

namespace dsp
{

template <Sample SampleType>
SampleProcessor<SampleType>::SampleProcessor (SampleProcessingFunc&& sampleFuncToUse)
	: sampleFunc (std::move (sampleFuncToUse)), blockFunc ([this] (const SampleType* input, SampleType* output, int numSamples)
														   {
	for (auto i = 0; i < numSamples; ++i)
		output[i] = sampleFunc(input[i]); })
{
}

template <Sample SampleType>
SampleProcessor<SampleType>::SampleProcessor (SampleProcessingFunc&& sampleFuncToUse, BlockProcessingFunc&& blockFuncToUse)
	: sampleFunc (std::move (sampleFuncToUse)), blockFunc (std::move (blockFuncToUse))
{
}

template <Sample SampleType>
SampleType SampleProcessor<SampleType>::processSample (SampleType inputSample) const
{
	return sampleFunc (inputSample);
}

template <Sample SampleType>
void SampleProcessor<SampleType>::processBlock (const SampleType* input, SampleType* output, int numSamples) const
{
	blockFunc (input, output, numSamples);
}

template <Sample SampleType>
void SampleProcessor<SampleType>::processBlock (const Buffer& input, Buffer& output)
{
	processBlock (input.getReadPointer (0), output.getWritePointer (0),
				  std::min (input.getNumSamples(), output.getNumSamples()));
}

template <Sample SampleType>
void SampleProcessor<SampleType>::skipSamples (int numToSkip) const
{
	for (auto i = 0; i < numToSkip; ++i)
		sampleFunc (SampleType (0));
}

template struct SampleProcessor<float>;
template struct SampleProcessor<double>;

}  // namespace dsp

LIMES_END_NAMESPACE
