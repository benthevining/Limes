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
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace midi
{

void VelocityMapper::setSensitivity (int newSensitivity)
{
	LIMES_ASSERT (newSensitivity >= 0 && newSensitivity <= 100);

	sensitivity = newSensitivity;
}

int VelocityMapper::getSensitivity() const noexcept
{
	return sensitivity;
}

float VelocityMapper::getGainForVelocity (float midiVelocity)
{
	LIMES_ASSERT (midiVelocity >= 0.f && midiVelocity <= 1.f);

	const auto sens = static_cast<float> (sensitivity) * 0.01f;

	return (1.f - midiVelocity) * (1.f - sens) + midiVelocity;
}

float VelocityMapper::getGainForVelocity (int midiVelocity)
{
	LIMES_ASSERT (midiVelocity >= 0 && midiVelocity <= 127);

	constexpr auto inv127 = 1.f / 127.f;

	return getGainForVelocity (static_cast<float> (midiVelocity) * inv127);
}

}  // namespace midi

LIMES_END_NAMESPACE
