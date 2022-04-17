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

#include "PitchbendMapper.h"

#include <limes_platform.h>

namespace limes::midi
{

void PitchbendMapper::setRange (float newStUp, float newStDown) noexcept
{
	rangeUp	  = newStUp;
	rangeDown = newStDown;
}

float PitchbendMapper::getRangeUp() const noexcept { return rangeUp; }
float PitchbendMapper::getRangeDown() const noexcept { return rangeDown; }

int PitchbendMapper::getLastReceivedPitchbend() const noexcept
{
	return lastReceivedPitchbend;
}

void PitchbendMapper::newPitchbendReceived (int newPitchbend) noexcept
{
	LIMES_ASSERT (newPitchbend >= 0 && newPitchbend <= 127);

	lastReceivedPitchbend = newPitchbend;
}

void PitchbendMapper::resetPitchbend() noexcept
{
	lastReceivedPitchbend = 64;
}

float PitchbendMapper::getAdjustedMidiPitch (float inputMidiPitch) const
{
	if (lastReceivedPitchbend == 64)
		return static_cast<float> (inputMidiPitch);

	if (lastReceivedPitchbend > 64)
		return static_cast<float> (((rangeUp * (lastReceivedPitchbend - 65.f)) / 62.f)
								   + static_cast<float> (inputMidiPitch));

	return static_cast<float> ((((1.f - rangeDown) * lastReceivedPitchbend) / 63.f) + static_cast<float> (inputMidiPitch)
							   - rangeDown);
}

}  // namespace limes::midi
