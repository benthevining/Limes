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
#include "./Misc.h"
#include "./AudioBuffer.h"
#include <limes_namespace.h>
#include <limes_core.h>

LIMES_BEGIN_NAMESPACE

namespace dsp
{

/** @todo write class documentation
	@todo write unit tests
 */
template <Sample SampleType>
class LIMES_EXPORT CircularBuffer final
{
public:

	using Buffer = AudioBuffer<SampleType, 1>;

	/** Creates a CircularBuffer with an initial size. */
	explicit CircularBuffer (std::size_t initialCapacity = 512);

	LIMES_NON_COPYABLE (CircularBuffer)
	LIMES_NON_MOVABLE (CircularBuffer)

	/** Stores samples in the buffer.
		If the buffer's capacity isn't big enough to hold all the passed samples, an assertion will be thrown.
	*/
	void storeSamples (const Buffer& samples);

	/** Stores samples in the buffer.
		If the buffer's capacity isn't big enough to hold all the passed samples, an assertion will be thrown.
	*/
	void storeSamples (const SampleType* const samples, std::size_t numSamples);

	/** Reads samples from the buffer.
		If you request more samples than are in the buffer, the first section of the output buffer will be filled with zeroes.
	*/
	void getSamples (Buffer& output);

	/** Reads samples from the buffer.
		If you request more samples than are in the buffer, the first section of the output buffer will be filled with zeroes.
	*/
	void getSamples (SampleType* const output, std::size_t numSamples);

	/** Returns the total capacity of the buffer. */
	[[nodiscard]] int getCapacity() const noexcept;

	/** Returns the number of samples currently stored in the buffer. */
	[[nodiscard]] int getNumStoredSamples() const noexcept;

	/** Changes the total capacity of the buffer. Calling this method also clears the buffer.
		@see clear()
	*/
	void resize (std::size_t newSize);

	/** Clears any previously stored data from the buffer. */
	void clear();

private:

	ds::AbstractFIFO fifo;

	Buffer storage;
};

}  // namespace dsp

LIMES_END_NAMESPACE
