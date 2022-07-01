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

FindJUCE
-------------------------

A find module for JUCE itself.

This file can be copied verbatim into any projects that depend on JUCE, and committed to their source control -- this is what I do for my projects.

You can then use the environment or command line variable :variable:`JUCE_PATH` to turn this find module into an :external:command:`add_subdirectory() <add_subdirectory>` of a local copy of JUCE;
if neither variable is set, this module will use :external:module:`FetchContent` to download the JUCE sources from GitHub.

Cache variables
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. cmake:variable:: JUCE_PATH

This variable may be set to a local copy of the JUCE repository, in which case calling ``find_package (JUCE)`` will result in this script executing ``add_subdirectory (${JUCE_PATH})``.

If this variable is not set, this script will use CMake's :external:module:`FetchContent` module to fetch the JUCE sources using Git.

This variable is initialized by the value of the :envvar:`JUCE_PATH` environment variable.

Environment variables
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. cmake:envvar:: JUCE_PATH

Initializes the :variable:`JUCE_PATH` variable.

#]=======================================================================]

include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.22 FATAL_ERROR)

#

include (FindPackageMessage)
include (FeatureSummary)

set_package_properties (
	"${CMAKE_FIND_PACKAGE_NAME}" PROPERTIES URL "https://github.com/juce-framework/JUCE"
	DESCRIPTION "Framework for cross-platform app and plugin development")

set (${CMAKE_FIND_PACKAGE_NAME}_FOUND FALSE)

#

if (NOT (DEFINED JUCE_PATH OR DEFINED CACHE{JUCE_PATH}))
	if (DEFINED ENV{JUCE_PATH})
		set (JUCE_PATH "$ENV{JUCE_PATH}" CACHE PATH "Path to the JUCE repository")
	endif ()
endif ()

#

include (FindPackageHandleStandardArgs)

# cmake-lint: disable=C0103
macro (__find_JUCE_check_version_from_cmakelists __cmakelists_path __result_var)
	if (NOT EXISTS "${__cmakelists_path}")
		if (NOT ${CMAKE_FIND_PACKAGE_NAME}_FIND_QUIETLY)
			message (WARNING "CMakeLists.txt does not exist at path ${__cmakelists_path}!")
		endif ()

		set (${__result_var} TRUE)
	else ()
		file (READ "${__cmakelists_path}" __cmakelists_text)

		string (FIND "${__cmakelists_text}" "project (" __project_cmd_pos)

		string (SUBSTRING "${__cmakelists_text}" "${__project_cmd_pos}" 50 __project_cmd_string)

		string (FIND "${__project_cmd_string}" "VERSION" __version_kwd_pos)

		math (EXPR __version_kwd_pos "${__version_kwd_pos} + 8" OUTPUT_FORMAT DECIMAL)

		string (SUBSTRING "${__project_cmd_string}" "${__version_kwd_pos}" 6
						  __project_version_string)

		find_package_check_version ("${__project_version_string}" "${__result_var}"
									HANDLE_VERSION_RANGE)
	endif ()
endmacro ()

#

macro (__find_JUCE_post_include_actions)
	set (${CMAKE_FIND_PACKAGE_NAME}_FOUND TRUE)
endmacro ()

#

# check for local path to dependency

if (JUCE_PATH)
	if (NOT IS_DIRECTORY "${JUCE_PATH}")
		if (NOT ${CMAKE_FIND_PACKAGE_NAME}_FIND_QUIETLY)
			message (WARNING "JUCE_PATH set to non-existent directory ${JUCE_PATH}!")
		endif ()
	else ()
		__find_juce_check_version_from_cmakelists ("${JUCE_PATH}/CMakeLists.txt" local_copy_found)

		if (local_copy_found)
			add_subdirectory ("${JUCE_PATH}" "${CMAKE_BINARY_DIR}/JUCE")

			__find_juce_post_include_actions ()

			find_package_message ("${CMAKE_FIND_PACKAGE_NAME}" "JUCE package found -- local"
								  "JUCE (local)[${JUCE_PATH}]")

			return ()
		else ()
			if (NOT ${CMAKE_FIND_PACKAGE_NAME}_FIND_QUIETLY)
				message (
					WARNING
						"Local copy of JUCE has an unsuitable version, falling back to fetching the sources with git..."
					)
			endif ()
		endif ()
	endif ()
endif ()

#

# fetch from git using FetchContent

set (FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/Cache" CACHE PATH "FetchContent dependency cache")

mark_as_advanced (FETCHCONTENT_BASE_DIR)

include (FetchContent)

FetchContent_Declare (JUCE GIT_REPOSITORY "https://github.com/juce-framework/JUCE.git"
					  GIT_TAG "origin/master")

FetchContent_MakeAvailable (JUCE)

__find_juce_post_include_actions ()

find_package_message ("${CMAKE_FIND_PACKAGE_NAME}" "JUCE package found -- Sources downloaded"
					  "JUCE (git)")
