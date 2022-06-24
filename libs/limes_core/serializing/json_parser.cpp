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

#include "./serializing.h"
#include <limes_namespace.h>
#include "../system/limes_assert.h"
#include "../text/UTF8.h"
#include <cctype>
#include <cstdlib>
#include <string_view>
#include <sstream>
#include <stdexcept>


LIMES_BEGIN_NAMESPACE

namespace serializing
{

Node parseJSON (const std::string_view& jsonText)
{
	struct Parser final
	{
		explicit Parser (const std::string_view& inputText)
			: source (inputText), current (inputText)
		{
		}

		Node parse()
		{
			skipWhitespace();

			if (popIf ('['))
				return parseArray();

			if (popIf ('{'))
				return parseObject();

			if (! isEOF())
				throwError ("Expected an object or array");

			return Node::createNull();
		}

	private:

		inline void skipWhitespace()
		{
			for (auto p = current;
				 std::isspace (static_cast<unsigned char> (p.popFirstChar()));
				 current = p)
				;
		}

		inline bool popIf (char c)
		{
			return current.skipIfStartsWith (c);
		}

		inline bool popIf (const char* c)
		{
			return current.skipIfStartsWith (c);
		}

		inline std::uint32_t pop()
		{
			return current.popFirstChar();
		}

		inline bool isEOF()
		{
			return current.empty();
		}

		Node parseArray()
		{
			const auto arrayStart = current;

			auto result = Node { ObjectType::Array };

			skipWhitespace();

			if (popIf (']'))
				return result;

			for (;;)
			{
				skipWhitespace();

				if (isEOF())
					throwError ("Unexpected EOF in array declaration", arrayStart);

				result.getArray().push_back (parseValue());

				skipWhitespace();

				if (popIf (','))
					continue;

				if (popIf (']'))
					break;

				throwError ("Expected ',' or ']'");
			}

			return result;
		}

		Node parseObject()
		{
			const auto objectStart = current;

			auto result = Node { ObjectType::Object };

			skipWhitespace();

			if (popIf ('}'))
				return result;

			for (;;)
			{
				skipWhitespace();

				if (isEOF())
					throwError ("Unexpected EOF in object declaration", objectStart);

				if (! popIf ('"'))
					throwError ("Expected a name");

				const auto errorPos = current;
				const auto name		= parseString();

				if (name.empty())
					throwError ("Property names cannot be empty", errorPos);

				skipWhitespace();

				if (! popIf (':'))
					throwError ("Expected ':'");

				result.getObject().emplace_back (name, parseValue());

				skipWhitespace();

				if (popIf (','))
					continue;

				if (popIf ('}'))
					break;

				throwError ("Expected ',' or '}'");
			}

			return result;
		}

		Node parseValue()
		{
			skipWhitespace();

			auto startPos = current;

			switch (pop())
			{
				case '[' : return parseArray();
				case '{' : return parseObject();
				case '"' : return Node::createString (parseString());
				case '-' :
				{
					skipWhitespace();
					return parseNumber (true);
				}
				case '0' : [[fallthrough]];
				case '1' : [[fallthrough]];
				case '2' : [[fallthrough]];
				case '3' : [[fallthrough]];
				case '4' : [[fallthrough]];
				case '5' : [[fallthrough]];
				case '6' : [[fallthrough]];
				case '7' : [[fallthrough]];
				case '8' : [[fallthrough]];
				case '9' :
				{
					current = startPos;
					return parseNumber (false);
				}
				default : break;
			}

			current = startPos;

			if (popIf ("null"))
				return Node::createNull();

			if (popIf ("true"))
				return Node::createBoolean (true);

			if (popIf ("false"))
				return Node::createBoolean (false);

			throwError ("Syntax error");
		}

		Node parseNumber (bool negate)
		{
			auto startPos = current;
			bool hadDot = false, hadExponent = false;

			for (;;)
			{
				const auto lastPos = current;
				const auto c	   = pop();

				if (c >= '0' && c <= '9')
					continue;

				if (c == '.' && ! hadDot)
				{
					hadDot = true;
					continue;
				}

				if (! hadExponent && (c == 'e' || c == 'E'))
				{
					hadDot		= true;
					hadExponent = true;
					continue;
				}

				if (std::isspace (static_cast<int> (c)) != 0
					|| c == ',' || c == '}' || c == ']' || c == 0)
				{
					current					= lastPos;
					char* endOfParsedNumber = nullptr;

					if (! (hadDot || hadExponent))
					{
						const auto v = std::strtoll (startPos.data(), &endOfParsedNumber, 10);

						if (endOfParsedNumber == lastPos.data()
							&& v != std::numeric_limits<decltype (v)>::max()
							&& v != std::numeric_limits<decltype (v)>::min())
							return Node::createNumber (static_cast<double> (negate ? -v : v));
					}

					if (endOfParsedNumber == lastPos.data())
					{
						const auto v = std::strtod (startPos.data(), &endOfParsedNumber);

						return Node::createNumber (negate ? -v : v);
					}
				}

				throwError ("Syntax error in number", lastPos);
			}
		}

		std::string parseString()
		{
			std::stringstream s;

			for (;;)
			{
				auto c = pop();

				if (c == '"')
					break;

				if (c == '\\')
				{
					const auto errorPos = current;

					c = pop();

					switch (c)
					{
						case 'a' : c = '\a'; break;
						case 'b' : c = '\b'; break;
						case 'f' : c = '\f'; break;
						case 'n' : c = '\n'; break;
						case 'r' : c = '\r'; break;
						case 't' : c = '\t'; break;
						case 'u' : c = parseUnicodeCharacterNumber (false); break;
						case 0 : throwError ("Unexpected EOF in string constant", errorPos);
						default : break;
					}
				}

				char utf8Bytes[8];

				const auto numBytes = text::utf8::fromUnicode (utf8Bytes, c);

				for (std::uint32_t i = 0; i < numBytes; ++i)
					s << utf8Bytes[i];
			}

			return s.str();
		}

		std::uint32_t parseUnicodeCharacterNumber (bool isLowSurrogate)
		{
			std::uint32_t result = 0;

			for (auto i = 4; --i >= 0;)
			{
				const auto errorPos = current;

				auto digit = pop();

				if (digit >= '0' && digit <= '9')
					digit -= '0';
				else if (digit >= 'a' && digit <= 'f')
					digit = 10 + (digit - 'a');
				else if (digit >= 'A' && digit <= 'F')
					digit = 10 + (digit - 'A');
				else
					throwError ("Syntax error in unicode character", errorPos);

				result = (result << 4) + digit;
			}

			if (isLowSurrogate && ! text::utf8::isLowSurrogate (result))
				throwError ("Expected a unicode low surrogate codepoint");

			if (text::utf8::isHighSurrogate (result))
			{
				if (! isLowSurrogate && popIf ("\\u"))
					return text::utf8::SurrogatePair::combineParts (result, parseUnicodeCharacterNumber (true));

				throwError ("Expected a unicode low surrogate codepoint");
			}

			return result;
		}

		[[noreturn]] void throwError (const std::string_view& message)
		{
			throwError (message, current);
		}

		[[noreturn]] void throwError (const std::string_view& message, text::utf8::Pointer errorPos)
		{
			throw JSONParseError { message, text::utf8::LineAndColumn::find (source, errorPos) };
		}

		text::utf8::Pointer source, current;
	};

	Parser p { jsonText };

	return p.parse();
}

}  // namespace serializing

LIMES_END_NAMESPACE
