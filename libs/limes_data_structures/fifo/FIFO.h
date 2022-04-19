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

#include <limes_export.h>
#include <limes_platform.h>
#include "AbstractFIFO.h"
#include "../vectors/vector.h"
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

template <typename ObjectType>
class LIMES_EXPORT FIFO final
{
public:

	explicit FIFO (int initialSize)
		: model (initialSize)
	{
		storage.reserve (initialSize);
	}

	[[nodiscard]] int getCapacity() const noexcept
	{
		return model.getCapacity();
	}

	[[nodiscard]] int getFreeSpace() const noexcept
	{
		return model.getFreeSpace();
	}

	[[nodiscard]] int numStoredObjects() const noexcept
	{
		return model.getNumStoredObjects();
	}

	void reset()
	{
		model.reset();
		storage.clear();
	}

	void setCapacity (int newCapacity)
	{
		model.setCapacity (newCapacity);
		storage.reserve (newCapacity);
	}

	void push (ObjectType&& object)
	{
		const auto scopedWrite = model.write (1);

		LIMES_ASSERT (scopedWrite.blockSize1 == 1);

		storage[scopedWrite.startIndex1] = object;
	}

	void push (const ObjectType* const data, int numObjects)
	{
		LIMES_ASSERT (storage.capacity() == model.getCapacity());

		const auto scopedWrite = model.write (numObjects);

		// the buffer isn't big enough to hold all the samples you want to store!
		LIMES_ASSERT (scopedWrite.blockSize1 + scopedWrite.blockSize2 == numObjects);

		auto* const writing = storage.data();

		for (auto i = scopedWrite.startIndex1, j = 0; i < scopedWrite.blockSize1; ++i, ++j)
			storage[i] = data[j];

		for (auto i = scopedWrite.startIndex2, j = scopedWrite.blockSize1; i < scopedWrite.blockSize2; ++i, ++j)
			storage[i] = data[j];
	}

	void push (const basic_vector<ObjectType>& data)
	{
		push (data.data(), data.numObjects());
	}

	[[nodiscard]] ObjectType pop()
	{
		const auto scopedRead = model.read (1);

		LIMES_ASSERT (scopedRead.blockSize1 == 1);

		return storage[scopedRead.startIndex1];
	}

	void pop (ObjectType* const data, int numObjects)
	{
		LIMES_ASSERT (storage.capacity() == model.getCapacity());

		const auto scopedRead = model.read (numObjects);

		const auto numNull = numObjects - (scopedRead.blockSize1 + scopedRead.blockSize2);

		for (auto i = 0; i < numNull; ++i)
			data[i] = nullptr;

		auto* const output = data + numNull;

		const auto* const reading = storage.data();

		for (auto i = scopedRead.startIndex1, j = 0; i < scopedRead.blockSize1; ++i, ++j)
			output[j] = reading[i];

		for (auto i = scopedRead.startIndex2, j = scopedRead.blockSize1; i < scopedRead.blockSize2; ++i, ++j)
			output[j] = reading[i];
	}

	void pop (basic_vector<ObjectType>& output)
	{
		pop (output.data(), output.capacity());
	}

private:

	vector<ObjectType> storage;

	AbstractFIFO model;
};

LIMES_END_NAMESPACE
