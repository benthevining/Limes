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

#include <limes_export.h>  // for LIMES_EXPORT
#include "./Misc.h"		   // for concept Sample - IWYU pragma: keep
#include "./AudioBuffer.h"
#include <limes_namespace.h>

/** @file
	This file defines the MonoStereoConverter class.
	@ingroup limes_audio
 */

LIMES_BEGIN_NAMESPACE

namespace dsp
{

/** An enum used to describe how a MonoStereoConverter will convert stereo signals to mono.
	@see MonoStereoConverter
	@ingroup limes_audio
 */
LIMES_EXPORT enum class StereoReductionMode {
	/** In this mode, a MonoStereoConverter will output only the left channel of a stereo signal as its mono reduction. */
	leftOnly,

	/** In this mode, a MonoStereoConverter will output only the right channel of a stereo signal as its mono reduction. */
	rightOnly,

	/** In this mode, a MonoStereoConverter will mix together the two channels of a stereo signal to create its mono reduction. */
	mixToMono
};

/** A utility class for converting mono signals to stereo, or stereo signals to mono.
	@ingroup limes_audio
	@see StereoReductionMode
 */
template <Sample SampleType>
class LIMES_EXPORT MonoStereoConverter final
{
public:

	using MonoBuffer   = AudioBuffer<SampleType, 1>;
	using StereoBuffer = AudioBuffer<SampleType, 2>;

	/** Prepares the converter to process a new maximum blocksize.
		@note This function may allocate memory, and should not be called from a realtime thread.
	 */
	void prepare (int blocksize);

	/** Releases the converter's internal resources.
		@note This function may deallocate memory, and should not be called from a realtime thread.
	 */
	void releaseResources();

	/** Sets the StereoReductionMode the converter will use for converting stereo signals to mono.
		@see StereoReductionMode
	 */
	void setStereoReductionMode (StereoReductionMode newmode) noexcept;

	/** Returns the StereoReductionMode the converter is currently using.
		@see StereoReductionMode
	 */
	[[nodiscard]] StereoReductionMode getStereoReductionMode() const noexcept;

	/** @name Stereo-mono reduction */
	///@{
	/** Converts a stereo signal to mono.
		The current StereoReductionMode determines how the stereo signal will be reduced:
		- copying only the left channel to the output
		- copying only the right channel to the output
		- summing the two channels together
	 */
	void convertStereoToMono (const SampleType* leftIn,
							  const SampleType* rightIn,
							  SampleType*		monoOut,
							  int				numSamples) noexcept;

	void convertStereoToMono (const MonoBuffer& leftIn,
							  const MonoBuffer& rightIn,
							  MonoBuffer&		monoOut) noexcept;
	///@}

	/** @name Mono-stereo expansion */
	///@{
	/** Converts a mono signal to stereo.
		The mono input signal will be copied to both channels of the output.
	 */
	static void convertMonoToStereo (const SampleType* monoIn,
									 SampleType*	   leftOut,
									 SampleType*	   rightOut,
									 int			   numSamples) noexcept;

	static void convertMonoToStereo (const MonoBuffer& monoIn,
									 MonoBuffer&	   leftOut,
									 MonoBuffer&	   rightOut) noexcept;
	///@}

private:

	StereoReductionMode toMonoMode { StereoReductionMode::leftOnly };

	MonoBuffer monoStorage;
};


}  // namespace dsp

LIMES_END_NAMESPACE
