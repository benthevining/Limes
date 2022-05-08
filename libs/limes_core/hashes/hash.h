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

#include <limes_export.h>	  // for LIMES_EXPORT
#include <limes_namespace.h>  // for LIMES_BEGIN_NAMESPACE, LIMES_END_NAMESPACE
#include <cstddef>			  // for size_t
#include <string>			  // for string
#include <string_view>		  // for string_view
#include <memory>			  // for unique_ptr


LIMES_BEGIN_NAMESPACE

namespace hash
{

enum class Type
{
	md5,
	sha1,
	sha224,
	sha256,
	sha384,
	sha512
};

class LIMES_EXPORT Hasher
{
public:

	virtual ~Hasher() = default;

	virtual void update (const unsigned char* input, std::size_t length) = 0;

	void update (const std::string_view& input);

	[[nodiscard]] virtual std::string getHash() = 0;
};

LIMES_EXPORT [[nodiscard]] std::unique_ptr<Hasher> createHasherForType (Type type);


LIMES_EXPORT [[nodiscard]] std::string hash (Type type, const char* input, std::size_t length);

LIMES_EXPORT [[nodiscard]] std::string hash (Type type, const std::string_view& input);

}  // namespace hash

LIMES_END_NAMESPACE
