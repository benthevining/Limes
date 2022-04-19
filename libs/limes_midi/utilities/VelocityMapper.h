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

LIMES_BEGIN_NAMESPACE

namespace midi
{

class LIMES_EXPORT VelocityMapper final
{
public:

	void setSensitivity (float newSensitivity);
	void setSensitivity (int newSensitivity);

	[[nodiscard]] float getGainForVelocity (float midiVelocity);
	[[nodiscard]] float getGainForVelocity (int midiVelocity);

	[[nodiscard]] float getSensitivity() const noexcept;
	[[nodiscard]] int	getIntSensitivity() const noexcept;

private:

	float sensitivity { 1.f };
};

}  // namespace midi

LIMES_END_NAMESPACE
