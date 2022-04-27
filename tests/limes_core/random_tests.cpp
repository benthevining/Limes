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
#include <type_traits>
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE ("Random number generator", "[core][math]")
{
	limes::math::Random random;

	for (auto i = 2000; --i >= 0;)
	{
		REQUIRE (random.next (0, 5) >= 0);
		REQUIRE (random.next (0, 5) < 5);
		REQUIRE (random.next (0, 1) == 0);

		auto n = random.next (0, 50) + 1;

		static_assert (std::is_same_v<decltype (n), int>);

		REQUIRE (random.next (0, n) >= 0);
		REQUIRE (random.next (0, n) < n);

		n = random.next (0, 0x7ffffffe) + 1;

		REQUIRE (random.next (0, n) >= 0);
		REQUIRE (random.next (0, n) < n);


		REQUIRE (random.next (0.f, 26.f) >= 0.f);
		REQUIRE (random.next (0.f, 26.f) < 26.f);

		const auto m = random.next (0.f, 1000000.f);

		static_assert (std::is_same_v<decltype (m), const float>);

		REQUIRE (random.next (0.f, m) >= 0.f);
		REQUIRE (random.next (0.f, m) < m);


		REQUIRE (random.next (0., 327.) >= 0.);
		REQUIRE (random.next (0., 327.) < 327.);

		const auto o = random.next (0., 1000000.);

		static_assert (std::is_same_v<decltype (o), const double>);

		REQUIRE (random.next (0., o) >= 0.);
		REQUIRE (random.next (0., o) < o);
	}

	// int numTrue = 0;

	// for (auto i = 0; i < 100; ++i)
	// 	if (random.nextBool())
	// 		++numTrue;

	// REQUIRE (numTrue == Catch::Approx (50).margin(5));
}
