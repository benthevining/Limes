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

/** @file
	This file defines the VelocityMapper class.
	@ingroup limes_midi
 */

LIMES_BEGIN_NAMESPACE

namespace midi
{

/** This class allows you to impose a "sensitivity" parameter on MIDI messages velocities.
	If the sensitivity is set to full (100), then the velocity is not changed at all.
	If the sensitivity is set to off (0), then the velocity is always output as 127, regardless of what the input velocity was.
	@ingroup limes_midi
 */
class LIMES_EXPORT VelocityMapper final
{
public:

	/** Sets the sensitivity of incoming velocity messages.
		If the sensitivity is set to full (100), then the velocity is not changed at all.
		If the sensitivity is set to off (0), then the velocity is always output as 127, regardless of what the input velocity was.
	 */
	void setSensitivity (int newSensitivity);

	/** Returns the sensitivity that was set. */
	[[nodiscard]] int getSensitivity() const noexcept;

	/** @name Mapping incoming velocities
		Maps an incoming MIDI velocity value to an output gain value between 0 and 1, based on the sensitivity that's been set.
	 */
	///@{
	[[nodiscard]] float getGainForVelocity (float midiVelocity) const;
	[[nodiscard]] float getGainForVelocity (int midiVelocity) const;
	///@}

private:

	int sensitivity { 100 };
};

}  // namespace midi

LIMES_END_NAMESPACE
