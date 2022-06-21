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
	if (type == ObjectType::Number)
		data.number = 0.;
	else if (type == ObjectType::String)
		data.string = "";
	else if (type == ObjectType::Boolean)
		data.boolean = false;
}

Node::Node (const Node& other)
	: type (other.type), parent (other.parent)
{
	switch (type)
	{
		case (ObjectType::Null) : return;
		case (ObjectType::Object) : data.object = other.data.object; return;
		case (ObjectType::Array) : data.array = other.data.array; return;
		case (ObjectType::Boolean) : data.boolean = other.data.boolean; return;
		case (ObjectType::String) : data.string = other.data.string; return;
		case (ObjectType::Number) : data.number = other.data.number; return;
		default : LIMES_UNREACHABLE;
	}
}

Node& Node::operator= (const Node& other)
{
	type   = other.type;
	parent = other.parent;

	switch (type)
	{
		case (ObjectType::Null) : return *this;
		case (ObjectType::Object) : data.object = other.data.object; return *this;
		case (ObjectType::Array) : data.array = other.data.array; return *this;
		case (ObjectType::Boolean) : data.boolean = other.data.boolean; return *this;
		case (ObjectType::String) : data.string = other.data.string; return *this;
		case (ObjectType::Number) : data.number = other.data.number; return *this;
		default : LIMES_UNREACHABLE;
	}

	return *this;
}

Node::Node (Node&& other)
	: type (other.type), parent (other.parent)
{
	switch (type)
	{
		case (ObjectType::Null) : return;
		case (ObjectType::Object) : data.object = std::move (other.data.object); return;
		case (ObjectType::Array) : data.array = std::move (other.data.array); return;
		case (ObjectType::Boolean) : data.boolean = other.data.boolean; return;
		case (ObjectType::String) : data.string = std::move (other.data.string); return;
		case (ObjectType::Number) : data.number = other.data.number; return;
		default : LIMES_UNREACHABLE;
	}
}

Node& Node::operator= (Node&& other)
{
	type   = other.type;
	parent = other.parent;

	switch (type)
	{
		case (ObjectType::Null) : return *this;
		case (ObjectType::Object) : data.object = std::move (other.data.object); return *this;
		case (ObjectType::Array) : data.array = std::move (other.data.array); return *this;
		case (ObjectType::Boolean) : data.boolean = other.data.boolean; return *this;
		case (ObjectType::String) : data.string = std::move (other.data.string); return *this;
		case (ObjectType::Number) : data.number = other.data.number; return *this;
		default : LIMES_UNREACHABLE;
	}

	return *this;
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

	for (auto& pair : data.object)
		if (pair.first == childName)  // cppcheck-suppress useStlAlgorithm
			return pair.second;

	throw std::runtime_error { "Child node could not be found!" };
}

Node& Node::operator[] (std::size_t idx)
{
	LIMES_ASSERT (isArray());

	if (idx >= static_cast<std::size_t> (data.array.size()))
		throw std::out_of_range { "Array index out of range!" };

	return data.array[static_cast<Array::size_type> (idx)];
}

int Node::getNumChildren() const noexcept
{
	if (isArray())
		return static_cast<int> (data.array.size());

	if (isObject())
		return static_cast<int> (data.object.size());

	return 0;
}

