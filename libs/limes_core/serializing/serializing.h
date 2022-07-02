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

#include "../limes_namespace.h"
#include <limes_export.h>
#include <string_view>
#include <string>
#include <vector>
#include <map>
#include <variant>
#include <utility>
#include <type_traits>
#include "../misc/preprocessor.h"
#include "../meta/TypeList.h"
#include <functional>  // for std::hash
#include <stdexcept>
#include "../text/UTF8.h"

/** @defgroup serializing Serializing
	Utilities for data serialization.
	@ingroup limes_core

	@todo clean up this directory - move node class into its own files
	@todo JSON encoder - options for pretty printing
	@todo XML parser, encoder
	@todo YAML parser, encoder
	@todo TOML parser, encoder
	@todo INI parser, encoder
	@todo use the SerializableData base class throughout the codebase
	@todo JSON/XML validate schema
	@todo write unit tests
 */

/** @dir libs/limes_core/serializing
	This directory contains serialization utilities.
	@ingroup limes_core
 */

/** @file
	This file defines the Node class and other serialization utilities.
	@ingroup serializing
 */

LIMES_BEGIN_NAMESPACE

/** This namespace contains utilities for working with serializing.
	@ingroup serializing
 */
namespace serializing
{

/** Represents the type of a serialization node.
	These types align with the JSON specification.
	@see Node
	@ingroup serializing
 */
LIMES_EXPORT enum class ObjectType : int {
	Number = 0, /**< A number. Numbers are always represented using doubles. */
	String,		/**< A string. */
	Boolean,	/**< A boolean. */
	Array,		/**< An array of other objects. Arrays can contain any kind of child ObjectTypes. */
	Object,		/**< An Object is a collection of key/value pairs, where the keys are string identifiers and the values are any ObjectType. */
	Null		/**< Represents null. */
};

/** Represents the format of data serialized as a string.
	@ingroup serializing
 */
LIMES_EXPORT enum class StringType : int {
	JSON, /**< Represents a JSON string. */
	XML	  /**< Represents an XML string. */
};

class Node;

/** The representation of Array types.
	@ingroup serializing
 */
LIMES_EXPORT using Array = std::vector<Node>;

/** The representation of Object types.
	@see ObjectField
	@ingroup serializing
 */
LIMES_EXPORT using Object = std::map<std::string, Node>;

/** This typedef evaluates to the appropriate C++ object type for the given ObjectType.

	The types evaluate as follows:

	ObjectType | C++ type
	---------- | -----------------------------------------------------
	Number     | double
	String     | std::string
	Boolean    | bool
	Array      | Array (std::vector<Node>)
	Object     | Object (std::map<std::string, Node>)
	Null       | meta::NullType

	@ingroup serializing
	@see ObjectType
 */
template <ObjectType Type>
LIMES_EXPORT using DataType = std::conditional_t<Type == ObjectType::Number, double,
												 std::conditional_t<Type == ObjectType::String, std::string,
																	std::conditional_t<Type == ObjectType::Boolean, bool,
																					   std::conditional_t<Type == ObjectType::Array, Array,
																										  std::conditional_t<Type == ObjectType::Object, Object, meta::NullType>>>>>;

#pragma mark Node

/** Represents any kind of serializable object.
	This class is designed around the JSON specification, but can also be parsed from or serialized to other formats, like XML.

	A Node object must be assigned an ObjectType upon construction, which will be constant for its lifetime.

	@ingroup serializing
 */
class LIMES_EXPORT Node final
{
public:
	/** Creates a Node with a specified type. */
	explicit Node (ObjectType typeToUse) noexcept;

	/** @name Copying */
	///@{
	/** Copy constructor. */
	Node (const Node& other);

	/** Copy assignment operator. */
	Node& operator= (const Node& other);
	///@}

	/** @name Moving */
	///@{
	/** Move constructor. */
	Node (Node&& other) noexcept;

	/** Move assignment operator. */
	Node& operator= (Node&& other) noexcept;
	///@}

	/** @name Subscript operators */
	///@{
	/** For Object nodes, finds and returns the child %node with the specified name.
		An assertion will be thrown if this %node is not an Object.
		@throws std::runtime_error An exception is thrown if no child %node with the specified name exists.
	 */
	Node& operator[] (const std::string_view& childName);

	/** For Array nodes, returns the child %node at the given index in the array.
		An assertion will be thrown if this %node is not an Array.
		@throws std::out_of_range An exception is thrown if the requested index is out of range of the array.
	 */
	Node& operator[] (std::size_t idx);
	///@}

	/** @name Querying child nodes */
	///@{
	/** For arrays or objects, returns the number of child nodes this %node contains.
		If this %node is not an array or object, returns 0.
	 */
	int getNumChildren() const noexcept;

	/** For Object nodes, returns true if this %node has a child with the specified name.
		If this %node is not an object, this always returns false.
	 */
	bool hasChildWithName (const std::string_view& childName);
	///@}

