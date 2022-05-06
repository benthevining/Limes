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

#include <limes_export.h>
#include <limes_namespace.h>
#include <string>
#include <string_view>

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

LIMES_EXPORT [[nodiscard]] std::string md5 (const char* input, std::size_t length);

LIMES_EXPORT [[nodiscard]] std::string md5 (const std::string_view& input);

LIMES_EXPORT [[nodiscard]] std::string sha1 (const char* input, std::size_t length);

LIMES_EXPORT [[nodiscard]] std::string sha1 (const std::string_view& input);

LIMES_EXPORT [[nodiscard]] std::string sha224 (const char* input, std::size_t length);

LIMES_EXPORT [[nodiscard]] std::string sha224 (const std::string_view& input);

LIMES_EXPORT [[nodiscard]] std::string sha256 (const char* input, std::size_t length);

LIMES_EXPORT [[nodiscard]] std::string sha256 (const std::string_view& input);

LIMES_EXPORT [[nodiscard]] std::string sha384 (const char* input, std::size_t length);

LIMES_EXPORT [[nodiscard]] std::string sha384 (const std::string_view& input);

LIMES_EXPORT [[nodiscard]] std::string sha512 (const char* input, std::size_t length);

LIMES_EXPORT [[nodiscard]] std::string sha512 (const std::string_view& input);

LIMES_EXPORT [[nodiscard]] std::string hash (Type type, const char* input, std::size_t length);

LIMES_EXPORT [[nodiscard]] std::string hash (Type type, const std::string_view& input);

}  // namespace hash

LIMES_END_NAMESPACE
