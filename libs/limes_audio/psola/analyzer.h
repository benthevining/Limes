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
#include <limes_data_structures.h>
#include "../util/PitchDetector.h"
#include "PeakFinder.h"
#include "../util/Misc.h"

namespace limes::dsp::psola
{

template <Sample SampleType>
class Shifter;

/** @ingroup lemons_psola_analysis
	A class that analyzes and stores a stream of audio, so that Shifter objects can repitch it.
	This process is separated so that multiple shifter instances can be used simultaneously while only needing to do the expensive analysis step once.
	Shifter objects reference an Analyzer for the duration of their lifetimes, and can be thought of as clients of the Analyzer -- things that affect the entire PSOLA algorithm, such as changing the samplerate, are only done through the Analyzer.
	This PSOLA algorithm is only suitable for monophonic pitched audio. If you want to shift more than one channel of audio, you'll need separate Analyzer and Shifter objects for each channel.
 */
template <Sample SampleType>
class LIMES_EXPORT Analyzer final
{
public:

	using SampleVector = scalar_vector<SampleType>;

	/** Creates an Analyzer with an initial minimum detectable frequency.
		@see setMinInputFreq()
	*/
	explicit Analyzer (int minFreqHz = 60);

	/**
		Analyzes a frame of audio. This must be called before Shifter::getSamples(), and should be called consistently.
		The caller must ensure that there are at least enough samples in this frame of audio for analysis to be performed; ie, that numSamples is greater than or equal to getLatencySamples().
	*/
	void analyzeInput (const SampleType* const inputAudio, int numSamples);

	/** Returns the latency in samples of the PSOLA algorithm. The latency is the same as the latency of the analyzer's internal PitchDetector object, so you can see that documentation for more details.
		@see setMinInputFreq(), dsp::PitchDetector
	*/
	[[nodiscard]] int getLatencySamples() const noexcept;

	/** Sets the samplerate, and informs any Shifter objects that are using this Analyzer.
		@returns The new latency, in samples, of the PSOLA algorithm at the new samplerate.
	*/
	int setSamplerate (double newSamplerate);

	/** Sets the minimum frequency detectable as pitched by the internal PitchDetector object. The latency of the PSOLA algorithm is equal to 2 times the period of the minimum detectable frequency; thus, setting a higher minimum input frequency will lower the latency of the algorithm.
		@returns The new latency, in samples, of the PSOLA algorithm with the new minimum frequency.
	*/
	int setMinInputFreq (int minFreqHz);

	/** Resets the analyzer to its initial state, without releasing any resources. This also calls reset() on all Shifter objects using this Analyzer. */
	void reset();

	/** Releases all resources used by the Analyzer object. This also calls releaseResources() on all Shifter objects using this Analyzer. */
	void releaseResources();

	/** Returns the last input pitch, in Hz, that the analyzer detected, or 0 if the last frame was unpitched or no audio has been analyzed yet. */
	[[nodiscard]] int getLastInputPitch() const noexcept;

private:

	friend class Shifter<SampleType>;

	void registerShifter (Shifter<SampleType>& shifter);

	void deregisterShifter (Shifter<SampleType>& shifter);

	[[nodiscard]] int latencyChanged();

	void makeWindow (int size);

	/*-----------------------------------------------------------------------------------*/

	struct Grain final : public ReferenceCountedObject
	{
		[[nodiscard]] SampleType getSample (int index) const noexcept;

		[[nodiscard]] int getSize() const noexcept;

		[[nodiscard]] int getOrigStart() const noexcept;

		void newBlockStarting (int last_blocksize) noexcept;

		void storeNewGrain (const SampleType* const origSamples, int startIndex, const SampleVector& windowSamples, int numSamples);

		void storeNewGrain (const SampleType* const origSamples1, int startIndex1, int blocksize1,
							const SampleType* const origSamples2, int blocksize2,
							const SampleVector& windowSamples, int totalNumSamples, int grainStartIdx);

		void storeNewGrainWithZeroesAtStart (int					 numZeroes,
											 const SampleType* const origSamples, int numSamples,
											 const SampleVector& windowSamples, int totalNumSamples, int grainStartIdx);

		void reserveSize (int numSamples);

		void clearGrain();

	private:

		int origStartIndex { 0 }, grainSize { 0 };

		SampleVector samples;
	};

	[[nodiscard]] Grain& getClosestGrain (int placeInBlock);

	[[nodiscard]] Grain& getGrainToStoreIn();

	/*-----------------------------------------------------------------------------------*/

	float currentPeriod { 0.f };

	PitchDetector<SampleType> pitchDetector;
	PeakFinder<SampleType>	  peakFinder;

	owned_vector<Grain> grains;

	SampleVector window, prevFrame;

	vector<int> incompleteGrainsFromLastFrame;

	int lastBlocksize { 0 }, lastFrameGrainSize { 0 };

	double samplerate { 0. };

	// juce::Random random;

	vector<Shifter<SampleType>*> shifters;
};

}  // namespace limes::dsp::psola
