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
#include <limes_core.h>
#include <vector>
#include <initializer_list>
#include <limes_export.h>
#include <limes_platform.h>
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

template <typename ElementType, class Allocator = std::allocator<ElementType>>
class LIMES_EXPORT basic_vector
{
public:

	using ObjectType	 = ElementType;
	using vector_type	 = std::vector<ElementType, Allocator>;
	using iterator		 = typename vector_type::iterator;
	using const_iterator = typename vector_type::const_iterator;

	basic_vector() = default;

	virtual ~basic_vector() = default;

	basic_vector (const std::initializer_list<ElementType>& objs)  // cppcheck-suppress noExplicitConstructor
		: objects (objs)
	{
	}

	template <class AllocatorType>
	explicit basic_vector (const std::vector<ElementType, AllocatorType>& vectorToInitFrom)
		: objects (vectorToInitFrom)
	{
	}

	template <typename OtherElementType, class UnaryOp>
	explicit basic_vector (const basic_vector<OtherElementType>& otherVector, UnaryOp&& transform)
	{
		*this = otherVector.template transformElementsTo<ElementType> (std::forward<UnaryOp> (transform));
	}

	LIMES_DEFAULT_MOVABLE (basic_vector);
	LIMES_DEFAULT_COPYABLE (basic_vector);

	basic_vector& operator= (std::initializer_list<ElementType> objs)
	{
		objects = objs;
		return *this;
	}

	template <class AllocatorType>
	basic_vector& operator= (const std::vector<ElementType, AllocatorType>& otherVector)
	{
		objects = otherVector;
		return *this;
	}

	template <typename UnaryFunction>
	constexpr void for_each (UnaryFunction&& func) const
	{
		std::for_each (objects.begin(), objects.end(), std::forward<UnaryFunction> (func));
	}

	[[nodiscard]] constexpr bool contains (const ElementType& object) const
	{
		return alg::contains (objects, object);
	}

	template <class UnaryPredicate>
	[[nodiscard]] constexpr bool contains_if (UnaryPredicate&& pred) const
	{
		return alg::contains_if (objects, std::forward<UnaryPredicate> (pred));
	}

	template <class UnaryPredicate>
	[[nodiscard]] constexpr ElementType* find_if (UnaryPredicate&& pred) const
	{
		return alg::find_if (objects, std::forward<UnaryPredicate> (pred));
	}

	template <class UnaryPredicate>
	[[nodiscard]] constexpr int num_of (UnaryPredicate&& pred) const
	{
		return alg::num_of (objects, std::forward<UnaryPredicate> (pred));
	}

	[[nodiscard]] constexpr int capacity() const noexcept
	{
		return static_cast<int> (objects.capacity());
	}

	[[nodiscard]] constexpr int numObjects() const noexcept
	{
		return static_cast<int> (objects.size());
	}

	constexpr void remove (const ElementType& toRemove)
	{
		alg::remove (objects, toRemove);
	}

	constexpr void removeDuplicates()
	{
		alg::removeDuplicates (objects);
	}

	constexpr void reverse()
	{
		alg::reverse (objects);
	}

	constexpr void sort (bool forward = true)
	{
		alg::sort (objects, forward);
	}

	template <class OtherContainer, class Callable>
	void zip_with (const OtherContainer& other, Callable&& callable)
	{
		alg::call_both (objects, other, std::forward<Callable> (callable));
	}

	[[nodiscard]] constexpr ElementType max() const
	{
		return alg::max_value (objects);
	}

	[[nodiscard]] constexpr ElementType min() const
	{
		return alg::min_value (objects);
	}

	[[nodiscard]] decltype (auto) begin() noexcept
	{
		return objects.begin();
	}

	[[nodiscard]] decltype (auto) end() noexcept
	{
		return objects.end();
	}

	[[nodiscard]] decltype (auto) begin() const noexcept
	{
		return objects.begin();
	}

	[[nodiscard]] decltype (auto) end() const noexcept
	{
		return objects.end();
	}

	void push_back (const ElementType& newObject)
	{
		objects.push_back (newObject);
	}

