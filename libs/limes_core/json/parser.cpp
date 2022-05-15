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

#include "json.h"
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace json
{

[[nodiscard]] inline std::vector<std::pair<std::string, std::string>> parseObjectIntoStrings (std::string& text)
{
	std::vector<std::pair<std::string, std::string>> strings;

	while (! text.empty())
	{
		strings::trim (text);

		std::pair<std::string, std::string> pair;

		pair.first = strings::upToFirstOccurrenceOf (text, ":");

		text = strings::fromFirstOccurrenceOf (text, ":");

		strings::trim (text);

		if (! (text.starts_with ('{') || text.starts_with ('[')))
		{
			pair.second = strings::upToFirstOccurrenceOf (text, ",");
			text		= strings::fromFirstOccurrenceOf (text, ",");
		}
		else
		{
			// iterate through string, until an even number of {} or[] are found
			// stop this token at the closing brace

			const auto endPos = [&text]
			{
				auto pos = text.begin();

				bool openCurly	 = *pos == '{';
				bool openBracket = *pos == '[';

				using size_type = std::string::size_type;

				while (pos != text.end())
				{
					const auto cur = *pos;

					if (cur == '{')
						openCurly = true;
					else if (cur == '}')
						openCurly = false;
					else if (cur == '[')
						openBracket = true;
					else if (cur == ']')
						openBracket = false;

					if (! (openCurly || openBracket))
						return static_cast<size_type> (std::distance (text.begin(), pos));

					++pos;
				}

				return static_cast<size_type> (text.length());
			}();

			pair.second = text.substr (0, endPos);

			text = text.substr (endPos, text.length());
		}

		strings::trim (pair.first);
		strings::trim (pair.second);

		strings.emplace_back (pair);
	}

	return strings;
}

Node::Node (const std::string_view& jsonText)
{
	std::string inputText { jsonText };

	strings::trim (inputText);

	type = [&inputText]
	{
		if (inputText.starts_with ('['))
			return ObjectType::Array;

		if (inputText.starts_with ('{')
			|| inputText.find (':') < inputText.find (',')
			|| inputText.find (':') < inputText.find ('['))
			return ObjectType::Object;

		if (inputText == "null")
			return ObjectType::Null;

		if (inputText == "true" || inputText == "false")
			return ObjectType::Boolean;

		if (inputText.starts_with ('"'))
			return ObjectType::String;

		return ObjectType::Number;
	}();

	switch (type)
	{
		case (ObjectType::Null) : return;

		case (ObjectType::Number) :
		{
			// TODO: deal with exponential E notation
			number = std::stod (std::string { inputText });
			return;
		}

		case (ObjectType::String) :
		{
			// TODO: deal with escaping
			string = strings::unquoted (inputText);
			return;
		}

		case (ObjectType::Boolean) :
		{
			boolean = (inputText == "true");
			return;
		}

		case (ObjectType::Array) :
		{
			inputText = strings::fromFirstOccurrenceOf (inputText, "[");
			strings::trim (inputText);

			// need to find the closing ]
			for (const auto& substring : strings::split (inputText, ",", false))
				array.emplace_back (Node { substring });

			return;
		}

		case (ObjectType::Object) :
		{
			inputText = strings::fromFirstOccurrenceOf (inputText, "{");
			strings::trim (inputText);

			if (inputText.ends_with ('}'))
				strings::dropLastChars (inputText, 1);

			for (const auto& pair : parseObjectIntoStrings (inputText))
				object.emplace_back (pair.first, Node { pair.second });

			return;
		}

		default : LIMES_UNREACHABLE;
	}
}

}  // namespace json

LIMES_END_NAMESPACE
