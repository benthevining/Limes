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

#include "hash.h"
#include <limes_namespace.h>
#include <string_view>
#include <string>
#include "../system/limes_assert.h"
#include "md5.h"
#include "sha1.h"
#include "sha224.h"
#include "sha256.h"
#include "sha384.h"
#include "sha512.h"

LIMES_BEGIN_NAMESPACE

namespace hash
{

void Hasher::update (const std::string_view& input)
{
	update (reinterpret_cast<const unsigned char*> (input.data()),
			static_cast<std::size_t> (input.length()));
}

std::unique_ptr<Hasher> createHasherForType (Type type)
{
	switch (type)
	{
		case (Type::md5) : return std::make_unique<MD5>();
		case (Type::sha1) : return std::make_unique<SHA1>();
		case (Type::sha224) : return std::make_unique<SHA224>();
		case (Type::sha256) : return std::make_unique<SHA256>();
		case (Type::sha384) : return std::make_unique<SHA384>();
		case (Type::sha512) : return std::make_unique<SHA512>();
		default : LIMES_UNREACHABLE; return nullptr;
	}
}


std::string hash (Type type, const char* input, std::size_t length)
{
	auto hasher = createHasherForType (type);

	hasher->update (reinterpret_cast<const unsigned char*> (input), length);

	return hasher->getHash();
}

std::string hash (Type type, const std::string_view& input)
{
	return hash (type, input.data(), input.length());
}

}  // namespace hash

LIMES_END_NAMESPACE
