
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
	monoStorage.resize (blocksize);
}

template <Sample SampleType>
void MonoStereoConverter<SampleType>::releaseResources()
{
	monoStorage.deallocate();
}

template <Sample SampleType>
void MonoStereoConverter<SampleType>::setStereoReductionMode (StereoReductionMode newmode) noexcept
{
	toMonoMode = newmode;
}

template <Sample SampleType>
void MonoStereoConverter<SampleType>::convertStereoToMono (const MonoBuffer& leftIn,
														   const MonoBuffer& rightIn,
														   MonoBuffer&		 monoOut) noexcept
{
	LIMES_ASSERT (leftIn.getNumSamples() == rightIn.getNumSamples());
	LIMES_ASSERT (leftIn.getNumSamples() == monoOut.getNumSamples());

	convertStereoToMono (leftIn.getReadPointer (0), rightIn.getReadPointer (0), monoOut.getWritePointer (0), monoOut.getNumSamples());
}

template <Sample SampleType>
void MonoStereoConverter<SampleType>::convertStereoToMono (const SampleType* const leftIn,
														   const SampleType* const rightIn,
														   SampleType* const	   monoOut,
														   int					   numSamples) noexcept
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
			LIMES_ASSERT (monoStorage.getNumSamples() >= numSamples);

			monoStorage.clear();

			const auto num = static_cast<std::size_t> (numSamples);

			monoStorage.copyFrom (leftIn, num, 0);

			monoStorage.addFrom (rightIn, num, 0);

			monoStorage.applyGain (SampleType (0.5));

			monoStorage.copyTo (monoOut, 0, num);

			return;
		}
		default : LIMES_UNREACHABLE;
	}
}

template <Sample SampleType>
void MonoStereoConverter<SampleType>::convertMonoToStereo (const MonoBuffer& monoIn,
														   MonoBuffer&		 leftOut,
														   MonoBuffer&		 rightOut) noexcept
{
	LIMES_ASSERT (leftOut.getNumSamples() == rightOut.getNumSamples());
	LIMES_ASSERT (leftOut.getNumSamples() == monoIn.getNumSamples());

	convertMonoToStereo (monoIn.getReadPointer (0), leftOut.getWritePointer (0), rightOut.getWritePointer (0), monoIn.getNumSamples());
}

template <Sample SampleType>
void MonoStereoConverter<SampleType>::convertMonoToStereo (const SampleType* const monoIn,
														   SampleType* const	   leftOut,
														   SampleType* const	   rightOut,
														   int					   numSamples) noexcept
{
	vecops::copy (leftOut, monoIn, numSamples);
	vecops::copy (rightOut, monoIn, numSamples);
}

template class MonoStereoConverter<float>;
template class MonoStereoConverter<double>;

}  // namespace dsp

LIMES_END_NAMESPACE
