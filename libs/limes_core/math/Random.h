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

#pragma once

#include <limes_namespace.h>
#include <limes_export.h>
#include <cstdint>
#include <type_traits>
#include <atomic>

LIMES_BEGIN_NAMESPACE

namespace math
{

class LIMES_EXPORT Random final
{
public:

	explicit Random (int64_t seedValue) noexcept;

	Random();

	Random (const Random& other) noexcept;

	[[nodiscard]] int nextInt() noexcept;

	[[nodiscard]] int nextInt (int maxValue) noexcept;

	[[nodiscard]] int nextInt (int minValue, int maxValue) noexcept;

	[[nodiscard]] float nextFloat() noexcept;

	[[nodiscard]] double nextDouble() noexcept;

	[[nodiscard]] bool nextBool() noexcept;

	template <typename T>
	[[nodiscard]] T next() noexcept
	{
		static_assert (std::is_same_v<T, int> || std::is_same_v<T, float> || std::is_same_v<T, double> || std::is_same_v<T, bool>);

		if constexpr (std::is_same_v<T, int>)
			return nextInt();
		else if constexpr (std::is_same_v<T, float>)
			return nextFloat();
		else if constexpr (std::is_same_v<T, double>)
			return nextDouble();
		else if constexpr (std::is_same_v<T, bool>)
			return nextBool();
		else
			return T {};
	}

	void setSeed (int64_t newSeed) noexcept;

	[[nodiscard]] int64_t getSeed() const noexcept;

	void setSeedRandomly();

	[[nodiscard]] Random fork() noexcept;

	[[nodiscard]] static Random& getSystemRandom() noexcept;

private:

	std::atomic<int64_t> seed;
};

}  // namespace math

LIMES_END_NAMESPACE
