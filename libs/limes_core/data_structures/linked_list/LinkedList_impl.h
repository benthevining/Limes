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

// IWYU pragma: private
// IWYU pragma: friend LinkedList.h

#pragma once

#include <limes_platform.h>
#include <utility>	// for std::forward
#include <limes_namespace.h>

/** @file
	This file contains implementation details for the LinkedList class.
	@see LinkedList.h
 */

LIMES_BEGIN_NAMESPACE

namespace ds
{

/// @cond

template <typename ObjectType>
template <typename... Args>
constexpr LinkedList<ObjectType>::Node::Node (Args&&... args) noexcept (noexcept (ObjectType (std::forward<Args> (args)...)))
	: object (std::forward<Args> (args)...)
{
}

template <typename ObjectType>
constexpr typename LinkedList<ObjectType>::Node* LinkedList<ObjectType>::getFirstNodeInChain() const noexcept
{
	if (nodes.isEmpty())
		return nullptr;

	return &(nodes.getFirst()->listNode.getFirstObjectInList());
}

template <typename ObjectType>
constexpr typename LinkedList<ObjectType>::Node* LinkedList<ObjectType>::getLastNodeInChain() const noexcept
{
	if (nodes.isEmpty())
		return nullptr;

	return &(nodes.getFirst()->listNode.getLastObjectInList());
}

template <typename ObjectType>
constexpr typename LinkedList<ObjectType>::Node& LinkedList<ObjectType>::addNode (Node& newNode)
{
	if (auto* last = nodes.getLast())
		last->listNode.insertAfterConnection (newNode);

	return *nodes.add (&newNode);
}

template <typename ObjectType>
template <typename... Args>
typename LinkedList<ObjectType>::Node& LinkedList<ObjectType>::createAndAddNode (Args&&... args)
{
	return addNode (*new Node (std::forward<Args> (args)...));
}

template <typename ObjectType>
constexpr typename LinkedList<ObjectType>::Node& LinkedList<ObjectType>::addNodeToFront (Node& newNode)
{
	if (auto* first = nodes.getFirst())
		first->listNode.insertBeforeConnection (newNode);

	return *nodes.add (newNode);
}

template <typename ObjectType>
template <typename... Args>
typename LinkedList<ObjectType>::Node& LinkedList<ObjectType>::createAndAddNodeToFront (Args&&... args)
{
	return addNodeToFront (*new Node (std::forward<Args> (args)...));
}

template <typename ObjectType>
constexpr typename LinkedList<ObjectType>::Node& LinkedList<ObjectType>::addNodeBefore (Node& newNode, Node& nodeToInsertBefore)
{
	LIMES_ASSERT (nodes.contains (nodeToInsertBefore));
	nodeToInsertBefore.insertBeforeConnection (newNode);
	return *nodes.add (newNode);
}

template <typename ObjectType>
template <typename... Args>
typename LinkedList<ObjectType>::Node& LinkedList<ObjectType>::createAndAddNodeBefore (Node& nodeToInsertBefore, Args&&... args)
{
	return addNodeBefore (*new Node (std::forward<Args> (args)...), nodeToInsertBefore);
}

template <typename ObjectType>
constexpr typename LinkedList<ObjectType>::Node& LinkedList<ObjectType>::addNodeAfter (Node& newNode, Node& nodeToInsertAfter)
{
	LIMES_ASSERT (nodes.contains (nodeToInsertAfter));
	nodeToInsertAfter.insertAfterConnection (newNode);
	return *nodes.add (newNode);
}

template <typename ObjectType>
template <typename... Args>
typename LinkedList<ObjectType>::Node& LinkedList<ObjectType>::createAndAddNodeAfter (Node& nodeToInsertAfter, Args&&... args)
{
	return addNodeAfter (*new Node (std::forward<Args> (args)...), nodeToInsertAfter);
}

template <typename ObjectType>
constexpr void LinkedList<ObjectType>::swapNodes (Node& node1, Node& node2)
{
	LIMES_ASSERT (nodes.contains (node1));
	LIMES_ASSERT (nodes.contains (node2));

	node1.listNode.swapWith (node2.listNode);
}

template <typename ObjectType>
constexpr void LinkedList<ObjectType>::removeNode (const Node& node)
{
	LIMES_ASSERT (nodes.contains (node));

	nodes.removeObject (&node);
}

template <typename ObjectType>
constexpr typename LinkedList<ObjectType>::Iterator LinkedList<ObjectType>::begin() const noexcept
{
	if (nodes.isEmpty())
		return Iterator {};

	return nodes.getFirst()->listNode.begin();
}

template <typename ObjectType>
constexpr typename LinkedList<ObjectType>::Iterator LinkedList<ObjectType>::end() const noexcept
{
	return Iterator {};
}

/// @endcond

}  // namespace ds

LIMES_END_NAMESPACE
