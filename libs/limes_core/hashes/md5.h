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

class LIMES_EXPORT MD5 final : public Hasher  // cppcheck-suppress noConstructor
{
public:

	void update (const unsigned char* input, std::size_t length) final;

	[[nodiscard]] std::string getHash() final;

private:

	static constinit const unsigned md5_blocksize = 64;

	void transform (const unsigned char block[md5_blocksize]) noexcept;

	std::uint32_t count[2] = { 0, 0 };

	std::uint32_t state[4] = { 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476 };

	unsigned char buffer[md5_blocksize];
};

LIMES_EXPORT [[nodiscard]] std::string md5 (const char* input, std::size_t length);

LIMES_EXPORT [[nodiscard]] std::string md5 (const std::string_view& input);

}  // namespace hash

LIMES_END_NAMESPACE
