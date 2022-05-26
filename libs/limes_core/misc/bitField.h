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
#include "../math/mathHelpers.h"  // for concept Integral
#include "preprocessor.h"
#include <cstdint>

LIMES_BEGIN_NAMESPACE

namespace misc
{

template <math::Integral StorageType = std::uint64_t>
struct BitField final
{
public:

	constexpr void setBit (std::size_t pos, bool value = true) noexcept;

	constexpr bool getBit (std::size_t pos) const noexcept;

	constexpr StorageType getEntireField() const noexcept;

private:

	StorageType storage { 0 };
};


template <math::Integral StorageType>
constexpr void BitField<StorageType>::setBit (std::size_t pos, bool value) noexcept
{
	auto baseVal = value ? StorageType (1) : StorageType (0);

	baseVal << pos;

	storage |= baseVal;
}

template <math::Integral StorageType>
constexpr bool BitField<StorageType>::getBit (std::size_t pos) const noexcept
{
	return (storage >> pos) & StorageType (1);
}

template <math::Integral StorageType>
constexpr StorageType BitField<StorageType>::getEntireField() const noexcept
{
	return storage;
}

}  // namespace misc

LIMES_END_NAMESPACE
