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
#include "../misc/preprocessor.h"

LIMES_BEGIN_NAMESPACE

namespace hash
{

std::string hash (Type type, const char* input, std::size_t length)
{
	switch (type)
	{
		case (Type::md5) : return md5 (input, length);
		case (Type::sha1) : return sha1 (input, length);
		case (Type::sha224) : return sha224 (input, length);
		case (Type::sha256) : return sha256 (input, length);
		case (Type::sha384) : return sha384 (input, length);
		case (Type::sha512) : return sha512 (input, length);
		default : LIMES_UNREACHABLE;
	}
}

std::string hash (Type type, const std::string_view& input)
{
	return hash (type, input.data(), input.length());
}

}  // namespace hash

LIMES_END_NAMESPACE
