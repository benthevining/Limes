
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
#include "Misc.h"

namespace lemons::dsp::psola
{

/** @ingroup lemons_psola_analysis
	A pitch detector based on the YIN algorithm.
 */
template <Sample SampleType>
class PitchDetector final
{
public:

	/** Creates a pitch detector with an initial minimum detectable frequency and confidence threshold.
		The latency of the algorithm is determined by 2 * the period of the minimum frequency. Therefore, pitch detectors with a higher minimum frequency will have a lower latency.
		@param minFreqHz The minimum frequency that this pitch detector will be able to detect.
		@param confidenceThreshold The initial YIN confidence threshold for the detector. See the setConfidenceThresh() method for more documentation on this.
		@see getLatencySamples(), setConfidenceThresh(), setMinHz()
	*/
	explicit PitchDetector (int minFreqHz = 60, float confidenceThreshold = 0.15f);

	/** @name Pitch detection
	 */
	///@{

	/** Detects the pitch in Hz for a frame of audio.
		This can only be used for one channel at a time. If you need to track the pitch of multiple channels of audio, you need one PitchDetector object for each channel.
		The caller must ensure that there are at least enough samples in this frame of audio for analysis to be performed; ie, that numSamples is greater than or equal to getLatencySamples().
		@return The pitch in Hz for this frame of audio, or 0.f if the frame is unpitched.
	*/
	[[nodiscard]] float detectPitch (const SampleType* const inputAudio, int numSamples);


	/** Detects the period, in samples, for a frame of audio.
		@return The period, in samples, of the fundamental frequency for this frame of audio, or 0.f if the frame is unpitched.
	*/
	[[nodiscard]] float detectPeriod (const SampleType* const inputAudio, int numSamples);

	///@}

	/** Returns the latency in samples of the detection algorithm.
		The latency is equal to 2 * the period of the lowest detectable frequency. Therefore, pitch detectors with a higher minimum frequency will have a lower latency.
		@see setMinHz()
	*/
	[[nodiscard]] int getLatencySamples() const noexcept;

	/** Sets the samplerate in Hz of the pitch detector.
		@return The latency, in samples, of the pitch detection algorithm at the new samplerate.
		@see getLatencySamples()
	*/
	int setSamplerate (double newSamplerate);

	/** Sets the minimum detectable frequency for the pitch detector.
		The latency of the algorithm is determined by 2 * the period of the minimum frequency. Therefore, pitch detectors with a higher minimum frequency will have a lower latency.
		Note that the pitch detector may report a frequency slightly lower than this, due to its usage of interpolation to attempt to find the most accurate period.
		@return The latency, in samples, of the pitch detection algorithm with the new minimum frequency.
	*/
	int setMinHz (int newMinHz);

	/** Returns the minimum frequency in Hz that this pitch detector is configured to detect. */
	[[nodiscard]] int getMinHz() const noexcept;

	/** Sets the confidence threshold of the pitch detection algorithm.
		This value should be between 0 and 1, inclusive, and can be thought of as the amount of aperiodic power tolerable in a signal determined to be pitched.
		In other words, the lower this value is, the "stricter" the pitch detector is in choosing its pitch estimates.
		When this value is 1, the pitch detector might output a "pitch" value for a frame of random noise; when this value is 0, the pitch detector might say that even a perfect, pure sine wave is "unpitched".
		Typical recommended values are between 0.1 and 0.2.
	*/
	void setConfidenceThresh (float newThresh) noexcept;

	/** Resets the internal state of the pitch detector without releasing any resources.
		The pitch detector assumes that the input pitch will not halve or double between consecutive pitched frames of audio, so if your input audio is expected to make jumps that large in its pitch, you can call reset() before analyzing the next input frame of audio.
	*/
	void reset();

	/** Releases all of the pitch detector's internal resources.
	 */
	void releaseResources();

	/** Returns the current legal period range for the last analyzed frame of audio. Because the detector assumes that the input pitch should not halve or double between consecutive pitched frames, this can be used to introspect the range of valid periods that the detector has actually considered for the most recent frame.
		Note that you must call one of the pitch detection functions before calling this function.
	*/
	void getCurrentLegalPeriodRange (int& min, int& max) const;

private:

	inline void updatePeriodBounds();

	[[nodiscard]] inline int absoluteThreshold() const;

	[[nodiscard]] inline float parabolicInterpolation (int periodEstimate) const;

	bool		   operator== (const PitchDetector& other) const = delete;
	bool		   operator!= (const PitchDetector& other) const = delete;
	PitchDetector& operator= (const PitchDetector& other) const = delete;
	PitchDetector (const PitchDetector& other)					= delete;

	int minHz { 60 };

	int minPeriod { 0 }, maxPeriod { 0 };

	float periodLastFrame { 0.f };

	double samplerate { 0.0 };

	SampleType confidenceThresh { 0.15 };

	vector<SampleType> yinDataStorage;
};

}  // namespace lemons::dsp::psola
