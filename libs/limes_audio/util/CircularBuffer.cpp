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

#include "CircularBuffer.h"
#include <limes_platform.h>
#include <limes_vecops.h>
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace dsp
{

template <Sample SampleType>
CircularBuffer<SampleType>::CircularBuffer (std::size_t initialCapacity)
	: fifo (initialCapacity), storage (initialCapacity)
{
}

template <Sample SampleType>
void CircularBuffer<SampleType>::storeSamples (const Buffer& samples)
{
	storeSamples (samples.getReadPointer (0), static_cast<std::size_t>(samples.getNumSamples()));
}

template <Sample SampleType>
void CircularBuffer<SampleType>::storeSamples (const SampleType* const samples, std::size_t numSamples)
{
	LIMES_ASSERT (storage.getNumSamples() >= fifo.getCapacity());

	const auto scopedWrite = fifo.write (numSamples);

	// the buffer isn't big enough to hold all the samples you want to store!
	LIMES_ASSERT (scopedWrite.blockSize1 + scopedWrite.blockSize2 == numSamples);

	auto* writing = storage.getWritePointer (0);

	if (scopedWrite.blockSize1 > 0)
		vecops::copy (writing + scopedWrite.startIndex1, samples, scopedWrite.blockSize1);

	if (scopedWrite.blockSize2 > 0)
	{
		vecops::copy (writing + scopedWrite.startIndex2,
					  samples + scopedWrite.blockSize1,
					  scopedWrite.blockSize2);
	}
}

template <Sample SampleType>
void CircularBuffer<SampleType>::getSamples (Buffer& output)
{
	getSamples (output.getWritePointer (0), static_cast<std::size_t>(output.getNumSamples()));
}

template <Sample SampleType>
void CircularBuffer<SampleType>::getSamples (SampleType* const output, std::size_t numSamples)
{
	LIMES_ASSERT (storage.getNumSamples() >= fifo.getCapacity());

	const auto scopedRead = fifo.read (numSamples);

	const auto numZeroes = numSamples - (scopedRead.blockSize1 + scopedRead.blockSize2);

	if (numZeroes > 0)
		vecops::clear (output, numZeroes);

	auto* const sampleOutput = output + numZeroes;

	const auto* const reading = storage.getReadPointer (0);

	if (scopedRead.blockSize1 > 0)
		vecops::copy (sampleOutput, reading + scopedRead.startIndex1, scopedRead.blockSize1);

	if (scopedRead.blockSize2 > 0)
	{
		vecops::copy (sampleOutput + scopedRead.blockSize1,
					  reading + scopedRead.startIndex2,
					  scopedRead.blockSize2);
	}
}

template <Sample SampleType>
void CircularBuffer<SampleType>::resize (std::size_t newSize)
{
	// NB. avoids edge cases when attempting to store the full capacity's worth of samples
	newSize += 1;

	storage.resize (newSize);

	fifo.setCapacity (newSize);

	LIMES_ASSERT (storage.getNumSamples() >= fifo.getCapacity());
}

template <Sample SampleType>
int CircularBuffer<SampleType>::getCapacity() const noexcept
{
	LIMES_ASSERT (storage.getNumSamples() >= fifo.getCapacity());
	return fifo.getCapacity() - 1;
}

template <Sample SampleType>
int CircularBuffer<SampleType>::getNumStoredSamples() const noexcept
{
	LIMES_ASSERT (storage.getNumSamples() >= fifo.getCapacity());
	return fifo.getNumStoredObjects();
}

template <Sample SampleType>
void CircularBuffer<SampleType>::clear()
{
	storage.clear();
	fifo.reset();
}

template class CircularBuffer<float>;
template class CircularBuffer<double>;

}  // namespace dsp

LIMES_END_NAMESPACE
