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

/** @file
	This file defines the MD5 hasher class.
	@ingroup hashes
 */

LIMES_BEGIN_NAMESPACE

namespace hash
{

/** A Hasher object that calculates an %MD5 hash.
	%MD5 is considered to be broken and insecure, so shouldn't be used for security-critical purposes in new applications, but is provided for backwards compatibility.
	@ingroup hashes
 */
class LIMES_EXPORT MD5 final : public Hasher  // cppcheck-suppress noConstructor
{
public:

	/** Updates the internal state of the hasher with new data. */
	void update (const unsigned char* input, std::size_t length) final;

	/** Retrieves the calculated hash value as a string. */
	[[nodiscard]] std::string getHash() final;

private:

	static constinit const unsigned md5_blocksize = 64;

	void transform (const unsigned char block[md5_blocksize]) noexcept;

	std::uint32_t count[2] = { 0, 0 };

	std::uint32_t state[4] = { 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476 };

	unsigned char buffer[md5_blocksize] = {};
};

/** Calculates an MD5 hash for the given data.
	@ingroup hashes
 */
LIMES_EXPORT [[nodiscard]] std::string md5 (const char* input, std::size_t length);

/** Calculates an MD5 hash for the given string.
	@ingroup hashes
 */
LIMES_EXPORT [[nodiscard]] std::string md5 (const std::string_view& input);

}  // namespace hash

LIMES_END_NAMESPACE
