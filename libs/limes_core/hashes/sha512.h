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
#include <string>
#include <string_view>
#include <cstdint>

LIMES_BEGIN_NAMESPACE

namespace hash
{

class LIMES_EXPORT SHA512 final : public Hasher	 // cppcheck-suppress noConstructor
{
public:

	void update (const unsigned char* data, std::size_t length) final;

	[[nodiscard]] std::string getHash() final;

private:

	void transform (const unsigned char* message, unsigned block_nb);

	static constinit const unsigned SHA384_512_BLOCK_SIZE = (1024 / 8);

	unsigned m_tot_len { 0 };
	unsigned m_len { 0 };

	unsigned char m_block[2 * SHA384_512_BLOCK_SIZE];

	std::uint64_t m_h[8] = { 0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL, 0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL, 0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL, 0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL };
};

LIMES_EXPORT [[nodiscard]] std::string sha512 (const char* input, std::size_t length);

LIMES_EXPORT [[nodiscard]] std::string sha512 (const std::string_view& input);

}  // namespace hash

LIMES_END_NAMESPACE
