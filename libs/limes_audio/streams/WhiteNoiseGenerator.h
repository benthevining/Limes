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
#include "./SampleStream.h"
#include "../util/Misc.h"  // for concept Sample - IWYU pragma: keep
#include <limes_namespace.h>

/** @file
	This file defines the WhiteNoiseGenerator class.
	@ingroup samplestreams
 */

LIMES_BEGIN_NAMESPACE

namespace dsp
{

/** A class that produces white noise.
	@see math::Random
	@ingroup samplestreams

	@todo other noise generators - pink, brown, etc
 */
template <Sample SampleType>
struct LIMES_EXPORT WhiteNoiseGenerator final : public SampleStream<SampleType>
{
public:

	/** @name Constructors */
	///@{
	/** Constructs a white noise generator with a random seed. */
	WhiteNoiseGenerator();

	/** Constructs a white noise generator with an explicit seed. */
	explicit WhiteNoiseGenerator (int64_t randomSeed);

	/** Constructs a white noise generator with a seed initialized by forking the given Random object. */
	explicit WhiteNoiseGenerator (math::Random& randomToFork);
	///@}

	LIMES_DEFAULT_MOVABLE (WhiteNoiseGenerator)
	LIMES_DEFAULT_COPYABLE (WhiteNoiseGenerator)

private:

	/** Returns the next white noise sample. */
	[[nodiscard]] SampleType getNextSampleValue() noexcept;

	math::Random random;
};

}  // namespace dsp

LIMES_END_NAMESPACE
