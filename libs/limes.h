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

	This group includes all component libraries of Limes.

	Limes consists of the following libraries:

	Name                  | Description                 | Dependencies
	--------------------- | --------------------------- | ------------------------
	limes_core            | Core utilities              | None
	limes_vecops          | SIMD operations for vectors | limes_core
	limes_data_structures | Data structures             | limes_vecops
	limes_locale          | Localization utilities      | limes_data_structures
	limes_music           | Music theory utilities      | limes_data_structures
	limes_midi            | MIDI utilities              | limes_core
	limes_audio           | Realtime audio utilities    | limes_data_structures

	Here is the dependency graph for Limes:
	@image html deps_graph.png
	@image latex deps_graph.png
	@image xml deps_graph.png
 */

/** @dir libs
	This directory contains all of the Limes libraries.
	@ingroup limes
 */

/** @file
	The main header for the entire Limes library.
	This file includes the headers for all Limes libraries.
	@ingroup limes
 */

#pragma once

// IWYU pragma: begin_exports

#include <limes_core.h>
#include <limes_vecops.h>
#include <limes_data_structures.h>
#include <limes_locale.h>
#include <limes_music.h>
#include <limes_midi.h>
#include <limes_audio.h>

// IWYU pragma: end_exports
