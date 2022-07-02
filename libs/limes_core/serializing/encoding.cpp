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

#include "../limes_namespace.h"
#include "./serializing.h"
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

	while (! text.empty())
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
		const auto number = std::get<double> (data);

		if (std::isfinite (number))
			return text::unquoted (std::to_string (number));

		if (std::isnan (number))
			return "\"NaN\"";

		if (number >= 0)
			return "\"Infinity\"";

		return "\"-Infinity\"";
	}

	if (isString())
		return getEscapedQuotedString (std::get<std::string> (data));

	if (isBoolean())
	{
		if (std::get<bool> (data))
			return "true";

		return "false";
	}

	if (isNull())
		return "null";

	std::vector<std::string> strings;

	if (isArray())
	{
		for (const auto& element : std::get<Array> (data))
			strings.emplace_back (element.getJsonString());	 // cppcheck-suppress useStlAlgorithm

		return "[ " + text::join (strings, ", ") + " ]";
	}

	LIMES_ASSERT (isObject());

	for (const auto& element : std::get<Object> (data))
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
		return text::quoted (std::to_string (std::get<double> (data)));

	if (isString())
		return text::quoted (std::get<std::string> (data));

	if (isBoolean())
	{
		if (std::get<bool> (data))
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
		default : LIMES_UNREACHABLE; return "";
	}
}

}  // namespace serializing

LIMES_END_NAMESPACE
