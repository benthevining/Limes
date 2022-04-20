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

#include <limes_core.h>
#include <tests_config.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>


TEST_CASE ("MIDI/frequency conversion", "[core][math]")
{
	using namespace limes::math;

	auto& rand = Random::getSystemRandom();

	SECTION ("MIDI to frequency")
	{
		for (auto i = 0; i < limes::tests::test_reps; ++i)
		{
			const auto midi = static_cast<double> (rand.nextInt (128));
			const auto freq = midiToFreq (midi);

			REQUIRE (freqToMidi (freq) == Catch::Approx (midi));
		}
	}

	SECTION ("Frequency to MIDI")
	{
		for (auto i = 0; i < limes::tests::test_reps; ++i)
		{
			const auto freq = static_cast<double> (rand.nextInt (80, 4500));
			const auto midi = freqToMidi (freq);

			REQUIRE (midiToFreq (midi) == Catch::Approx (freq));
		}
	}

	SECTION ("Hard-coded correct pairs")
	{
		const auto testPair = [&] (double midi, double freq)
		{
			const auto estMidi = freqToMidi (freq);
			const auto estFreq = midiToFreq (midi);

			REQUIRE (estMidi == Catch::Approx (midi).margin (0.0001));
			REQUIRE (estFreq == Catch::Approx (freq));

			REQUIRE (freqToMidi (estFreq) == Catch::Approx (midi));
			REQUIRE (midiToFreq (estMidi) == Catch::Approx (freq));
		};

		testPair (0., 8.1757989156);
		testPair (39., 77.7817459305);
		testPair (57., 220.);
		testPair (69., 440.);
	}
}

TEST_CASE ("Period/frequency conversion", "[core][math]")
{
	using namespace limes::math;

	auto& rand = Random::getSystemRandom();

	for (const auto samplerate : limes::tests::test_samplerates)
	{
		for (auto i = 0; i < limes::tests::test_reps; ++i)
		{
			const auto period  = static_cast<double> (rand.nextInt (1, 450));
			const auto estFreq = freqFromPeriod (samplerate, period);

			REQUIRE (static_cast<double> (periodInSamples (samplerate, estFreq)) == Catch::Approx (period));
		}
	}
}

TEST_CASE ("Samples/ms conversion", "[core][math]")
{
	using namespace limes::math;

	auto& rand = Random::getSystemRandom();

	SECTION ("Samples to ms")
	{
		for (const auto samplerate : limes::tests::test_samplerates)
		{
			const auto samples = rand.nextInt (1024);
			const auto estMs   = sampsToMs (samplerate, samples);

			REQUIRE (msToSamps (samplerate, estMs) == samples);
		}
	}

	SECTION ("Ms to samples")
	{
		for (const auto samplerate : limes::tests::test_samplerates)
		{
			const auto ms		  = rand.nextDouble();
			const auto estSamples = msToSamps (samplerate, ms);

			REQUIRE (sampsToMs (samplerate, estSamples) == Catch::Approx (ms).margin (0.01));
		}
	}
}
