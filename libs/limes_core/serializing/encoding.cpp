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
#include <vector>

LIMES_BEGIN_NAMESPACE

namespace serializing
{

std::string Node::getJsonString() const
{
	if (isNumber())
		return strings::unquoted (std::to_string (number));

	if (isString())
		return strings::quoted (string);

	if (isBoolean())
	{
		if (boolean)
			return "true";

		return "false";
	}

	if (isArray())
	{
		std::vector<std::string> strings;

		for (const auto& element : array)
			strings.emplace_back (element.getJsonString());	 // cppcheck-suppress useStlAlgorithm

		return "[" + strings::join (strings, ",") + "]";
	}

	if (isObject())
	{
		std::vector<std::string> strings;

		for (const auto& element : object)
		{
			auto str = strings::quoted (element.first);
			str += ':';
			str += element.second.getJsonString();

			strings.emplace_back (str);
		}

		return "{" + strings::join (strings, ",") + "}";
	}

	LIMES_ASSERT (isNull());

	return "null";
}

std::string Node::getXMLString() const
{
}

std::string Node::getString (StringType type) const
{
	switch (type)
	{
		case (StringType::JSON) : return getJsonString();
		case (StringType::XML) : return getXMLString();
		default : LIMES_UNREACHABLE;
	}
}

}  // namespace serializing

LIMES_END_NAMESPACE
