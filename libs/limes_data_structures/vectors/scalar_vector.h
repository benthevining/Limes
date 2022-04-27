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

#include "basic_vector.h"
#include <limes_core.h>
#include <limes_vecops.h>
#include <limes_export.h>
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

template <typename ElementType, class Allocator = std::allocator<ElementType>>
class LIMES_EXPORT scalar_vector final : public basic_vector<ElementType, Allocator>
{
public:

	static_assert (std::is_scalar_v<ElementType>, "ElementType must be a scalar type!");

	using basic_vector<ElementType, Allocator>::basic_vector;

	LIMES_DEFAULT_MOVABLE (scalar_vector);
	LIMES_DEFAULT_COPYABLE (scalar_vector);

	scalar_vector& reserveAndZero (int newSize)
	{
		this->reserveAndInit (newSize, 0);
		return *this;
	}

	scalar_vector& clearAndZero (int newNumObjects)
	{
		this->clearAndInit (newNumObjects, 0);
		return *this;
	}

	scalar_vector& zero()
	{
		vecops::clear (this->objects.data(), this->numObjects());
		return *this;
	}

	void fill (const ElementType& fillWith)
	{
		vecops::fill (this->objects.data(), this->numObjects(), fillWith);
	}

	scalar_vector& copyFrom (const scalar_vector& other)
	{
		vecops::copy (this->objects.data(), other.objects.data(), minNumElemsWith (other));
		return *this;
	}

	scalar_vector& copyFrom (const ElementType* const data, int dataSize)
	{
		vecops::copy (this->objects.data(), data, minNumElemsWith (dataSize));
		return *this;
	}

	scalar_vector& copyTo (ElementType* const dest, int destBufferSize)
	{
		vecops::copy (dest, this->objects.data(), minNumElemsWith (destBufferSize));
		return *this;
	}

	// addition

	scalar_vector& add (ElementType toAdd)
	{
		vecops::add (this->objects.data(), this->numObjects(), toAdd);
		return *this;
	}

	scalar_vector& addFrom (const scalar_vector& other)
	{
		vecops::add (this->objects.data(), minNumElemsWith (other), other.objects.data());
		return *this;
	}

	scalar_vector& addFrom (const ElementType* const data, int dataSize)
	{
		vecops::add (this->objects.data(), minNumElemsWith (dataSize), data);
		return *this;
	}

	scalar_vector& addTo (scalar_vector& dest) const
	{
		vecops::add (dest.objects.data(), minNumElemsWith (dest), this->objects.data());
		return *this;
	}

	scalar_vector& addTo (ElementType* const destData, int destDataSize)
	{
		vecops::add (destData, minNumElemsWith (destDataSize), this->objects.data());
		return *this;
	}


	// subtraction

	scalar_vector& subtract (ElementType toSubtract)
	{
		vecops::subtract (this->objects.data(), this->numObjects(), toSubtract);
		return *this;
	}

	scalar_vector& subtractFrom (const scalar_vector& other)
	{
		vecops::subtract (this->objects.data(), minNumElemsWith (other), other.objects.data());
		return *this;
	}

	scalar_vector& subtractFrom (const ElementType* const data, int dataSize)
	{
		vecops::subtract (this->objects.data(), minNumElemsWith (dataSize), data);
		return *this;
	}

	scalar_vector& subtractTo (scalar_vector& dest) const
	{
		vecops::subtract (dest.objects.data(), minNumElemsWith (dest), this->objects.data());
		return *this;
	}

	scalar_vector& subtractTo (ElementType* const destData, int destDataSize)
	{
		vecops::subtract (destData, minNumElemsWith (destDataSize), this->objects.data());
		return *this;
	}


	// multiplication

	scalar_vector& multiply (ElementType toMultiply)
	{
		vecops::multiply (this->objects.data(), this->numObjects(), toMultiply);
		return *this;
	}

	scalar_vector& multiplyFrom (const scalar_vector& other)
	{
		vecops::multiply (this->objects.data(), minNumElemsWith (other), other.objects.data());
		return *this;
	}

	scalar_vector& multiplyFrom (const ElementType* const data, int dataSize)
	{
		vecops::multiply (this->objects.data(), minNumElemsWith (dataSize), data);
		return *this;
	}

	scalar_vector& multiplyTo (scalar_vector& dest) const
	{
		vecops::multiply (dest.objects.data(), minNumElemsWith (dest), this->objects.data());
		return *this;
	}

	scalar_vector& multiplyTo (ElementType* const destData, int destDataSize)
	{
		vecops::multiply (destData, minNumElemsWith (destDataSize), this->objects.data());
		return *this;
	}


	// division

	scalar_vector& divide (ElementType toDivide)
	{
		vecops::divide (this->objects.data(), this->numObjects(), toDivide);
		return *this;
	}

	scalar_vector& divideFrom (const scalar_vector& other)
	{
		vecops::divide (this->objects.data(), minNumElemsWith (other), other.objects.data());
		return *this;
	}

	scalar_vector& divideFrom (const ElementType* const data, int dataSize)
	{
		vecops::divide (this->objects.data(), minNumElemsWith (dataSize), data);
		return *this;
	}

	scalar_vector& divideTo (scalar_vector& dest) const
	{
		vecops::divide (dest.objects.data(), minNumElemsWith (dest), this->objects.data());
		return *this;
	}

	scalar_vector& divideTo (ElementType* const destData, int destDataSize)
	{
		vecops::divide (destData, minNumElemsWith (destDataSize), this->objects.data());
		return *this;
	}

private:

	[[nodiscard]] constexpr auto minNumElemsWith (const scalar_vector& other) const noexcept
	{
		return std::min (this->numObjects(), other.numObjects());
	}

	[[nodiscard]] constexpr auto minNumElemsWith (int dataSize) const noexcept
	{
		return std::min (this->numObjects(), dataSize);
	}
};

LIMES_END_NAMESPACE
