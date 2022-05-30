
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

#include <limes_export.h>			// for LIMES_EXPORT
#include <limes_data_structures.h>	// for vector
#include "Misc.h"					// for concept Sample - IWYU pragma: keep
#include <limes_namespace.h>
#include <limes_core.h>

/** @defgroup psola PSOLA
	Utilities for working with PSOLA-like algorithms.

	PSOLA (Pitch-Synchronous Overlap-Add) is an industry standard for realtime pitch shifting of monophonic audio.
	This module provides classes for implementing a basic PSOLA algorithm.

	@ingroup limes_audio
 */

/** @file
	This file defines the PitchDetector class.
	@ingroup psola
 */

LIMES_BEGIN_NAMESPACE

/** This namespace provides classes for implementing a PSOLA algorithm.
	@ingroup psola
 */
namespace dsp::psola
{

/** A pitch detector based on the YIN algorithm.

	A pitch detection algorithm based on the influential YIN @cite de_cheveigné_kawahara_2002 paper, with a few alterations of my own.

	@section algorithm The algorithm

	@subsection difference The difference function

	The core of the algorithm is the difference function, defined as:

	@f[
		d_t(\tau)=\sum\limits_{j=t}^{j<N-\tau} (x_j-x_{j+\tau})^2
	@f]
	@f[
		D(\tau)=\sum\limits_{t=0}^{t<N} d_t(\tau)
	@f]

	where:
	- @f$ d_t(\tau) @f$ is the autocorrelation function at lag @f$ \tau @f$ of input signal @f$ x @f$ at sample @f$ t @f$;
	- @f$ D(\tau) @f$ is the summation of all values @f$ d_t(\tau) @f$ for each sample index @f$ t @f$ in the current frame of audio of length @f$ N @f$.

	This autocorrelation function is calculated for every lag @f$ \tau @f$ within the set of possible period values, and the @f$ \tau @f$ value whose autocorrelation function
	shows the lowest amount of difference between the original and delayed signal (ie, the lowest value of @f$ D(\tau) @f$) is determined to be the period.

	@subsection cmndf The cumulative mean normalized difference function

	To attempt to stabilize results for different integration window sizes across a frame of audio, the YIN paper introduces the concept of a cumulative mean normalized difference function, which is defined as:

	@f[
		d\prime_t(\tau)=\frac{d_t(\tau)}{(1/\tau)\sum\limits_{i=1}^{\tau}d_t(i)}
	@f]

	Essentially, the result of the original difference function is divided by its average over shorter-lag values.
	The final summation step then becomes:

	@f[
		D(\tau)=\sum\limits_{t=0}^{t<N} \frac{d_t(\tau)}{(1/\tau)\sum\limits_{i=1}^{\tau}d_t(i)}
	@f]

	@subsection thresh Absolute threshold

	Once the cumulative mean normalized difference function has been applied, the @f$ D(\tau) @f$ values are suitably normalized, allowing us to apply a simple threshold to the entire set of @f$ D(\tau) @f$
	values for each @f$ \tau @f$ in the range of possible periods.

	This threshold value can be thought of as the amount of aperiodic power tolerable in a signal determined to be pitched.
	In other words, the lower this value is, the "stricter" the pitch detector is in choosing its pitch estimates.
	When this value is 1, the pitch detector might output a "pitch" value for a frame of random noise; when this value is 0, the pitch detector might say that even a perfect, pure sine wave is "unpitched".
	Typical recommended values are between 0.1 and 0.3.

	The YIN paper recommends to implement this threshold such that the smallest value of @f$ D(\tau) @f$ is chosen that gives a minimum of @f$ d\prime_t(\tau) @f$ that is lower than our threshold.
	In other words, we choose the smallest period that satisfies our periodicity threshold.
	The exact value of this threshold does not critically affect error rates, though it can be tuned for specific use cases to provide more optimal results.

	@subsection interp Parabolic interpolation

	Finally, once we've determined an integer estimate of @f$ \tau @f$ that is the best candidate to be the period, parabolic interpolation is used to determine the actual period (which probably is not an even number of samples).

	Interpolation is done by looking at local minima of the set of @f$ D(\tau) @f$ values around our candidate @f$ \tau @f$, and the abscissa of the chosen minimum becomes the actual period estimate.
	To avoid any biasing in this interpolation step, the *raw* difference function data is used -- @f$ d_t(\tau) @f$, not @f$ d\prime_t(\tau) @f$.

	@section Discussion Discussion

	This pitch detection algorithm is performed entirely in the time domain.

	The detector is stateful; it assumes that the input pitch shouldn't halve or double between consecutive pitched frames of audio,
	and uses this heuristic to limit its period search range for each frame of audio.
	Limiting the number of possible period candidates reduces the number of times the difference function needs to be calculated.

	Pitch detector objects can only be used for one channel of audio at a time, because they are stateful.
	If you need to run pitch detection on multiple channels of audio, you need multiple PitchDetector objects.

	@section Latency Latency

	The latency of the algorithm is determined by 2 times the period of the lowest detectable frequency;
	therefore, detectors configured to not try to detect lower pitches will have less latency.
	You can adjust the lowest detectable frequency of the pitch detector to fit your needs.

	This class does not do any internal buffering; the caller must ensure that the buffers sent to the detection functions contain enough samples for the detection algorithm.

	@ingroup psola
 */
template <Sample SampleType>
class LIMES_EXPORT PitchDetector final
{
public:

	using SampleVector = ds::scalar_vector<SampleType>;

