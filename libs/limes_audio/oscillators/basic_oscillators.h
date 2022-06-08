
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
#include <limes_core.h>

/** @dir libs/limes_audio/oscillators
	This directory contains some basic oscillator classes.
	@ingroup oscillators
 */

/** @file
	This file defines basic oscillator classes.
	@ingroup oscillators
 */

LIMES_BEGIN_NAMESPACE

/** This namespace contains basic oscillator classes.
	@ingroup oscillators
 */
namespace dsp::osc
{

template <Sample SampleType>
struct Triangle;

/** Convenience typedef for the oscillator base class. */
template <Sample T>
using Oscillator = dsp::Oscillator<T>;

#pragma mark Sine

/** A basic sine wave oscillator.
	@see Oscillator
	@ingroup oscillators
 */
template <Sample SampleType>
struct LIMES_EXPORT Sine final : public Oscillator<SampleType>
{
	/** Constructs a default sine oscillator. */
	explicit Sine();

	LIMES_DEFAULT_MOVABLE (Sine)
	LIMES_DEFAULT_COPYABLE (Sine)

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

#pragma mark Saw

/** A basic sawtooth wave oscillator.
	@see Oscillator
	@ingroup oscillators
 */
template <Sample SampleType>
struct LIMES_EXPORT Saw final : public Oscillator<SampleType>
{
	/** Constructs a default sawtooth oscillator. */
	explicit Saw();

	LIMES_DEFAULT_MOVABLE (Saw)
	LIMES_DEFAULT_COPYABLE (Saw)

	/** Resets the sawtooth wave's phase. */
	void resetPhase() final;

	/** Sets the output frequency and samplerate of the sawtooth wave. */
	void setFrequency (SampleType frequency, SampleType sampleRate) final;

	/** Returns the sawtooth wave's frequency. */
	[[nodiscard]] SampleType getFrequency() const noexcept final;

private:

	typename Oscillator<SampleType>::Phase phase;
	SampleType							   freq { 0 };
};

/*--------------------------------------------------------------------------------------------*/

#pragma mark Square

/** A basic square wave oscillator.
	@see Oscillator
	@ingroup oscillators
 */
template <Sample SampleType>
struct LIMES_EXPORT Square final : public Oscillator<SampleType>
{
	/** Consructs a default square wave oscillator. */
	explicit Square();

	LIMES_DEFAULT_MOVABLE (Square)
	LIMES_DEFAULT_COPYABLE (Square)

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

#pragma mark Triangle

/** A basic triangle wave oscillator.
	@see Oscillator
	@ingroup oscillators
 */
template <Sample SampleType>
struct LIMES_EXPORT Triangle final : public Oscillator<SampleType>
{
	/** Constructs a default triangle wave. */
	explicit Triangle();

	LIMES_DEFAULT_MOVABLE (Triangle)
	LIMES_DEFAULT_COPYABLE (Triangle)

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
