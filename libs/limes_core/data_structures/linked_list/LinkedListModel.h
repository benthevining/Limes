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
#include <limes_export.h>
#include <limes_namespace.h>

/** @file
	This file defines the LinkedListModel class.
	@ingroup data_structures
 */

LIMES_BEGIN_NAMESPACE

namespace ds
{

/** This class is an abstract model of a node in a linked list.

	This class does not contain the data it references, it is designed to be insertable into any other
	predefined class to make the class a node of a linked list. For example:
	@code
	struct MyStruct
	{
		int someData;
		double someOtherData;

		limes::ds::LinkedListModel<MyStruct> listNode { *this };
	};
	@endcode

	Internally, this class simply holds pointers to the next and previous nodes of the list.

	@tparam ObjectType The type of objects the linked list will hold

	@see LinkedList
	@ingroup data_structures

	@todo add a method to reverse an entire linked list
	@todo write unit tests for this class
 */
template <typename ObjectType>
struct LIMES_EXPORT LinkedListModel final
{
public:

	/** Creates a linked list node.
		@param objectForThisNode The data object this node in the list refers to
		@param objectBeforeThisNode Pointer to the LinkedListModel object representing the node before this one in the list.
		If this is null, this node is assumed to be the head of the list.
		@param objectAfterThisNode Pointer to the LinkedListModel object representing the node after this one in the list.
		If this is null, this node is assumed to be the tail of the list.
	 */
	constexpr explicit LinkedListModel (ObjectType&		 objectForThisNode,
										LinkedListModel* objectBeforeThisNode = nullptr,
										LinkedListModel* objectAfterThisNode  = nullptr);

	/** Destructor. */
	~LinkedListModel();

	LIMES_CONSTEXPR_MOVABLE (LinkedListModel)
	LIMES_CONSTEXPR_COPYABLE (LinkedListModel)

	/** @name Connection removal
		You should usually prefer to use the connection replacement functions, as these functions can leave you with a split list or some orphaned nodes.
	 */
	///@{
	/** Removes this node's connection to the node before it.
		The previous node's \c after pointer is updated to *this* node's \c after pointer; this operation does not change this node's \c after pointer.
		@see removeAfterConnection, removeBothConnections
	 */
	constexpr void removeBeforeConnection() noexcept;

	/** Removes this node's connection to the node after it.
		The next node's \c before pointer is updated to *this* node's \c before pointer; this operation does not change this node's \c before pointer.
		@see removeBeforeConnection, removeBothConnections
	 */
	constexpr void removeAfterConnection() noexcept;

	/** Removes both of this node's connections, essentially removing it from its list.
		@see removeBeforeConnection, removeAfterConnection
	 */
	constexpr void removeBothConnections() noexcept;
	///@}

	/** Connection replacement */
	///@{
	/** Replaces the current connection to the previous node with a connection to the given node.
		After calling this function, calling \c getBefore() will return \c &newNodeBeforeThisOne ,
		and calling \c newNodeBeforeThisOne.getAfter() will return a pointer to this node.
		@see replaceAfterConnection, replaceConnections, insertBeforeConnection
	 */
	constexpr void replaceBeforeConnection (LinkedListModel& newNodeBeforeThisOne) noexcept;

	/** Replaces the current connection to the next node with a connection to the given node.
		After calling this function, calling \c getAfter() will return \c &newNodeAfterThisOne ,
		and calling \c newNodeAfterThisOne.getBefore() will return a pointer to this node.
		@see replaceBeforeConnection, replaceConnections
	 */
	constexpr void replaceAfterConnection (LinkedListModel& newNodeAfterThisOne) noexcept;

	/** Replaces both of this nodes's connections with connections to new nodes.
		@see replaceBeforeConnection, replaceAfterConnection
	 */
	constexpr void replaceConnections (LinkedListModel* beforeThisNode, LinkedListModel* afterThisNode) noexcept;

	/** Replaces both of this nodes's connections with connections to new nodes.
		@see replaceBeforeConnection, replaceAfterConnection
	 */
	constexpr void replaceConnections (LinkedListModel& beforeThisNode, LinkedListModel& afterThisNode) noexcept;
	///@}

	/** @name Connection insertion */
	///@{
	/** Inserts a node into the list before this node.
		After calling this function, \c newNodeBeforeThisOne.getBefore() will return this node's previous \c before pointer;
		\c newNodeBeforeThisOne.getAfter() will return a pointer to this node;
		and \c getBefore() will return \c &newNodeBeforeThisOne .
		@see insertAfterConnection, replaceBeforeConnection
	 */
	constexpr void insertBeforeConnection (LinkedListModel& newNodeBeforeThisOne) noexcept;

