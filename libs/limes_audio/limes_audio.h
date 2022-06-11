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

	All classes and functions in this module are accessible after linking to the \c limes_audio library and including limes_audio.h.

	This library provides classes for processing and analysis of realtime audio signals.

	Dependencies: limes_data_structures

	@ingroup limes
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