	/** @name Querying parent nodes */
	///@{
	/** Returns this node's parent %node, if it has one.
		This may return nullptr.
		@see hasParent()
	 */
	Node* getParent() const noexcept;

	/** Returns true if this %node is a child of another %node.
		@see getParent()
	 */
	bool hasParent() const noexcept;

	/** Traverses all parent nodes until a %node is found that does not have a parent.
		If this %node doesn't have a parent, returns a reference to this %node.
	 */
	Node& getRoot() noexcept;
	///@}

	/** @name Querying this node's name */
	///@{
	/** Returns true if this %node is a child of an Object %node.
		@see getName()
	 */
	bool hasName() const noexcept;

	/** Returns this node's name.
		If \c hasName() returns false, this will return an empty string.
		@see hasName()
	 */
	std::string_view getName() const noexcept;
	///@}

	/** @name Encoding */
	///@{
	/** Returns this %node (and any child nodes) encoded as JSON. */
	[[nodiscard]] std::string getJsonString() const;

	/** Returns this %node (and any child nodes) encoded as XML. */
	[[nodiscard]] std::string getXMLString() const;

	/** Returns this %node (and any child nodes) encoded in the desired format. */
	[[nodiscard]] std::string getString (StringType stringType) const;
	///@}

	/** @name Type queries */
	///@{
	/** Returns true if this %node is a number. */
	bool isNumber() const noexcept;

	/** Returns true if this %node is a string. */
	bool isString() const noexcept;

	/** Returns true if this %node is a boolean. */
	bool isBoolean() const noexcept;

	/** Returns true if this %node is an array. */
	bool isArray() const noexcept;

	/** Returns true if this %node is an object. */
	bool isObject() const noexcept;

	/** Returns true if this %node is null. */
	bool isNull() const noexcept;

	/** Returns the type of this %node. */
	ObjectType getType() const noexcept;

	/** Returns a string representation of this node's type. */
	[[nodiscard]] std::string_view getTypeAsString() const noexcept;
	///@}

	/** @name Member accessors */
	///@{

	/** Returns a reference to the internal number object stored by this %node.
		An assertion will be thrown if this %node is not a number (ie, if \c isNumber() returns false).
	 */
	double& getNumber() noexcept;

	/** Returns a reference to the internal string object stored by this %node.
		An assertion will be thrown if this %node is not a string (ie, if \c isString() returns false).
	 */
	std::string& getString() noexcept;

	/** Returns a reference to the internal boolean object stored by this %node.
		An assertion will be thrown if this %node is not a boolean (ie, if \c isBoolean() returns false).
	 */
	bool& getBoolean() noexcept;

	/** Returns a reference to the internal array object stored by this %node.
		An assertion will be thrown if this %node is not an array (ie, if \c isArray() returns false).
	 */
	Array& getArray() noexcept;

	/** Returns a reference to the internal object stored by this %node.
		An assertion will be thrown if this %node is not an object (ie, if \c isObject() returns false).
	 */
	Object& getObject() noexcept;

	/** Templated function that accesses the stored data of the specified type.
		If you attempt to access a datatype that this Node isn't currently storing, an exception will be thrown.
	 */
	template <typename Type>
	Type& get();
	///@}

	/** @name Assignment operators */
	///@{
	/** Assigns this %node to a new number value.
		An assertion will be thrown if this %node is not a number (ie, if \c isNumber() returns false).
	 */
	Node& operator= (double value) noexcept;

	/** Assigns this %node to a new string value.
		An assertion will be thrown if this %node is not a string (ie, if \c isString() returns false).
	 */
	Node& operator= (const std::string_view& value) noexcept;

	/** Assigns this %node to a new boolean value.
		An assertion will be thrown if this %node is not a boolean (ie, if \c isBoolean() returns false).
	 */
	Node& operator= (bool value) noexcept;

	/** Assigns this %node to a new Array value.
		An assertion will be thrown if this %node is not an array (ie, if \c isArray() returns false).
	 */
	Node& operator= (const Array& value);

	/** Assigns this %node to a new Object value.
		An assertion will be thrown if this %node is not an object (ie, if \c isObject() returns false).
	 */
	Node& operator= (const Object& value);
	///@}

	/** @name Adding child nodes */
	///@{
	/** Adds a new child %node that holds a number value.
		Child nodes can only be added to Array or Object nodes.
		@returns A reference to the new child %node
	 */
	Node& addChildNumber (const std::string_view& childName = "");

	/** Adds a new child %node that holds the specified number value.
		Child nodes can only be added to Array or Object nodes.
		@returns A reference to the new child %node
	 */
	Node& addChildNumber (double value, const std::string_view& childName = "");

	/** Adds a new child %node that holds a string value.
		Child nodes can only be added to Array or Object nodes.
		@returns A reference to the new child %node
	 */
	Node& addChildString (const std::string_view& childName = "");

	/** Adds a new child %node that holds a boolean value.
		Child nodes can only be added to Array or Object nodes.
		@returns A reference to the new child %node
	 */
	Node& addChildBoolean (const std::string_view& childName = "");

