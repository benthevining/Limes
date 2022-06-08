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
#include <limes_namespace.h>
#include <limes_core.h>

/** @defgroup samplestreams Streams
	Classes that can produce a continuous stream of output samples with no input.
	@ingroup limes_audio
 */

/** @dir libs/limes_audio/streams
	This directory contains classes that can produce a continuous stream of output samples with no input.
	@ingroup samplestreams
 */

/** @file
	This file defines the SampleStream class.
	@ingroup samplestreams
 */

LIMES_BEGIN_NAMESPACE

namespace dsp
{

/** Base class for any object that can output a stream of audio samples, with no audio or MIDI input.
	The constructor takes a lambda that must produce the next sample value when called with no arguments.
	@ingroup samplestreams
 */
template <Sample SampleType>
struct LIMES_EXPORT SampleStream
{
public:

	using SampleVector = ds::scalar_vector<SampleType>;

	using SampleGenerationFunc = std::function<SampleType()>;

	using BlockProcessingFunc = std::function<void (SampleType*, int)>;

	/** @name Constructors */
	///@{
	/** Creates a sample stream with a specified lambda function for producing the next sample. */
	explicit SampleStream (SampleGenerationFunc&& sampleFuncToUse);

	/** Creates a sample stream with lambda functions for producing a single sample, and for processing a block of samples.
		This constructor should be used if you want to specify a special function for block processing in order to enable SIMD accelerations, etc.
	 */
	explicit SampleStream (SampleGenerationFunc&& sampleFuncToUse, BlockProcessingFunc&& blockFuncToUse);
	///@}

	/** Destructor. */
	virtual ~SampleStream() = default;

	LIMES_DEFAULT_MOVABLE (SampleStream)
	LIMES_DEFAULT_COPYABLE (SampleStream)

	/** Returns the next output sample from the stream. */
	[[nodiscard]] SampleType getSample() const;

	///@{
	/** Returns a stream of samples. */
	void getSamples (SampleType* output, int numSamples) const;
	void getSamples (SampleVector& output);
	///@}

	/** Skips a number of samples in the stream.
		This calls the specified sample generation function once for each sample to be skipped, and does nothing with its output.
	 */
	void skipSamples (int numToSkip) const;

private:

	SampleGenerationFunc sampleFunc;

	BlockProcessingFunc blockFunc;
};

}  // namespace dsp

LIMES_END_NAMESPACE
