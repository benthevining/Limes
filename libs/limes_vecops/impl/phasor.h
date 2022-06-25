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
// IWYU pragma: friend limes_vecops/impl/*

#pragma once

#include <limes_export.h>
#include <limes_namespace.h>

/** @file
	This file contains a phasor function used by multiple implementations of the vecops functions.
	@ingroup limes_vecops
 */

LIMES_BEGIN_NAMESPACE

/** This namespace contains implementation details for the vecops functions.
	@ingroup limes_vecops
 */
namespace vecops::detail
{

/// @cond internals

template <typename T>
LIMES_NO_EXPORT void phasor (T* const real, T* const imag, T phase) noexcept;

/// @endcond

}  // namespace vecops::detail

LIMES_END_NAMESPACE
