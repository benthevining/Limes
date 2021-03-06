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

#include "./SampleStream.h"
#include <utility>	// for move
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace dsp
{

template <Sample SampleType>
SampleStream<SampleType>::SampleStream (SampleGenerationFunc&& sampleFuncToUse)
	: sampleFunc (std::move (sampleFuncToUse)), blockFunc ([this] (SampleType* output, int num)
														   {
	for (auto i = 0; i < num; ++i)
		output[i] = sampleFunc(); })	 // NOLINT
{
}

template <Sample SampleType>
SampleStream<SampleType>::SampleStream (SampleGenerationFunc&& sampleFuncToUse, BlockProcessingFunc&& blockFuncToUse)
	: sampleFunc (std::move (sampleFuncToUse)), blockFunc (std::move (blockFuncToUse))
{
}

template <Sample SampleType>
SampleType SampleStream<SampleType>::getSample() const
{
	return sampleFunc();
}

template <Sample SampleType>
void SampleStream<SampleType>::getSamples (SampleType* const output, int numSamples) const
{
	blockFunc (output, numSamples);
}

template <Sample SampleType>
void SampleStream<SampleType>::getSamples (Buffer& output)
{
	blockFunc (output.getWritePointer (0), output.getNumSamples());
}

template <Sample SampleType>
void SampleStream<SampleType>::skipSamples (int numToSkip) const
{
	for (auto i = 0; i < numToSkip; ++i)
		sampleFunc();
}

template struct SampleStream<float>;
template struct SampleStream<double>;

}  // namespace dsp

LIMES_END_NAMESPACE
