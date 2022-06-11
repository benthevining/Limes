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
#include "../util/Misc.h"  // for concept Sample - IWYU pragma: keep

/** @defgroup pitch_detection Pitch detection
	This module contains implementations of several pitch detection algorithms.

	@ingroup limes_audio
 */

/** @dir libs/limes_audio/pitch_detection
	This directory contains implementations of several pitch detection algorithms.
	@ingroup pitch_detection
 */

/** @file
	This file defines the PitchData class.
	@ingroup pitch_detection
 */

LIMES_BEGIN_NAMESPACE

/** This namespace contains utilities for pitch detection.
	@ingroup pitch_detection
 */
namespace dsp::pitch
{

/** This struct represents the data output by any pitch detection algorithm.
	This struct contains the actual ppitch estimate (a frequency in Hz), and a confidence measure.
	@ingroup pitch_detection
 */
template <Sample SampleType>
struct LIMES_EXPORT PitchData final
{
	/** @name Data members */
	///@{
	/** The estimated fundamental frequency, in Hz. */
	SampleType freqHz { 0 };

	/** The confidence measure of this pitch estimation. Higher values mean that the detector was more confident in its estimation. */
	SampleType confidence { 0 };
	///@}

	/** Sets the frequency stored by this object to a specified period at a certain sample rate. */
	void setPeriod (SampleType period, double samplerate) noexcept;

	/** Returns the period, in samples, of this pitch estimate at a certain sample rate. */
	SampleType getPeriod (double samplerate) const noexcept;

	/** Resets the frequency and period to 0. */
	void reset() noexcept;

	/** Returns true if this pitch estimation is pitched (ie, has a frequency and confidence both greater than 0).
		@see isUnpitched
	 */
	bool isPitched() const noexcept;

	/** Returns true if this pitch estimation is unpitched (ie, has a frequency or confidence of 0).
		@see isPitched
	 */
	bool isUnpitched() const noexcept;
};

}  // namespace dsp::pitch

LIMES_END_NAMESPACE
