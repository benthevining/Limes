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
// IWYU pragma: friend limes_resampler.cpp

#pragma once

#include "./resampler_common.h"
#include <limes_export.h>
#include <limes_namespace.h>
#include <ipps.h>

/** @file
	This file contains a resampler implementation using the IPP library.
	@ingroup resampling
 */

LIMES_BEGIN_NAMESPACE

namespace vecops
{

/// @cond internals

template <Scalar SampleType>
class LIMES_NO_EXPORT IPPResampler final : public ResamplerImpl<SampleType>
{
public:
private:
	int resample (SampleType* const * const		  out,
				  int							  outspace,
				  const SampleType* const * const in,
				  int							  incount,
				  double						  ratio) noexcept final;

	int resampleInterleaved (SampleType* const		 out,
							 int					 outspace,
							 const SampleType* const in,
							 int					 incount,
							 double					 ratio) noexcept final;

	double getEffectiveRatio (double inputRatio) const noexcept final;

	void reset() final;
};

/// @endcond

}  // namespace vecops

LIMES_END_NAMESPACE
