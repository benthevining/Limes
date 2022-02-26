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

#include "vector.h"
#include <memory>
#include <functional>

namespace lemons
{

template <typename ElementType>
class owned_vector final : public vector<std::unique_ptr<ElementType>>
{
	using vector_type = vector<std::unique_ptr<ElementType>>;

	static_assert (std::is_base_of_v<vector_type, owned_vector<ElementType>>, "");

public:

	/*
	 - for_each
	 - contains
	 - contains_if
	 - find_if
	 - num_of
	 - fill
	 - clearAndInit
	 - first() and last()
	 - reserveAndInit
	 - transform
	 - data()
	 */

	using OwnedObjectType = ElementType;

	using ObjectCreationFunction = std::function<OwnedObjectType*()>;

	explicit owned_vector (
		ObjectCreationFunction&& objectCreationFunction = []
		{ return new ElementType; },
		int initNumObjects = 0)
		: createObject (std::move (objectCreationFunction))
	{
		resize (initNumObjects);
	}

	[[nodiscard]] ElementType& operator[] (int index)
	{
		return *(this->objects[static_cast<size_type> (index)]);
	}

	[[nodiscard]] const ElementType& operator[] (int index) const
	{
		return *(this->objects[static_cast<size_type> (index)]);
	}

	template <bool is_const>
	struct iterator_base final
	{
	public:

		using iterator_type = std::conditional_t<is_const,
												 typename vector_type::const_iterator,
												 typename vector_type::iterator>;

		using difference_type	= typename std::iterator_traits<iterator_type>::difference_type;
		using value_type		= std::conditional_t<is_const, const ElementType, ElementType>;
		using reference			= value_type&;
		using pointer			= value_type*;
		using iterator_category = std::input_iterator_tag;

		iterator_base() = default;

		explicit iterator_base (const iterator_type& it_in) noexcept
			: it (it_in)
		{
		}

		[[nodiscard]] iterator_base& operator++() noexcept
		{
			++it;
			return *this;
		}

		[[nodiscard]] bool operator== (const iterator_base& other) const noexcept
		{
			return it == other.it;
		}

		[[nodiscard]] bool operator!= (const iterator_base& other) const noexcept
		{
			return it != other.it;
		}

		[[nodiscard]] reference operator*() const noexcept
		{
			return *(*it);
		}

	private:

		iterator_type it;
	};

	using iterator		 = iterator_base<false>;
	using const_iterator = iterator_base<true>;

	[[nodiscard]] iterator begin() noexcept
	{
		return iterator { this->objects.begin() };
	}

	[[nodiscard]] iterator end() noexcept
	{
		return iterator { this->objects.end() };
	}

	[[nodiscard]] const_iterator begin() const noexcept
	{
		return const_iterator { this->objects.begin() };
	}

	[[nodiscard]] const_iterator end() const noexcept
	{
		return const_iterator { this->objects.end() };
	}

	auto resize (int newNumObjects, bool allowDeallocating = true)
	{
		return resize_impl (newNumObjects, allowDeallocating, [this]
							{ append(); });
	}

	template <typename... Args>
	auto resize (int newNumObjects, bool allowDeallocating, Args&&... args)
	{
		return resize_impl (newNumObjects, allowDeallocating,
							[this, ... args = std::forward<Args> (args)]
							{ append (std::forward<Args> (args)...); });
	}

	ElementType& append()
	{
		this->objects.emplace_back (std::make_unique<ElementType> (createObject()));

		return *this->last();
	}

	template <typename... Args>
	ElementType& append (Args&&... args)
	{
		this->objects.emplace_back (std::make_unique<ElementType> (std::forward<Args> (args)...));

		return *this->last();
	}

	[[nodiscard]] ElementType& releaseObject (int index)
	{
		return *(this->objects[static_cast<size_type> (index)].release());
	}

private:

	using size_type = typename vector_type::size_type;

	template <typename AddObjectFunction>
	inline int resize_impl (int newNumObjects, bool allowDeallocating, AddObjectFunction&& addObject)
	{
		if (newNumObjects < 1)
		{
			if (allowDeallocating)
				this->clearAndFree();
			else
				this->clear();

			return 0;
		}

		this->reserve (newNumObjects);

		{
			auto numCreated = 0;

			for (auto i = this->numObjects(); i < newNumObjects; ++i, ++numCreated)
				append (addObject());

			if (numCreated > 0)
				return numCreated;
		}

		if (allowDeallocating)
		{
			auto numDeleted = 0;

			for (auto i = this->numObjects(); i > newNumObjects; --i, --numDeleted)
				this->objects.pop_back();

			return numDeleted;
		}

		return 0;
	}

	ObjectCreationFunction createObject;
};

}  // namespace lemons
