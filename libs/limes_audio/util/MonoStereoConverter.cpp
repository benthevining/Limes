
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

#include "MonoStereoConverter.h"
#include <limes_vecops.h>	 // for copy
#include <limes_platform.h>	 // for LIMES_ASSERT, LIMES_ASSERT_FALSE
#include <limes_namespace.h>
#include <utility>

LIMES_BEGIN_NAMESPACE

namespace dsp
{

template <Sample SampleType>
StereoReductionMode MonoStereoConverter<SampleType>::getStereoReductionMode() const noexcept
{
	return toMonoMode;
}

template <Sample SampleType>
void MonoStereoConverter<SampleType>::prepare (int blocksize)
{
	monoStorage.reserveAndZero (blocksize);
}


template <Sample SampleType>
void MonoStereoConverter<SampleType>::setStereoReductionMode (StereoReductionMode newmode)
{
	toMonoMode = newmode;
}

template <Sample SampleType>
void MonoStereoConverter<SampleType>::convertStereoToMono (const SampleVector& leftIn,
														   const SampleVector& rightIn,
														   SampleVector&	   monoOut)
{
	LIMES_ASSERT (leftIn.numObjects() == rightIn.numObjects() == monoOut.numObjects());

	convertStereoToMono (leftIn.data(), rightIn.data(), monoOut.data(), monoOut.numObjects());
}

template <Sample SampleType>
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
		default : LIMES_UNREACHABLE;
	}
}

template <Sample SampleType>
void MonoStereoConverter<SampleType>::convertMonoToStereo (const SampleVector& monoIn,
														   SampleVector&	   leftOut,
														   SampleVector&	   rightOut)
{
	LIMES_ASSERT (monoIn.numObjects() == leftOut.numObjects() == rightOut.numObjects());

	convertMonoToStereo (monoIn.data(), leftOut.data(), rightOut.data(), monoIn.numObjects());
}

template <Sample SampleType>
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

}  // namespace dsp

LIMES_END_NAMESPACE
