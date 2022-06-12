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

#include "PitchData.h"
#include "yin.h"
#include "zeroCrossing.h"
#include <limes_core.h>
#include "PitchDetectionAlgorithm.h"
#include <array>

/** @file
	This file defines the PitchDetector class.
	@ingroup pitch_detection
 */

LIMES_BEGIN_NAMESPACE

namespace dsp::pitch
{

/**
	@ingroup pitch_detection
 */
template <Sample SampleType>
class LIMES_EXPORT PitchDetector final : public PitchDetectionAlgorithm<SampleType>
{
public:

	/** Convenience typedef for the datatype returned by the algorithm. */
	using Result = PitchData<SampleType>;

	/** @name Pitch detection
		Detect the period in samples
	 */
	///@{
	/** Detects the period, in samples, for a frame of audio.
		This can only be used for one channel at a time. If you need to track the pitch of multiple channels of audio, you need one PitchDetector object for each channel.
		The caller must ensure that there are at least enough samples in this frame of audio for analysis to be performed; ie, that numSamples is greater than or equal to \c getLatencySamples().
		@note You must call \c setSamplerate() to prepare the pitch detector before calling this function!
		@warning The caller must ensure that the buffer sent to this function contains at least \c getLatencySamples() samples.
	*/
	[[nodiscard]] Result detectPeriod (const SampleType* const inputAudio, int numSamples) noexcept final;
	///@}

	/** Returns the last detected period data. */
	[[nodiscard]] Result getLastDetectedPeriod() const noexcept final;

	/** Returns the latency in samples of the detection algorithm.

		The latency defines the minimum size of the buffers that must be sent to any of the detection functions, though you can send larger buffers if you wish.
		@see setMinHz()
	*/
	[[nodiscard]] int getLatencySamples() const noexcept final;

	/** Sets the samplerate in Hz of the pitch detector.
		@note This function may allocate memory, so should not be called from a realtime thread.
		@return The latency, in samples, of the pitch detection algorithm at the new samplerate.
		@see getLatencySamples()
	*/
	int setSamplerate (double newSamplerate) final;

	/** Resets the internal state of the pitch detector without releasing any resources.
		This function is safe to call from a realtime thread.
	*/
	void reset() noexcept final;

	/** Releases all of the pitch detector's internal resources.
		After calling this function, you must call \c setSamplerate() to re-prepare the detector before calling any of the pitch detection functions.
		@note This function may deallocate memory, so should not be called from a realtime thread.
	 */
	void releaseResources() final;

	/** @name YIN access */
	///@{
	/** Returns a reference to the internal YIN pitch detector. */
	Yin<SampleType>&	   getYin() noexcept { return yin; }
	const Yin<SampleType>& getYin() const noexcept { return yin; }
	///@}

	/** @name ZTX access */
	///@{
	/** Returns a reference to the internal zero-crossing pitch detector. */
	ZeroCrossings<SampleType>&		 getZTX() noexcept { return ztx; }
	const ZeroCrossings<SampleType>& getZTX() const noexcept { return ztx; }
	///@}

private:

	Yin<SampleType> yin;

	ZeroCrossings<SampleType> ztx;

	static constinit const size_t num_algos = 2;

	std::array<PitchDetectionAlgorithm<SampleType>*, num_algos> algos = { &yin, &ztx };

	std::array<Result, num_algos> algoResults;

	const Result* chosenResult { nullptr };
};

}  // namespace dsp::pitch

LIMES_END_NAMESPACE
