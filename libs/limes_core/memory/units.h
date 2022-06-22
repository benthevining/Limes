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

#include <limes_namespace.h>
#include <limes_export.h>
#include <cstdint>
#include "../math/mathFunctions.h"

/** @file
	This file contains utilities for converting between various units of computer memory.
	@ingroup memory
 */

LIMES_BEGIN_NAMESPACE

/** This namespace contains utilities for converting between various units of computer memory.
	@ingroup memory
 */
namespace memory::units
{

/** This enum represents different units of computer memory. */
LIMES_EXPORT enum class Type : int {
	Bit,	  /**< The smallest unit of computer memory, a single bit. */
	Byte,	  /**< A byte is 8 bits. */
	Kilobyte, /**< A kilobyte is 1024 bytes. */
	Megabyte, /**< A megabyte is 1024 kilobytes. */
	Gigabyte, /**< A gigabyte is 1024 megabytes. */
	Terabyte  /**< A terabyte is 1024 gigabytes. */
};

/** This function converts between units of memory. */
template <Type OrigUnits, Type DestUnits>
[[nodiscard]] LIMES_EXPORT constexpr std::uintmax_t convert (std::uintmax_t numOrigUnits) noexcept
{
	if constexpr (OrigUnits == DestUnits)
		return numOrigUnits;
	else
	{
		auto numSteps = static_cast<std::uintmax_t> (OrigUnits) - static_cast<std::uintmax_t> (DestUnits);

		constexpr auto WorkingWithBits = (OrigUnits == Type::Bit || DestUnits == Type::Bit);

		if constexpr (WorkingWithBits)
			--numSteps;

		auto result = numOrigUnits * math::power (std::uintmax_t (1024), numSteps);

		if constexpr (WorkingWithBits)
		{
			if constexpr (OrigUnits > DestUnits)
				result *= 8;
			else
				result /= 8;
		}

		return result;
	}
}

}  // namespace memory::units

LIMES_END_NAMESPACE
