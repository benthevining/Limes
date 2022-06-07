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

#include "serializing.h"
#include <limes_namespace.h>
#include <string>
#include <string_view>
#include <vector>
#include <utility>
#include <stdexcept>
#include "../text/StringUtils.h"
#include "../system/limes_assert.h"

LIMES_BEGIN_NAMESPACE

namespace serializing
{

namespace json
{

[[nodiscard]] inline std::string_view findMatchingDelimiters (const std::string_view& text, char beginDelim, char endDelim)
{
	const auto beginIdx = text.find (beginDelim);

	bool inObject = false, inArray = false;

	for (auto i = beginIdx + 1; i < text.length(); ++i)
	{
		const auto currentChar = text[i];

		if (currentChar == '{')
		{
			inObject = true;
			continue;
		}

		if (currentChar == '[')
		{
			inArray = true;
			continue;
		}

		if (inObject && currentChar == '}')
		{
			inObject = false;
			continue;
		}

		if (inArray && currentChar == ']')
		{
			inArray = false;
			continue;
		}

		if (! (inObject || inArray))
			if (currentChar == endDelim)
				return text.substr (beginIdx, i);
	}

	LIMES_ASSERT_FALSE;

	throw std::runtime_error { "JSON parsing error!" };
}

[[nodiscard]] inline std::vector<std::pair<std::string, std::string>> parseObjectIntoStrings (const std::string_view& text)
{
	std::vector<std::pair<std::string, std::string>> strings;

	for (const auto& str : strings::split (findMatchingDelimiters (text, '{', '}'),
										   ",", false))
	{
		strings.emplace_back (strings::upToFirstOccurrenceOf (str, ","),  // cppcheck-suppress useStlAlgorithm
							  strings::fromFirstOccurrenceOf (str, ","));
	}

	return strings;
}

[[nodiscard]] inline std::vector<std::string> parseArrayIntoStrings (const std::string_view& text)
{
	return strings::split (findMatchingDelimiters (text, '[', ']'),
						   ",",
						   false);
}

[[nodiscard]] inline ObjectType parseType (const std::string_view& inputText)
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
}

}  // namespace json


Node parseJSON (const std::string_view& jsonText)
{
	std::string inputText { jsonText };

	strings::trim (inputText);

	const auto type = json::parseType (inputText);

	Node node { type };

	switch (type)
	{
		case (ObjectType::Null) : return node;

		case (ObjectType::Number) :
		{
			// TODO: deal with exponential E notation
			node.getNumber() = std::stod (inputText);
			return node;
		}

		case (ObjectType::String) :
		{
			// TODO: deal with escaping
			node.getString() = strings::unquoted (inputText);
			return node;
		}

		case (ObjectType::Boolean) :
		{
			node.getBoolean() = (inputText == "true" || inputText == "True");
			return node;
		}

		case (ObjectType::Array) :
		{
			auto& array = node.getArray();

			for (const auto& substring : json::parseArrayIntoStrings (inputText))
				array.emplace_back (parseJSON (substring));	 // cppcheck-suppress useStlAlgorithm

			return node;
		}

		case (ObjectType::Object) :
		{
			auto& object = node.getObject();

			for (const auto& pair : json::parseObjectIntoStrings (inputText))
				object.emplace_back (pair.first, parseJSON (pair.second));	// cppcheck-suppress useStlAlgorithm

			return node;
		}

		default : LIMES_UNREACHABLE;
	}
}

}  // namespace serializing

LIMES_END_NAMESPACE
