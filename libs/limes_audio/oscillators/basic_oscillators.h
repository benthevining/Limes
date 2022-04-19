
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

#include <functional>				// for function
#include <limes_export.h>			// for LIMES_EXPORT
#include "../streams/oscillator.h"	// for Oscillator<>::Phase, Oscillator
#include "../util/Misc.h"			// for concept Sample - IWYU pragma: keep
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace dsp::osc
{

template <Sample SampleType>
struct Triangle;

template <Sample T>
using Oscillator = dsp::Oscillator<T>;

/** @defgroup lemons_oscillators Oscillators
	@ingroup lemons_dsp
	%Oscillator classes.
 */


/** @defgroup lemons_basic_oscillators Basic wave shapes
	@ingroup lemons_oscillators
	Basic oscillator necessities.
 */

/** @ingroup lemons_basic_oscillators
	A basic sine wave oscillator.
	@see Oscillator, Phase
 */
template <Sample SampleType>
struct LIMES_EXPORT Sine final : public Oscillator<SampleType>
{
	/** Constructs a default sine oscillator. */
	explicit Sine();

	/** Resets the sine wave's phase. */
	void resetPhase() final;

	/** Sets the output frequency and samplerate of the sine wave. */
	void setFrequency (SampleType frequency, SampleType sampleRate) final;

	/** Returns the sine wave's frequency. */
	[[nodiscard]] SampleType getFrequency() const noexcept final;

private:

	typename Oscillator<SampleType>::Phase phase;
	SampleType							   freq { 0 };
};

/*--------------------------------------------------------------------------------------------*/

/** @ingroup lemons_basic_oscillators
	A basic sawtooth wave oscillator.
	@see SuperSaw, Oscillator, Phase
 */
template <Sample SampleType>
struct LIMES_EXPORT Saw final : public Oscillator<SampleType>
{
	/** Constructs a default sawtooth oscillator. */
	explicit Saw();

	/** Resets the sawtooth wave's phase. */
	void resetPhase();

	/** Sets the output frequency and samplerate of the sawtooth wave. */
	void setFrequency (SampleType frequency, SampleType sampleRate) final;

	/** Returns the sawtooth wave's frequency. */
	[[nodiscard]] SampleType getFrequency() const noexcept final;

private:

	typename Oscillator<SampleType>::Phase phase;
	SampleType							   freq { 0 };
};

/*--------------------------------------------------------------------------------------------*/


/** @ingroup lemons_basic_oscillators
	A basic square wave oscillator.
	@see Oscillator, Phase
 */
template <Sample SampleType>
struct LIMES_EXPORT Square final : public Oscillator<SampleType>
{
	/** Consructs a default square wave oscillator. */
	explicit Square();

	/** Resets the square wave's phase. */
	void resetPhase() final;

	/** Sets the square wave's output frequency and samplerate. */
	void setFrequency (SampleType frequency, SampleType sampleRate) final;

	/** Returns the square wave's frequency. */
	[[nodiscard]] SampleType getFrequency() const noexcept final;

private:

	friend struct Triangle<SampleType>;

	typename Oscillator<SampleType>::Phase phase;
	SampleType							   freq { 0 };
};

/*--------------------------------------------------------------------------------------------*/

/** @ingroup lemons_basic_oscillators
	A basic triangle wave oscillator.
	@see Square, Oscillator, Phase
 */
template <Sample SampleType>
struct LIMES_EXPORT Triangle final : public Oscillator<SampleType>
{
	/** Constructs a default triangle wave. */
	explicit Triangle();

	/** Resets the triangle wave's phase. */
	void resetPhase() final;

	/** Sets the triangle wave's output frequency and samplerate. */
	void setFrequency (SampleType frequency, SampleType sampleRate) final;

	/** Returns the square wave's frequency. */
	[[nodiscard]] SampleType getFrequency() const noexcept final;

private:

	Square<SampleType> square;
	SampleType		   sum { 1 };
	SampleType		   freq { 0 };
};

}  // namespace dsp::osc

LIMES_END_NAMESPACE
