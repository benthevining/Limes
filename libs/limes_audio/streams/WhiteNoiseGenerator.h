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

#include <cstdint>
#include <limes_export.h>
#include <limes_core.h>
#include "SampleStream.h"
#include "../util/Misc.h"  // for concept Sample - IWYU pragma: keep
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace dsp
{

template <Sample SampleType>
struct LIMES_EXPORT WhiteNoiseGenerator final : public SampleStream<SampleType>
{
public:

	WhiteNoiseGenerator();

	explicit WhiteNoiseGenerator (int64_t randomSeed);

	explicit WhiteNoiseGenerator (math::Random& randomToFork);

private:

	[[nodiscard]] SampleType getNextSampleValue() noexcept;

	math::Random random;
};

}  // namespace dsp

LIMES_END_NAMESPACE
