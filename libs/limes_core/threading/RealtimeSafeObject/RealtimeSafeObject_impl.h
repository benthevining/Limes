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

#include "../ThreadSafeObject.h"
#include "../RealtimeSafeObject.h"

namespace limes::threads
{

template <typename ObjectType, bool RealtimeMutatable>
ThreadedObjectWriter<ObjectType> RealtimeSafeObject<ObjectType, RealtimeMutatable>::write()
{
	struct Writer final : public ThreadedObjectWriter<ObjectType>
	{
		~Writer()
		{
			parent.release_write();
		}

	private:

		explicit Writer (RealtimeSafeObject& rtso)
			: ThreadedObjectWriter<ObjectType> (rtso.acquire_write()), parent (rtso)
		{
		}

		friend class RealtimeSafeObject;

		RealtimeSafeObject& parent;
	};

	return Writer { *this };
}

/*-------------------------------------------------------------------------------------------------------------------------*/

template <typename ObjectType, bool RealtimeMutatable>
ThreadedObjectReader<ObjectType> RealtimeSafeObject<ObjectType, RealtimeMutatable>::realtime_read()
{
	struct RTmutatableRTread final : public ThreadedObjectReader<ObjectType>
	{
		~RTmutatableRTread()
		{
			rtso.release_write();
		}

	private:

		explicit RTmutatableRTread (RealtimeSafeObject& objectToReference)
			: ThreadedObjectReader<ObjectType> (objectToReference.acquire_write()), rtso (objectToReference)
		{
		}

		friend class RealtimeSafeObject;

		RealtimeSafeObject& rtso;
	};

	struct NRTmutatableRTread final : public ThreadedObjectReader<ObjectType>
	{
		~NRTmutatableRTread()
		{
			rtso.release_read();
		}

	private:

		explicit NRTmutatableRTread (RealtimeSafeObject& objectToReference)
			: ThreadedObjectReader<ObjectType> (objectToReference.acquire_read()), rtso (objectToReference)
		{
		}

		friend class RealtimeSafeObject;

		RealtimeSafeObject& rtso;
	};

	using ReaderType = std::conditional_t<RealtimeMutatable, RTmutatableRTread, NRTmutatableRTread>;

	return ReaderType { *this };
}

template <typename ObjectType, bool RealtimeMutatable>
ThreadedObjectReader<ObjectType> RealtimeSafeObject<ObjectType, RealtimeMutatable>::nonRealtime_read()
{
	struct RTmutatableNRTread final : public ThreadedObjectReader<ObjectType>
	{
		~RTmutatableNRTread()
		{
			rtso.release_read();
		}

	private:

		explicit RTmutatableNRTread (RealtimeSafeObject& objectToReference)
			: ThreadedObjectReader<ObjectType> (objectToReference.acquire_read()), rtso (objectToReference)
		{
		}

		friend class RealtimeSafeObject;

		RealtimeSafeObject& rtso;
	};

	struct NRTmutatableNRTread final : public ThreadedObjectReader<ObjectType>
	{
		~NRTmutatableNRTread()
		{
			rtso.release_write();
		}

	private:

		explicit NRTmutatableNRTread (RealtimeSafeObject& objectToReference)
			: ThreadedObjectReader<ObjectType> (objectToReference.acquire_write()), rtso (objectToReference)
		{
		}

		friend class RealtimeSafeObject;

		RealtimeSafeObject& rtso;
	};

	using ReaderType = std::conditional_t<RealtimeMutatable, RTmutatableNRTread, NRTmutatableNRTread>;

	return ReaderType { *this };
}


template <typename ObjectType, bool RealtimeMutatable>
ThreadedObjectReader<ObjectType> RealtimeSafeObject<ObjectType, RealtimeMutatable>::read (bool isRealtimeThread)
{
	if (isRealtimeThread)
		return realtime_read();

	return nonRealtime_read();
}

}  // namespace limes::threads
