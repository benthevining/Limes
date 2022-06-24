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
#include <limes_core.h>
#include "../pitch_detection/PitchDetector.h"
#include "./PeakFinder.h"
#include "../util/Misc.h"
#include "../util/AudioBuffer.h"
#include <limes_namespace.h>

/** @file
	This file defines the Analyzer class.
	@ingroup psola
 */

LIMES_BEGIN_NAMESPACE

namespace dsp::psola
{

template <Sample SampleType>
class Shifter;

/** @class Analyzer
	This class analyzes and stores a stream of audio, so that Shifter objects can repitch it.

	This process is separated so that multiple shifter instances can be used simultaneously while only needing to do the expensive analysis step once.
	Shifter objects reference an Analyzer for the duration of their lifetimes, and can be thought of as clients of the Analyzer -- things that affect the entire PSOLA algorithm, such as changing the samplerate, are only done through the Analyzer.

	This class contains both a PitchDetector and a PeakFinder and calls them in sequence.
	For each block of input audio sent into the Analyzer, it performs these steps:
	- Detect the pitch of the input audio
	- Identify peaks in the signal for each period
	- Store grains created from the incoming audio signal, such that each grain's start sample is \c peak-period and its end sample is \c peak+period

	This PSOLA algorithm is only suitable for monophonic pitched audio. If you want to shift more than one channel of audio, you'll need separate Analyzer and Shifter objects for each channel.

	The latency of the PSOLA algorithm is equal to 2 times the period of the minimum detectable frequency; thus, setting a higher minimum input frequency will lower the latency of the algorithm.

	@section analyzer_alg The algorithm

	@subsection analyzer_alg_grains Grain identification & storage

	Given the set of peak indices @f$ P @f$ output by the PeakFinder object, the beginning and ending of each grain in the input audio is defined as:
	@f[
		b_i=P_i-\tau
	@f]
	@f[
		e_i=P_i+\tau
	@f]
	where:
	- @f$ b_i @f$ is the start sample index of this grain in the original audio stream
	- @f$ e_i @f$ is the end sample index of this grain in the original audio stream
	- @f P_i @f$ is the peak chosen for this grain (that it is centered on)
	- @f$ \tau @f$ is the period of this frame of input audio

	These grains are stored by the Analyzer object, and are overwritten once no Shifters are playing them anymore.

	In the event that a grain's ending index is "off the end" of the input buffer, as many samples as possible will be stored from the current frame of audio,
	and the remainder of the grain will be populated with the next samples to be input to the Analyzer.

	@subsection analyzer_grain_sel Grain selection

	When Shifter objects need to start playing back a new grain of audio, they "ask" the Analyzer for a grain to reference.
	The Analyzer finds the best grain for the Shifter to play by examining the current place in the output block, and comparing it to each stored grain's original place in the input block.
	The Analyzer returns the grain whose original onset is the closest to the current position in the output block.

	@ingroup psola
	@see Shifter

	@todo write unit tests
 */
template <Sample SampleType>
class LIMES_EXPORT Analyzer final
{
public:

	/** Convenience typedef for a vector of samples. */
	using Buffer = AudioBuffer<SampleType, 1>;

	/** Creates an Analyzer with an initial minimum detectable frequency.
		The latency of the PSOLA algorithm is equal to 2 times the period of the minimum detectable frequency; thus, setting a higher minimum input frequency will lower the latency of the algorithm.
		@see setMinInputFreq()
	*/
	explicit Analyzer (int minFreqHz = 60);

	LIMES_NON_COPYABLE (Analyzer)
	LIMES_DEFAULT_MOVABLE (Analyzer)

	/** @name Performing analysis */
	///@{
	/**
		Analyzes a frame of audio. This must be called before \c Shifter::getSamples() , and should be called consistently.
		The caller must ensure that there are at least enough samples in this frame of audio for analysis to be performed; ie, that numSamples is greater than or equal to \c getLatencySamples() .
		@see getLatencySamples()
	*/
	void analyzeInput (const SampleType* const inputAudio, int numSamples) noexcept;
	void analyzeInput (const Buffer& input) noexcept;
	///@}

