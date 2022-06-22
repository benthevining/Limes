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

/** @defgroup limes Limes
	The Limes library.

	@version 4.8.1
	@author Ben Vining
	@date 2020 - 2022
	@copyright GNU public license

	This group includes all component libraries of Limes.

	@todo Create a page documenting all environment variables that affect the C++ code - LIMES_ASSERTION_LOG_FILE, FFTW_WISDOM_FILE_DIR, HOME
	@todo Review int vs size_t APIs
	@todo Fix protobuf build
	@todo Integrate documentation of CMake modules into Doxygen
	@todo Android cross compile
	@todo libharu wrapper for PDFs
	@todo libUSB wrapper
	@todo move GPS utils into this library
	@todo is caps lock on (and other modifier keys?)
 */

/** @dir libs
	This directory contains all of the Limes libraries.
	@ingroup limes
 */

/** @file
	The main header for the entire Limes library.
	This file includes the headers for all Limes libraries.

	This file is only included by the interface \c Limes::Limes target, not any of the individual library targets.

	@ingroup limes
 */

#pragma once

// IWYU pragma: begin_exports

#include <limes_core.h>
#include <limes_vecops.h>
#include <limes_music.h>
#include <limes_midi.h>
#include <limes_audio.h>

// IWYU pragma: end_exports
