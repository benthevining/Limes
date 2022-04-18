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
#include <cstdint>
#include <limes_data_structures.h>
#include "Misc.h"

namespace limes::dsp
{

template <Sample SampleType>
class LIMES_EXPORT PerlinNoise final
{
public:

	PerlinNoise();

	explicit PerlinNoise (int64_t randomSeed);

	[[nodiscard]] SampleType getNextSample (SampleType x, SampleType y = 0, SampleType z = 0);

private:

	scalar_vector<int> state;
};

}  // namespace limes::dsp
