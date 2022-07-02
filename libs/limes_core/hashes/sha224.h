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

#include "../limes_namespace.h"
#include <limes_export.h>
#include "./hash.h"
#include <string>
#include <string_view>
#include <cstdint>
#include <array>

/** @file
	This file defines the SHA224 hasher class.
	@ingroup hashes
 */

LIMES_BEGIN_NAMESPACE

namespace hash
{

/** A Hasher object that calculates a %SHA224 hash.
	@ingroup hashes
 */
class LIMES_EXPORT SHA224 final : public Hasher	 // cppcheck-suppress noConstructor
{
public:
	/** Updates the internal state of the hasher with new data. */
	void update (const unsigned char* data, std::size_t length) final;

	/** Retrieves the calculated hash value as a string. */
	[[nodiscard]] std::string getHash() final;

private:
	static constinit const unsigned SHA224_256_BLOCK_SIZE = (512 / 8);

	void transform (const unsigned char* message, unsigned block_nb) noexcept;

	unsigned m_len { 0 };

	unsigned m_tot_len { 0 };

	std::array<std::uint32_t, 8> m_h { 0xc1059ed8, 0x367cd507, 0x3070dd17, 0xf70e5939, 0xffc00b31, 0x68581511, 0x64f98fa7, 0xbefa4fa4 };

	std::array<unsigned char, 2UL * SHA224_256_BLOCK_SIZE> m_block {};
};

/** Calculates a SHA224 hash for the given data.
	@ingroup hashes
 */
LIMES_EXPORT [[nodiscard]] std::string sha224 (const char* input, std::size_t length);

/** Calculates a SHA224 hash for the given data.
	@ingroup hashes
 */
LIMES_EXPORT [[nodiscard]] std::string sha224 (const std::string_view& input);

}  // namespace hash

LIMES_END_NAMESPACE
