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

#include <limes_namespace.h>
#include "serializing.h"
#include "../text/StringUtils.h"
#include "../system/limes_assert.h"
#include <string>
#include <sstream>
#include <vector>
#include <cmath>

LIMES_BEGIN_NAMESPACE

namespace serializing
{

static inline std::string getEscapedQuotedString (const std::string& input)
{
	std::stringstream stream;

	stream << '"';

	text::utf8::Pointer text { input };

	auto writeUnicode = [&stream] (auto digit)
	{
		auto hexDigit = [] (auto value) -> char
		{ return "0123456789abcdef"[value & 15]; };

		stream << "\\u" << hexDigit (digit >> 12) << hexDigit (digit >> 8) << hexDigit (digit >> 4) << hexDigit (digit);
	};

	for (;;)
	{
		const auto c = *text;

		switch (c)
		{
			case 0 : break;

			case '\"' : stream << "\\\""; break;
			case '\\' : stream << "\\\\"; break;
			case '\n' : stream << "\\n"; break;
			case '\r' : stream << "\\r"; break;
			case '\t' : stream << "\\t"; break;
			case '\a' : stream << "\\a"; break;
			case '\b' : stream << "\\b"; break;
			case '\f' : stream << "\\f"; break;

			default :
			{
				if (c > 31 && c < 127)
				{
					stream << static_cast<char> (c);
					break;
				}

				if (c >= 0x10000)
				{
					const auto pair = text::utf8::SurrogatePair::fromFullCodepoint (c);

					writeUnicode (pair.high);
					writeUnicode (pair.low);

					break;
				}

				writeUnicode (c);
				break;
			}
		}

		++text;
	}

	stream << '"';

	return stream.str();
}

std::string Node::getJsonString() const
{
	if (isNumber())
	{
		if (std::isfinite (data.number))
			return text::unquoted (std::to_string (data.number));

		if (std::isnan (data.number))
			return "\"NaN\"";

		if (data.number >= 0)
			return "\"Infinity\"";

		return "\"-Infinity\"";
	}

	if (isString())
		return getEscapedQuotedString (data.string);

	if (isBoolean())
	{
		if (data.boolean)
			return "true";

		return "false";
	}

	if (isNull())
		return "null";

	if (isArray())
	{
		std::vector<std::string> strings;

		for (const auto& element : data.array)
			strings.emplace_back (element.getJsonString());	 // cppcheck-suppress useStlAlgorithm

		return "[ " + text::join (strings, ", ") + " ]";
	}

	LIMES_ASSERT (isObject());

	std::vector<std::string> strings;

	for (const auto& element : data.object)
	{
		auto str = text::quoted (element.first);
		str += ':';
		str += element.second.getJsonString();

		strings.emplace_back (str);
	}

	return "{ " + text::join (strings, ", ") + " }";
}

/*-----------------------------------------------------------------------------------------------------------------------*/

std::string Node::getXMLString() const
{
	if (isNumber())
		return text::quoted (std::to_string (data.number));

	if (isString())
		return text::quoted (data.string);

	if (isBoolean())
	{
		if (data.boolean)
			return "true";

		return "false";
	}

	if (isNull())
		return "";

	if (isArray())
	{
	}

	LIMES_ASSERT (isObject());

	return "";
}

/*-----------------------------------------------------------------------------------------------------------------------*/

std::string Node::getString (StringType stringType) const
{
	switch (stringType)
	{
		case (StringType::JSON) : return getJsonString();
		case (StringType::XML) : return getXMLString();
		default : LIMES_UNREACHABLE;
	}
}

}  // namespace serializing

LIMES_END_NAMESPACE
