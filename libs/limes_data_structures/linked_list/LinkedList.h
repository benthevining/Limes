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
#include "../vectors/owned_vector.h"
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

template <typename ObjectType>
class LIMES_EXPORT LinkedList final
{
public:

	struct LIMES_EXPORT Node final
	{
		template <typename... Args>
		explicit Node (Args&&... args);

		ObjectType			  object;
		LinkedListModel<Node> listNode { *this };
	};

	[[nodiscard]] Node* getFirstNodeInChain() const noexcept;
	[[nodiscard]] Node* getLastNodeInChain() const noexcept;

	Node& addNode (Node& newNode);

	template <typename... Args>
	Node& createAndAddNode (Args&&... args);

	Node& addNodeToFront (Node& newNode);

	template <typename... Args>
	Node& createAndAddNodeToFront (Args&&... args);

	Node& addNodeBefore (Node& newNode, Node& nodeToInsertBefore);

	template <typename... Args>
	Node& createAndAddNodeBefore (Node& nodeToInsertBefore, Args&&... args);

	Node& addNodeAfter (Node& newNode, Node& nodeToInsertAfter);

	template <typename... Args>
	Node& createAndAddNodeAfter (Node& nodeToInsertAfter, Args&&... args);

	void swapNodes (Node& node1, Node& node2);

	void removeNode (const Node& node);

private:

	owned_vector<Node> nodes;
};

LIMES_END_NAMESPACE

#include "LinkedList_impl.h"  // IWYU pragma: export
