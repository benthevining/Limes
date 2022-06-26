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

/** @defgroup limes_midi limes_midi
	The Limes library of MIDI utilities.

	@anchor lib_limes_midi

	All classes and functions in this module are accessible after linking to the \c limes_midi library and including limes_midi.h.

	This library provides functions for working with and manipulating MIDI data.

	@par "Limes library dependencies"
	@ref lib_limes_core "limes_core"

	@cmaketarget \b Limes::limes_midi
	The @ref lib_limes_midi "limes_midi"
	library target.

	@installcomp \b limes_midi_runtime
	Runtime install component for the @ref lib_limes_midi "limes_midi"
	library.

	@installcomp \b limes_midi_dev
	@parblock
	Development install component for the @ref lib_limes_midi "limes_midi"
	library. Depends on the \c limes_midi_runtime component.

	This component is analogous to the `find_package()` component \c MIDI .
	@endparblock

	@installcompgroup \b limes_midi
	This group includes the \c limes_midi_dev and \c limes_midi_runtime components.

	@ingroup limes

	@todo MIDI glide engine
	@todo MIDI vibrato engine
	@todo MIDI chord bank
	@todo synth model
	@todo predictive harmony engine based on scale and input scale degree
 */

/** @dir libs/limes_midi
	This directory contains the limes_midi library.
	@ingroup limes_midi
 */

/** @file
	The main header for the limes_midi library.
	@ingroup limes_midi
 */

#pragma once

// IWYU pragma: begin_exports

#include "utilities/PitchbendMapper.h"
#include "utilities/VelocityMapper.h"

//#include "tuning/client.h"

// IWYU pragma: end_exports
