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

#pragma once

#include <atomic>
#include <limes_export.h>
#include <limes_namespace.h>
#include "../../misc/preprocessor.h"
#include "../../misc/Functions.h"

/** @dir libs/limes_core/data_structures/fifo
	This directory contains a FIFO implementation.
	@ingroup data_structures
 */

/** @file
	This file defines the AbstractFIFO class.
	@ingroup data_structures
 */

LIMES_BEGIN_NAMESPACE

namespace ds
{

/** This class manages a read and write position for a buffer of data.
	@ingroup data_structures
	@todo AbstractFIFO class methods need documentation
	@todo write unit tests for this class
 */
class LIMES_EXPORT AbstractFIFO final
{
public:

	explicit AbstractFIFO (std::size_t initialSize);

	LIMES_NON_COPYABLE (AbstractFIFO)
	LIMES_NON_MOVABLE (AbstractFIFO)

	[[nodiscard]] int getCapacity() const noexcept;

	[[nodiscard]] int getFreeSpace() const noexcept;

	[[nodiscard]] int getNumStoredObjects() const noexcept;

	void reset();

	void setCapacity (std::size_t newCapacity);

	class LIMES_EXPORT ScopedRead final
	{
	public:

		explicit ScopedRead (AbstractFIFO& fifo, std::size_t num);

		~ScopedRead() noexcept;

		LIMES_NON_COPYABLE (ScopedRead)
		LIMES_NON_MOVABLE (ScopedRead)

		std::size_t startIndex1, blockSize1, startIndex2, blockSize2;

		template <func::Function Function>
		void for_each (Function&& func) const
		{
			for (auto i = startIndex1, e = startIndex1 + blockSize1; i != e; ++i) func (i);
			for (auto i = startIndex2, e = startIndex2 + blockSize2; i != e; ++i) func (i);
		}

	private:

		const std::size_t numObjects;

		AbstractFIFO& fifoModel;
	};

	class LIMES_EXPORT ScopedWrite final
	{
	public:

		explicit ScopedWrite (AbstractFIFO& fifo, std::size_t num);

		~ScopedWrite() noexcept;

		LIMES_NON_COPYABLE (ScopedWrite)
		LIMES_NON_MOVABLE (ScopedWrite)

		std::size_t startIndex1, blockSize1, startIndex2, blockSize2;

		template <func::Function Function>
		void for_each (Function&& func) const
		{
			for (auto i = startIndex1, e = startIndex1 + blockSize1; i != e; ++i) func (i);
			for (auto i = startIndex2, e = startIndex2 + blockSize2; i != e; ++i) func (i);
		}

	private:

		const std::size_t numObjects;

		AbstractFIFO& fifoModel;
	};

	[[nodiscard]] ScopedRead read (std::size_t num);

	[[nodiscard]] ScopedWrite write (std::size_t num);

private:

	void prepareToWrite (std::size_t  numToWrite,
						 std::size_t& startIndex1, std::size_t& blockSize1,
						 std::size_t& startIndex2, std::size_t& blockSize2) const noexcept;

	void finishedWrite (std::size_t numWritten) noexcept;

	void prepareToRead (std::size_t	 numWanted,
						std::size_t& startIndex1, std::size_t& blockSize1,
						std::size_t& startIndex2, std::size_t& blockSize2) const noexcept;

	void finishedRead (std::size_t numRead) noexcept;

	std::size_t bufferSize;

	std::atomic<std::size_t> validStart { 0 }, validEnd { 0 };
};

}  // namespace ds

LIMES_END_NAMESPACE
