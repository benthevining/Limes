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

#include "../limes_namespace.h"
#include <limes_platform.h>	 // for LIMES_LITTLE_ENDIAN, LIMES_MSVC
#include "../system/compiler_defs.h"
#include <cstdint>		// for uint32_t, uint8_t
#include <cstdio>		// for snprintf, size_t
#include <cstring>		// for memcpy, strcat, strcpy
#include <string>		// for basic_string
#include <string_view>	// for string_view
#include "./sha1.h"		// for sha1
#include <bit>			// for std::endian

#if LIMES_MSVC
#	include <cstdlib>
#endif

LIMES_BEGIN_NAMESPACE

namespace hash
{

void SHA1::update (const unsigned char* data, std::size_t length)
{
	std::size_t j = ((m_count[0] >> 3) & 0x3F);

	if ((m_count[0] += (length << 3)) < (length << 3))
		++m_count[1];  // Overflow

	m_count[1] += (length >> 29);

	std::size_t i = 0;

	if ((j + length) > 63)
	{
		i = 64 - j;
		std::memcpy (&m_buffer[j], data, i);

		transform (m_buffer);

		for (; (i + 63) < length; i += 64)
			transform (&data[i]);

		j = 0;
	}

	if ((length - i) != 0)
		std::memcpy (&m_buffer[j], &data[i], length - i);
}

std::string SHA1::getHash()
{
	std::uint8_t pbFinalCount[8];

	for (std::uint32_t i = 0; i < 8; ++i)
		pbFinalCount[i] = static_cast<std::uint8_t> ((m_count[((i >= 4) ? 0 : 1)] >> ((3 - (i & 3)) * 8)) & 0xFF);	// Endian independent

	update (reinterpret_cast<const std::uint8_t*> ("\200"), 1);

	while ((m_count[0] & 504) != 448)
		update (reinterpret_cast<const std::uint8_t*> ("\0"), 1);

	update (pbFinalCount, 8);  // Cause a transform()

	std::uint8_t m_digest[20];

	for (std::uint32_t i = 0; i < 20; ++i)
		m_digest[i] = static_cast<std::uint8_t> ((m_state[i >> 2] >> ((3 - (i & 3)) * 8)) & 0xFF);

	char hashOut[84];
	char hashTemp[16];

	std::snprintf (hashTemp, sizeof (hashTemp) - 1, "%02X", m_digest[0]);
	std::strcpy (hashOut, hashTemp);  // NOLINT

	for (size_t i = 1; i < 20; ++i)
	{
		std::snprintf (hashTemp, sizeof (hashTemp) - 1, "%02X", m_digest[i]);
		std::strcat (hashOut, hashTemp);  // NOLINT
	}

	return { hashOut };
}

void SHA1::transform (const std::uint8_t* data)
{
	std::uint32_t a = m_state[0], b = m_state[1], c = m_state[2], d = m_state[3], e = m_state[4];

	std::memcpy (m_block, data, 64);

	// 4 rounds of 20 operations each, loop unrolled
	// clang-format off
	S_R0(a,b,c,d,e, 0); S_R0(e,a,b,c,d, 1); S_R0(d,e,a,b,c, 2); S_R0(c,d,e,a,b, 3);
	S_R0(b,c,d,e,a, 4); S_R0(a,b,c,d,e, 5); S_R0(e,a,b,c,d, 6); S_R0(d,e,a,b,c, 7);
	S_R0(c,d,e,a,b, 8); S_R0(b,c,d,e,a, 9); S_R0(a,b,c,d,e,10); S_R0(e,a,b,c,d,11);
	S_R0(d,e,a,b,c,12); S_R0(c,d,e,a,b,13); S_R0(b,c,d,e,a,14); S_R0(a,b,c,d,e,15);
	S_R1(e,a,b,c,d,16); S_R1(d,e,a,b,c,17); S_R1(c,d,e,a,b,18); S_R1(b,c,d,e,a,19);
	S_R2(a,b,c,d,e,20); S_R2(e,a,b,c,d,21); S_R2(d,e,a,b,c,22); S_R2(c,d,e,a,b,23);
	S_R2(b,c,d,e,a,24); S_R2(a,b,c,d,e,25); S_R2(e,a,b,c,d,26); S_R2(d,e,a,b,c,27);
	S_R2(c,d,e,a,b,28); S_R2(b,c,d,e,a,29); S_R2(a,b,c,d,e,30); S_R2(e,a,b,c,d,31);
	S_R2(d,e,a,b,c,32); S_R2(c,d,e,a,b,33); S_R2(b,c,d,e,a,34); S_R2(a,b,c,d,e,35);
	S_R2(e,a,b,c,d,36); S_R2(d,e,a,b,c,37); S_R2(c,d,e,a,b,38); S_R2(b,c,d,e,a,39);
	S_R3(a,b,c,d,e,40); S_R3(e,a,b,c,d,41); S_R3(d,e,a,b,c,42); S_R3(c,d,e,a,b,43);
	S_R3(b,c,d,e,a,44); S_R3(a,b,c,d,e,45); S_R3(e,a,b,c,d,46); S_R3(d,e,a,b,c,47);
	S_R3(c,d,e,a,b,48); S_R3(b,c,d,e,a,49); S_R3(a,b,c,d,e,50); S_R3(e,a,b,c,d,51);
	S_R3(d,e,a,b,c,52); S_R3(c,d,e,a,b,53); S_R3(b,c,d,e,a,54); S_R3(a,b,c,d,e,55);
	S_R3(e,a,b,c,d,56); S_R3(d,e,a,b,c,57); S_R3(c,d,e,a,b,58); S_R3(b,c,d,e,a,59);
	S_R4(a,b,c,d,e,60); S_R4(e,a,b,c,d,61); S_R4(d,e,a,b,c,62); S_R4(c,d,e,a,b,63);
	S_R4(b,c,d,e,a,64); S_R4(a,b,c,d,e,65); S_R4(e,a,b,c,d,66); S_R4(d,e,a,b,c,67);
	S_R4(c,d,e,a,b,68); S_R4(b,c,d,e,a,69); S_R4(a,b,c,d,e,70); S_R4(e,a,b,c,d,71);
	S_R4(d,e,a,b,c,72); S_R4(c,d,e,a,b,73); S_R4(b,c,d,e,a,74); S_R4(a,b,c,d,e,75);
	S_R4(e,a,b,c,d,76); S_R4(d,e,a,b,c,77); S_R4(c,d,e,a,b,78); S_R4(b,c,d,e,a,79);
	// clang-format on

	// Add the working vars back into state
	m_state[0] += a;
	m_state[1] += b;
	m_state[2] += c;
	m_state[3] += d;
	m_state[4] += e;
}

[[nodiscard]] static LIMES_FORCE_INLINE std::uint32_t rotate_left (std::uint32_t value, std::uint32_t bits) noexcept
{
	return ((value) << (bits)) | ((value) >> (32 - (bits)));
}

void SHA1::S_R0 (std::uint32_t v, std::uint32_t& w, std::uint32_t x, std::uint32_t y, std::uint32_t& z, std::uint32_t i) noexcept
{
	z += ((w & (x ^ y)) ^ y) + SHABLK0 (i) + 0x5A827999 + rotate_left (v, 5);
	w = rotate_left (w, 30);
}

void SHA1::S_R1 (std::uint32_t v, std::uint32_t& w, std::uint32_t x, std::uint32_t y, std::uint32_t& z, std::uint32_t i) noexcept
{
	z += ((w & (x ^ y)) ^ y) + SHABLK (i) + 0x5A827999 + rotate_left (v, 5);
	w = rotate_left (w, 30);
}

void SHA1::S_R2 (std::uint32_t v, std::uint32_t& w, std::uint32_t x, std::uint32_t y, std::uint32_t& z, std::uint32_t i) noexcept
{
	z += (w ^ x ^ y) + SHABLK (i) + 0x6ED9EBA1 + rotate_left (v, 5);
	w = rotate_left (w, 30);
}

void SHA1::S_R3 (std::uint32_t v, std::uint32_t& w, std::uint32_t x, std::uint32_t y, std::uint32_t& z, std::uint32_t i) noexcept
{
	z += (((w | x) & y) | (w & x)) + SHABLK (i) + 0x8F1BBCDC + rotate_left (v, 5);
	w = rotate_left (w, 30);
}

void SHA1::S_R4 (std::uint32_t v, std::uint32_t& w, std::uint32_t x, std::uint32_t y, std::uint32_t& z, std::uint32_t i) noexcept
{
	z += (w ^ x ^ y) + SHABLK (i) + 0xCA62C1D6 + rotate_left (v, 5);
	w = rotate_left (w, 30);
}

std::uint32_t SHA1::SHABLK0 (std::uint32_t i) noexcept
{
	if constexpr (std::endian::native == std::endian::little)
		return m_block[i] = static_cast<std::uint8_t> ((rotate_left (m_block[i], 24) & 0xFF00FF00)
													   | (rotate_left (m_block[i], 8) & 0x00FF00FF));
	else
		return m_block[i];
}

std::uint32_t SHA1::SHABLK (std::uint32_t i) noexcept
{
	return m_block[i & 15] = static_cast<std::uint8_t> (rotate_left (m_block[(i + 13) & 15]
																		 ^ m_block[(i + 8) & 15]
																		 ^ m_block[(i + 2) & 15]
																		 ^ m_block[i & 15],
																	 1));
}


std::string sha1 (const char* input, std::size_t length)
{
	SHA1 hasher;

	hasher.update (reinterpret_cast<const unsigned char*> (input), length);

	return hasher.getHash();
}

std::string sha1 (const std::string_view& input)
{
	return sha1 (input.data(), input.length());
}

}  // namespace hash

LIMES_END_NAMESPACE
