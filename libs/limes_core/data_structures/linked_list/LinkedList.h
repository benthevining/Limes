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
#include <limes_core.h>

/** @dir libs/limes_core/data_structures/linked_list
	This directory contains a linked list implementation.
	@ingroup data_structures
 */

/** @file
	This file defines the LinkedList class.
	@ingroup data_structures
 */

LIMES_BEGIN_NAMESPACE

namespace ds
{

/** This class is a linked list.

	This class builds upon LinkedListModel, but each node actually contains the data object, instead of just a reference to one.

	@tparam ObjectType The type of objects the list will hold.

	@see LinkedListModel
	@ingroup data_structures
 */
template <typename ObjectType>
class LIMES_EXPORT LinkedList final
{
public:

	/** A node in a linked list.
		This object contains a LinkedListModel and the actual data object itself.
	 */
	struct LIMES_EXPORT Node final
	{
		/** All constructor arguments are forwarded to the constructor of \c ObjectType . */
		template <typename... Args>
		constexpr explicit Node (Args&&... args) noexcept (noexcept (ObjectType (std::forward<Args> (args)...)));

		LIMES_DEFAULT_MOVABLE (Node);
		LIMES_DEFAULT_COPYABLE (Node);

		ObjectType			  object;
		LinkedListModel<Node> listNode { *this };
	};

	/** Default constructor. */
	LinkedList() = default;

	LIMES_DEFAULT_MOVABLE (LinkedList);
	LIMES_DEFAULT_COPYABLE (LinkedList);

	/** Returns the first node in the list.
		@see getLastNodeInChain
	 */
	[[nodiscard]] constexpr Node* getFirstNodeInChain() const noexcept;

	/** Returns the last node in the list.
		@see getFirstNodeInChain
	 */
	[[nodiscard]] constexpr Node* getLastNodeInChain() const noexcept;

	/** @name Adding nodes */
	///@{
	/** Adds a node to the end of the list.
		@returns A reference to the newly added node.
		@see createAndAddNode
	 */
	constexpr Node& addNode (Node& newNode);

	/** Adds a node to the front of the list.
		@returns A reference to the newly added node.
		@see createAndAddNodeToFront
	 */
	constexpr Node& addNodeToFront (Node& newNode);

	/** Adds a node to the list before another node.
		@returns A reference to the newly added node.
		@see createAndAddNodeBefore
	 */
	constexpr Node& addNodeBefore (Node& newNode, Node& nodeToInsertBefore);

	/** Adds a node to the list after another node.
		@returns A reference to the newly added node.
		@see createAndAddNodeAfter
	 */
	constexpr Node& addNodeAfter (Node& newNode, Node& nodeToInsertAfter);
	///@}

	/** @name Constructing and adding nodes */
	///@{
	/** Constructs and adds a node to the end of the list.
		All arguments are forwarded to the constructor of \c ObjectType .
		@returns A reference to the newly constructed node.
		@see addNode
	 */
	template <typename... Args>
	Node& createAndAddNode (Args&&... args);

	/** Constructs and adds a node to the front of the list.
		All arguments are forwarded to the constructor of \c ObjectType .
		@returns A reference to the newly constructed node.
		@see addNodeToFront
	 */
	template <typename... Args>
	Node& createAndAddNodeToFront (Args&&... args);

	/** Constructs and adds a node to the list before another node.
		All arguments are forwarded to the constructor of \c ObjectType .
		@returns A reference to the newly constructed node.
		@see addNodeBefore
	 */
	template <typename... Args>
	Node& createAndAddNodeBefore (Node& nodeToInsertBefore, Args&&... args);

	/** Constructs and adds a node to the list after another node.
		All arguments are forwarded to the constructor of \c ObjectType .
		@returns A reference to the newly constructed node.
		@see addNodeAfter
	 */
	template <typename... Args>
	Node& createAndAddNodeAfter (Node& nodeToInsertAfter, Args&&... args);
	///@}

	/** Swaps the positions of two nodes in the list. */
	constexpr void swapNodes (Node& node1, Node& node2);

	/** Removes (and destroys) a node from the list. */
	constexpr void removeNode (const Node& node);

	/** Convenience typedef for an iterator. */
	using Iterator = typename LinkedListModel<Node>::Iterator;

	/** Returns an iterator to the beginning of the list. */
	constexpr Iterator begin() const noexcept;

	/** Returns an iterator to the end of the list. */
	constexpr Iterator end() const noexcept;

private:

	std::vector<Node> nodes;
};

}  // namespace ds

LIMES_END_NAMESPACE

#include "LinkedList_impl.h"  // IWYU pragma: export
