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

#include "UTF8.h"
#include <limes_namespace.h>
#include "../system/limes_assert.h"

LIMES_BEGIN_NAMESPACE

namespace text::utf8
{

const char* Pointer::data() const noexcept
{
	return text;
}

Char Pointer::operator*() const
{
	return Pointer (*this).popFirstChar();
}

Pointer& Pointer::operator++()
{
	LIMES_ASSERT (! empty());

	const auto firstByte = static_cast<signed char> (*text++);

	if (firstByte >= 0)
		return *this;

	const auto unicodeChar = static_cast<unsigned char> (firstByte);

	std::uint32_t testBit = 0x40;

	while ((unicodeChar & testBit) != 0 && testBit > 8)
	{
		++text;
		testBit >>= 1;
	}

	return *this;
}

Pointer Pointer::operator++ (int)  // NOLINT
{
	auto prev = *this;
		 operator++();
	return prev;
}

Pointer& Pointer::operator--()
{
	LIMES_ASSERT (text != nullptr);

	[[maybe_unused]] std::uint32_t bytesSkipped = 0;

	while ((*--text & 0xc0) == 0x80)
	{
		LIMES_ASSERT (bytesSkipped < 3);
		++bytesSkipped;
	}

	return *this;
}

Char Pointer::popFirstChar() noexcept
{
	LIMES_ASSERT (text != nullptr);

	const auto firstByte = static_cast<signed char> (*text++);

	Char unicodeChar = static_cast<unsigned char> (firstByte);

	if (firstByte < 0)
	{
		std::uint32_t bitMask = 0x7f, numExtraBytes = 0;

		for (std::uint32_t testBit = 0x40;
			 (unicodeChar & testBit) != 0 && testBit > 8;
			 ++numExtraBytes)
		{
			bitMask >>= 1;
			testBit >>= 1;
		}

		unicodeChar &= bitMask;

		for (std::uint32_t i = 0; i < numExtraBytes; ++i)
		{
			std::uint32_t nextByte = static_cast<unsigned char> (*text);

			LIMES_ASSERT ((nextByte & 0xc0) == 0x80);  // error in the data - you should always make sure the source
													   // gets validated before iterating a Pointer over it

			unicodeChar = (unicodeChar << 6) | (nextByte & 0x3f);
			++text;
		}
	}

	return unicodeChar;
}

bool Pointer::skipIfStartsWith (char charToMatch) noexcept
{
	if (text != nullptr && *text == charToMatch && charToMatch != 0)
	{
		++text;
		return true;
	}

	return false;
}

bool Pointer::skipIfStartsWith (const std::string_view& textToMatch) noexcept
{
	const auto* matching = textToMatch.begin();

	if (const auto* p = text)
	{
		while (*matching != 0)
			if (*matching++ != *p++)
				return false;

		text = p;
		return true;
	}

	return false;
}

bool Pointer::empty() const noexcept
{
	return text == nullptr || *text == 0;
}

Pointer::operator bool() const noexcept
{
	return text != nullptr;
}

bool Pointer::startsWith (const std::string_view& prefix) const
{
	LIMES_ASSERT (! prefix.empty());

	if (const auto* p = text)
	{
		const auto* textToMatch = prefix.begin();

		while (*textToMatch != 0)
			if (*textToMatch++ != *p++)
				return false;

		return true;
	}

	return false;
}

Pointer Pointer::find (const std::string_view& stringToFind) const
{
	LIMES_ASSERT (! stringToFind.empty());

	for (auto t = *this;; ++t)
		if (t.startsWith (stringToFind) || t.empty())
			return t;

	return {};
}

bool Pointer::operator== (decltype (nullptr)) const noexcept { return text == nullptr; }

bool Pointer::operator!= (decltype (nullptr)) const noexcept { return text != nullptr; }

bool Pointer::operator== (const Pointer& other) const noexcept
{
	return text == other.text;
}

bool Pointer::operator!= (const Pointer& other) const noexcept
{
	return text != other.text;
}

bool Pointer::operator> (const Pointer& other) const noexcept
{
	return text > other.text;
}

bool Pointer::operator< (const Pointer& other) const noexcept
{
	return text < other.text;
}

/*--------------------------------------------------------------------------------------------------------------*/

#pragma mark Free functions

std::uint32_t fromUnicode (char* dest, Char codepoint)
{
	if (codepoint < 0x80)
	{
		*dest = static_cast<char> (codepoint);
		return 1;
	}

	std::uint32_t extraBytes = 1;

	if (codepoint >= 0x800)
	{
		++extraBytes;

		if (codepoint >= 0x10000)
			++extraBytes;
	}

	dest[0] = static_cast<char> ((0xffu << (7 - extraBytes)) | (codepoint >> (extraBytes * 6)));

	for (std::uint32_t i = 1; i <= extraBytes; ++i)
		dest[i] = static_cast<char> (0x80u | (0x3fu & (codepoint >> ((extraBytes - i) * 6))));

	return extraBytes + 1;
}

bool isLowSurrogate (Char codepoint) noexcept
{
	return codepoint >= 0xdc00 && codepoint <= 0xdfff;
}

bool isHighSurrogate (Char codepoint) noexcept
{
	return codepoint >= 0xd800 && codepoint <= 0xdbff;
}

/*--------------------------------------------------------------------------------------------------------------*/

Char SurrogatePair::combineToChar() const
{
	if (! isHighSurrogate (high))
		return high;

	if (! isLowSurrogate (low))
		return 0;

	return (high << 10) + low - 0x35fdc00u;
}

Char SurrogatePair::combineParts (Char lowPart, Char highPart)
{
	SurrogatePair p { lowPart, highPart };

	return p.combineToChar();
}

SurrogatePair SurrogatePair::fromFullCodepoint (Char codepoint)
{
	LIMES_ASSERT (codepoint >= 0x10000);

	return { static_cast<Char> (0xd800u + ((codepoint - 0x10000u) >> 10)),
			 static_cast<Char> (0xdc00u + (codepoint & 0x3ffu)) };
}

/*--------------------------------------------------------------------------------------------------------------*/

LineAndColumn LineAndColumn::find (Pointer start, Pointer targetPosition)
{
	if (start == nullptr || targetPosition == nullptr)
		return {};

	LIMES_ASSERT (start <= targetPosition);

	LineAndColumn lc { 1, 1 };

	while (start < targetPosition && ! start.empty())
	{
		++lc.column;

		if (*start++ == '\n')
		{
			lc.line++;
			lc.column = 1;
		}
	}

	return lc;
}

}  // namespace text::utf8

LIMES_END_NAMESPACE
