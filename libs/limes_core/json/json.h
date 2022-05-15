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

#pragma once

#include <limes_namespace.h>
#include <limes_export.h>
#include <string_view>
#include <string>
#include <vector>
#include <variant>
#include <utility>
#include <limes_core.h>

LIMES_BEGIN_NAMESPACE

/** This namespace contains utilities for working with JSON. */
namespace json
{

/** Represents the type of a JSON node. */
LIMES_EXPORT enum class ObjectType : int {
	Number,
	String,
	Boolean,
	Array,
	Object,
	Null
};

/** Represents any kind of JSON object. */
class LIMES_EXPORT Node final
{
public:

	explicit Node (ObjectType typeToUse) noexcept;

	explicit Node (const std::string_view& jsonText);

	LIMES_DEFAULT_COPYABLE (Node);
	LIMES_DEFAULT_MOVABLE (Node);

	/** Re-encodes this object into its JSON string representation. */
	[[nodiscard]] std::string getJsonText (bool compact = false) const;

	/** Returns the type of this object. */
	[[nodiscard]] ObjectType getType() const noexcept;

	[[nodiscard]] std::string_view getTypeAsString() const noexcept;

	Node& addChild (ObjectType childType, const std::string_view& childName = "");

	bool isNumber() const noexcept;

	double& getNumber() noexcept;

	Node& operator= (double value) noexcept;

	Node& addChildNumber (const std::string_view& childName = "");

	bool isString() const noexcept;

	std::string& getString() noexcept;

	Node& operator= (const std::string_view& value) noexcept;

	Node& addChildString (const std::string_view& childName = "");

	bool isBoolean() const noexcept;

	bool& getBoolean() noexcept;

	Node& operator= (bool value) noexcept;

	Node& addChildBoolean (const std::string_view& childName = "");

	bool isArray() const noexcept;

	using Array = std::vector<Node>;

	Array& getArray() noexcept;

	Node& operator= (const Array& value);

	Node& addChildArray (const std::string_view& childName = "");

	using ObjectField = std::pair<std::string, Node>;

	using Object = std::vector<ObjectField>;

	bool isObject() const noexcept;

	Object& getObject() noexcept;

	Node& operator= (const Object& value);

	Node& addChildObject (const std::string_view& childName = "");

	bool isNull() const noexcept;

	Node& addChildNull (const std::string_view& childName = "");

	[[nodiscard]] std::string getDebugString() const;

private:

	ObjectType type { ObjectType::Null };

	double number { 0. };

	std::string string;

	bool boolean { false };

	Array array;

	Object object;
};

LIMES_EXPORT [[nodiscard]] Node parseJSON (const std::string_view& jsonText);

}  // namespace json

LIMES_END_NAMESPACE