bool Node::hasChildWithName (const std::string_view& childName)
{
	if (! isObject())
		return false;

	for (auto& pair : data.object)
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

Node& Node::addChild (const Node& childNode, const std::string_view& childName)
{
	if (isArray())
	{
		LIMES_ASSERT (childName.empty());

		auto& child = data.array.emplace_back (childNode);

		child.parent = this;

		return child;
	}

	if (isObject())
	{
		LIMES_ASSERT (! childName.empty());

		auto& child = data.object.emplace_back (childName, childNode).second;

		child.parent = this;

		return child;
	}

	LIMES_ASSERT_FALSE;
	return *this;
}

bool Node::isNumber() const noexcept
{
	return type == ObjectType::Number;
}

double& Node::getNumber() noexcept
{
	LIMES_ASSERT (isNumber());
	return data.number;
}

Node& Node::operator= (double value) noexcept
{
	LIMES_ASSERT (isNumber());
	data.number = value;
	return *this;
}

Node& Node::addChildInternal (const std::string_view& childName, ObjectType childType)
{
	if (isArray())
	{
		LIMES_ASSERT (childName.empty());

		auto& child = data.array.emplace_back (childType);

		child.parent = this;

		return child;
	}

	if (isObject())
	{
		LIMES_ASSERT (! childName.empty());

		auto& child = data.object.emplace_back (childName, childType).second;

		child.parent = this;

		return child;
	}

	LIMES_ASSERT_FALSE;
	return *this;
}

Node& Node::addChildNumber (const std::string_view& childName)
{
	return addChildInternal (childName, ObjectType::Number);
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
	return data.string;
}

Node& Node::operator= (const std::string_view& value) noexcept
{
	LIMES_ASSERT (isString());

	data.string = value;

	return *this;
}

Node& Node::addChildString (const std::string_view& childName)
{
	return addChildInternal (childName, ObjectType::String);
}

bool Node::isBoolean() const noexcept
{
	return type == ObjectType::Boolean;
}

bool& Node::getBoolean() noexcept
{
	LIMES_ASSERT (isBoolean());
	return data.boolean;
}

Node& Node::operator= (bool value) noexcept
{
	LIMES_ASSERT (isBoolean());

	data.boolean = value;

	return *this;
}

Node& Node::addChildBoolean (const std::string_view& childName)
{
	return addChildInternal (childName, ObjectType::Boolean);
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

Array& Node::getArray() noexcept
{
	LIMES_ASSERT (isArray());
	return data.array;
}

Node& Node::operator= (const Array& value)
{
	LIMES_ASSERT (isArray());
	data.array = value;
	return *this;
}

Node& Node::addChildArray (const std::string_view& childName)
{
	return addChildInternal (childName, ObjectType::Array);
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
	return data.object;
}

Node& Node::operator= (const Object& value)
{
	LIMES_ASSERT (isObject());
	data.object = value;
	return *this;
}

Node& Node::addChildObject (const std::string_view& childName)
{
	return addChildInternal (childName, ObjectType::Object);
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
	return addChildInternal (childName, ObjectType::Null);
}

Node* Node::getParent() const noexcept
{
	return parent;
}

Node& Node::getRoot() noexcept
{
	if (parent == nullptr)
		return *this;

	auto* curr = parent;

	while (curr->parent != nullptr)
	{
		curr = curr->parent;
	}

	LIMES_ASSERT (curr != nullptr);

	return *curr;
}

bool Node::hasParent() const noexcept
{
	return parent != nullptr;
}

bool Node::hasName() const noexcept
{
	if (parent == nullptr)
		return false;

	return parent->isObject();
}

std::string_view Node::getName() const noexcept
{
	if (! hasName())
		return "";

	for (const auto& pair : parent->data.object)
		if (&pair.second == this)  // cppcheck-suppress useStlAlgorithm
			return pair.first;

	LIMES_ASSERT_FALSE;
	return "";
}

Node Node::createNumber (double value)
{
	auto result = Node { ObjectType::Number };

	result.getNumber() = value;

	return result;
}

Node Node::createString (const std::string_view& value)
{
	auto result = Node { ObjectType::String };

	result.getString() = value;

	return result;
}

Node Node::createBoolean (bool value)
{
	auto result = Node { ObjectType::Boolean };

	result.getBoolean() = value;

	return result;
}

Node Node::createNull()
{
	return Node { ObjectType::Null };
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

namespace std
{

size_t hash<limes::serializing::Node>::operator() (const limes::serializing::Node& n) const noexcept
{
	return hash<std::string> {}(n.getJsonString());
}

}  // namespace std
