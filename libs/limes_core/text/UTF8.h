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
#include "../limes_namespace.h"
#include <string_view>
#include <cstdint>
#include "../misc/preprocessor.h"

/** @file
	This file contains utilities for working with UTF-8 text.
	@ingroup text
 */

LIMES_BEGIN_NAMESPACE

/** This namespace contains utilities for working with UTF-8 text.
	@ingroup text
 */
namespace text::utf8
{

/** An integer type to represent a unicode code-point.
	@ingroup text
 */
using Char = std::uint32_t;

/** A non-owning pointer that can read null-terminated UTF-8 text as wide unicode characters.
	@ingroup text
 */
class LIMES_EXPORT Pointer final
{
public:
	/** @name Constructors */
	///@{
	/** Creates a UTF-8 %pointer referring to the given text. */
	constexpr explicit Pointer (const char* utf8Text)
		: text (utf8Text)
	{
	}  // cppcheck-suppress missingReturn

	/** Creates a UTF-8 %pointer referring to the given text. */
	constexpr explicit Pointer (const std::string_view& utf8Text)
		: text (utf8Text.data())
	{
	}  // cppcheck-suppress missingReturn

	/** Creates a null %pointer. */
	constexpr Pointer() = default;
	///@}

	LIMES_CONSTEXPR_COPYABLE (Pointer)
	LIMES_CONSTEXPR_MOVABLE (Pointer)

	/** Returns the first unicode character in the string. */
	Char operator*() const;

	/** @name Incrementing */
	///@{
	/** Skips past the first unicode character.
		Moving beyond the end of the string is undefined behaviour and will trigger an assertion.
	 */
	Pointer& operator++();

	Pointer operator++ (int);
	///@}

	/** Moves backwards to the previous unicode character.
		Moving beyond the beginning of the string is undefined behaviour and will trigger an assertion.
	 */
	Pointer& operator--();

	/** @name Equality comparison */
	///@{
	/** Note that this performs a pointer comparison, this doesn't compare the contents of the strings! */
	bool operator== (const Pointer& other) const noexcept;
	bool operator!= (const Pointer& other) const noexcept;

	bool operator== (decltype (nullptr)) const noexcept;
	bool operator!= (decltype (nullptr)) const noexcept;
	///@}

	/** @name Greater/less than comparison */
	///@{
	/** Note that this performs a pointer comparison, this doesn't compare the contents of the strings! */
	bool operator> (const Pointer& other) const noexcept;
	bool operator<(const Pointer& other) const noexcept;
	///@}

	/** Returns true if this pointer is not null.
		@see empty()
	 */
	operator bool() const noexcept;

	/** Returns the data this pointer is currently pointing to. */
	const char* data() const noexcept;

	/** Returns the character the pointer is currently pointing at, and advances the pointer to the next character.
		Calling this when the current character is the terminator will leave the pointer in an invalid state.
	 */
	Char popFirstChar() noexcept;

	/** @name Skipping prefixes */
	///@{
	/** If the start of the text matches the given string, this will advance this pointer to skip past it and return true.
		@see startsWith
	 */
	bool skipIfStartsWith (char charToMatch) noexcept;
	bool skipIfStartsWith (const std::string_view& textToMatch) noexcept;
	///@}

	/** Returns true if this pointer is null or points to a null terminator character. */
	bool empty() const noexcept;

	/** Searches for an occurrence of the given substring in this pointer's text.
		If a match is found, returns a pointer pointing to the beginning of the matching substring;
		otherwise, returns a nullptr.
	 */
	Pointer find (const std::string_view& stringToFind) const;

	/** Returns true if this pointer's text begins with the given substring.
		@see skipIfStartsWith
	 */
	bool startsWith (const std::string_view& prefix) const;

private:
	const char* text { nullptr };
};

#pragma mark Free functions

/** Writes the bytes for a unicode character, and returns the number of bytes that were needed.
	The passed buffer should have at least 4 bytes capacity.
	@ingroup text
 */
LIMES_EXPORT std::uint32_t fromUnicode (char* dest, Char codepoint);

/** Returns true if the given Unicode codepoint is a low-surrogate.
	@ingroup text
 */
LIMES_EXPORT bool isLowSurrogate (Char codepoint) noexcept;

/** Returns true if the given Unicode codepoint is a high-surrogate.
	@ingroup text
 */
LIMES_EXPORT bool isHighSurrogate (Char codepoint) noexcept;

/** Represents a pair of low and high surrogate parts of a unicode codepoints.
	@ingroup text
 */
struct LIMES_EXPORT SurrogatePair final
{
	/** The low-surrogate of the pair. */
	Char low;

	/** The high-surrogate of the pair. */
	Char high;

	/** Combines a high and low surrogate into a single codepoint. */
	Char combineToChar() const;

	/** Convenience static function to combine two unicode characters into a single codepoint. */
	static Char combineParts (Char lowPart, Char highPart);

	/** Creates a surrogate pair from a full codepoint. */
	[[nodiscard]] static SurrogatePair fromFullCodepoint (Char codepoint);
};

/** This simple struct represents a line and column position in a bit of text.
	@ingroup text
 */
struct LIMES_EXPORT LineAndColumn final
{
	/** The line number. */
	std::size_t line { 0 };

	/** The column number. */
	std::size_t column { 0 };

	/** Given a chunk of text and an offset into the text, returns a LineAndColumn object
		representing the offset's position in the original text.
		@param start The full text to find the offset/position within
		@param targetPosition The offset within the full text to find the position of
	 */
	[[nodiscard]] static LineAndColumn find (Pointer start, Pointer targetPosition);
};

}  // namespace text::utf8

LIMES_END_NAMESPACE
