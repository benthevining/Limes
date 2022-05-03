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

#include <catch2/catch_test_macros.hpp>
#include <limes_data_structures.h>
#include <limes_core.h>

// TEST_CASE ("Abstract FIFO", "[data_structures]")
// {
// 	static constexpr auto array_size = 5000;

// 	std::array<int, array_size> buffer;

// 	limes::AbstractFIFO fifo (array_size);

// 	int					writerN = 0;
// 	limes::math::Random writerRandom;

// 	auto writer_thread = [&fifo, &writerN, &writerRandom, &buffer]
// 	{
// 		const auto num = writerRandom.next (1, 2000);

// 		const auto writer = fifo.write (num);

// 		LIMES_ASSERT (writer.blockSize1 >= 0 && writer.blockSize2 >= 0);
// 		LIMES_ASSERT (writer.blockSize1 == 0
// 					  || (writer.startIndex1 >= 0 && writer.startIndex1 < fifo.getCapacity()));
// 		LIMES_ASSERT (writer.blockSize2 == 0
// 					  || (writer.startIndex2 >= 0 && writer.startIndex2 < fifo.getCapacity()));

// 		writer.for_each ([&buffer, &writerN] (int index)
// 						 { buffer[static_cast<std::array<int, array_size>::size_type> (index)] = writerN++; });
// 	};

// 	const limes::threads::ScopedLoopingThread writer { std::move (writer_thread) };

// 	limes::math::Random random;

// 	for (auto count = 100000, n = 0; --count >= 0;)
// 	{
// 		const auto num = random.next (1, 6000);

// 		const auto reader = fifo.read (num);

// 		REQUIRE (reader.blockSize1 >= 0);
// 		REQUIRE (reader.blockSize2 >= 0);
// 		REQUIRE ((reader.blockSize1 == 0
// 				  || (reader.startIndex1 >= 0 && reader.startIndex1 < fifo.getCapacity())));
// 		REQUIRE ((reader.blockSize2 == 0
// 				  || (reader.startIndex2 >= 0 && reader.startIndex2 < fifo.getCapacity())));

// 		bool failed = false;

// 		reader.for_each ([&failed, &buffer, &n] (int index)
// 						 { failed = (buffer[static_cast<std::array<int, array_size>::size_type> (index)] != n++) || failed; });

// 		REQUIRE (! failed);
// 	}
// }
