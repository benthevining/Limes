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

#include <limes_platform.h>
#include <algorithm>
#include "AbstractFIFO.h"
#include <limes_namespace.h>
#include "../../system/limes_assert.h"

LIMES_BEGIN_NAMESPACE

namespace ds
{

AbstractFIFO::AbstractFIFO (std::size_t initialSize)
	: bufferSize (initialSize)
{
}

int AbstractFIFO::getCapacity() const noexcept
{
	return static_cast<int>(bufferSize);
}

int AbstractFIFO::getFreeSpace() const noexcept
{
	return getCapacity() - getNumStoredObjects() - 1;
}

int AbstractFIFO::getNumStoredObjects() const noexcept
{
	const auto vs = validStart.load();
	const auto ve = validEnd.load();

	if (ve >= vs)
		return static_cast<int>(ve - vs);

	return static_cast<int>(bufferSize - (vs - ve));
}

void AbstractFIFO::reset()
{
	validEnd.store (0);
	validStart.store (0);
}

void AbstractFIFO::setCapacity (std::size_t newCapacity)
{
	reset();
	bufferSize = newCapacity;
}

void AbstractFIFO::prepareToWrite (std::size_t numToWrite,
								   std::size_t& startIndex1, std::size_t& blockSize1,
								   std::size_t& startIndex2, std::size_t& blockSize2) const noexcept
{
	startIndex2 = 0;

	const auto vs = validStart.load();
	const auto ve = validEnd.load();

	const auto freeSpace = [ve, vs, size = bufferSize]
	{
		if (ve >= vs)
			return size - (ve - vs);

		return vs - ve;
	}();

	numToWrite = std::min (numToWrite, freeSpace - 1);

	if (numToWrite <= 0)
	{
		startIndex1 = 0;
		blockSize1	= 0;
		blockSize2	= 0;
		return;
	}

	startIndex1 = ve;
	blockSize1	= std::min (bufferSize - ve, numToWrite);

	numToWrite -= blockSize1;

	if (numToWrite <= 0)
		blockSize2 = 0;
	else
		blockSize2 = std::min (numToWrite, vs);
}

void AbstractFIFO::finishedWrite (std::size_t numWritten) noexcept
{
	LIMES_ASSERT (numWritten >= 0 && numWritten < bufferSize);

	auto newEnd = validEnd.load() + numWritten;

	if (newEnd >= bufferSize)
		newEnd -= bufferSize;

	validEnd.store (newEnd);
}

void AbstractFIFO::prepareToRead (std::size_t numWanted,
								  std::size_t& startIndex1, std::size_t& blockSize1,
								  std::size_t& startIndex2, std::size_t& blockSize2) const noexcept
{
	startIndex2 = 0;

	const auto vs = validStart.load();
	const auto ve = validEnd.load();

	const auto numReady = [ve, vs, size = bufferSize]
	{
		if (ve >= vs)
			return ve - vs;

		return size - (vs - ve);
	}();

	numWanted = std::min (numWanted, numReady);

	if (numWanted <= 0)
	{
		startIndex1 = 0;
		blockSize1	= 0;
		blockSize2	= 0;
	}

	startIndex1 = vs;
	blockSize1	= std::min (bufferSize - vs, numWanted);

	numWanted -= blockSize1;

	if (numWanted <= 0)
		blockSize2 = 0;
	else
		blockSize2 = std::min (numWanted, ve);
}

void AbstractFIFO::finishedRead (std::size_t numRead) noexcept
{
	LIMES_ASSERT (numRead >= 0 && numRead <= bufferSize);

	auto newStart = validStart.load() + numRead;

	if (newStart >= bufferSize)
		newStart -= bufferSize;

	validStart.store (newStart);
}

AbstractFIFO::ScopedRead AbstractFIFO::read (std::size_t num)
{
	return ScopedRead { *this, num };
}

AbstractFIFO::ScopedWrite AbstractFIFO::write (std::size_t num)
{
	return ScopedWrite { *this, num };
}

/*---------------------------------------------------------------------------------------------------------------------------*/

AbstractFIFO::ScopedRead::ScopedRead (AbstractFIFO& fifo, std::size_t num)
	: numObjects (num), fifoModel (fifo)
{
	fifoModel.prepareToRead (num, startIndex1, blockSize1, startIndex2, blockSize2);
}

AbstractFIFO::ScopedRead::~ScopedRead() noexcept
{
	fifoModel.finishedRead (numObjects);
}


AbstractFIFO::ScopedWrite::ScopedWrite (AbstractFIFO& fifo, std::size_t num)
	: numObjects (num), fifoModel (fifo)
{
	fifoModel.prepareToWrite (num, startIndex1, blockSize1, startIndex2, blockSize2);
}

AbstractFIFO::ScopedWrite::~ScopedWrite() noexcept
{
	fifoModel.finishedWrite (numObjects);
}

}  // namespace ds

LIMES_END_NAMESPACE
