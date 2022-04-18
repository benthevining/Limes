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

#include "PerlinNoise.h"
#include <cmath>
#include <limes_core.h>
#include <algorithm>

namespace limes::dsp
{

template <Sample SampleType>
[[nodiscard]] static inline SampleType fade (SampleType t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

template <Sample SampleType>
[[nodiscard]] static inline SampleType lerp (SampleType t, SampleType a, SampleType b)
{
	return a + t * (b - a);
}

template <Sample SampleType>
[[nodiscard]] static inline SampleType grad (int hash, SampleType x, SampleType y, SampleType z)
{
	const auto h = hash & 15;

	const auto res1 = [h, x, y]
	{
		const auto u = [h, x, y]
		{
			if (h < 8)
				return x;

			return y;
		}();

		if ((h & 1) == 0)
			return u;

		return -u;
	}();

	const auto res2 = [h, x, y, z]
	{
		const auto v = [h, x, y, z]
		{
			if (h < 4)
				return y;

			if (h == 12 || h == 14)
				return x;

			return z;
		}();

		if ((h & 2) == 0)
			return v;

		return -v;
	}();

	return res1 + res2;
}


template <Sample SampleType>
PerlinNoise<SampleType>::PerlinNoise()
	: state ({ 151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142,
			   8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117,
			   35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71,
			   134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41,
			   55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89,
			   18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226,
			   250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182,
			   189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167,
			   43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246,
			   97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239,
			   107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
			   138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180 })
{
}

template <Sample SampleType>
PerlinNoise<SampleType>::PerlinNoise (int64_t randomSeed)
{
	math::Random r (randomSeed);

	for (auto i = 0; i <= 255; ++i)
		state.emplace_back (i);

	for (auto i = state.numObjects() - 1; i >= 1; --i)
	{
		const auto j = r.nextInt (i + 1);

		std::swap (state[i], state[j]);
	}

	for (auto i = 0; i < state.numObjects(); ++i)
		state.emplace_back (state[i]);
}

template <Sample SampleType>
SampleType PerlinNoise<SampleType>::getNextSample (SampleType x, SampleType y, SampleType z)
{
	const auto xFloor = std::floor (x);
	const auto yFloor = std::floor (y);
	const auto zFloor = std::floor (z);

	const auto X = static_cast<int> (xFloor) & 255;
	const auto Y = static_cast<int> (yFloor) & 255;
	const auto Z = static_cast<int> (zFloor) & 255;

	x -= xFloor;
	y -= yFloor;
	z -= zFloor;

	const auto u = fade (x);

	const auto idxA	 = state[X] + Y;
	const auto idxAA = state[idxA] + Z;
	const auto idxAB = state[idxA + 1] + Z;
	const auto idxB	 = state[X + 1] + Y;
	const auto idxBA = state[idxB] + Z;
	const auto idxBB = state[idxB + 1] + Z;

	const auto lerp1 = lerp (u,
							 grad (state[idxAB + 1], x, y - 1, z - 1),
							 grad (state[idxBB + 1], x - 1, y - 1, z - 1));

	const auto lerp2 = lerp (u,
							 grad (state[idxAA + 1], x, y, z - 1),
							 grad (state[idxBA + 1], x - 1, y, z - 1));

	const auto lerp3 = lerp (u,
							 grad (state[idxAB], x, y - 1, z),
							 grad (state[idxBB], x - 1, y - 1, z));

	const auto lerp4 = lerp (u,
							 grad (state[idxAA], x, y, z),
							 grad (state[idxBA], x - 1, y, z));

	const auto v = fade (y);

	const auto res = lerp (fade (z),
						   lerp (v, lerp4, lerp3),
						   lerp (v, lerp2, lerp1));

	return (res + SampleType (1.)) / SampleType (2.);
}


template class PerlinNoise<float>;
template class PerlinNoise<double>;

}  // namespace limes::dsp
