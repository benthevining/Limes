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

#include <limes_core.h>
#include "../util/Misc.h"  // for concept Sample - IWYU pragma: keep
#include "./PitchData.h"
#include "../util/AudioBuffer.h"

/** @file
	This file defines the PitchDetectionAlgorithm interface class.
	@ingroup pitch_detection
 */

LIMES_BEGIN_NAMESPACE

namespace dsp::pitch
{

/** The base class for any kind of pitch detection algorithm.
	@ingroup pitch_detection
 */
template <Sample SampleType>
class LIMES_EXPORT PitchDetectionAlgorithm
{
public:

	virtual ~PitchDetectionAlgorithm() = default;

	/** Convenience typedef for a vector of samples. */
	using Buffer = AudioBuffer<SampleType, 1>;

	/** @name Pitch detection
		Detect the period in samples
	 */
	///@{
	/** Detects the period, in samples, for a frame of audio.
		This can only be used for one channel at a time. If you need to track the pitch of multiple channels of audio, you need one pitch detector object for each channel.
		The caller must ensure that there are at least enough samples in this frame of audio for analysis to be performed; ie, that numSamples is greater than or equal to \c getLatencySamples().
		@note You must call \c setSamplerate() to prepare the pitch detector before calling this function!
		@warning The caller must ensure that the buffer sent to this function contains at least \c getLatencySamples() samples.
	*/
	[[nodiscard]] virtual PitchData<SampleType> detectPeriod (const SampleType* const inputAudio, int numSamples) noexcept = 0;

	[[nodiscard]] PitchData<SampleType> detectPeriod (const Buffer& inputAudio) noexcept;
	///@}

	/** Returns the last detected period data. */
	[[nodiscard]] virtual PitchData<SampleType> getLastDetectedPeriod() const noexcept = 0;

	/** Returns the latency in samples of the detection algorithm.

		The latency defines the minimum size of the buffers that must be sent to any of the detection functions, though you can send larger buffers if you wish.
		@see setMinHz()
	*/
	[[nodiscard]] virtual int getLatencySamples() const noexcept = 0;

	/** Sets the samplerate in Hz of the pitch detector.
		@note This function may allocate memory, so should not be called from a realtime thread.
		@return The latency, in samples, of the pitch detection algorithm at the new samplerate.
		@see getLatencySamples()
	*/
	virtual int setSamplerate (double newSamplerate) = 0;

	/** Resets the internal state of the pitch detector without releasing any resources.
		This function is safe to call from a realtime thread.
	*/
	virtual void reset() noexcept = 0;

	/** Releases all of the pitch detector's internal resources.
		After calling this function, you must call \c setSamplerate() to re-prepare the detector before calling any of the pitch detection functions.
		@note This function may deallocate memory, so should not be called from a realtime thread.
	 */
	virtual void releaseResources() = 0;
};

}  // namespace dsp::pitch

LIMES_END_NAMESPACE
