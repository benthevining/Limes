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

	This group includes all component libraries of Limes:
	- @ref lib_limes_core "limes_core"
	- @ref lib_limes_audio "limes_audio"
	- @ref lib_limes_midi "limes_midi"
	- @ref lib_limes_music "limes_music"
	- @ref lib_limes_vecops "limes_vecops"

	@par CMake targets
	@cmaketarget \b Limes::Limes
	Interface library that links to all of the Limes library targets and includes \c limes.h .

	The top-level Limes project accepts the following CMake options:

	@cmakeopt \b LIMES_BUILD_PROGRAMS
	Builds the command-line utility programs that ship with Limes. This option defaults to \c ON .
	In order to use the \c LimesBinaryBuilder.cmake module, this option must be set to \c ON .
	@cmakeopt \b LIMES_BUILD_TESTS
	Builds the Limes unit tests. Defaults to \c OFF .
	@cmakeopt \b LIMES_BUILD_DOCS
	Builds the Limes documentation. Defaults to \c OFF .
	@cmakeopt \b LIMES_INTERNAL_DOCS
	When building the documentation, includes extra information on internal implementation details
	not meant for inclusion in the public documentation. This option always defaults to \c OFF ,
	and is intended for internal use only.
	@cmakeopt \b LIMES_BUILD_JUCE
	This option enables building the JUCE bindings for the Limes libraries. This option defaults to \c ON .
	@cmakeopt \b LIMES_BUILD_EXAMPLE_PLUGINS
	Builds the example JUCE plugins. This option requires that the \c LIMES_BUILD_JUCE option is \c ON .
	@cmakeopt \b LIMES_MINIMAL
	This option sets \c LIMES_BUILD_PROGRAMS , \c LIMES_BUILD_TESTS , \c LIMES_BUILD_DOCS and \c LIMES_BUILD_JUCE to \c OFF .

	@dependency \b Oranges
	@parblock
	Limes's CMake configuration depends on <a href="https://github.com/benthevining/Oranges">the Oranges library of CMake modules</a>.

	You can set the environment or command-line variable \c ORANGES_PATH to direct the Limes CMake configuration to use a local copy.
	If no local copy is present and Oranges cannot be found on your system, invoking Limes's CMake configuration will fetch and cache
	a copy of the Oranges source code from GitHub at configure time.
	@endparblock

	@envvar \b ORANGES_PATH
	This variable can be set to the root directory of a local copy of the Oranges repository.

	@installcompgroup \b limes_libs_runtime
	Installs all Limes library runtime components.

	@installcompgroup \b limes_libs_dev
	@parblock
	Installs all Limes library development components.
	Depends on the \c limes_libs_runtime component.

	This component is analogous to the `find_package()` component \c Libs .
	@endparblock

	@installcompgroup \b limes_libs
	Installs all Limes library components. Depends on the \c limes_libs_dev component.

	@todo Fix dependency graph build
	@todo Docs Docker container
	@todo Review int vs size_t APIs
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
