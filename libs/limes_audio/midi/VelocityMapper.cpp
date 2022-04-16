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

#include "VelocityMapper.h"
#include <limes_core.h>
#include <limes_platform.h>

namespace limes::dsp
{

void VelocityMapper::setSensitivity (float newSensitivity)
{
	LIMES_ASSERT (newSensitivity >= 0.f && newSensitivity <= 1.f);
	sensitivity = newSensitivity;
}

void VelocityMapper::setSensitivity (int newSensitivity)
{
	setSensitivity (static_cast<float> (newSensitivity) * 0.01f);
}

float VelocityMapper::getGainForVelocity (float midiVelocity)
{
	LIMES_ASSERT (midiVelocity >= 0.f && midiVelocity <= 1.f && sensitivity >= 0.f && sensitivity <= 1.f);

	return (1.f - midiVelocity) * (1.f - sensitivity) + midiVelocity;
}

float VelocityMapper::getGainForVelocity (int midiVelocity)
{
	LIMES_ASSERT (midiVelocity >= 0 && midiVelocity <= 127);

	constexpr auto inv127 = 1.f / 127.f;

	return getGainForVelocity (static_cast<float> (midiVelocity) * inv127);
}

float VelocityMapper::getSensitivity() const noexcept
{
	return sensitivity;
}

int VelocityMapper::getIntSensitivity() const noexcept
{
	return math::round (sensitivity * 100.f);
}
}  // namespace limes::dsp
