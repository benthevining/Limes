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

#include <limes_audio.h>
#include <limes_data_structures.h>
#include "audio_test_utils.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

static constexpr auto test_blocksizes = { 512, 967, 2042 };

TEST_CASE ("Circular buffer")
{
	using FloatType = float;

	limes::vector<FloatType> origStorage, circOutput;

	limes::dsp::CircularBuffer<FloatType> circularBuffer;

	limes::dsp::WhiteNoiseGenerator<FloatType> noise;

	auto resizeAllBuffers = [&circularBuffer, &origStorage, &circOutput] (int newSize)
	{
		circularBuffer.resize (newSize);
		origStorage.reserveAndZero (newSize);
		circOutput.reserveAndZero (newSize);
	};

	for (const auto numSamples : test_blocksizes)
	{
		DYNAMIC_SECTION ("Blocksize: " << numSamples)
		{
			resizeAllBuffers (numSamples);

			noise.getSamples (origStorage);

			// expect (noSamplesAreClipping (origStorage));

			circularBuffer.storeSamples (origStorage);

			REQUIRE (circularBuffer.getNumStoredSamples() == numSamples);

			circularBuffer.getSamples (circOutput);

			REQUIRE (circularBuffer.getNumStoredSamples() == 0);

			// expect (allSamplesAreValid (circOutput));
			// expect (noSamplesAreClipping (circOutput));

			REQUIRE (limes::tests::allSamplesAreEqual<FloatType> (circOutput, origStorage));

			const auto halfNumSamples = numSamples / 2;

			SECTION ("Retrieve fewer samples than were passed in")
			{
				circularBuffer.storeSamples (origStorage);

				circularBuffer.getSamples (circOutput.data(), halfNumSamples);

				REQUIRE (limes::tests::allSamplesAreEqual (circOutput.data(), origStorage.data(), halfNumSamples));
			}

			SECTION ("Retrieve more samples than are left in circ buffer")
			{
				if (limes::math::numberIsEven (numSamples))
					REQUIRE (circularBuffer.getNumStoredSamples() == halfNumSamples);
				else
					REQUIRE (circularBuffer.getNumStoredSamples() == Catch::Approx (halfNumSamples).margin (1));

				circularBuffer.getSamples (circOutput);

				REQUIRE (limes::tests::allSamplesAreZero (circOutput.data(), halfNumSamples));

				REQUIRE (limes::tests::allSamplesAreEqual (circOutput.data() + halfNumSamples,
														   origStorage.data() + halfNumSamples,
														   halfNumSamples));
			}

			SECTION ("Resizing")
			{
				circularBuffer.storeSamples (origStorage);

				circularBuffer.resize (halfNumSamples);

				REQUIRE (circularBuffer.getCapacity() == halfNumSamples);
				REQUIRE (circularBuffer.getNumStoredSamples() == 0);
			}
		}
	}
}