	template <typename... Args>
	void emplace_back (Args&&... args)
	{
		objects.emplace_back (std::forward<Args> (args)...);
	}

	[[nodiscard]] bool isEmpty() const noexcept
	{
		return objects.empty();
	}

	[[nodiscard]] bool isNotEmpty() const noexcept
	{
		return ! objects.empty();
	}

	[[nodiscard]] ElementType& operator[] (int index)
	{
		return objects[static_cast<size_type> (index)];
	}

	[[nodiscard]] const ElementType& operator[] (int index) const
	{
		return objects[static_cast<size_type> (index)];
	}

	void clear()
	{
		objects.clear();
	}

	template <typename... Args>
	void clearAndInit (int numObjectsToCreate, Args&&... args)
	{
		objects.clear();

		for (auto idx = 0; idx < numObjectsToCreate; ++idx)
			objects.emplace_back (std::forward<Args> (args)...);
	}

	void clearAndInit (int numObjectsToCreate, const ElementType& objectToInitializeTo)
	{
		objects.clear();

		for (auto idx = 0; idx < numObjectsToCreate; ++idx)
			objects.push_back (objectToInitializeTo);
	}

	void clearAndFree()
	{
		objects.clear();
		objects.resize (static_cast<size_type> (0));
	}

	[[nodiscard]] ElementType& first()
	{
		return objects.front();
	}

	[[nodiscard]] const ElementType& first() const
	{
		return objects.front();
	}

	[[nodiscard]] ElementType& last()
	{
		return objects.back();
	}

	[[nodiscard]] const ElementType& last() const
	{
		return objects.back();
	}

	void reserve (int newSize)
	{
		LIMES_ASSERT (newSize > 0);
		LIMES_ASSERT (static_cast<size_type> (newSize) < std::numeric_limits<size_type>::max());

		objects.reserve (static_cast<size_type> (newSize));

		LIMES_ASSERT (capacity() >= newSize);
	}

	template <typename... Args>
	int reserveAndInit (int newSize, Args&&... args)
	{
		reserve (newSize);

		auto numCreated = 0;

		for (auto i = numObjects(); i < newSize; ++i, ++numCreated)
			objects.emplace_back (std::forward<Args> (args)...);

		return numCreated;
	}

	int reserveAndInit (int newSize, const ElementType& objectToInitializeTo)
	{
		reserve (newSize);

		auto numCreated = 0;

		for (auto i = numObjects(); i < newSize; ++i, ++numCreated)
			objects.push_back (objectToInitializeTo);

		return numCreated;
	}

	[[nodiscard]] bool operator== (const basic_vector& other) const
	{
		return objects == other.objects;
	}

	[[nodiscard]] bool operator!= (const basic_vector& other) const
	{
		return objects != other.objects;
	}

	template <class OtherContainerType, class UnaryOp>
	void transform (const OtherContainerType& otherContainer, UnaryOp&& transform)
	{
		alg::transform (otherContainer, objects, std::forward<UnaryOp> (transform));
	}

	template <class OtherContainerType, class UnaryOp>
	[[nodiscard]] OtherContainerType createFromTransform (UnaryOp&& transform) const
	{
		return alg::createFromTransform<OtherContainerType> (objects, std::forward<UnaryOp> (transform));
	}

	template <typename OtherElementType, class UnaryOp>
	[[nodiscard]] basic_vector<OtherElementType> transformElementsTo (UnaryOp&& transform) const
	{
		return createFromTransform<basic_vector<OtherElementType>> (std::forward<UnaryOp> (transform));
	}

	template <class UnaryOp>
	void fillFromFunctor (int numElements, UnaryOp&& functor)
	{
		reserve (numElements);

		objects.clear();

		for (auto i = 0; i < numElements; ++i)
			objects.push_back (functor());
	}

	[[nodiscard]] ElementType* data() noexcept
	{
		return objects.data();
	}

	[[nodiscard]] const ElementType* data() const noexcept
	{
		return objects.data();
	}

protected:

	vector_type objects;

	using size_type = typename vector_type::size_type;
};

LIMES_END_NAMESPACE