	/** Adds a new child %node that holds the specified boolean value.
		Child nodes can only be added to Array or Object nodes.
		@returns A reference to the new child %node
	 */
	Node& addChildBoolean (bool value, const std::string_view& childName = "");

	/** Adds a new child %node that holds an Array.
		Child nodes can only be added to Array or Object nodes.
		@returns A reference to the new child %node
	 */
	Node& addChildArray (const std::string_view& childName = "");

	/** Adds a new child %node that holds the specified Array.
		Child nodes can only be added to Array or Object nodes.
		@returns A reference to the new child %node
	 */
	Node& addChildArray (const Array& value, const std::string_view& childName = "");

	/** Adds a new child %node that holds an Object.
		Child nodes can only be added to Array or Object nodes.
		@returns A reference to the new child %node
	 */
	Node& addChildObject (const std::string_view& childName = "");

	/** Adds a new child %node that holds the specified Object.
		Child nodes can only be added to Array or Object nodes.
		@returns A reference to the new child %node
	 */
	Node& addChildObject (const Object& value, const std::string_view& childName = "");

	/** Adds a new child %node that is null.
		Child nodes can only be added to Array or Object nodes.
		@returns A reference to the new child %node
	 */
	Node& addChildNull (const std::string_view& childName = "");

	/** Adds a new child %node of the specified type.
		Child nodes can only be added to Array or Object nodes.
		@returns A reference to the new child %node
	 */
	Node& addChild (ObjectType childType, const std::string_view& childName = "");

	/** Adds the passed %node as a child of this one.
		Internally, a copy of the passed node will be created.
		@returns A reference to the new child %node
	 */
	Node& addChild (const Node& childNode, const std::string_view& childName = "");
	///@}

	/** @name Creation functions */
	///@{
	/** Creates a number Node. */
	[[nodiscard]] static Node createNumber (double value);

	/** Creates a string Node. */
	[[nodiscard]] static Node createString (const std::string_view& value);

	/** Creates a boolean Node. */
	[[nodiscard]] static Node createBoolean (bool value);

	/** Creates a null Node. */
	[[nodiscard]] static Node createNull();
	///@}

private:
	Node& addChildInternal (const std::string_view& childName, ObjectType childType);

	ObjectType type { ObjectType::Null };

	std::variant<double, std::string, bool, Array, Object> data;

	Node* parent { nullptr };
};

#pragma mark Parsing functions

/** An exception that is thrown by the \c parseJSON() function if errors are encountered.
 */
struct LIMES_EXPORT JSONParseError final : public std::runtime_error
{
	JSONParseError (const std::string_view& message, const text::utf8::LineAndColumn& lc)
		: std::runtime_error (message.data()), position (lc) { }

	text::utf8::LineAndColumn position;
};

/** Parses a JSON string and returns the root Node of the resulting data structure.
	@throws JSONParseError Throws an exception if there is a JSON parsing error.
	@see parseXML
	@ingroup serializing
 */
LIMES_EXPORT [[nodiscard]] Node parseJSON (const std::string_view& jsonText);

/** Parses an XML string and returns the root Node of the resulting data structure.
	@see parseJSON
	@ingroup serializing
 */
LIMES_EXPORT [[nodiscard]] Node parseXML (const std::string_view& xmlText);

/** Parses a string of serialized data, treating it as the specified type.
	@ingroup serializing
 */
LIMES_EXPORT [[nodiscard]] Node parse (const std::string_view& string, StringType type);

#pragma mark SerializableData

/** This class provides an interface for any C++ object that can be serialized to and from a Node.
	@ingroup serializing
 */
class LIMES_EXPORT SerializableData
{
public:
	/** Destructor. */
	virtual ~SerializableData() = default;

	/** This function must reproducibly save the state of this object. */
	virtual Node serialize() = 0;

	/** This function must restore a previous state saved by \c serialize() . */
	virtual void deserialize (const Node&) = 0;
};

#pragma mark Conversion functions

/** This namespace contains utilities for converting between various formats of serialized data.
	@ingroup serializing
 */
namespace conversion
{

/** Converts an XML string to JSON.
	@ingroup serializing
 */
LIMES_EXPORT [[nodiscard]] std::string toJSON (const std::string_view& xmlText);

/** Converts a JSON string to XML.
	@ingroup serializing
 */
LIMES_EXPORT [[nodiscard]] std::string toXML (const std::string_view& jsonText);

}  // namespace conversion

}  // namespace serializing

LIMES_END_NAMESPACE

namespace std
{

/** A specialization of \c std::hash for Node objects.
	The hash value is computed based on the JSON string created from the Node.
	@ingroup serializing
 */
template <>
struct LIMES_EXPORT hash<limes::serializing::Node> final
{
	hash() = default;

	LIMES_DEFAULT_COPYABLE (hash)
	LIMES_DEFAULT_MOVABLE (hash)

	size_t operator() (const limes::serializing::Node& n) const noexcept;
};

}  // namespace std
