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

#include <limes_export.h>  // for LIMES_EXPORT
#include "../limes_namespace.h"
#include <cstddef>		// for size_t
#include <string>		// for string
#include <string_view>	// for string_view
#include <memory>		// for unique_ptr

/** @defgroup hashes Hashes
	Cryptographic hash functions.
	Limes provides MD5, SHA1, SHA224, SHA256, SHA384, and SHA512 hashing functions for any opaque data, and a common interface for calculating any of them.
	@ingroup limes_core

	@todo write unit tests for each hash alg?
 */

/** @dir libs/limes_core/hashes
	This directory contains cryptographic hash functions.
	@ingroup hashes
 */

/** @file
	This file defines the Hasher class and the \c hash() free functions.
	@ingroup hashes
 */

LIMES_BEGIN_NAMESPACE

/** This namespace contains some common %hash functions and utilities.
	@ingroup hashes
 */
namespace hash
{

/** @ingroup hashes
	@{
 */

/** Represents a type of hash function that comes with the Limes library.
 */
enum class Type
{
	md5,
	sha1,
	sha224,
	sha256,
	sha384,
	sha512
};

/** A base class representing an object that calculates a hash function.
	You should call the \c update() method with any data that needs to be hashed, and then call \c getHash() to finalize the internal calculations and retrieve the final hash value.
	You shouldn't call \c update() again after calling \c getHash() .
	@see MD5, SHA1, SHA224, SHA256, SHA384, SHA512
 */
class LIMES_EXPORT Hasher
{
public:

	/** Destructor. */
	virtual ~Hasher() = default;

	/** Updates the internal state of the hasher with some new data. */
	virtual void update (const unsigned char* input, std::size_t length) = 0;

	/** Updates the internal state of the hasher with some new data. */
	void update (const std::string_view& input);

	/** Retrieves the calculated hash value as a string.
		You shouldn't call \c update() again after calling this function.
	 */
	[[nodiscard]] virtual std::string getHash() = 0;
};

/** Creates an appropriate Hasher for the given Type.
	@see hash
 */
LIMES_EXPORT [[nodiscard]] std::unique_ptr<Hasher> createHasherForType (Type type);

/** Calculates a hash value for the input data using a hasher appropriate for the desired type.
	@see createHasherForType
 */
LIMES_EXPORT [[nodiscard]] std::string hash (Type type, const char* input, std::size_t length);

/** Calculates a hash value for the input string using a hasher appropriate for the desired type.
	@see createHasherForType
 */
LIMES_EXPORT [[nodiscard]] std::string hash (Type type, const std::string_view& input);

/** @}*/

}  // namespace hash

LIMES_END_NAMESPACE
