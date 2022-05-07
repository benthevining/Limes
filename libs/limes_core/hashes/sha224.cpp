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

#include <algorithm>		  // for min
#include <limes_namespace.h>  // for LIMES_BEGIN_NAMESPACE, LIMES_END_NAMESPACE
#include <cstdint>			  // for uint32_t, uint8_t
#include <cstdio>			  // for sprintf, size_t
#include <cstring>			  // for memcpy, memset
#include <string>			  // for basic_string
#include <string_view>		  // for string_view
#include "hash.h"			  // for sha224


LIMES_BEGIN_NAMESPACE

namespace hash
{

class SHA224 final	// cppcheck-suppress noConstructor
{
	static constexpr unsigned SHA224_256_BLOCK_SIZE = (512 / 8);

public:

	inline void update (const unsigned char* message, unsigned len)
	{
		auto rem_len = std::min (len, SHA224_256_BLOCK_SIZE - m_len);

		std::memcpy (&m_block[m_len], message, rem_len);

		if (m_len + len < SHA224_256_BLOCK_SIZE)
		{
			m_len += len;
			return;
		}

		const auto		  new_len		  = len - rem_len;
		const auto		  block_nb		  = new_len / SHA224_256_BLOCK_SIZE;
		const auto* const shifted_message = message + rem_len;

		transform (m_block, 1);
		transform (shifted_message, block_nb);

		rem_len = new_len % SHA224_256_BLOCK_SIZE;

		std::memcpy (m_block, &shifted_message[block_nb << 6], rem_len);

		m_len = rem_len;
		m_tot_len += (block_nb + 1) << 6;
	}

	[[nodiscard]] inline std::string getHash()
	{
		const unsigned block_nb = (1 + ((SHA224_256_BLOCK_SIZE - 9) < (m_len % SHA224_256_BLOCK_SIZE)));

		const unsigned len_b  = (m_tot_len + m_len) << 3;
		const unsigned pm_len = block_nb << 6;

		std::memset (m_block + m_len, 0, pm_len - m_len);

		m_block[m_len] = 0x80;

		SHA2_UNPACK32 (len_b, m_block + pm_len - 4);

		transform (m_block, block_nb);

		static constexpr unsigned DIGEST_SIZE = (224 / 8);

		unsigned char digest[DIGEST_SIZE];
		std::memset (digest, 0, DIGEST_SIZE);

		for (auto i = 0; i < 7; i++)
			SHA2_UNPACK32 (m_h[i], &digest[i << 2]);

		char buf[2 * DIGEST_SIZE + 1];
		buf[2 * DIGEST_SIZE] = 0;

		for (auto i = 0; i < static_cast<decltype (i)> (DIGEST_SIZE); ++i)
			std::sprintf (buf + i * 2, "%02x", digest[i]);	// NOLINT

		return { buf };
	}

private:

	inline void transform (const unsigned char* message, unsigned block_nb) noexcept
	{
		std::uint32_t w[64];
		std::uint32_t wv[8];

		for (auto i = 0; i < static_cast<decltype (i)> (block_nb); ++i)
		{
			const auto* const sub_block = message + (i << 6);

			for (auto j = 0; j < 16; ++j)
				SHA2_PACK32 (&sub_block[j << 2], w[j]);

			for (auto j = 16; j < 64; ++j)
				w[j] = SHA256_F4 (w[j - 2]) + w[j - 7] + SHA256_F3 (w[j - 15]) + w[j - 16];

			for (auto j = 0; j < 8; ++j)
				wv[j] = m_h[j];

			for (auto j = 0; j < 64; ++j)
			{
				const auto t1 = wv[7] + SHA256_F2 (wv[4]) + SHA2_CH (wv[4], wv[5], wv[6])
							  + sha256_k[j] + w[j];

				const auto t2 = SHA256_F1 (wv[0]) + SHA2_MAJ (wv[0], wv[1], wv[2]);

				wv[7] = wv[6];
				wv[6] = wv[5];
				wv[5] = wv[4];
				wv[4] = wv[3] + t1;
				wv[3] = wv[2];
				wv[2] = wv[1];
				wv[1] = wv[0];
				wv[0] = t1 + t2;
			}

			for (auto j = 0; j < 8; ++j)
				m_h[j] += wv[j];
		}
	}

	static inline void SHA2_PACK32 (const unsigned char* str, std::uint32_t& x) noexcept
	{
		x = static_cast<std::uint32_t> (str[3])
		  | (static_cast<std::uint32_t> (str[2]) << 8)
		  | (static_cast<std::uint32_t> (str[1]) << 16)
		  | (static_cast<std::uint32_t> (str[0]) << 24);
	}

	static inline void SHA2_UNPACK32 (std::uint32_t x, unsigned char* str) noexcept
	{
		str[3] = static_cast<std::uint8_t> (x);
		str[2] = static_cast<std::uint8_t> (x >> 8);
		str[1] = static_cast<std::uint8_t> (x >> 16);
		str[0] = static_cast<std::uint8_t> (x >> 24);
	}

	static inline std::uint32_t SHA256_F4 (std::uint32_t x) noexcept
	{
		return SHA2_ROTR (x, 17) ^ SHA2_ROTR (x, 19) ^ (x >> 10);
	}

	static inline std::uint32_t SHA256_F3 (std::uint32_t x) noexcept
	{
		return SHA2_ROTR (x, 7) ^ SHA2_ROTR (x, 18) ^ (x >> 3);
	}

	static inline std::uint32_t SHA256_F2 (std::uint32_t x) noexcept
	{
		return SHA2_ROTR (x, 6) ^ SHA2_ROTR (x, 11) ^ SHA2_ROTR (x, 25);
	}

	static inline std::uint32_t SHA256_F1 (std::uint32_t x) noexcept
	{
		return SHA2_ROTR (x, 2) ^ SHA2_ROTR (x, 13) ^ SHA2_ROTR (x, 22);
	}

	static inline std::uint32_t SHA2_ROTR (std::uint32_t x, std::uint32_t n) noexcept
	{
		return (x >> n) | (x << ((sizeof (x) << 3) - n));
	}

	static inline std::uint32_t SHA2_MAJ (std::uint32_t x, std::uint32_t y, std::uint32_t z) noexcept
	{
		return (x & y) ^ (x & z) ^ (y & z);
	}

	static inline std::uint32_t SHA2_CH (std::uint32_t x, std::uint32_t y, std::uint32_t z) noexcept
	{
		return (x & y) ^ (~x & z);
	}

	unsigned m_len { 0 };

	unsigned m_tot_len { 0 };

	std::uint32_t m_h[8] = { 0xc1059ed8, 0x367cd507, 0x3070dd17, 0xf70e5939, 0xffc00b31, 0x68581511, 0x64f98fa7, 0xbefa4fa4 };

	unsigned char m_block[2 * SHA224_256_BLOCK_SIZE];

	static constexpr unsigned sha256_k[64] = { 0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
											   0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
											   0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
											   0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
											   0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
											   0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
											   0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
											   0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
											   0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
											   0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
											   0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
											   0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
											   0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
											   0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
											   0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
											   0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2 };
};


std::string sha224 (const char* input, std::size_t length)
{
	SHA224 hasher;

	hasher.update (reinterpret_cast<const unsigned char*> (input), static_cast<unsigned> (length));

	return hasher.getHash();
}

std::string sha224 (const std::string_view& input)
{
	return sha224 (input.data(), input.length());
}

}  // namespace hash

LIMES_END_NAMESPACE
