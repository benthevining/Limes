# ======================================================================================
#    ____  _____            _   _  _____ ______  _____
#   / __ \|  __ \     /\   | \ | |/ ____|  ____|/ ____|
#  | |  | | |__) |   /  \  |  \| | |  __| |__  | (___
#  | |  | |  _  /   / /\ \ | . ` | | |_ |  __|  \___ \
#  | |__| | | \ \  / ____ \| |\  | |__| | |____ ____) |
#   \____/|_|  \_\/_/    \_\_| \_|\_____|______|_____/
#
#  This file is part of the Oranges open source CMake library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

#[=======================================================================[.rst:

FindOranges
-------------------------

A find module for Oranges itself.
This file can be copied verbatim into any projects that depend on Oranges, and committed to their source control -- this is what I do for my projects.
You can then use the environment or command line variable ORANGES_PATH to turn this find module into an add_subdirectory of a local copy of Oranges;
if neither variable is set, this module will use FetchContent to download the Oranges sources from GitHub.

Output variables
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- Oranges_FOUND

Environment variables
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- ORANGES_PATH

Cache variables
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- FETCHCONTENT_BASE_DIR

#]=======================================================================]

include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

#

include (FindPackageMessage)
include (FeatureSummary)

set_package_properties (Oranges PROPERTIES URL "https://github.com/benthevining/Oranges"
						DESCRIPTION "CMake modules and toolchains")

set (Oranges_FOUND TRUE)

#

if(NOT ORANGES_PATH)
	if(DEFINED ENV{ORANGES_PATH})
		set (ORANGES_PATH "$ENV{ORANGES_PATH}")
	endif()
endif()

if(ORANGES_PATH)
	if(IS_DIRECTORY "${ORANGES_PATH}")
		add_subdirectory ("${ORANGES_PATH}" "${CMAKE_BINARY_DIR}/Oranges")

		list (APPEND CMAKE_MODULE_PATH "${ORANGES_CMAKE_MODULE_PATH}")

		find_package_message (Oranges "Oranges package found -- local"
							  "Oranges (local)[${ORANGES_PATH}]")

		unset (ORANGES_PATH)

		return ()
	else()
		message (WARNING "ORANGES_PATH set to non-existent directory ${ORANGES_PATH}")
	endif()
endif()

unset (ORANGES_PATH)

#

set (FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/Cache" CACHE PATH "FetchContent dependency cache")

mark_as_advanced (FORCE FETCHCONTENT_BASE_DIR)

include (FetchContent)

FetchContent_Declare (Oranges GIT_REPOSITORY https://github.com/benthevining/Oranges.git
					  GIT_TAG origin/main)

FetchContent_MakeAvailable (Oranges)

list (APPEND CMAKE_MODULE_PATH "${ORANGES_CMAKE_MODULE_PATH}")

find_package_message (Oranges "Oranges package found -- Sources downloaded" "Oranges (GitHub)")
