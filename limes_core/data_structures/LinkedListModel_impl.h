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

#include <limes_vecops.h>

namespace lemons
{


template <typename ObjectType>
LinkedListModel<ObjectType>::~LinkedListModel()
{
	removeBothConnections();
}

template <typename ObjectType>
constexpr LinkedListModel<ObjectType>::LinkedListModel (ObjectType& objectForThisNode, LinkedListModel* objectBeforeThisNode, LinkedListModel* objectAfterThisNode)
	: before (objectBeforeThisNode), after (objectAfterThisNode), thisNode (objectForThisNode)
{
}

template <typename ObjectType>
constexpr LinkedListModel<ObjectType>::LinkedListModel (const LinkedListModel& other) noexcept
	: before (other.before), after (other.after), thisNode (other.thisNode)
{
}

template <typename ObjectType>
constexpr LinkedListModel<ObjectType>& LinkedListModel<ObjectType>::operator= (const LinkedListModel& other) noexcept
{
	before	 = other.before;
	after	 = other.after;
	thisNode = other.thisNode;

	return *this;
}

template <typename ObjectType>
constexpr ObjectType& LinkedListModel<ObjectType>::getObject() const noexcept
{
	return thisNode.get();
}

template <typename ObjectType>
constexpr void LinkedListModel<ObjectType>::removeBeforeConnection() noexcept
{
	if (before != nullptr)
	{
		before->after = after;
		before		  = nullptr;
	}
}

template <typename ObjectType>
constexpr void LinkedListModel<ObjectType>::removeAfterConnection() noexcept
{
	if (after != nullptr)
	{
		after->before = before;
		after		  = nullptr;
	}
}

template <typename ObjectType>
constexpr void LinkedListModel<ObjectType>::removeBothConnections() noexcept
{
	removeBeforeConnection();
	removeAfterConnection();
}

template <typename ObjectType>
constexpr bool LinkedListModel<ObjectType>::hasBeforeConnection() const noexcept
{
	return before != nullptr;
}

template <typename ObjectType>
constexpr bool LinkedListModel<ObjectType>::hasAfterConnection() const noexcept
{
	return after != nullptr;
}

template <typename ObjectType>
constexpr int LinkedListModel<ObjectType>::sizeOfList() const noexcept
{
	return alg::size (*this);
}

template <typename ObjectType>
constexpr void LinkedListModel<ObjectType>::replaceConnections (LinkedListModel* beforeThisNode, LinkedListModel* afterThisNode) noexcept
{
	if (beforeThisNode == nullptr && afterThisNode == nullptr)
	{
		removeBothConnections();
		return;
	}

	LIMES_ASSERT (beforeThisNode != afterThisNode);

	if (before != nullptr)
		replaceBeforeConnection (*beforeThisNode);

	if (after != nullptr)
		replaceAfterConnection (*afterThisNode);
}

template <typename ObjectType>
constexpr void LinkedListModel<ObjectType>::replaceBeforeConnection (LinkedListModel& newNodeBeforeThisOne) noexcept
{
	removeBeforeConnection();

	before = &newNodeBeforeThisOne;

	before->removeAfterConnection();
	before->after = this;
}

template <typename ObjectType>
constexpr void LinkedListModel<ObjectType>::replaceAfterConnection (LinkedListModel& newNodeAfterThisOne) noexcept
{
	removeAfterConnection();

	after = &newNodeAfterThisOne;

	after->removeBeforeConnection();
	after->before = this;
}

template <typename ObjectType>
constexpr void LinkedListModel<ObjectType>::insertBeforeConnection (LinkedListModel& newNodeBeforeThisOne) noexcept
{
	if (before != nullptr)
		newNodeBeforeThisOne.before = before;

	newNodeBeforeThisOne.after = this;
	before					   = &newNodeBeforeThisOne;
}

template <typename ObjectType>
constexpr void LinkedListModel<ObjectType>::insertAfterConnection (LinkedListModel& newNodeAfterThisOne) noexcept
{
	if (after != nullptr)
		newNodeAfterThisOne.after = after;

	newNodeAfterThisOne.before = this;
	after					   = &newNodeAfterThisOne;
}

template <typename ObjectType>
constexpr void LinkedListModel<ObjectType>::swapWith (LinkedListModel& otherNode) noexcept
{
	const auto* const prev_this_before = before;
	const auto* const prev_this_after  = after;

	before = otherNode.before;
	after  = otherNode.after;

	otherNode.before = prev_this_before;
	otherNode.after	 = prev_this_after;
}

template <typename ObjectType>
constexpr void LinkedListModel<ObjectType>::replaceConnections (LinkedListModel& beforeThisNode, LinkedListModel& afterThisNode) noexcept
{
	replaceConnections (&beforeThisNode, &afterThisNode);
}

template <typename ObjectType>
constexpr LinkedListModel<ObjectType>& LinkedListModel<ObjectType>::getFirstNodeInList() noexcept
{
	if (before == nullptr)
		return *this;

	auto* ptr = before;

	while (ptr->before != nullptr)
		ptr = ptr->before;

	return *ptr;
}

template <typename ObjectType>
constexpr const LinkedListModel<ObjectType>& LinkedListModel<ObjectType>::getFirstNodeInList() const noexcept
{
	if (before == nullptr)
		return *this;

	auto* ptr = before;

	while (ptr->before != nullptr)
		ptr = ptr->before;

	return *ptr;
}

template <typename ObjectType>
constexpr LinkedListModel<ObjectType>& LinkedListModel<ObjectType>::getLastNodeInList() noexcept
{
	if (after == nullptr)
		return *this;

	auto* ptr = after;

	while (ptr->after != nullptr)
		ptr = ptr->after;

	return *ptr;
}

template <typename ObjectType>
constexpr const LinkedListModel<ObjectType>& LinkedListModel<ObjectType>::getLastNodeInList() const noexcept
{
	if (after == nullptr)
		return *this;

	auto* ptr = after;

	while (ptr->after != nullptr)
		ptr = ptr->after;

	return *ptr;
}

template <typename ObjectType>
constexpr ObjectType& LinkedListModel<ObjectType>::getFirstObjectInList() const noexcept
{
	return getFirstNodeInList().getObject();
}

template <typename ObjectType>
constexpr ObjectType& LinkedListModel<ObjectType>::getLastObjectInList() const noexcept
{
	return getLastNodeInList().getObject();
}

template <typename ObjectType>
constexpr bool LinkedListModel<ObjectType>::isFirstNodeInList() const noexcept
{
	return before == nullptr && after != nullptr;
}

template <typename ObjectType>
constexpr bool LinkedListModel<ObjectType>::isLastNodeInList() const noexcept
{
	return before != nullptr && after == nullptr;
}

/*--------------------------------------------------------------------------------------------------------------------*/

template <typename ObjectType>
constexpr typename LinkedListModel<ObjectType>::Iterator LinkedListModel<ObjectType>::begin() const noexcept
{
	return Iterator { getFirstNodeInList() };
}

template <typename ObjectType>
constexpr typename LinkedListModel<ObjectType>::Iterator LinkedListModel<ObjectType>::end() const noexcept
{
	return Iterator {};
}

template <typename ObjectType>
constexpr LinkedListModel<ObjectType>::Iterator::Iterator (const LinkedListModel& listNode) noexcept
	: currentNode (&listNode)
{
}

template <typename ObjectType>
constexpr typename LinkedListModel<ObjectType>::Iterator& LinkedListModel<ObjectType>::Iterator::operator++() noexcept
{
	if (currentNode != nullptr)
		currentNode = currentNode->after;

	return *this;
}

template <typename ObjectType>
constexpr typename LinkedListModel<ObjectType>::Iterator& LinkedListModel<ObjectType>::Iterator::operator--() noexcept
{
	if (currentNode != nullptr)
		currentNode = currentNode->before;

	return *this;
}

template <typename ObjectType>
constexpr bool LinkedListModel<ObjectType>::Iterator::operator== (const Iterator& other) const noexcept
{
	return currentNode == other.currentNode;
}

template <typename ObjectType>
constexpr bool LinkedListModel<ObjectType>::Iterator::operator!= (const Iterator& other) const noexcept
{
	return currentNode != other.currentNode;
}

template <typename ObjectType>
constexpr typename LinkedListModel<ObjectType>::Iterator::reference LinkedListModel<ObjectType>::Iterator::operator*() const noexcept
{
	LIMES_ASSERT (currentNode != nullptr);
	return currentNode->getObject();
}

}  // namespace lemons
