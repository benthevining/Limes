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

#include <limes_export.h>	 // for LIMES_EXPORT
#include "./SampleStream.h"	 // for SampleStream
#include "../util/Misc.h"	 // for concept Sample - IWYU pragma: keep
#include <limes_namespace.h>

/** @defgroup oscillators Oscillators
	Sample streams that produce a pitched, periodic output.
	@ingroup samplestreams
 */

/** @file
	This file defines the Oscillator class.
	@ingroup oscillators
 */

LIMES_BEGIN_NAMESPACE

namespace dsp
{

/** Base class for any kind of oscillator.
	Oscillators process only a single channel of samples at a time.
	@ingroup oscillators
 */
template <Sample SampleType>
struct LIMES_EXPORT Oscillator : public SampleStream<SampleType>
{
	/** You should provide the Oscillator constructor with a lambda for producing the next sample, identical to the SampleStream constructor. */
	using SampleStream<SampleType>::SampleStream;

	/** Resets the oscillator's phase. */
	virtual void resetPhase() = 0;

	/** Sets the oscillator's output frequency and samplerate. */
	virtual void setFrequency (SampleType frequency, SampleType sampleRate) = 0;

	/** Returns the oscillator's frequency. */
	[[nodiscard]] virtual SampleType getFrequency() const noexcept = 0;

	/** Represents the phase of an oscillator. */
	struct LIMES_EXPORT Phase final
	{
		/** Resets the phase, */
		void resetPhase() noexcept;

		/** Sets the output frequency and samplerate. */
		void setFrequency (SampleType frequency, SampleType sampleRate);

		/** Returns the current phase increment. */
		[[nodiscard]] SampleType getIncrement() const noexcept;

		/** Returns the next phase value and handles wraparound logic. */
		[[nodiscard]] SampleType next (SampleType wrapLimit) noexcept;

	private:
		SampleType phase { 0 }, increment { 0 };
	};
};

}  // namespace dsp

LIMES_END_NAMESPACE
