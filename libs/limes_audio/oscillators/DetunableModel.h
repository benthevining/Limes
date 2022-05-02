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
#include <limes_data_structures.h>
#include <limes_namespace.h>
#include <limes_core.h>

LIMES_BEGIN_NAMESPACE

namespace dsp::osc
{

class LIMES_EXPORT DetunableModel final
{
public:

	explicit DetunableModel (int initialNumVoices);

	LIMES_DEFAULT_MOVABLE (DetunableModel);
	LIMES_DEFAULT_COPYABLE (DetunableModel);

	void setFrequency (float frequency);

	[[nodiscard]] float getCenterFrequency() const noexcept;

	void setDetuneAmount (int totalPitchSpreadInCents);

	[[nodiscard]] int getPitchSpreadCents() const noexcept;

	void changeNumVoices (int newNumVoices);

	[[nodiscard]] int getNumVoices() const noexcept;

	[[nodiscard]] float getFrequency (int voiceNumber) const;

	void applyFrequencies (std::function<void (float)>&& func) const;

private:

	int totalSpreadCents { 0 }, numVoices;

	float lastFrequency { 440.f };

	scalar_vector<float> frequencies;
};

}  // namespace dsp::osc

LIMES_END_NAMESPACE