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

class LIMES_EXPORT PitchbendMapper final
{
public:

	void setRange (float newStUp, float newStDown) noexcept;

	[[nodiscard]] float getRangeUp() const noexcept;
	[[nodiscard]] float getRangeDown() const noexcept;

	[[nodiscard]] int getLastReceivedPitchbend() const noexcept;

	void newPitchbendReceived (int newPitchbend) noexcept;

	void resetPitchbend() noexcept;

	[[nodiscard]] float getAdjustedMidiPitch (float inputMidiPitch) const;


private:

	float rangeUp { 2.f }, rangeDown { 2.f };

	int lastReceivedPitchbend { 64 };
};

}  // namespace midi

LIMES_END_NAMESPACE
