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
CircularBuffer<SampleType>::CircularBuffer (int initialCapacity)
	: fifo (initialCapacity)
{
	storage.reserveAndZero (initialCapacity);
}

template <Sample SampleType>
void CircularBuffer<SampleType>::storeSamples (const SampleVector& samples)
{
	storeSamples (samples.data(), samples.numObjects());
}

template <Sample SampleType>
void CircularBuffer<SampleType>::storeSamples (const SampleType* const samples, int numSamples)
{
	LIMES_ASSERT (storage.numObjects() == fifo.getCapacity());

	const auto scopedWrite = fifo.write (numSamples);

	// the buffer isn't big enough to hold all the samples you want to store!
	LIMES_ASSERT (scopedWrite.blockSize1 + scopedWrite.blockSize2 == numSamples);

	auto* writing = storage.data();

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
void CircularBuffer<SampleType>::getSamples (SampleVector& output)
{
	getSamples (output.data(), output.numObjects());
}

template <Sample SampleType>
void CircularBuffer<SampleType>::getSamples (SampleType* const output, int numSamples)
{
	LIMES_ASSERT (storage.numObjects() == fifo.getCapacity());

	const auto scopedRead = fifo.read (numSamples);

	const auto numZeroes = numSamples - (scopedRead.blockSize1 + scopedRead.blockSize2);

	if (numZeroes > 0)
		vecops::clear (output, numZeroes);

	auto* const sampleOutput = output + numZeroes;

	const auto* const reading = storage.data();

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
void CircularBuffer<SampleType>::resize (int newSize)
{
	LIMES_ASSERT (newSize > 0);

	clear();

	// NB. avoids edge cases when attempting to store the full capacity's worth of samples
	newSize += 1;

	storage.reserveAndZero (newSize);

	fifo.setCapacity (newSize);

	LIMES_ASSERT (storage.numObjects() == fifo.getCapacity());
}

template <Sample SampleType>
int CircularBuffer<SampleType>::getCapacity() const noexcept
{
	LIMES_ASSERT (storage.numObjects() == fifo.getCapacity());
	return storage.numObjects() - 1;
}

template <Sample SampleType>
int CircularBuffer<SampleType>::getNumStoredSamples() const noexcept
{
	LIMES_ASSERT (storage.numObjects() == fifo.getCapacity());
	return fifo.getNumStoredObjects();
}

template <Sample SampleType>
void CircularBuffer<SampleType>::clear()
{
	storage.zero();
	fifo.reset();
}

template class CircularBuffer<float>;
template class CircularBuffer<double>;

}  // namespace dsp

LIMES_END_NAMESPACE
