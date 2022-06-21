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
#include <limes_namespace.h>
#include <limes_core.h>

/** @file
	This file defines the DetunableModel class.
	@ingroup oscillators
 */

LIMES_BEGIN_NAMESPACE

namespace dsp::osc
{

/** This class manages a number of oscillator voices, and detunes them around a central pitch, according to a spread parameter.
	@ingroup oscillators
 */
class LIMES_EXPORT DetunableModel final
{
public:

	/** Creates a detunable model that manages a certain number of voices. */
	explicit DetunableModel (std::size_t initialNumVoices);

	LIMES_DEFAULT_MOVABLE (DetunableModel)
	LIMES_DEFAULT_COPYABLE (DetunableModel)

	/** Sets the center frequency around which all the voices will be detuned. */
	void setFrequency (float frequency);

	/** Returns the center frequency all the voices are being detuned around. */
	[[nodiscard]] float getCenterFrequency() const noexcept;

	/** Sets the total detune amount, in cents.
		@param totalPitchSpreadInCents The number of cents between the highest and lowest voices in the spread around the central pitch.
	 */
	void setDetuneAmount (int totalPitchSpreadInCents);

	/** Returns the current spread amount, in cents. */
	[[nodiscard]] int getPitchSpreadCents() const noexcept;

	/** Changes the number of voices being managed. */
	void changeNumVoices (std::size_t newNumVoices);

	/** Returns the number of voices being managed. */
	[[nodiscard]] int getNumVoices() const noexcept;

	/** Returns the frequency for a specific voice number.
		Voices are numbered 0 through \c numVoices and are assigned frequencies in ascending order through the pitch spread.
		@see applyFrequencies
	 */
	[[nodiscard]] float getFrequency (std::size_t voiceNumber) const;

	/** Calls a function with each voice's frequency in the pitch spread.
		This calls the function with the lowest pitch first, then each voice's pitch in ascending order until the top of the pitch spread is reached.
		@see getFrequency
	 */
	void applyFrequencies (std::function<void (float)>&& func) const;

private:

	int totalSpreadCents { 0 };

	std::size_t numVoices { 0 };

	float lastFrequency { 440.f };

	std::vector<float> frequencies;
};

}  // namespace dsp::osc

LIMES_END_NAMESPACE
