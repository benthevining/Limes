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

#include <limes_namespace.h>  // for LIMES_BEGIN_NAMESPACE, LIMES_END_NAMESPACE
#include <limes_platform.h>
#include <cstdint>		// for uint32_t
#include <cstdio>		// for sprintf, size_t
#include <cstring>		// for memcpy
#include <string>		// for basic_string
#include <string_view>	// for string_view
#include "md5.h"		// for MD5


LIMES_BEGIN_NAMESPACE

namespace hash
{

void MD5::update (const unsigned char* input, std::size_t length)
{
	std::uint32_t index = count[0] / 8 % md5_blocksize;

	if ((count[0] += (length << 3)) < (length << 3))
		count[1]++;

	count[1] += (length >> 29);

	// number of bytes we need to fill in buffer
	const auto firstpart = 64 - index;

	std::uint32_t i = 0;

	// transform as many times as possible.
	if (length >= firstpart)
	{
		// fill buffer first, transform
		std::memcpy (&buffer[index], input, firstpart);
		transform (buffer);

		// transform chunks of blocksize (64 bytes)
		for (i = firstpart; i + md5_blocksize <= length; i += md5_blocksize)
			transform (&input[i]);

		index = 0;
	}

	// buffer remaining input
	std::memcpy (&buffer[index], &input[i], length - i);
}

std::string MD5::getHash()
{
	auto encode = [] (unsigned char output[], const std::uint32_t input_[], std::uint32_t len)
	{
		for (std::uint32_t i = 0, j = 0;
			 j < len;
			 ++i, j += 4)
		{
			output[j]	  = input_[i] & 0xff;
			output[j + 1] = (input_[i] >> 8) & 0xff;
			output[j + 2] = (input_[i] >> 16) & 0xff;
			output[j + 3] = (input_[i] >> 24) & 0xff;
		}
	};

	// Save number of bits
	unsigned char bits[8];
	encode (bits, count, 8);

	static constinit const unsigned char padding[64] = {
		0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

	// pad out to 56 mod 64.
	std::uint32_t index	 = count[0] / 8 % 64;
	std::uint32_t padLen = (index < 56) ? (56 - index) : (120 - index);
	update (padding, padLen);

	// Append length (before padding)
	update (bits, 8);

	unsigned char digest[16];

	// Store state in digest
	encode (digest, state, 16);

	char buf[33];

	for (auto i = 0; i < 16; ++i)
		std::sprintf (buf + i * 2, "%02x", digest[i]);	// NOLINT

	buf[32] = 0;

	return { buf };
}

[[nodiscard]] static LIMES_FORCE_INLINE std::uint32_t rotate_left (std::uint32_t x, std::uint32_t n) noexcept
{
	return (x << n) | (x >> (32 - n));
}

static LIMES_FORCE_INLINE void FF (std::uint32_t& a, std::uint32_t b, std::uint32_t c, std::uint32_t d,
								   std::uint32_t x, std::uint32_t s, std::uint32_t ac) noexcept
{
	const auto F = ((b & c) | (~b & d));

	a = rotate_left (a + F + x + ac, s) + b;
}

static LIMES_FORCE_INLINE void GG (std::uint32_t& a, std::uint32_t b, std::uint32_t c, std::uint32_t d,
								   std::uint32_t x, std::uint32_t s, std::uint32_t ac) noexcept
{
	const auto G = ((b & d) | (c & ~d));

	a = rotate_left (a + G + x + ac, s) + b;
}

static LIMES_FORCE_INLINE void HH (std::uint32_t& a, std::uint32_t b, std::uint32_t c, std::uint32_t d,
								   std::uint32_t x, std::uint32_t s, std::uint32_t ac) noexcept
{
	const auto H = (b ^ c ^ d);

	a = rotate_left (a + H + x + ac, s) + b;
}

static LIMES_FORCE_INLINE void II (std::uint32_t& a, std::uint32_t b, std::uint32_t c, std::uint32_t d,
								   std::uint32_t x, std::uint32_t s, std::uint32_t ac) noexcept
{
	const auto I = (c ^ (b | ~d));

	a = rotate_left (a + I + x + ac, s) + b;
}

void MD5::transform (const unsigned char block[md5_blocksize]) noexcept
{
	std::uint32_t x[16];

	for (unsigned i = 0, j = 0;
		 j < md5_blocksize;
		 ++i, j += 4)
	{
		x[i] = static_cast<std::uint32_t> (block[j])
			 | (static_cast<std::uint32_t> (block[j + 1]) << 8)
			 | (static_cast<std::uint32_t> (block[j + 2]) << 16)
			 | (static_cast<std::uint32_t> (block[j + 3]) << 24);
	}

	auto a = state[0], b = state[1], c = state[2], d = state[3];

	static constexpr std::uint32_t S11 = 7;
	static constexpr std::uint32_t S12 = 12;
	static constexpr std::uint32_t S13 = 17;
	static constexpr std::uint32_t S14 = 22;
	static constexpr std::uint32_t S21 = 5;
	static constexpr std::uint32_t S22 = 9;
	static constexpr std::uint32_t S23 = 14;
	static constexpr std::uint32_t S24 = 20;
	static constexpr std::uint32_t S31 = 4;
	static constexpr std::uint32_t S32 = 11;
	static constexpr std::uint32_t S33 = 16;
	static constexpr std::uint32_t S34 = 23;
	static constexpr std::uint32_t S41 = 6;
	static constexpr std::uint32_t S42 = 10;
	static constexpr std::uint32_t S43 = 15;
	static constexpr std::uint32_t S44 = 21;

	/* Round 1 */
	FF (a, b, c, d, x[0], S11, 0xd76aa478);	 /* 1 */
	FF (d, a, b, c, x[1], S12, 0xe8c7b756);	 /* 2 */
	FF (c, d, a, b, x[2], S13, 0x242070db);	 /* 3 */
	FF (b, c, d, a, x[3], S14, 0xc1bdceee);	 /* 4 */
	FF (a, b, c, d, x[4], S11, 0xf57c0faf);	 /* 5 */
	FF (d, a, b, c, x[5], S12, 0x4787c62a);	 /* 6 */
	FF (c, d, a, b, x[6], S13, 0xa8304613);	 /* 7 */
	FF (b, c, d, a, x[7], S14, 0xfd469501);	 /* 8 */
	FF (a, b, c, d, x[8], S11, 0x698098d8);	 /* 9 */
	FF (d, a, b, c, x[9], S12, 0x8b44f7af);	 /* 10 */
	FF (c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
	FF (b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
	FF (a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
	FF (d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
	FF (c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
	FF (b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

	/* Round 2 */
	GG (a, b, c, d, x[1], S21, 0xf61e2562);	 /* 17 */
	GG (d, a, b, c, x[6], S22, 0xc040b340);	 /* 18 */
	GG (c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
	GG (b, c, d, a, x[0], S24, 0xe9b6c7aa);	 /* 20 */
	GG (a, b, c, d, x[5], S21, 0xd62f105d);	 /* 21 */
	GG (d, a, b, c, x[10], S22, 0x2441453);	 /* 22 */
	GG (c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
	GG (b, c, d, a, x[4], S24, 0xe7d3fbc8);	 /* 24 */
	GG (a, b, c, d, x[9], S21, 0x21e1cde6);	 /* 25 */
	GG (d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
	GG (c, d, a, b, x[3], S23, 0xf4d50d87);	 /* 27 */
	GG (b, c, d, a, x[8], S24, 0x455a14ed);	 /* 28 */
	GG (a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
	GG (d, a, b, c, x[2], S22, 0xfcefa3f8);	 /* 30 */
	GG (c, d, a, b, x[7], S23, 0x676f02d9);	 /* 31 */
	GG (b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

	/* Round 3 */
	HH (a, b, c, d, x[5], S31, 0xfffa3942);	 /* 33 */
	HH (d, a, b, c, x[8], S32, 0x8771f681);	 /* 34 */
	HH (c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
	HH (b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
	HH (a, b, c, d, x[1], S31, 0xa4beea44);	 /* 37 */
	HH (d, a, b, c, x[4], S32, 0x4bdecfa9);	 /* 38 */
	HH (c, d, a, b, x[7], S33, 0xf6bb4b60);	 /* 39 */
	HH (b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
	HH (a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
	HH (d, a, b, c, x[0], S32, 0xeaa127fa);	 /* 42 */
	HH (c, d, a, b, x[3], S33, 0xd4ef3085);	 /* 43 */
	HH (b, c, d, a, x[6], S34, 0x4881d05);	 /* 44 */
	HH (a, b, c, d, x[9], S31, 0xd9d4d039);	 /* 45 */
	HH (d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
	HH (c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
	HH (b, c, d, a, x[2], S34, 0xc4ac5665);	 /* 48 */

	/* Round 4 */
	II (a, b, c, d, x[0], S41, 0xf4292244);	 /* 49 */
	II (d, a, b, c, x[7], S42, 0x432aff97);	 /* 50 */
	II (c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
	II (b, c, d, a, x[5], S44, 0xfc93a039);	 /* 52 */
	II (a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
	II (d, a, b, c, x[3], S42, 0x8f0ccc92);	 /* 54 */
	II (c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
	II (b, c, d, a, x[1], S44, 0x85845dd1);	 /* 56 */
	II (a, b, c, d, x[8], S41, 0x6fa87e4f);	 /* 57 */
	II (d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
	II (c, d, a, b, x[6], S43, 0xa3014314);	 /* 59 */
	II (b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
	II (a, b, c, d, x[4], S41, 0xf7537e82);	 /* 61 */
	II (d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
	II (c, d, a, b, x[2], S43, 0x2ad7d2bb);	 /* 63 */
	II (b, c, d, a, x[9], S44, 0xeb86d391);	 /* 64 */

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
}


std::string md5 (const char* input, std::size_t length)
{
	MD5 hasher;

	hasher.update (reinterpret_cast<const unsigned char*> (input), length);

	return hasher.getHash();
}

std::string md5 (const std::string_view& input)
{
	return md5 (input.data(), input.length());
}

}  // namespace hash

LIMES_END_NAMESPACE
