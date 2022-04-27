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
#include <catch2/catch_test_macros.hpp>

TEST_CASE ("Random number generator", "[core][math]")
{
	limes::math::Random random;

	for (auto i = 2000; --i >= 0;)
	{
		REQUIRE (random.nextDouble() >= 0.);
		REQUIRE (random.nextDouble() < 1.);
		REQUIRE (random.nextFloat() >= 0.f);
		REQUIRE (random.nextFloat() < 1.f);
		REQUIRE (random.nextInt (5) >= 0);
		REQUIRE (random.nextInt (5) < 5);
		REQUIRE (random.nextInt (1) == 0);

		auto n = random.nextInt (50) + 1;

		REQUIRE (random.nextInt (n) >= 0);
		REQUIRE (random.nextInt (n) < n);

		n = random.nextInt (0x7ffffffe) + 1;

		REQUIRE (random.nextInt (n) >= 0);
		REQUIRE (random.nextInt (n) < n);
	}
}