	/** Creates a pitch detector with an initial minimum detectable frequency and confidence threshold.
		The latency of the algorithm is determined by 2 * the period of the minimum frequency.
		Therefore, pitch detectors with a higher minimum frequency will have a lower latency.
		@param minFreqHz The minimum frequency that this pitch detector will be able to detect.
		@param confidenceThreshold The initial YIN confidence threshold for the detector. See the \c setConfidenceThresh() method for more documentation on this.
		@see getLatencySamples(), setConfidenceThresh(), setMinHz()
	*/
	explicit PitchDetector (int minFreqHz = 60, float confidenceThreshold = 0.15f);

	LIMES_DEFAULT_MOVABLE (PitchDetector);
	LIMES_DEFAULT_COPYABLE (PitchDetector);

	/** @name Pitch detection */
	///@{

	/** Detects the pitch in Hz for a frame of audio.
		This can only be used for one channel at a time. If you need to track the pitch of multiple channels of audio, you need one PitchDetector object for each channel.
		The caller must ensure that there are at least enough samples in this frame of audio for analysis to be performed; ie, that numSamples is greater than or equal to \c getLatencySamples().
		@note You must call \c setSamplerate() to prepare the pitch detector before calling this function!
		@return The pitch in Hz for this frame of audio, or 0 if the frame is unpitched.
	*/
	[[nodiscard]] float detectPitch (const SampleType* const inputAudio, int numSamples);
	[[nodiscard]] float detectPitch (const SampleVector& inputAudio);
	///@}

	/** @name Period detection */
	///@{
	/** Detects the period, in samples, for a frame of audio.
		This can only be used for one channel at a time. If you need to track the pitch of multiple channels of audio, you need one PitchDetector object for each channel.
		The caller must ensure that there are at least enough samples in this frame of audio for analysis to be performed; ie, that numSamples is greater than or equal to \c getLatencySamples().
		@note You must call \c setSamplerate() to prepare the pitch detector before calling this function!
		@return The period, in samples, of the fundamental frequency for this frame of audio, or 0 if the frame is unpitched.
	*/
	[[nodiscard]] float detectPeriod (const SampleType* const inputAudio, int numSamples);
	[[nodiscard]] float detectPeriod (const SampleVector& inputAudio);
	///@}

	/** Returns the latency in samples of the detection algorithm.
		The latency is equal to 2 * the period of the lowest detectable frequency. Therefore, pitch detectors with a higher minimum frequency will have a lower latency.
		@see setMinHz()
	*/
	[[nodiscard]] int getLatencySamples() const noexcept;

	/** Sets the samplerate in Hz of the pitch detector.
		@note This function may allocate memory, so should not be called from a realtime thread.
		@return The latency, in samples, of the pitch detection algorithm at the new samplerate.
		@see getLatencySamples()
	*/
	int setSamplerate (double newSamplerate);

	/** Sets the minimum detectable frequency for the pitch detector.
		The latency of the algorithm is determined by 2 * the period of the minimum frequency.
		Therefore, pitch detectors with a higher minimum frequency will have a lower latency.
		@note The pitch detector may report a frequency slightly lower than this, due to its usage of interpolation to attempt to find the most accurate period.
		@note This function may allocate memory, so should not be called from a realtime thread.
		@return The latency, in samples, of the pitch detection algorithm with the new minimum frequency.
	*/
	int setMinHz (int newMinHz);

	/** Returns the minimum frequency in Hz that this pitch detector is configured to detect. */
	[[nodiscard]] int getMinHz() const noexcept;

	/** Sets the confidence threshold of the pitch detection algorithm.
		This value should be between 0 and 1, inclusive, and can be thought of as the amount of aperiodic power tolerable in a signal determined to be pitched.
		In other words, the lower this value is, the "stricter" the pitch detector is in choosing its pitch estimates.
		When this value is 1, the pitch detector might output a "pitch" value for a frame of random noise; when this value is 0, the pitch detector might say that even a perfect, pure sine wave is "unpitched".
		Typical recommended values are between 0.1 and 0.3.
	*/
	void setConfidenceThresh (float newThresh) noexcept;

	/** Resets the internal state of the pitch detector without releasing any resources.
		The pitch detector assumes that the input pitch will not halve or double between consecutive pitched frames of audio,
		so if your input audio is expected to make jumps that large in its pitch, you can call reset() before analyzing the next input frame of audio.
	*/
	void reset();

	/** Releases all of the pitch detector's internal resources.
		After calling this function, you must call \c setSamplerate() to re-prepare the detector before calling any of the pitch detection functions.
		This function may deallocate memory, so should not be called from a realtime thread.
	 */
	void releaseResources();

	/** Returns the current legal period range for the last analyzed frame of audio.
		Because the detector assumes that the input pitch should not halve or double between consecutive pitched frames, this can be used to introspect the range of valid periods that the detector has actually considered for the most recent frame.
		Note that you must call one of the pitch detection functions before calling this function.
	*/
	void getCurrentLegalPeriodRange (int& min, int& max) const;

private:

	inline void updatePeriodBounds();

	[[nodiscard]] inline int absoluteThreshold() const;

	[[nodiscard]] inline float parabolicInterpolation (int periodEstimate) const;

	bool operator== (const PitchDetector& other) const = delete;
	bool operator!= (const PitchDetector& other) const = delete;

	int minHz { 60 };

	int minPeriod { 0 }, maxPeriod { 0 };

	float periodLastFrame { 0.f };

	double samplerate { 0.0 };

	SampleType confidenceThresh { 0.15 };

	SampleVector yinDataStorage;
};

}  // namespace dsp::psola

LIMES_END_NAMESPACE
