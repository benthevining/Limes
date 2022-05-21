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

Environment variables
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- LIMES_PATH

Cache variables
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- FETCHCONTENT_BASE_DIR

#]=======================================================================]

include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.22 FATAL_ERROR)

#

include (FindPackageMessage)
include (FeatureSummary)

set_package_properties (
	"${CMAKE_FIND_PACKAGE_NAME}" PROPERTIES URL "https://github.com/benthevining/Limes"
	DESCRIPTION "C++ utility libraries")

set (${CMAKE_FIND_PACKAGE_NAME}_FOUND FALSE)

#

if (NOT (DEFINED LIMES_PATH OR DEFINED CACHE{LIMES_PATH}))
	if (DEFINED ENV{LIMES_PATH})
		set (LIMES_PATH "$ENV{LIMES_PATH}" CACHE PATH "Path to the Limes repository")
	endif ()
endif ()

# cmake-lint: disable=C0103
function (_find_limes_try_local_dir)
	if (NOT IS_DIRECTORY "${LIMES_PATH}")
		message (WARNING "LIMES_PATH set to non-existent directory ${LIMES_PATH}!")
		return ()
	endif ()

	set (limes_cmakelists "${LIMES_PATH}/CMakeLists.txt")

	if (NOT EXISTS "${limes_cmakelists}")
		message (WARNING "CMakeLists.txt does not exist in supplied LIMES_PATH: ${LIMES_PATH}!")
		return ()
	endif ()

	if (${CMAKE_FIND_PACKAGE_NAME}_FIND_VERSION)
		file (READ "${limes_cmakelists}" cmakelists_text)

		string (FIND "${cmakelists_text}" "project (" project_pos)

		string (SUBSTRING "${cmakelists_text}" "${project_pos}" 50 project_string)

		string (FIND "${project_string}" "VERSION" version_pos)

		math (EXPR version_pos "${version_pos} + 8" OUTPUT_FORMAT DECIMAL)

		string (SUBSTRING "${project_string}" "${version_pos}" 6 version_string)

		if (${CMAKE_FIND_PACKAGE_NAME}_FIND_VERSION_EXACT)
			if (NOT "${version_string}" VERSION_EQUAL "${${CMAKE_FIND_PACKAGE_NAME}_FIND_VERSION}")
				message (
					WARNING
						"Local version of Limes doesn't have EXACT version requested (${version_string}, requested ${${CMAKE_FIND_PACKAGE_NAME}_FIND_VERSION})"
					)
				return ()
			endif ()
		else ()
			if ("${version_string}" VERSION_LESS "${${CMAKE_FIND_PACKAGE_NAME}_FIND_VERSION}")
				message (
					WARNING
						"Local version of Limes has too old version (${version_string}, requested ${${CMAKE_FIND_PACKAGE_NAME}_FIND_VERSION})"
					)
				return ()
			endif ()
		endif ()
	endif ()

	add_subdirectory ("${LIMES_PATH}" "${CMAKE_BINARY_DIR}/Limes")

	find_package_message ("${CMAKE_FIND_PACKAGE_NAME}" "Limes package found -- local"
						  "Limes (local)[${LIMES_PATH}]")

	set (${CMAKE_FIND_PACKAGE_NAME}_FOUND TRUE PARENT_SCOPE)
endfunction ()

if (LIMES_PATH)
	_find_limes_try_local_dir ()
endif ()

if (${CMAKE_FIND_PACKAGE_NAME}_FOUND)
	return ()
endif ()

#

set (FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/Cache" CACHE PATH "FetchContent dependency cache")

mark_as_advanced (FORCE FETCHCONTENT_BASE_DIR)

include (FetchContent)

FetchContent_Declare (Limes GIT_REPOSITORY https://github.com/benthevining/Limes.git
					  GIT_TAG origin/main)

FetchContent_MakeAvailable (Limes)

find_package_message ("${CMAKE_FIND_PACKAGE_NAME}" "Limes package found -- Sources downloaded"
					  "Limes (GitHub)")

set (${CMAKE_FIND_PACKAGE_NAME}_FOUND TRUE)
