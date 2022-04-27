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
#include <limes_core.h>
#include <tests_config.h>
#include <catch2/catch_test_macros.hpp>

TEST_CASE ("Interval tests", "[music][harmony]")
{
	using namespace limes::music;

	using Q = Interval::Quality;

	SECTION ("Enharmonic intervals and num semitones")
	{
		auto test_enharmonic = [] (const Interval& interval1, const Interval& interval2, int numSemitones)
		{
			REQUIRE (interval1.getNumSemitones() == interval2.getNumSemitones());

			REQUIRE (interval1.isEnharmonicTo (interval2));
			REQUIRE (interval2.isEnharmonicTo (interval1));

			REQUIRE (interval1.getCents_EqualTemperament() == interval2.getCents_EqualTemperament());

			REQUIRE (interval1.getNumSemitones() == numSemitones);
		};

		test_enharmonic (Interval { 0, Q::Perfect }, Interval { 2, Q::Diminished }, 0);
		test_enharmonic (Interval { 0, Q::Augmented }, Interval { 2, Q::Minor }, 1);
		test_enharmonic (Interval { 2, Q::Major }, Interval { 3, Q::Diminished }, 2);
		test_enharmonic (Interval { 3, Q::Minor }, Interval { 2, Q::Augmented }, 3);
		test_enharmonic (Interval { 3, Q::Major }, Interval { 4, Q::Diminished }, 4);
		test_enharmonic (Interval { 4, Q::Perfect }, Interval { 3, Q::Augmented }, 5);
		test_enharmonic (Interval { 4, Q::Augmented }, Interval { 5, Q::Diminished }, 6);
		test_enharmonic (Interval { 5, Q::Perfect }, Interval { 6, Q::Diminished }, 7);
		test_enharmonic (Interval { 6, Q::Minor }, Interval { 5, Q::Augmented }, 8);
		test_enharmonic (Interval { 6, Q::Major }, Interval { 7, Q::Diminished }, 9);
		test_enharmonic (Interval { 7, Q::Minor }, Interval { 6, Q::Augmented }, 10);
		test_enharmonic (Interval { 7, Q::Major }, Interval { 8, Q::Diminished }, 11);
		test_enharmonic (Interval { 8, Q::Perfect }, Interval { 7, Q::Augmented }, 12);

		// special case
		const Interval octave { 8, Q::Augmented };
		REQUIRE (octave.getNumSemitones() == 13);
	}

	SECTION ("Get inverse")
	{
		auto test_inverse = [] (const Interval& interval1, const Interval& interval2)
		{
			REQUIRE (interval1.getInverse() == interval2);
			REQUIRE (interval2.getInverse() == interval1);
		};

		test_inverse (Interval { 0, Q::Perfect }, Interval { 8, Q::Perfect });
		test_inverse (Interval { 0, Q::Augmented }, Interval { 8, Q::Diminished });
		test_inverse (Interval { 2, Q::Diminished }, Interval { 7, Q::Augmented });
		test_inverse (Interval { 2, Q::Minor }, Interval { 7, Q::Major });
		test_inverse (Interval { 2, Q::Major }, Interval { 7, Q::Minor });
		test_inverse (Interval { 2, Q::Augmented }, Interval { 7, Q::Diminished });
		test_inverse (Interval { 3, Q::Diminished }, Interval { 6, Q::Augmented });
		test_inverse (Interval { 3, Q::Minor }, Interval { 6, Q::Major });
		test_inverse (Interval { 3, Q::Major }, Interval { 6, Q::Minor });
		test_inverse (Interval { 3, Q::Augmented }, Interval { 6, Q::Diminished });
		test_inverse (Interval { 4, Q::Diminished }, Interval { 5, Q::Augmented });
		test_inverse (Interval { 4, Q::Perfect }, Interval { 5, Q::Perfect });
		test_inverse (Interval { 4, Q::Augmented }, Interval { 5, Q::Diminished });
		test_inverse (Interval { 5, Q::Diminished }, Interval { 4, Q::Augmented });
		test_inverse (Interval { 5, Q::Perfect }, Interval { 4, Q::Perfect });
		test_inverse (Interval { 5, Q::Augmented }, Interval { 4, Q::Diminished });
		test_inverse (Interval { 6, Q::Diminished }, Interval { 3, Q::Augmented });
		test_inverse (Interval { 6, Q::Minor }, Interval { 3, Q::Major });
		test_inverse (Interval { 6, Q::Major }, Interval { 3, Q::Minor });
		test_inverse (Interval { 6, Q::Augmented }, Interval { 3, Q::Diminished });
		test_inverse (Interval { 7, Q::Diminished }, Interval { 2, Q::Augmented });
		test_inverse (Interval { 7, Q::Minor }, Interval { 2, Q::Major });
		test_inverse (Interval { 7, Q::Major }, Interval { 2, Q::Minor });
		test_inverse (Interval { 7, Q::Augmented }, Interval { 2, Q::Diminished });
		test_inverse (Interval { 8, Q::Diminished }, Interval { 0, Q::Augmented });
		test_inverse (Interval { 8, Q::Perfect }, Interval { 0, Q::Perfect });
	}

	SECTION ("From number of semitones")
	{
		auto& rand = limes::math::Random::getSystem();

		auto test_from_semitones = [&rand] (int semitones, const Interval& expected)
		{
			REQUIRE (Interval::fromNumSemitones (semitones) == expected);

			for (auto i = 0; i < limes::tests::test_reps; ++i)
			{
				const auto startingNote = rand.next (semitones, 127 - semitones);

				REQUIRE (Interval::fromPitches (startingNote, startingNote + semitones) == expected);
			}
		};

		test_from_semitones (0, Interval { 0, Q::Perfect });
		test_from_semitones (1, Interval { 2, Q::Minor });
		test_from_semitones (2, Interval { 2, Q::Major });
		test_from_semitones (3, Interval { 3, Q::Minor });
		test_from_semitones (4, Interval { 3, Q::Major });
		test_from_semitones (5, Interval { 4, Q::Perfect });
		test_from_semitones (6, Interval { 4, Q::Augmented });
		test_from_semitones (7, Interval { 5, Q::Perfect });
		test_from_semitones (8, Interval { 6, Q::Minor });
		test_from_semitones (9, Interval { 6, Q::Major });
		test_from_semitones (10, Interval { 7, Q::Minor });
		test_from_semitones (11, Interval { 7, Q::Major });
		test_from_semitones (12, Interval { 8, Q::Perfect });
	}

	SECTION ("Increment operator")
	{
		auto test_increment = [] (Interval orig, const Interval& expected)
		{
			REQUIRE (orig < expected);
			REQUIRE (expected > orig);

			++orig;

			REQUIRE (orig == expected);
		};

		test_increment (Interval { 0, Q::Perfect }, Interval { 0, Q::Augmented });
		test_increment (Interval { 0, Q::Augmented }, Interval { 2, Q::Major });
		test_increment (Interval { 2, Q::Diminished }, Interval { 2, Q::Minor });
		test_increment (Interval { 2, Q::Minor }, Interval { 2, Q::Major });
		test_increment (Interval { 2, Q::Major }, Interval { 2, Q::Augmented });
		test_increment (Interval { 2, Q::Augmented }, Interval { 3, Q::Major });
		test_increment (Interval { 3, Q::Diminished }, Interval { 3, Q::Minor });
		test_increment (Interval { 3, Q::Minor }, Interval { 3, Q::Major });
		test_increment (Interval { 3, Q::Major }, Interval { 3, Q::Augmented });
		test_increment (Interval { 3, Q::Augmented }, Interval { 4, Q::Augmented });
		test_increment (Interval { 4, Q::Diminished }, Interval { 4, Q::Perfect });
		test_increment (Interval { 4, Q::Perfect }, Interval { 4, Q::Augmented });
		test_increment (Interval { 4, Q::Augmented }, Interval { 5, Q::Perfect });
		test_increment (Interval { 5, Q::Diminished }, Interval { 5, Q::Perfect });
		test_increment (Interval { 5, Q::Perfect }, Interval { 5, Q::Augmented });
		test_increment (Interval { 5, Q::Augmented }, Interval { 6, Q::Major });
		test_increment (Interval { 6, Q::Diminished }, Interval { 6, Q::Minor });
		test_increment (Interval { 6, Q::Minor }, Interval { 6, Q::Major });
		test_increment (Interval { 6, Q::Major }, Interval { 6, Q::Augmented });
		test_increment (Interval { 6, Q::Augmented }, Interval { 7, Q::Major });
		test_increment (Interval { 7, Q::Diminished }, Interval { 7, Q::Minor });
		test_increment (Interval { 7, Q::Minor }, Interval { 7, Q::Major });
		test_increment (Interval { 7, Q::Major }, Interval { 7, Q::Augmented });
		test_increment (Interval { 8, Q::Diminished }, Interval { 8, Q::Perfect });
		test_increment (Interval { 8, Q::Perfect }, Interval { 8, Q::Augmented });

		// special case
		Interval octave { 8, Q::Augmented };
		++octave;
		REQUIRE (octave == Interval { 8, Q::Augmented });
	}

	SECTION ("Decrement operator")
	{
		auto test_decrement = [] (Interval orig, const Interval& expected)
		{
			REQUIRE (orig > expected);
			REQUIRE (expected < orig);

			--orig;

			REQUIRE (orig == expected);
		};

		test_decrement (Interval { 0, Q::Augmented }, Interval { 0, Q::Perfect });
		test_decrement (Interval { 2, Q::Diminished }, Interval { 0, Q::Perfect });	 // special case
		test_decrement (Interval { 2, Q::Minor }, Interval { 2, Q::Diminished });
		test_decrement (Interval { 2, Q::Major }, Interval { 2, Q::Minor });
		test_decrement (Interval { 2, Q::Augmented }, Interval { 2, Q::Major });
		test_decrement (Interval { 3, Q::Diminished }, Interval { 2, Q::Minor });
		test_decrement (Interval { 3, Q::Minor }, Interval { 3, Q::Diminished });
		test_decrement (Interval { 3, Q::Major }, Interval { 3, Q::Minor });
		test_decrement (Interval { 3, Q::Augmented }, Interval { 3, Q::Major });
		test_decrement (Interval { 4, Q::Diminished }, Interval { 3, Q::Minor });
		test_decrement (Interval { 4, Q::Perfect }, Interval { 4, Q::Diminished });
		test_decrement (Interval { 4, Q::Augmented }, Interval { 4, Q::Perfect });
		test_decrement (Interval { 5, Q::Diminished }, Interval { 4, Q::Perfect });
		test_decrement (Interval { 5, Q::Perfect }, Interval { 5, Q::Diminished });
		test_decrement (Interval { 5, Q::Augmented }, Interval { 5, Q::Perfect });
		test_decrement (Interval { 6, Q::Diminished }, Interval { 5, Q::Diminished });
		test_decrement (Interval { 6, Q::Minor }, Interval { 6, Q::Diminished });
		test_decrement (Interval { 6, Q::Major }, Interval { 6, Q::Minor });
		test_decrement (Interval { 6, Q::Augmented }, Interval { 6, Q::Major });
		test_decrement (Interval { 7, Q::Diminished }, Interval { 6, Q::Minor });
		test_decrement (Interval { 7, Q::Minor }, Interval { 7, Q::Diminished });
		test_decrement (Interval { 7, Q::Major }, Interval { 7, Q::Minor });
		test_decrement (Interval { 7, Q::Augmented }, Interval { 7, Q::Major });
		test_decrement (Interval { 8, Q::Diminished }, Interval { 7, Q::Minor });
		test_decrement (Interval { 8, Q::Perfect }, Interval { 8, Q::Diminished });
		test_decrement (Interval { 8, Q::Augmented }, Interval { 8, Q::Perfect });

		// special case
		Interval unison { 0, Q::Perfect };
		--unison;
		REQUIRE (unison == Interval { 0, Q::Perfect });
	}
}
