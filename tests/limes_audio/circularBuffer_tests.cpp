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
#include <tests_config.h>
#include "audio_test_utils.h"
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEMPLATE_TEST_CASE ("Circular buffer", "[audio]", float, double)
{
	limes::vector<TestType> origStorage, circOutput;

	limes::dsp::CircularBuffer<TestType> circularBuffer;

	limes::dsp::WhiteNoiseGenerator<TestType> noise;

	auto resizeAllBuffers = [&circularBuffer, &origStorage, &circOutput] (int newSize)
	{
		circularBuffer.resize (newSize);
		origStorage.reserveAndZero (newSize);
		circOutput.reserveAndZero (newSize);
	};

	for (const auto numSamples : limes::tests::test_blocksizes)
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

			REQUIRE (limes::tests::allSamplesAreEqual<TestType> (circOutput, origStorage));

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

				REQUIRE (circularBuffer.getCapacity() >= halfNumSamples);
				REQUIRE (circularBuffer.getNumStoredSamples() == 0);
			}
		}
	}
}
