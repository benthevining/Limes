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
#include <limes_core.h>

LIMES_BEGIN_NAMESPACE

namespace ds
{

class LIMES_EXPORT AbstractFIFO final
{
public:

	explicit AbstractFIFO (int initialSize);

	LIMES_NON_COPYABLE (AbstractFIFO);
	LIMES_NON_MOVABLE (AbstractFIFO);

	[[nodiscard]] int getCapacity() const noexcept;

	[[nodiscard]] int getFreeSpace() const noexcept;

	[[nodiscard]] int getNumStoredObjects() const noexcept;

	void reset();

	void setCapacity (int newCapacity);

	void prepareToWrite (int numToWrite, int& startIndex1, int& blockSize1, int& startIndex2, int& blockSize2) const noexcept;

	void finishedWrite (int numWritten) noexcept;

	void prepareToRead (int numWanted, int& startIndex1, int& blockSize1, int& startIndex2, int& blockSize2) const noexcept;

	void finishedRead (int numRead) noexcept;

	class LIMES_EXPORT ScopedRead final
	{
	public:

		explicit ScopedRead (AbstractFIFO& fifo, int num);

		~ScopedRead() noexcept;

		LIMES_NON_COPYABLE (ScopedRead);
		LIMES_NON_MOVABLE (ScopedRead);

		int startIndex1, blockSize1, startIndex2, blockSize2;

		template <class Function>
		void for_each (Function&& func) const
		{
			for (auto i = startIndex1, e = startIndex1 + blockSize1; i != e; ++i) func (i);
			for (auto i = startIndex2, e = startIndex2 + blockSize2; i != e; ++i) func (i);
		}

	private:

		const int numObjects;

		AbstractFIFO& fifoModel;
	};

	class LIMES_EXPORT ScopedWrite final
	{
	public:

		explicit ScopedWrite (AbstractFIFO& fifo, int num);

		~ScopedWrite() noexcept;

		LIMES_NON_COPYABLE (ScopedWrite);
		LIMES_NON_MOVABLE (ScopedWrite);

		int startIndex1, blockSize1, startIndex2, blockSize2;

		template <class Function>
		void for_each (Function&& func) const
		{
			for (auto i = startIndex1, e = startIndex1 + blockSize1; i != e; ++i) func (i);
			for (auto i = startIndex2, e = startIndex2 + blockSize2; i != e; ++i) func (i);
		}

	private:

		const int numObjects;

		AbstractFIFO& fifoModel;
	};

	[[nodiscard]] ScopedRead read (int num);

	[[nodiscard]] ScopedWrite write (int num);

private:

	int bufferSize;

	std::atomic<int> validStart { 0 }, validEnd { 0 };
};

}  // namespace ds

LIMES_END_NAMESPACE
