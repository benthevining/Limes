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
#include <array>					// for array
#include <limes_data_structures.h>	// for vector
#include "../util/Misc.h"			// for concept Sample - IWYU pragma: keep
#include <limes_namespace.h>

/** @file
	This file defines the PeakFinder class.
	@ingroup psola
 */

LIMES_BEGIN_NAMESPACE

namespace dsp::psola
{

/** A class that identifies pitch peaks for PSOLA pitch shifting.
	You probably won't ever need to use this class directly, as it's mainly intended as a utility for the Analyzer class.

	Pitch peak locations are used to locate and extract from the original signal a series of "grains" which are then respaced by the Shifter class.

	Ultimately, the grains themselves are chosen using these heuristics:
	- Grains should be about 2 periods long
	- Grains should have about 50% overlap
	- Each grain should be about centered on the local maxima in the signal

	The set of grains that maximizes these criteria is chosen, and what is returned is a list of sample indices representing the *middle* sample of each grain (ie, the peak that the grain should be centered on).

	@see Analyzer
	@ingroup psola
 */
template <Sample SampleType>
class LIMES_NO_EXPORT PeakFinder final
{
public:

	/** Constructor. */
	PeakFinder() = default;

	/** Analyzes a stream of audio and identifies the best set of pitch peaks to use for PSOLA pitch shifting.
		The heuristics are that the grains should be approximately 2 periods long, with approximately 50% overlap, approximately centered on pitch peaks.
		This algorithm attempts to maximize all three criteria in the stream of selected peaks.
		To obtain the actual grains' start and end indices from the list of peak indices, you should do \c peak-period and \c peak+period , respectively, because the grains are 2 periods long and centered on the peaks.
	*/
	[[nodiscard]] const ds::scalar_vector<int>& findPeaks (const SampleType* const inputSamples, int numSamples, float period) noexcept;

	/** Prepares the analyzer for a new maximum blocksize.
		@note This function may allocate memory, and should not be called from a realtime thread.
	 */
	void prepare (int maxBlocksize);

	/** Resets the analyzer to its initial state, without freeing any resources.
		This function is safe to call from a realtime thread.
	 */
	void reset();

	/** Releases all resources used by the analyzer.
		@note This function may deallocate memory, and should not be called from a realtime thread.
	 */
	void releaseResources();

private:

	bool		operator== (const PeakFinder& other) const = delete;
	bool		operator!= (const PeakFinder& other) const = delete;
	PeakFinder& operator= (const PeakFinder& other) = delete;
	PeakFinder (const PeakFinder& other)			= delete;

	[[nodiscard]] int findNextPeak (int frameStart, int frameEnd, int predictedPeak,
									const SampleType* const inputSamples, int period, int grainSize) noexcept;

	void sortSampleIndicesForPeakSearching (int startSample, int endSample, int predictedPeak) noexcept;

	[[nodiscard]] int getPeakCandidateInRange (const SampleType* const inputSamples, int startSample, int endSample, int predictedPeak) const noexcept;

	[[nodiscard]] int choosePeakWithGreatestPower (const SampleType* const inputSamples) const noexcept;

	[[nodiscard]] int chooseIdealPeakCandidate (const SampleType* const inputSamples, int deltaTarget1, int deltaTarget2) noexcept;

	ds::scalar_vector<int> peakIndices, peakSearchingOrder, peakCandidates, finalHandful, candidateDeltas, finalHandfulDeltas;

	const std::array<ds::scalar_vector<int>*, 6> arrays { &peakIndices,
														  &peakSearchingOrder,
														  &peakCandidates,
														  &finalHandful,
														  &candidateDeltas,
														  &finalHandfulDeltas };

	int analysisFrameStart { 0 };

	static constexpr auto numPeaksToTest = 15, defaultFinalHandfulSize = 5;
};

}  // namespace dsp::psola

LIMES_END_NAMESPACE
