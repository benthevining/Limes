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

#include <limes_music.h>

#include <catch2/catch_test_macros.hpp>

TEST_CASE ("Compound interval tests", "[music][harmony]")
{
	/*
	 - create from num semitones
	 - create from pitches
	 - ==, isEnharmonicTo
	 - >, <
	 - get num semitones
	 - hasSameSimpleInterval
	 */
	using namespace limes::music;

	using Q = Interval::Quality;

	SECTION ("Constructors")
	{
		auto test_constructors = [] (Q quality, int simpleKind, int octaves)
		{
			const Interval simple { simpleKind, quality };

			const auto compoundKind = (octaves * 7) + simpleKind;

			const CompoundInterval interval1 { compoundKind, quality };
			const CompoundInterval interval2 { octaves, simple };

			REQUIRE (interval1 == interval2);

			REQUIRE (interval1.getNumOctaves() == octaves);
			REQUIRE (interval2.getNumOctaves() == octaves);

			REQUIRE (interval1.getSimpleInterval() == simple);
			REQUIRE (interval2.getSimpleInterval() == simple);

			REQUIRE (interval1.getKind() == compoundKind);
			REQUIRE (interval2.getKind() == compoundKind);
		};

		for (auto octaves = 0; octaves < 5; ++octaves)
		{
			test_constructors (Q::Minor, 2, octaves);
			test_constructors (Q::Major, 2, octaves);
			test_constructors (Q::Diminished, 3, octaves);
			test_constructors (Q::Minor, 3, octaves);
			test_constructors (Q::Major, 3, octaves);
			test_constructors (Q::Augmented, 3, octaves);
			test_constructors (Q::Diminished, 4, octaves);
			test_constructors (Q::Perfect, 4, octaves);
			test_constructors (Q::Augmented, 4, octaves);
			test_constructors (Q::Diminished, 5, octaves);
			test_constructors (Q::Perfect, 5, octaves);
			test_constructors (Q::Augmented, 5, octaves);
			test_constructors (Q::Diminished, 6, octaves);
			test_constructors (Q::Minor, 6, octaves);
			test_constructors (Q::Major, 6, octaves);
			test_constructors (Q::Augmented, 6, octaves);
			test_constructors (Q::Diminished, 7, octaves);
			test_constructors (Q::Minor, 7, octaves);
			test_constructors (Q::Major, 7, octaves);
			test_constructors (Q::Augmented, 7, octaves);
		}
	}

	SECTION ("Constructor special cases")
	{
		const CompoundInterval interval1 {
			1, Interval { 8, Q::Perfect }
		};

		REQUIRE (interval1.getNumOctaves() == 2);

		REQUIRE (interval1.getSimpleInterval() == Interval());

		const CompoundInterval interval2 {
			1, Interval { 8, Q::Augmented }
		};

		REQUIRE (interval2.getNumOctaves() == 2);

		REQUIRE (interval2.getSimpleInterval() == Interval { 0, Q::Augmented });

		const CompoundInterval interval3 {
			1, Interval { 8, Q::Diminished }
		};

		REQUIRE (interval3.getNumOctaves() == 1);

		REQUIRE (interval3.getSimpleInterval() == Interval { 8, Q::Diminished });
	}
}
