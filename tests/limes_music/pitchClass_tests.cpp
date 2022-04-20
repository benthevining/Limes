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
#include <string>
#include <catch2/catch_test_macros.hpp>

// TEST_CASE("Pitch classes")
// {
// 	using namespace limes::music;

// 	SECTION ("Pitch class / string conversion")
// 	{
// 		auto testPitchClass = [] (int pitchClass, const std::string& string)
// 		{
// 			//			const PitchClass fromString { string };
// 			//			const PitchClass fromInt { pitchClass };
// 			//
// 			//			expectEquals (fromString.getAsInt(), fromInt.getAsInt());
// 			//			expectEquals (fromString.getAsInt(), pitchClass);
// 			//			expectEquals (fromInt.getAsString(), string);
// 		};

// 		testPitchClass (0, "C");
// 		testPitchClass (2, "D");
// 		testPitchClass (4, "E");
// 		testPitchClass (5, "F");
// 		testPitchClass (7, "G");
// 		testPitchClass (9, "A");
// 		testPitchClass (11, "B");

// 		auto testPitchClassAccidental = [] (int pitchClass, const std::string& sharpString, const std::string& flatString)
// 		{
// 			//			const PitchClass fromSharpString { sharpString };
// 			//			const PitchClass fromFlatString { flatString };
// 			//
// 			//			expectEquals (fromSharpString.getAsInt(), pitchClass);
// 			//			expectEquals (fromFlatString.getAsInt(), pitchClass);
// 		};

// 		testPitchClassAccidental (1, "C#", "Db");
// 		testPitchClassAccidental (3, "D#", "Eb");
// 		testPitchClassAccidental (6, "F#", "Gb");
// 		testPitchClassAccidental (8, "G#", "Ab");
// 		testPitchClassAccidental (10, "A#", "Bb");
// 	}
// }
