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

LIMES_BEGIN_NAMESPACE

template <typename ObjectType>
struct LIMES_EXPORT LinkedListModel final
{
public:

	~LinkedListModel();

	constexpr explicit LinkedListModel (ObjectType&		 objectForThisNode,
										LinkedListModel* objectBeforeThisNode = nullptr,
										LinkedListModel* objectAfterThisNode  = nullptr);

	LIMES_CONSTEXPR_MOVABLE (LinkedListModel);
	LIMES_CONSTEXPR_COPYABLE (LinkedListModel);

	constexpr void removeBeforeConnection() noexcept;
	constexpr void removeAfterConnection() noexcept;

	constexpr void removeBothConnections() noexcept;

	[[nodiscard]] constexpr bool hasBeforeConnection() const noexcept;
	[[nodiscard]] constexpr bool hasAfterConnection() const noexcept;

	constexpr void replaceBeforeConnection (LinkedListModel& newNodeBeforeThisOne) noexcept;
	constexpr void replaceAfterConnection (LinkedListModel& newNodeAfterThisOne) noexcept;

	constexpr void replaceConnections (LinkedListModel* beforeThisNode, LinkedListModel* afterThisNode) noexcept;
	constexpr void replaceConnections (LinkedListModel& beforeThisNode, LinkedListModel& afterThisNode) noexcept;

	constexpr void insertBeforeConnection (LinkedListModel& newNodeBeforeThisOne) noexcept;
	constexpr void insertAfterConnection (LinkedListModel& newNodeAfterThisOne) noexcept;

	constexpr void swapWith (LinkedListModel& otherNode) noexcept;

	[[nodiscard]] constexpr LinkedListModel&	   getFirstNodeInList() noexcept;
	[[nodiscard]] constexpr LinkedListModel&	   getLastNodeInList() noexcept;
	[[nodiscard]] constexpr const LinkedListModel& getFirstNodeInList() const noexcept;
	[[nodiscard]] constexpr const LinkedListModel& getLastNodeInList() const noexcept;

	[[nodiscard]] constexpr ObjectType& getFirstObjectInList() const noexcept;
	[[nodiscard]] constexpr ObjectType& getLastObjectInList() const noexcept;

	[[nodiscard]] constexpr bool isFirstNodeInList() const noexcept;
	[[nodiscard]] constexpr bool isLastNodeInList() const noexcept;

	[[nodiscard]] constexpr int sizeOfList() const noexcept;

	[[nodiscard]] constexpr ObjectType& getObject() const noexcept;

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

	[[nodiscard]] constexpr Iterator begin() const noexcept;

	[[nodiscard]] constexpr Iterator end() const noexcept;

private:

	LinkedListModel *before { nullptr }, *after { nullptr };

	std::reference_wrapper<ObjectType> thisNode;
};

LIMES_END_NAMESPACE


#include "LinkedListModel_impl.h"
