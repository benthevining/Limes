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

/** @defgroup limes_audio limes_audio
	The Limes library of realtime audio utilities.

	@anchor lib_limes_audio

	All classes and functions in this module are accessible after linking to the \c limes_audio library and including limes_audio.h.

	This library provides classes for processing and analysis of realtime audio signals.

	@par "Limes library dependencies"
	@ref lib_limes_vecops "limes_vecops"

	@cmaketarget \b Limes::limes_audio
	The @ref lib_limes_audio "limes_audio"
	library target.

	@installcomp \b limes_audio_runtime
	Runtime install component for the @ref lib_limes_audio "limes_audio"
	library.

	@installcomp \b limes_audio_dev
	@parblock
	Development install component for the @ref lib_limes_audio "limes_audio"
	library. Depends on the \c limes_audio_runtime component.

	This component is analogous to the `find_package()` component \c Audio .
	@endparblock

	@installcompgroup \b limes_audio
	This group includes the \c limes_audio_dev and \c limes_audio_runtime components.

	@ingroup limes

	@todo Envelope follower class
	@todo Vocoder class
	@todo Audio FX: freezer, looper, beat repeat / stutter, compressor, limiter, EQ, delay, chorus, flanger
 */

/** @dir libs/limes_audio
	This directory contains the limes_audio library.
	@ingroup limes_audio
 */

/** @file
	The main header for the limes_audio library.
	@ingroup limes_audio
 */

#pragma once

// IWYU pragma: begin_exports

#include "util/Misc.h"
#include "util/AudioBuffer.h"
#include "util/MonoStereoConverter.h"
#include "util/CircularBuffer.h"
#include "util/PerlinNoise.h"

#include "streams/SampleStream.h"
#include "streams/oscillator.h"
#include "streams/WhiteNoiseGenerator.h"

#include "oscillators/basic_oscillators.h"
#include "oscillators/DetunableModel.h"

#include "filters/Filter.h"

#include "pitch_detection/yin.h"
#include "pitch_detection/PitchDetector.h"

#include "psola/analyzer.h"
#include "psola/shifter.h"

// IWYU pragma: end_exports
