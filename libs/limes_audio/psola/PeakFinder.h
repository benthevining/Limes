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


namespace limes::dsp::psola
{

/** @defgroup lemons_psola lemons_psola
	@ingroup Audio
	Utilities for PSOLA pitch shifting of realtime audio signals.
 */

/** @defgroup lemons_psola_analysis Analysis
	@ingroup lemons_psola
	PSOLA analysis classes.
 */

/** @ingroup lemons_psola_analysis
	A class that identifies pitch peaks for PSOLA pitch shifting.
	You probably won't ever need to use this class directly, as it's mainly intended as a utility for the Analyzer class.
	@see Analyzer
 */
template <Sample SampleType>
class LIMES_EXPORT PeakFinder final
{
public:

	explicit PeakFinder() = default;

	/** Analyzes a stream of audio and identifies the best set of pitch peaks to use for PSOLA pitch shifting.
		The heuristics are that the grains should be approximately 2 periods long, with approximately 50% overlap, approximately centered on pitch peaks. This algorithm attempts to maximize all three criteria in the stream of selected peaks. To obtain the actual grains' start and end indices from the list of peak indices, you should do @code peak - period @endcode and @code peak + period @endcode, respectively, because the grains are 2 periods long and centered on the peaks.
	*/
	[[nodiscard]] const vector<int>& findPeaks (const SampleType* const inputSamples, int numSamples, float period);

	/** Prepares the analyzer for a new maximum blocksize. */
	void prepare (int maxBlocksize);

	/** Resets the analyzer to its initial state, without freeing any resources. */
	void reset();

	/** Releases all resources used by the analyzer. */
	void releaseResources();

private:

	bool		operator== (const PeakFinder& other) const = delete;
	bool		operator!= (const PeakFinder& other) const = delete;
	PeakFinder& operator= (const PeakFinder& other) = delete;
	PeakFinder (const PeakFinder& other)			= delete;

	[[nodiscard]] int findNextPeak (int frameStart, int frameEnd, int predictedPeak,
									const SampleType* const inputSamples, int period, int grainSize);

	void sortSampleIndicesForPeakSearching (int startSample, int endSample, int predictedPeak);

	[[nodiscard]] int getPeakCandidateInRange (const SampleType* const inputSamples, int startSample, int endSample, int predictedPeak) const;

	[[nodiscard]] int choosePeakWithGreatestPower (const SampleType* const inputSamples) const;

	[[nodiscard]] int chooseIdealPeakCandidate (const SampleType* const inputSamples, int deltaTarget1, int deltaTarget2);

	scalar_vector<int> peakIndices, peakSearchingOrder, peakCandidates, finalHandful, candidateDeltas, finalHandfulDeltas;

	const std::array<vector<int>*, 6> arrays { &peakIndices, &peakSearchingOrder, &peakCandidates, &finalHandful, &candidateDeltas, &finalHandfulDeltas };

	int analysisFrameStart { 0 };

	static constexpr auto numPeaksToTest = 15, defaultFinalHandfulSize = 5;
};

}  // namespace limes::dsp::psola
