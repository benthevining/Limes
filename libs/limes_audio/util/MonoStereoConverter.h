
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

#include <limes_export.h>			// for LIMES_EXPORT
#include <limes_data_structures.h>	// for vector
#include "Misc.h"					// for concept Sample - IWYU pragma: keep
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace dsp
{

/** An enum used to describe how a MonoStereoConverter will convert stereo signals to mono. */
LIMES_EXPORT enum class StereoReductionMode {
	/** In this mode, a MonoStereoConverter will output only the left channel of a stereo signal as its mono reduction. */
	leftOnly,

	/** In this mode, a MonoStereoConverter will output only the right channel of a stereo signal as its mono reduction. */
	rightOnly,

	/** In this mode, a MonoStereoConverter will mix together the two channels of a stereo signal to create its mono reduction. */
	mixToMono
};

/** A utility class for converting mono signals to stereo, or stereo signals to mono.
 */
template <Sample SampleType>
class LIMES_EXPORT MonoStereoConverter final
{
public:

	using SampleVector = ds::scalar_vector<SampleType>;

	/** Prepares the converter to process a new maximum blocksize. */
	void prepare (int blocksize);

	/** Sets the StereoReductionMode the converter will use for converting stereo signals to mono. */
	void setStereoReductionMode (StereoReductionMode newmode);

	/** Returns the StereoReductionMode the converter is currently using. */
	[[nodiscard]] StereoReductionMode getStereoReductionMode() const noexcept;

	/** Converts a stereo signal to mono. */
	void convertStereoToMono (const SampleType* leftIn,
							  const SampleType* rightIn,
							  SampleType*		monoOut,
							  int				numSamples);

	void convertStereoToMono (const SampleVector& leftIn,
							  const SampleVector& rightIn,
							  SampleVector&		  monoOut);

	/** Converts a mono signal to stereo. */
	void convertMonoToStereo (const SampleType* monoIn,
							  SampleType*		leftOut,
							  SampleType*		rightOut,
							  int				numSamples);

	void convertMonoToStereo (const SampleVector& monoIn,
							  SampleVector&		  leftOut,
							  SampleVector&		  rightOut);


private:

	StereoReductionMode toMonoMode { StereoReductionMode::leftOnly };

	SampleVector monoStorage;
};


}  // namespace dsp

LIMES_END_NAMESPACE
