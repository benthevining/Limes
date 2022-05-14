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
#include "hash.h"
#include <cstdint>
#include <string>
#include <string_view>

LIMES_BEGIN_NAMESPACE

namespace hash
{

/** A Hasher object that calculates a SHA1 hash.
	SHA1 is considered to be insecure and shouldn't be used for security-critical purposes in new projects, but is provided for backwards compatability.
 */
class LIMES_EXPORT SHA1 final : public Hasher  // cppcheck-suppress noConstructor
{
public:

	/** Updates the internal state of the hasher with new data. */
	void update (const unsigned char* data, std::size_t length) final;

	/** Retrieves the calculated hash value as a string. */
	[[nodiscard]] std::string getHash() final;

private:

	void transform (const std::uint8_t* data);

	void S_R0 (std::uint32_t v, std::uint32_t& w, std::uint32_t x, std::uint32_t y, std::uint32_t& z, std::uint32_t i) noexcept;
	void S_R1 (std::uint32_t v, std::uint32_t& w, std::uint32_t x, std::uint32_t y, std::uint32_t& z, std::uint32_t i) noexcept;
	void S_R2 (std::uint32_t v, std::uint32_t& w, std::uint32_t x, std::uint32_t y, std::uint32_t& z, std::uint32_t i) noexcept;
	void S_R3 (std::uint32_t v, std::uint32_t& w, std::uint32_t x, std::uint32_t y, std::uint32_t& z, std::uint32_t i) noexcept;
	void S_R4 (std::uint32_t v, std::uint32_t& w, std::uint32_t x, std::uint32_t y, std::uint32_t& z, std::uint32_t i) noexcept;

	[[nodiscard]] std::uint32_t SHABLK0 (std::uint32_t i) noexcept;

	[[nodiscard]] std::uint32_t SHABLK (std::uint32_t i) noexcept;

	std::uint32_t m_count[2] = { 0, 0 };

	std::uint8_t m_buffer[64];

	std::uint32_t m_state[5] = { 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0 };

	std::uint8_t m_block[64];
};

/** Calculates a SHA1 hash for the given data. */
LIMES_EXPORT [[nodiscard]] std::string sha1 (const char* input, std::size_t length);

/** Calculates a SHA1 hash for the given string. */
LIMES_EXPORT [[nodiscard]] std::string sha1 (const std::string_view& input);

}  // namespace hash

LIMES_END_NAMESPACE
