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

#include "../util/Misc.h"  // for concept Sample - IWYU pragma: keep
#include <functional>	   // for function
#include <limes_export.h>  // for LIMES_EXPORT
#include <limes_data_structures.h>

namespace limes::dsp
{

/** Base class for any object that can output a stream of audio samples, with no audio or MIDI input.
	The constructor takes a lambda that must produce the next sample value when called with no arguments.
 */
template <Sample SampleType>
struct LIMES_EXPORT SampleStream
{
public:

	using SampleVector = scalar_vector<SampleType>;

	/** Creates a sample stream with a specified lambda function for producing the next sample. */
	explicit SampleStream (std::function<SampleType()>&& sampleFuncToUse);

	/** Destructor. */
	virtual ~SampleStream() = default;

	/** Returns the next output sample from the stream. */
	[[nodiscard]] SampleType getSample() const;

	/** Returns a stream of samples. */
	void getSamples (SampleType* output, int numSamples) const;

	void getSamples (SampleVector& output);

	/** Skips a number of samples in the stream. */
	void skipSamples (int numToSkip) const;

private:

	const std::function<SampleType()> sampleFunc;
};

}  // namespace limes::dsp
