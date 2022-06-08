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
#include <limes_data_structures.h>
#include "../streams/SampleStream.h"
#include "analyzer.h"
#include "../util/Misc.h"
#include <limes_namespace.h>
#include <limes_core.h>

/** @file
	This file defines the Shifter class.
	@ingroup psola
 */

LIMES_BEGIN_NAMESPACE

namespace dsp::psola
{

/** A class that repitches a stream of monophonic audio using PSOLA-like techniques.
	This class is essentially a "client" of an Analyzer object, which allows multiple Shifters to be used simultaneously without requiring the expensive analysis process to be repeated.
	@ingroup psola
	@see Analyzer
 */
template <Sample SampleType>
class LIMES_EXPORT Shifter : public SampleStream<SampleType>
{
public:

	/** Creates a Shifter. The shifter will reference the specified Analyzer for the duration of its lifetime; make sure you don't delete the Analyzer before the Shifter!
	 */
	explicit Shifter (Analyzer<SampleType>& analyzerToUse);

	/** Destructor. */
	virtual ~Shifter();

	LIMES_DEFAULT_MOVABLE (Shifter)
	LIMES_DEFAULT_COPYABLE (Shifter)

	/** Sets the pitch, in Hz, of the shifter's output.
		Note that before calling this, you must set the samplerate of the algorithm using \c Analyzer::setSamplerate() !
	*/
	void setPitchHz (int pitchHz) noexcept;

	/** Returns the pitch, in Hz, this shifter is outputting. */
	[[nodiscard]] float getPitchHz() const noexcept;

	/** Resets the shifter to its initial state, without releasing any resources. Note that this is called for you if you call \c Analyzer::reset() . */
	void reset() noexcept;

	/** Releases all the resources used by the Shifter. Note that this is called for you if you call \c Analyzer::releaseResources() . */
	void releaseResources();

protected:

	/** The parent Analyzer object that this shifter is a client of. */
	Analyzer<SampleType>& analyzer;

private:

	friend class Analyzer<SampleType>;

	[[nodiscard]] SampleType getNextSample() noexcept;

	void newBlockStarting() noexcept;

	void samplerateChanged() noexcept;

	void latencyChanged (int newLatency);

	virtual void onNewBlock() { }
	virtual void pitchHzChanged (int /*pitchHz*/) { }

	/*-----------------------------------------------------------------------------------*/

	struct Grain final
	{
		using AnalysisGrain = typename Analyzer<SampleType>::Grain;

		Grain() = default;

		LIMES_DEFAULT_COPYABLE (Grain);
		LIMES_DEFAULT_MOVABLE (Grain);

		~Grain();

		[[nodiscard]] SampleType getNextSample() noexcept;

		[[nodiscard]] bool isActive() const noexcept;

		void clearGrain() noexcept;

		void startNewGrain (AnalysisGrain& analysisGrainToUse) noexcept;

	private:

		AnalysisGrain* analysisGrain { nullptr };

		int sampleIdx { 0 };
	};

	[[nodiscard]] Grain& getGrainToStart() noexcept;

	/*-----------------------------------------------------------------------------------*/

	float targetPeriod { 0.f };

	int samplesToNextGrain { 0 }, placeInBlock { 0 }, targetPitchHz { 0 };

	ds::owned_vector<Grain> grains;
};

}  // namespace dsp::psola

LIMES_END_NAMESPACE