	/** Returns the latency in samples of the PSOLA algorithm.
		The latency is the same as the latency of the analyzer's internal PitchDetector object, so you can see that documentation for more details.
		@see setMinInputFreq(), PitchDetector
	*/
	[[nodiscard]] int getLatencySamples() const noexcept;

	/** Sets the samplerate, and informs any Shifter objects that are using this Analyzer.
		@note This function may allocate memory, and should not be called from a realtime thread.
		@returns The new latency, in samples, of the PSOLA algorithm at the new samplerate.
	*/
	int setSamplerate (double newSamplerate);

	/** Sets the minimum frequency detectable as pitched by the internal PitchDetector object.
		The latency of the PSOLA algorithm is equal to 2 times the period of the minimum detectable frequency; thus, setting a higher minimum input frequency will lower the latency of the algorithm.
		@note This function may allocate memory, and should not be called from a realtime thread.
		@returns The new latency, in samples, of the PSOLA algorithm with the new minimum frequency.
	*/
	int setMinInputFreq (int minFreqHz);

	/** Resets the analyzer to its initial state, without releasing any resources.
		This also calls reset() on all Shifter objects using this Analyzer.
		This function is realtime-safe.
	 */
	void reset() noexcept;

	/** Releases all resources used by the Analyzer object.
		This also calls releaseResources() on all Shifter objects using this Analyzer.
		@note This function may deallocate memory and should not be called from a realtime thread.
	 */
	void releaseResources();

	/** Returns the last input pitch, in Hz, that the analyzer detected, or 0 if the last frame was unpitched or no audio has been analyzed yet. */
	[[nodiscard]] int getLastInputPitch() const noexcept;

private:

	friend class Shifter<SampleType>;

	void registerShifter (Shifter<SampleType>& shifter);

	void deregisterShifter (Shifter<SampleType>& shifter);

	[[nodiscard]] int latencyChanged();

	void makeWindow (int size) noexcept;

	/*-----------------------------------------------------------------------------------*/

	struct Grain final : public memory::ReferenceCountedObject
	{
		[[nodiscard]] SampleType getSample (int index) const noexcept;

		[[nodiscard]] int getSize() const noexcept;

		[[nodiscard]] int getOrigStart() const noexcept;

		void newBlockStarting (int last_blocksize) noexcept;

		void storeNewGrain (const SampleType* const origSamples, int startIndex, const Buffer& windowSamples, int numSamples) noexcept;

		void storeNewGrain (const SampleType* const origSamples1, int startIndex1, int blocksize1,
							const SampleType* const origSamples2, int blocksize2,
							const Buffer& windowSamples, int totalNumSamples, int grainStartIdx) noexcept;

		void storeNewGrainWithZeroesAtStart (int					 numZeroes,
											 const SampleType* const origSamples, int numSamples,
											 const Buffer& windowSamples, int totalNumSamples, int grainStartIdx) noexcept;

		void reserveSize (int numSamples);

		void clearGrain();

	private:

		std::size_t origStartIndex { 0 }, grainSize { 0 };

		Buffer samples;
	};

	[[nodiscard]] Grain& getClosestGrain (int placeInBlock) noexcept;

	[[nodiscard]] Grain& getGrainToStoreIn() noexcept;

	/*-----------------------------------------------------------------------------------*/

	SampleType currentPeriod { 0.f };

	pitch::PitchDetector<SampleType> pitchDetector;
	PeakFinder<SampleType>			 peakFinder;

	std::vector<Grain> grains;

	Buffer window, prevFrame;

	std::vector<int> incompleteGrainsFromLastFrame;

	int lastBlocksize { 0 }, lastFrameGrainSize { 0 };

	double samplerate { 0. };

	math::Random random;

	std::vector<Shifter<SampleType>*> shifters;
};

}  // namespace dsp::psola

LIMES_END_NAMESPACE
