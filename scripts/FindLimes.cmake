# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

#[=======================================================================[.rst:

FindLimes
-------------------------

A find module for Limes itself.
This file can be copied verbatim into any projects that depend on Limes, and committed to their source control -- this is what I do for my projects.
You can then use the environment or command line variable LIMES_PATH to turn this find module into an add_subdirectory of a local copy of Limes;
if neither variable is set, this module will use FetchContent to download the Limes sources from GitHub.

Output variables
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- Limes_FOUND

Environment variables
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- LIMES_PATH

Cache variables
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- FETCHCONTENT_BASE_DIR

#]=======================================================================]

include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

#

include (FindPackageMessage)
include (FeatureSummary)

set_package_properties (Limes PROPERTIES URL "https://github.com/benthevining/Limes"
						DESCRIPTION "C++ utility libraries")

set (Limes_FOUND TRUE)

#

if(NOT LIMES_PATH)
	if(DEFINED ENV{LIMES_PATH})
		set (LIMES_PATH "$ENV{LIMES_PATH}")
	endif()
endif()

if(LIMES_PATH)
	if(IS_DIRECTORY "${LIMES_PATH}")
		add_subdirectory ("${LIMES_PATH}" "${CMAKE_BINARY_DIR}/Limes")

		find_package_message (Limes "Limes package found -- local" "Limes (local)[${LIMES_PATH}]")

		unset (LIMES_PATH)

		return ()
	else()
		message (WARNING "LIMES_PATH set to non-existent directory ${LIMES_PATH}")
	endif()
endif()

unset (LIMES_PATH)

#

set (FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/Cache" CACHE PATH "FetchContent dependency cache")

mark_as_advanced (FORCE FETCHCONTENT_BASE_DIR)

include (FetchContent)

FetchContent_Declare (Limes GIT_REPOSITORY https://github.com/benthevining/Limes.git
					  GIT_TAG origin/main)

FetchContent_MakeAvailable (Limes)

find_package_message (Limes "Limes package found -- Sources downloaded" "Limes (GitHub)")
