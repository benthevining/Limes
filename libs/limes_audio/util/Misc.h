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

#include <limes_core.h>
#include <limes_export.h>
#include <limes_namespace.h>

/** @dir libs/limes_audio/util
	This directory contains miscellaneous audio utilities.
	@ingroup limes_audio
 */

/** @file
	This file contains miscellaneous audio utilities.
	@ingroup limes_audio
 */

LIMES_BEGIN_NAMESPACE

/** This namespace contains utilities for working with realtime audio.
	@ingroup limes_audio
 */
namespace dsp
{

/** @concept Sample
	Either \c float or \c double .
	@ingroup limes_audio
 */
template <typename T>
concept Sample = std::is_same_v<T, float> || std::is_same_v<T, double>;

/** An inverse root 2 constant, useful for default filter Q values.
	@ingroup limes_audio
 */
template <Sample SampleType>
LIMES_EXPORT static constexpr SampleType inverseRootTwo = 0.70710678118655f;

}  // namespace dsp

LIMES_END_NAMESPACE
