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

#include <algorithm>  // for min
#include "../limes_namespace.h"
#include <cstdint>		// for uint32_t, uint8_t
#include <cstdio>		// for sprintf, size_t
#include <cstring>		// for memcpy, memset
#include <string>		// for basic_string
#include <string_view>	// for string_view
#include "./hash.h"		// for sha256
#include "./sha256.h"
#include "./common.h"

LIMES_BEGIN_NAMESPACE

namespace hash
{

void SHA256::update (const unsigned char* data, std::size_t length)
{
	const auto len = static_cast<unsigned> (length);

	auto rem_len = std::min (len, SHA224_256_BLOCK_SIZE - m_len);

	std::memcpy (&m_block[m_len], data, rem_len);

	if (m_len + len < SHA224_256_BLOCK_SIZE)
	{
		m_len += len;
		return;
	}

	const auto new_len	= len - rem_len;
	const auto block_nb = new_len / SHA224_256_BLOCK_SIZE;

	const auto* const shifted_message = data + rem_len;

	transform (m_block, 1);
	transform (shifted_message, block_nb);

	rem_len = new_len % SHA224_256_BLOCK_SIZE;

	std::memcpy (m_block, &shifted_message[block_nb << 6], rem_len);

	m_len = rem_len;
	m_tot_len += (block_nb + 1) << 6;
}

std::string SHA256::getHash()
{
	const unsigned block_nb = (1u + ((SHA224_256_BLOCK_SIZE - 9u) < (m_len % SHA224_256_BLOCK_SIZE)));

	const unsigned len_b  = (m_tot_len + m_len) << 3;
	const unsigned pm_len = block_nb << 6;

	std::memset (m_block + m_len, 0, pm_len - m_len);

	m_block[m_len] = 0x80;

	util::unpack32 (len_b, m_block + pm_len - 4);

	transform (m_block, block_nb);

	static constinit const unsigned DIGEST_SIZE = (256 / 8);

	unsigned char digest[DIGEST_SIZE];

	std::memset (digest, 0, DIGEST_SIZE);

	for (auto i = 0; i < 8; i++)
		util::unpack32 (m_h[i], &digest[i << 2]);

	char buf[2 * DIGEST_SIZE + 1];

	buf[2 * DIGEST_SIZE] = 0;

	for (auto i = 0; i < static_cast<decltype (i)> (DIGEST_SIZE); ++i)
		std::sprintf (buf + i * 2, "%02x", digest[i]);	// NOLINT

	return { buf };
}

void SHA256::transform (const unsigned char* message, unsigned block_nb) noexcept
{
	std::uint32_t w[64];
	std::uint32_t wv[8];

	for (auto i = 0; i < static_cast<decltype (i)> (block_nb); ++i)
	{
		const auto* const sub_block = message + (i << 6);

		for (auto j = 0; j < 16; ++j)
			util::pack32 (&sub_block[j << 2], w[j]);

		for (auto j = 16; j < 64; ++j)
			w[j] = util::sha256_F4 (w[j - 2]) + w[j - 7] + util::sha256_F3 (w[j - 15]) + w[j - 16];

		for (auto j = 0; j < 8; ++j)
			wv[j] = m_h[j];

		for (auto j = 0; j < 64; ++j)
		{
			const auto t1 = wv[7] + util::sha256_F2 (wv[4]) + util::ch (wv[4], wv[5], wv[6])
						  + util::sha256_k[j] + w[j];

			const auto t2 = util::sha256_F1 (wv[0]) + util::maj (wv[0], wv[1], wv[2]);

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


std::string sha256 (const char* input, std::size_t length)
{
	SHA256 hasher;

	hasher.update (reinterpret_cast<const unsigned char*> (input), length);

	return hasher.getHash();
}

std::string sha256 (const std::string_view& input)
{
	return sha256 (input.data(), input.length());
}

}  // namespace hash

LIMES_END_NAMESPACE
