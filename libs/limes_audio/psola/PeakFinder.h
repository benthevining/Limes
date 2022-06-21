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

#include <limes_export.h>  // for LIMES_EXPORT
#include <array>		   // for array
#include "../util/Misc.h"  // for concept Sample - IWYU pragma: keep
#include <limes_namespace.h>

/** @defgroup psola PSOLA
	Utilities for working with PSOLA-like algorithms.

	PSOLA (Pitch-Synchronous Overlap-Add) is an industry standard for realtime pitch shifting of monophonic audio.
	This module provides classes for implementing a basic PSOLA algorithm.

	PSOLA was first described by Keith Lent @cite lent_1989 as a computationally efficient method of repitching pseudo-periodic sampled sounds.
	PSOLA works using the following steps:
	- identify the pitch of the input signal - see the PitchDetector class
	- based on this pitch, identify peaks in the signal for each period - see the PeakFinder class
	- based on these peaks, split the input signal into grains centred on the peaks - see the Analyzer class
	- output a new stream of these grains, respaced to form the new desired pitch - see the Shifter class

	Robert Bristow Johnson's paper @cite bristow-johnson_1995 provides a great overview of the technical details of PSOLA.

	PSOLA can be used for both pitch and time scaling.

	PSOLA is only intended for use with input signals that are monophonic - ie, a sound with a single fundamental frequency, like a voice or a cello, not a group of instruments or a chord.
	Results may be unpredictable if the input signal contains more than one pitch.
	Best results are achieved when the input signal is clearly pitched, though this algorithm can be used with any kind of monophonic input signal, even unpitched percussive sounds.
	It is often desirable to do some preprocessing of the input signal, to remove noise, perhaps even apply some compression, etc.

	Note that the Analyzer class performs the first three steps listed above; to achieve PSOLA pitch shifting, you only need an Analyzer and a Shifter object, you don't need to use a PitchDetector or PeakFinder directly.

	@ingroup limes_audio
 */

/** @dir libs/limes_audio/psola
	This directory contains classes for implementing a PSOLA algorithm.
	@ingroup psola
 */

/** @file
	This file defines the PeakFinder class.
	@ingroup psola
 */

LIMES_BEGIN_NAMESPACE

namespace dsp::psola
{

/** @class PeakFinder
	A class that identifies pitch peaks for PSOLA pitch shifting.
	You probably won't ever need to use this class directly, as it's mainly intended as a utility for the Analyzer class.

	Pitch peak locations are used to locate and extract from the original signal a series of "grains" which are then respaced by the Shifter class.

	Ultimately, the grains themselves are chosen using these heuristics:
	- Grains should be about 2 periods long
	- Grains should have about 50% overlap
	- Each grain should be about centered on the local maxima in the signal

	The set of grains that maximizes these criteria is chosen, and what is returned is a list of sample indices representing the *middle* sample of each grain (ie, the peak that the grain should be centered on).

	@section peak_alg The algorithm

	This is a completely custom algorithm based on several sources, including @cite chalamandaris_tsiakoulis_karabetsos_raptis_2009 and @cite colotte_laprie .

	Peaks are searched for in the input signal in a sequence of overlapping analysis windows, which are 1 period long and overlap by 50% --
	so analysis window #2 begins at sample @f$ \frac{\tau}{2} @f$, analysis window #3 begins at sample @f$ \tau @f$, etc, until the entire frame of audio has been covered by an analysis window.
	One peak is chosen within each analysis window.

	This windowing scheme is used because the resulting grains used in the pitch shifting are meant to be 2 periods long, overlapping by 50% --
	so if we identify a peak in the signal roughly once per period, then if we center 2-period long grains on each of these peaks, these grains should end up overlapping one another by roughly 50%.

	@subsection peak_cand_sel Peak candidate selection

	For each analysis window, its *predicted peak* location is the sample index exactly 1 period greater than the previous analysis window's chosen peak:
	@f[
		pidx_t=idx\prime+\tau
	@f]
	where:
	- @f$ idx\prime @f$ is the chosen peak index for the previous analysis window;
	- @f$ \tau @f$ is the period of this frame of audio.

	However, the peak chosen for this analysis window may not be exactly at this location, if the signal demonstrates significant energy in a slightly different area.

	Therefore, a set of *peak candidates* for this analysis window are identified by weighting the original sample values based on how close the sample is to the *predicted peak* location,
	and then choosing the set of sample indices whose weighted sample values have the greatest absolute values.

	The set of peak candidates for the analysis window beginning at sample @f$ t @f$ is defined as:
	@f[
		C_t=\{\, \max{(1-\frac{\lvert pidx_t-i\rvert}{N_t})\lvert x_i\rvert} \notin C_t : |C_t|\le 15 \,\}
	@f]
	where:
	- @f$ i @f$ is the sample index in the original audio signal which is being evaluated as a potential peak candidate;
	- @f$ N_t @f$ is the number of samples in this analysis window.

	This algorithm selects 15 initial peak candidates within each analysis window. This number is somewhat arbitrary.
	Fewer than 15 candidates may be chosen if only a small set of candidates have significant signal power close to the predicted peak.

	@subsection peak_can_eval Peak candidate evaluation

	Once the set of peak candidates @f$ C_t @f$ has been identified, the actual location of the peak for this analysis window is chosen using a more involved weighting scheme.

	Because the actual grains created from the selected peaks are *two periods long*, I take into account not only the distance from the previous peak to each candidate,
	but also the distance to the peak *before* the previous peak.

	For each peak candidate, we now have *two* predicted peak locations based on previously chosen peaks:
	@f[
		pidx_t=idx\prime+\tau
	@f]
	@f[
		pidx\prime_t=idx''+2\tau
	@f]
	where:
	- @f$ idx\prime @f$ is the chosen peak index for the previous analysis window;
	- @f$ \tau @f$ is the period of this frame of audio;
	- @f$ idx'' @f$ is the chosen peak for the analysis frame *before* the previous one.

	For each peak candidate, a delta value is calculated which represents its amount of jitter from both of these predicted peaks:
	@f[
		\delta_c=\lvert c-pidx_t\rvert+\lvert c-pidx\prime_t\rvert
	@f]
	where @f$ c @f$ is the sample index of the peak candidate being tested.

	The final peak is chosen from the candidates by again comparing weighted sample values, this time with the samples weighted by the candidates' @f$ \delta_c @f$ values.
	The weighted sample value for peak candidate @f$ c @f$ is defined by:
	@f[
		w(c)=\lvert x_c\rvert(1-\frac{\delta_c}{\delta_R})
	@f]
	where:
	- @f$ x_c @f$ is the value of the sample at index @f$ c @f$ in the original input signal;
	- @f$ \delta_R @f$ is the range between the minimum and maximum @f$ \delta_c @f$ values of any remaining peak candidate.

	The peak candidate with the highest resulting @f$ w(c) @f$ value is chosen as the peak for this analysis window.

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
	[[nodiscard]] const std::vector<int>& findPeaks (const SampleType* const inputSamples, int numSamples, SampleType period) noexcept;

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

	std::vector<int> peakIndices, peakCandidates, candidateDeltas;

	const std::array<std::vector<int>*, 3> arrays { &peakIndices,
													&peakCandidates,
													&candidateDeltas };

	int analysisFrameStart { 0 };

	// these are NEGATIVE numbers storing for each of the last 2 peaks from the previous frame
	// the number of samples from that peak to the end of the frame
	// this is used to space out the first peak in the current frame appropriately
	int lastPeak { 0 }, peakBeforeLast { 0 };
};

}  // namespace dsp::psola

LIMES_END_NAMESPACE