	/** Inserts a node into the list after this node.
		After calling this function, \c newNodeAfterThisOne.getAfter() will return this node's previous \c after pointer;
		\c newNodeAfterThisOne.getBefore() will return a pointer to this node;
		and \c getAfter() will return \c &newNodeAfterThisOne .
		@see insertBeforeConnection, replaceAfterConnection
	 */
	constexpr void insertAfterConnection (LinkedListModel& newNodeAfterThisOne) noexcept;
	///@}

	/** Swaps both of this node's connections with the other node's connections. */
	constexpr void swapWith (LinkedListModel& otherNode) noexcept;

	/** Returns true if this node has a connection to a node before it in the list.
		If this returns false, this node is the head of a list.
		@see getBefore, hasAfterConnection, isFirstNodeInList
	 */
	[[nodiscard]] constexpr bool hasBeforeConnection() const noexcept;

	/** Returns true if this node has a connection to a node after it in the list.
		If this returns false, this node is the tail of a list.
		@see getAfter, hasBeforeConnection, isLastNodeInList
	 */
	[[nodiscard]] constexpr bool hasAfterConnection() const noexcept;

	/** Returns a pointer to the previous node in the list.
		This may return a nullptr!
		@see hasBeforeConnection, getAfter
	 */
	[[nodiscard]] constexpr LinkedListModel* getBefore() const noexcept;

	/** Returns a pointer to the next node in the list.
		This may return a nullptr!
		@see hasAfterConnection, getBefore
	 */
	[[nodiscard]] constexpr LinkedListModel* getAfter() const noexcept;

	/** @name Access the beginning of the list */
	///@{
	/** Returns the first node in this node's list.
		Each node's \c before connection is traversed until a node with no \c before connection is found.
		@see getFirstObjectInList
	 */
	[[nodiscard]] constexpr LinkedListModel&	   getFirstNodeInList() noexcept;
	[[nodiscard]] constexpr const LinkedListModel& getFirstNodeInList() const noexcept;

	/** The same as \c getFirstNodeInList() , except this returns the object the head node references instead of the node itself.
		@see getFirstNodeInList
	 */
	[[nodiscard]] constexpr ObjectType& getFirstObjectInList() const noexcept;
	///@}

	/** @name Access the end of the list */
	///@{
	/** Returns the last node in this node's list.
		Each node's \c after connection is traversed until a node with no \c after connection is found.
		@see getLastObjectInList
	 */
	[[nodiscard]] constexpr LinkedListModel&	   getLastNodeInList() noexcept;
	[[nodiscard]] constexpr const LinkedListModel& getLastNodeInList() const noexcept;

	/** The same as \c getLastNodeInList() , except this returns the object the tail node references instead of the node itself.
		@see getLastNodeInList
	 */
	[[nodiscard]] constexpr ObjectType& getLastObjectInList() const noexcept;
	///@}

	/** Returns true if this node is the head of a list -- ie, has no \c before connection.
		@see hasBeforeConnection
	 */
	[[nodiscard]] constexpr bool isFirstNodeInList() const noexcept;

	/** Returns true if this node is the tail of a list -- ie, has no \c after connection.
		@see hasAfterConnection
	 */
	[[nodiscard]] constexpr bool isLastNodeInList() const noexcept;

	/** Returns the size of the linked list this node is in. */
	[[nodiscard]] constexpr int sizeOfList() const noexcept;

	/** Returns the object this list node represents. */
	[[nodiscard]] constexpr ObjectType& getObject() const noexcept;

	/** An iterator for a linked list.
		Any node of a linked list is iterable; no matter which node \c begin() is called on, an iterator to the beginning of the list will be returned.
	 */
	struct Iterator final
	{
		// using difference_type   = typename std::iterator_traits<Iterator>::difference_type;
		using value_type		= ObjectType;
		using reference			= ObjectType&;
		using pointer			= ObjectType*;
		using iterator_category = std::bidirectional_iterator_tag;

		constexpr Iterator() = default;

		constexpr explicit Iterator (const LinkedListModel& listNode) noexcept;

		constexpr Iterator& operator++() noexcept;
		constexpr Iterator& operator--() noexcept;

		[[nodiscard]] constexpr bool operator== (const Iterator& other) const noexcept;
		[[nodiscard]] constexpr bool operator!= (const Iterator& other) const noexcept;

		[[nodiscard]] constexpr reference operator*() const noexcept;

	private:

		const LinkedListModel* currentNode { nullptr };
	};

	/** Returns an iterator to the beginning of this node's list.  */
	[[nodiscard]] constexpr Iterator begin() const noexcept;

	/** Returns an iterator to the end of this node's list.  */
	[[nodiscard]] constexpr Iterator end() const noexcept;

private:

	LinkedListModel *before { nullptr }, *after { nullptr };

	std::reference_wrapper<ObjectType> thisNode;
};

}  // namespace ds

LIMES_END_NAMESPACE


#include "LinkedListModel_impl.h"  // IWYU pragma: export
