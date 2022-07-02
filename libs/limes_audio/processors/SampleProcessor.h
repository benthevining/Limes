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
#include "../util/AudioBuffer.h"
#include <limes_core.h>

/** @defgroup sampleprocessors Processors
	Classes that take input samples and process them to produce output samples.
	@ingroup limes_audio
 */

/** @dir libs/limes_audio/processors
	This directory contains classes that take input samples and process them to produce output samples.
	@ingroup sampleprocessors
 */

/** @file
	This file defines the SampleProcessor class.
	@ingroup sampleprocessors
 */

LIMES_BEGIN_NAMESPACE

namespace dsp
{

/** Base class for processors that accept input samples to produce their output samples.
	@ingroup sampleprocessors
	@see SampleStream
 */
template <Sample SampleType>
struct LIMES_EXPORT SampleProcessor
{
public:
	/** Convenience typedef for a vector of samples. */
	using Buffer = AudioBuffer<SampleType, 1>;

	/** A lambda function that returns the next sample value. */
	using SampleProcessingFunc = std::function<SampleType (SampleType)>;

	/** A lambda function that processes a block of sample values at once. */
	using BlockProcessingFunc = std::function<void (const SampleType*, SampleType*, int)>;

	/** @name Constructors */
	///@{
	/** Creates a sample processor with a specified lambda function for processing the next sample. */
	explicit SampleProcessor (SampleProcessingFunc&& sampleFuncToUse);

	/** Creates a sample stream with lambda functions for processing a single sample, and for processing a block of samples.
		This constructor should be used if you want to specify a special function for block processing (in order to enable SIMD accelerations, etc).
	 */
	explicit SampleProcessor (SampleProcessingFunc&& sampleFuncToUse, BlockProcessingFunc&& blockFuncToUse);
	///@}

	/** Destructor. */
	virtual ~SampleProcessor() = default;

	LIMES_DEFAULT_MOVABLE (SampleProcessor)
	LIMES_DEFAULT_COPYABLE (SampleProcessor)

	/** Processes the next sample. */
	[[nodiscard]] SampleType processSample (SampleType inputSample) const;

	/** @name Block processing */
	///@{
	/** Processes a block of samples. */
	void processBlock (const SampleType* input, SampleType* output, int numSamples) const;
	void processBlock (const Buffer& input, Buffer& output);
	///@}

	/** Skips a number of samples in the stream.
		This calls the specified sample generation function with a 0-valued input sample once for each sample to be skipped, and does nothing with its output.
	 */
	virtual void skipSamples (int numToSkip) const;

private:
	SampleProcessingFunc sampleFunc;

	BlockProcessingFunc blockFunc;
};

}  // namespace dsp

LIMES_END_NAMESPACE
