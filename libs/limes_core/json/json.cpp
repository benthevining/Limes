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
#include <limes_core.h>
#include "json.h"
#include <string_view>
#include <string>
#include <cmath>
#include <sstream>

LIMES_BEGIN_NAMESPACE

namespace json
{

Node::Node (ObjectType typeToUse) noexcept
	: type (typeToUse)
{
}

std::string Node::getJsonText (bool compact) const
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
			strings.emplace_back (element.getJsonText());  // cppcheck-suppress useStlAlgorithm

		return "[" + strings::join (strings, ",") + "]";
	}

	if (isObject())
	{
		std::vector<std::string> strings;

		for (const auto& element : object)
		{
			auto str = strings::quoted (element.first);
			str += ':';
			str += element.second.getJsonText();

			strings.emplace_back (str);
		}

		return "{" + strings::join (strings, ",") + "}";
	}

	LIMES_ASSERT (isNull());

	return "null";
}

ObjectType Node::getType() const noexcept
{
	return type;
}

std::string_view Node::getTypeAsString() const noexcept
{
	switch (type)
	{
		case (ObjectType::Null) : return "Null";
		case (ObjectType::Object) : return "Object";
		case (ObjectType::Array) : return "Array";
		case (ObjectType::Boolean) : return "Boolean";
		case (ObjectType::String) : return "String";
		case (ObjectType::Number) : return "Number";
		default : LIMES_UNREACHABLE;
	}
}

Node& Node::addChild (ObjectType childType, const std::string_view& childName)
{
	switch (childType)
	{
		case (ObjectType::Null) : return addChildNull (childName);
		case (ObjectType::Object) : return addChildObject (childName);
		case (ObjectType::Array) : return addChildArray (childName);
		case (ObjectType::Boolean) : return addChildBoolean (childName);
		case (ObjectType::String) : return addChildString (childName);
		case (ObjectType::Number) : return addChildNumber (childName);
		default : LIMES_UNREACHABLE;
	}
}

bool Node::isNumber() const noexcept
{
	return type == ObjectType::Number;
}

double& Node::getNumber() noexcept
{
	LIMES_ASSERT (isNumber());
	return number;
}

Node& Node::operator= (double value) noexcept
{
	LIMES_ASSERT (isNumber());
	number = value;
	return *this;
}

Node& Node::addChildNumber (const std::string_view& childName)
{
	if (isArray())
	{
		LIMES_ASSERT (childName.empty());
		return array.emplace_back (ObjectType::Number);
	}

	if (isObject())
	{
		LIMES_ASSERT (! childName.empty());
		return object.emplace_back (childName, ObjectType::Number).second;
	}

	LIMES_ASSERT_FALSE;
	return *this;
}

bool Node::isString() const noexcept
{
	return type == ObjectType::String;
}

std::string& Node::getString() noexcept
{
	LIMES_ASSERT (isString());
	return string;
}

Node& Node::operator= (const std::string_view& value) noexcept
{
	LIMES_ASSERT (isString());

	string = value;

	return *this;
}

Node& Node::addChildString (const std::string_view& childName)
{
	if (isArray())
	{
		LIMES_ASSERT (childName.empty());
		return array.emplace_back (ObjectType::String);
	}

	if (isObject())
	{
		LIMES_ASSERT (! childName.empty());
		return object.emplace_back (childName, ObjectType::String).second;
	}

	LIMES_ASSERT_FALSE;
	return *this;
}

bool Node::isBoolean() const noexcept
{
	return type == ObjectType::Boolean;
}

bool& Node::getBoolean() noexcept
{
	LIMES_ASSERT (isBoolean());
	return boolean;
}

Node& Node::operator= (bool value) noexcept
{
	LIMES_ASSERT (isBoolean());

	boolean = value;

	return *this;
}

Node& Node::addChildBoolean (const std::string_view& childName)
{
	if (isArray())
	{
		LIMES_ASSERT (childName.empty());
		return array.emplace_back (ObjectType::Boolean);
	}

	if (isObject())
	{
		LIMES_ASSERT (! childName.empty());
		return object.emplace_back (childName, ObjectType::Boolean).second;
	}

	LIMES_ASSERT_FALSE;
	return *this;
}

bool Node::isArray() const noexcept
{
	return type == ObjectType::Array;
}

std::vector<Node>& Node::getArray() noexcept
{
	LIMES_ASSERT (isArray());
	return array;
}

Node& Node::operator= (const Array& value)
{
	LIMES_ASSERT (isArray());
	array = value;
	return *this;
}

Node& Node::addChildArray (const std::string_view& childName)
{
	if (isArray())
	{
		LIMES_ASSERT (childName.empty());
		return array.emplace_back (ObjectType::Array);
	}

	if (isObject())
	{
		LIMES_ASSERT (! childName.empty());
		return object.emplace_back (childName, ObjectType::Array).second;
	}

	LIMES_ASSERT_FALSE;
	return *this;
}

bool Node::isObject() const noexcept
{
	return type == ObjectType::Object;
}

Node::Object& Node::getObject() noexcept
{
	LIMES_ASSERT (isObject());
	return object;
}

Node& Node::operator= (const Object& value)
{
	LIMES_ASSERT (isObject());
	object = value;
	return *this;
}

Node& Node::addChildObject (const std::string_view& childName)
{
	if (isArray())
	{
		LIMES_ASSERT (childName.empty());
		return array.emplace_back (ObjectType::Object);
	}

	if (isObject())
	{
		LIMES_ASSERT (! childName.empty());
		return object.emplace_back (childName, ObjectType::Object).second;
	}

	LIMES_ASSERT_FALSE;
	return *this;
}

bool Node::isNull() const noexcept
{
	return type == ObjectType::Null;
}

Node& Node::addChildNull (const std::string_view& childName)
{
	if (isArray())
	{
		LIMES_ASSERT (childName.empty());
		return array.emplace_back (ObjectType::Null);
	}

	if (isObject())
	{
		LIMES_ASSERT (! childName.empty());
		return object.emplace_back (childName, ObjectType::Null).second;
	}

	LIMES_ASSERT_FALSE;
	return *this;
}

std::string Node::getDebugString() const
{
	std::stringstream stream;

	stream << "Type: " << getTypeAsString() << strings::new_line;

	if (isNull())
		return stream.str();

	if (isNumber() || isBoolean() || isString())
	{
		stream << "Value: " << getJsonText();
	}
	else if (isArray())
	{
		for (const auto& element : array)
			stream << " - " << element.getDebugString() << strings::new_line;
	}
	else if (isObject())
	{
		for (const auto& field : object)
			stream << " - " << field.first << " : " << field.second.getDebugString() << strings::new_line;
	}

	return stream.str();
}

Node parseJSON (const std::string_view& jsonText)
{
	return Node { jsonText };
}

}  // namespace json

LIMES_END_NAMESPACE
