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

LIMES_BEGIN_NAMESPACE

namespace dsp
{

template <typename T>
concept Sample = std::is_same_v<T, float> || std::is_same_v<T, double>;

template <Sample SampleType>
LIMES_EXPORT static constexpr SampleType inverseRootTwo = 0.70710678118655f;

}  // namespace dsp

LIMES_END_NAMESPACE
