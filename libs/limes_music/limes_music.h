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

/** @defgroup limes_music limes_music
	The Limes library of music theory utilities.

	@anchor lib_limes_music

	All classes and functions in this module are accessible after linking to the \c limes_music library and including limes_music.h.

	This library provides data structures for working with music theory concepts, such as pitches, intervals, note durations, scales, etc.
	The documentation aims to be as exhaustive and complete as possible, but is meant to document the usage of this library's API;
	this documentation is not meant to serve as an exhaustive authority on music theory itself.

	@par "Limes library dependencies"
	@ref lib_limes_core "limes_core"

	@cmaketarget \b Limes::limes_music
	The @ref lib_limes_music "limes_music"
	library target.

	@installcomp \b limes_music_runtime
	Runtime install component for the @ref lib_limes_music "limes_music"
	library.

	@installcomp \b limes_music_dev
	@parblock
	Development install component for the @ref lib_limes_music "limes_music"
	library. Depends on the \c limes_music_runtime component.

	This component is analogous to the `find_package()` component \c Music .
	@endparblock

	@installcompgroup \b limes_music
	This group includes the \c limes_music_dev and \c limes_music_runtime components.

	@ingroup limes

	@todo funcs to get unicode music symbols - flat, sharp, natural, etc
	@todo time signature class
 */

/** @dir libs/limes_music
	This directory contains the limes_music library.
	@ingroup limes_music
 */

/** @file
	The main header for the limes_music library.
	@ingroup limes_music
 */

#pragma once

// IWYU pragma: begin_exports

#include "harmony/PitchUtils.h"
#include "harmony/PitchClass.h"
#include "harmony/Pitch.h"

#include "harmony/intervals/Interval.h"
#include "harmony/intervals/CompoundInterval.h"
#include "harmony/intervals/MicrotonalInterval.h"

#include "harmony/scales/Scale.h"
#include "harmony/scales/Chromatic.h"
#include "harmony/scales/KeySignature.h"
#include "harmony/scales/Mode.h"
#include "harmony/scales/Octatonic.h"
#include "harmony/scales/WholeTone.h"

#include "harmony/Chord.h"

#include "rhythm/NoteDuration.h"
#include "rhythm/NoteLength.h"
#include "rhythm/TimeSignature.h"

// IWYU pragma: end_exports
