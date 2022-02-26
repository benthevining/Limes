
/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

#include "MonoStereoConverter.h"
#include <limes_vecops.h>

namespace lemons::dsp
{

template <typename SampleType>
StereoReductionMode MonoStereoConverter<SampleType>::getStereoReductionMode() const noexcept
{
	return toMonoMode;
}

template <typename SampleType>
void MonoStereoConverter<SampleType>::prepare (int blocksize)
{
	monoStorage.reserveAndZero (blocksize);
}


template <typename SampleType>
void MonoStereoConverter<SampleType>::setStereoReductionMode (StereoReductionMode newmode)
{
	toMonoMode = newmode;
}


template <typename SampleType>
void MonoStereoConverter<SampleType>::convertStereoToMono (const SampleType* const leftIn,
														   const SampleType* const rightIn,
														   SampleType* const	   monoOut,
														   int					   numSamples)
{
	switch (toMonoMode)
	{
		case (StereoReductionMode::leftOnly) :
		{
			vecops::copy (monoOut, leftIn, numSamples);
			return;
		}
		case (StereoReductionMode::rightOnly) :
		{
			vecops::copy (monoOut, rightIn, numSamples);
			return;
		}
		case (StereoReductionMode::mixToMono) :
		{
			LIMES_ASSERT (monoStorage.capacity() >= numSamples);
			monoStorage.clearAndZero (numSamples).copyFrom (leftIn, numSamples);
			monoStorage.addFrom (rightIn, numSamples).divide (SampleType (2));
			monoStorage.copyTo (monoOut, numSamples);
			return;
		}
		default : LIMES_ASSERT_FALSE;
	}
}

template <typename SampleType>
void MonoStereoConverter<SampleType>::convertMonoToStereo (const SampleType* const monoIn,
														   SampleType* const	   leftOut,
														   SampleType* const	   rightOut,
														   int					   numSamples)
{
	vecops::copy (leftOut, monoIn, numSamples);
	vecops::copy (rightOut, monoIn, numSamples);
}

template class MonoStereoConverter<float>;
template class MonoStereoConverter<double>;

}  // namespace lemons::dsp
