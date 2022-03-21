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

#include <limes_core.h>
#include <limes_export.h>

namespace lemons::dsp
{

template <typename T>
concept Sample = std::is_same_v<T, float> || std::is_same_v<T, double>;

template <Sample SampleType>
LIMES_EXPORT static constexpr SampleType default_Q = 0.70710678118655f;


template <Sample SampleType>
LIMES_EXPORT [[nodiscard]] static inline SampleType midiVelocityToGain (SampleType velocity, float sensitivity = 1.f)
{
	LIMES_ASSERT (sensitivity >= 0.f && sensitivity <= 1.f);
	LIMES_ASSERT (velocity >= 0 && velocity <= 1);

	const auto v = velocity * static_cast<SampleType> (sensitivity) + SampleType (1) - static_cast<SampleType> (sensitivity);

	return math::limit (std::pow (SampleType (25), v) * SampleType (0.04),
						SampleType (0), SampleType (1));
}

}  // namespace lemons::dsp
