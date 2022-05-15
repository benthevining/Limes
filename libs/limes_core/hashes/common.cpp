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

#include "common.h"
#include <limes_namespace.h>
#include "../system/compiler_defs.h"

LIMES_BEGIN_NAMESPACE

namespace hash::util
{

void pack32 (const unsigned char* str, std::uint32_t& x) noexcept
{
	x = static_cast<std::uint32_t> (str[3])
	  | (static_cast<std::uint32_t> (str[2]) << 8)
	  | (static_cast<std::uint32_t> (str[1]) << 16)
	  | (static_cast<std::uint32_t> (str[0]) << 24);
}

void pack64 (const unsigned char* str, std::uint64_t& x) noexcept
{
	x = static_cast<std::uint64_t> (str[7])
	  | (static_cast<std::uint64_t> (str[6]) << 8)
	  | (static_cast<std::uint64_t> (str[5]) << 16)
	  | (static_cast<std::uint64_t> (str[4]) << 24)
	  | (static_cast<std::uint64_t> (str[3]) << 32)
	  | (static_cast<std::uint64_t> (str[2]) << 40)
	  | (static_cast<std::uint64_t> (str[1]) << 48)
	  | (static_cast<std::uint64_t> (str[0]) << 56);
}

template <typename T>
void unpack32 (T x, unsigned char* str) noexcept
{
	str[3] = static_cast<std::uint8_t> (x);
	str[2] = static_cast<std::uint8_t> (x >> 8);
	str[1] = static_cast<std::uint8_t> (x >> 16);
	str[0] = static_cast<std::uint8_t> (x >> 24);
}

template void unpack32 (std::uint32_t, unsigned char*) noexcept;
template void unpack32 (std::uint64_t, unsigned char*) noexcept;

void unpack64 (std::uint64_t x, unsigned char* str) noexcept
{
	str[7] = static_cast<std::uint8_t> (x);
	str[6] = static_cast<std::uint8_t> (x >> 8);
	str[5] = static_cast<std::uint8_t> (x >> 16);
	str[4] = static_cast<std::uint8_t> (x >> 24);
	str[3] = static_cast<std::uint8_t> (x >> 32);
	str[2] = static_cast<std::uint8_t> (x >> 40);
	str[1] = static_cast<std::uint8_t> (x >> 48);
	str[0] = static_cast<std::uint8_t> (x >> 56);
}

template <typename T>
T ch (T x, T y, T z) noexcept
{
	return (x & y) ^ (~x & z);
}

template std::uint32_t ch (std::uint32_t, std::uint32_t, std::uint32_t) noexcept;
template std::uint64_t ch (std::uint64_t, std::uint64_t, std::uint64_t) noexcept;

template <typename T>
T maj (T x, T y, T z) noexcept
{
	return (x & y) ^ (x & z) ^ (y & z);
}

template std::uint32_t maj (std::uint32_t, std::uint32_t, std::uint32_t) noexcept;
template std::uint64_t maj (std::uint64_t, std::uint64_t, std::uint64_t) noexcept;

template <typename T>
LIMES_FORCE_INLINE T rot_r (T x, int n) noexcept
{
	const auto m = static_cast<T> (n);
	return (x >> m) | (x << ((sizeof (x) << 3) - m));
}

std::uint32_t sha256_F4 (std::uint32_t x) noexcept
{
	return util::rot_r (x, 17) ^ util::rot_r (x, 19) ^ (x >> 10);
}

std::uint32_t sha256_F3 (std::uint32_t x) noexcept
{
	return util::rot_r (x, 7) ^ util::rot_r (x, 18) ^ (x >> 3);
}

std::uint32_t sha256_F2 (std::uint32_t x) noexcept
{
	return util::rot_r (x, 6) ^ util::rot_r (x, 11) ^ util::rot_r (x, 25);
}

std::uint32_t sha256_F1 (std::uint32_t x) noexcept
{
	return util::rot_r (x, 2) ^ util::rot_r (x, 13) ^ util::rot_r (x, 22);
}

std::uint64_t sha512_F4 (std::uint64_t x) noexcept
{
	return util::rot_r (x, 19) ^ util::rot_r (x, 61) ^ (x >> 6);
}

std::uint64_t sha512_F3 (std::uint64_t x) noexcept
{
	return util::rot_r (x, 1) ^ util::rot_r (x, 8) ^ (x >> 7);
}

std::uint64_t sha512_F2 (std::uint64_t x) noexcept
{
	return util::rot_r (x, 14) ^ util::rot_r (x, 18) ^ util::rot_r (x, 41);
}

std::uint64_t sha512_F1 (std::uint64_t x) noexcept
{
	return util::rot_r (x, 28) ^ util::rot_r (x, 34) ^ util::rot_r (x, 39);
}

}  // namespace hash::util

LIMES_END_NAMESPACE
