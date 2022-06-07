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
#include <string_view>
#include <string>
#include <cmath>
#include <sstream>
#include "../system/limes_assert.h"
#include <stdexcept>

LIMES_BEGIN_NAMESPACE

namespace serializing
{

Node::Node (ObjectType typeToUse) noexcept
	: type (typeToUse)
{
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

Node& Node::operator[] (const std::string_view& childName)
{
	LIMES_ASSERT (isObject());

	for (auto& pair : object)
		if (pair.first == childName)  // cppcheck-suppress useStlAlgorithm
			return pair.second;

	throw std::runtime_error { "Child node could not be found!" };
}

Node& Node::operator[] (std::size_t idx)
{
	LIMES_ASSERT (isArray());

	if (idx >= static_cast<std::size_t> (array.size()))
		throw std::out_of_range { "Array index out of range!" };

	return array[static_cast<Array::size_type> (idx)];
}

int Node::getNumChildren() const noexcept
{
	if (isArray())
		return static_cast<int> (array.size());

	if (isObject())
		return static_cast<int> (object.size());

	return 0;
}

bool Node::hasChildWithName (const std::string_view& childName)
{
	if (! isObject())
		return false;

	for (auto& pair : object)
		if (pair.first == childName)  // cppcheck-suppress useStlAlgorithm
			return true;

	return false;
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

Node& Node::addChildNumber (double value, const std::string_view& childName)
{
	auto& child = addChildNumber (childName);

	child.getNumber() = value;

	return child;
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

Node& Node::addChildBoolean (bool value, const std::string_view& childName)
{
	auto& child = addChildBoolean (childName);

	child.getBoolean() = value;

	return child;
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

Node& Node::addChildArray (const Array& value, const std::string_view& childName)
{
	auto& child = addChildArray (childName);

	child.getArray() = value;

	return child;
}

bool Node::isObject() const noexcept
{
	return type == ObjectType::Object;
}

Object& Node::getObject() noexcept
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

Node& Node::addChildObject (const Object& value, const std::string_view& childName)
{
	auto& child = addChildObject (childName);

	child.getObject() = value;

	return child;
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

/*--------------------------------------------------------------------------------------------------------*/

Node parse (const std::string_view& string, StringType type)
{
	switch (type)
	{
		case (StringType::JSON) : return parseJSON (string);
		case (StringType::XML) : return parseXML (string);
		default : LIMES_UNREACHABLE;
	}
}

namespace conversion
{

std::string toJSON (const std::string_view& xmlText)
{
	return parseXML (xmlText).getJsonString();
}

std::string toXML (const std::string_view& jsonText)
{
	return parseJSON (jsonText).getXMLString();
}

}  // namespace conversion

}  // namespace serializing

LIMES_END_NAMESPACE
