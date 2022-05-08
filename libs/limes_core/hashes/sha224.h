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

class LIMES_EXPORT SHA224 final : public Hasher	 // cppcheck-suppress noConstructor
{
public:

	void update (const unsigned char* data, std::size_t length) final;

	[[nodiscard]] std::string getHash() final;

private:

	static constinit const unsigned SHA224_256_BLOCK_SIZE = (512 / 8);

	void transform (const unsigned char* message, unsigned block_nb) noexcept;

	unsigned m_len { 0 };

	unsigned m_tot_len { 0 };

	std::uint32_t m_h[8] = { 0xc1059ed8, 0x367cd507, 0x3070dd17, 0xf70e5939, 0xffc00b31, 0x68581511, 0x64f98fa7, 0xbefa4fa4 };

	unsigned char m_block[2 * SHA224_256_BLOCK_SIZE];
};

LIMES_EXPORT [[nodiscard]] std::string sha224 (const char* input, std::size_t length);

LIMES_EXPORT [[nodiscard]] std::string sha224 (const std::string_view& input);

}  // namespace hash

LIMES_END_NAMESPACE
