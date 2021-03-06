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

#include <limes_audio.h>

namespace limes::tests
{

template <typename SampleType>
[[nodiscard]] inline bool allSamplesAreEqual (const SampleType* const vecA, const SampleType* const vecB, int numSamples)
{
	for (auto i = 0; i < numSamples; ++i)
		if (vecA[i] != vecB[i])
			return false;

	return true;
}

template <typename SampleType>
[[nodiscard]] inline bool allSamplesAreEqual (const dsp::AudioBuffer<SampleType, 1>& vecA, const dsp::AudioBuffer<SampleType, 1>& vecB)
{
	const auto num = std::min (vecA.getNumSamples(), vecB.getNumSamples());

	return allSamplesAreEqual (vecA.getReadPointer (0), vecB.getReadPointer (0), num);
}

template <typename SampleType>
[[nodiscard]] inline bool allSamplesAreZero (const SampleType* const data, int numSamples)
{
	for (auto i = 0; i < numSamples; ++i)
		if (data[i] != SampleType (0))
			return false;

	return true;
}

}  // namespace limes::tests
