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

#include <limes_data_structures.h>
#include <limes_export.h>
#include "Misc.h"
#include <limes_namespace.h>
#include <limes_core.h>

LIMES_BEGIN_NAMESPACE

namespace dsp
{

template <Sample SampleType>
class LIMES_EXPORT CircularBuffer final
{
public:

	using SampleVector = scalar_vector<SampleType>;

	/** Creates a CircularBuffer with an initial size. */
	explicit CircularBuffer (int initialCapacity = 512);

	LIMES_NON_COPYABLE (CircularBuffer);
	LIMES_NON_MOVABLE (CircularBuffer);

	/** Stores samples in the buffer.
		If the buffer's capacity isn't big enough to hold all the passed samples, an assertion will be thrown.
	*/
	void storeSamples (const SampleVector& samples);

	/** Stores samples in the buffer.
		If the buffer's capacity isn't big enough to hold all the passed samples, an assertion will be thrown.
	*/
	void storeSamples (const SampleType* const samples, int numSamples);

	/** Reads samples from the buffer.
		If you request more samples than are in the buffer, the first section of the output buffer will be filled with zeroes.
	*/
	void getSamples (SampleVector& output);

	/** Reads samples from the buffer.
		If you request more samples than are in the buffer, the first section of the output buffer will be filled with zeroes.
	*/
	void getSamples (SampleType* const output, int numSamples);

	/** Returns the total capacity of the buffer. */
	[[nodiscard]] int getCapacity() const noexcept;

	/** Returns the number of samples currently stored in the buffer. */
	[[nodiscard]] int getNumStoredSamples() const noexcept;

	/** Changes the total capacity of the buffer. Calling this method also clears the buffer.
		@see clear()
	*/
	void resize (int newSize);

	/** Clears any previously stored data from the buffer. */
	void clear();

private:

	limes::AbstractFIFO fifo;

	SampleVector storage;
};

}  // namespace dsp

LIMES_END_NAMESPACE
