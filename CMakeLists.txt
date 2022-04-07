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

Limes
-------------------------

The Limes library.

Options
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- LIMES_IGNORE_VDSP
- LIMES_IGNORE_IPP
- LIMES_IGNORE_MIPP

Required packages
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- Oranges

Targets
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- Limes::limes_core
- Limes::limes_vecops
- Limes::limes_music
- Limes::limes_audio
- Limes::Limes

Install components
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- limes_vecops_dev
- limes_vecops_runtime
- limes_music_dev
- limes_music_runtime
- limes_core_dev
- limes_core_runtime
- limes_audio_dev
- limes_audio_runtime
- limes_dev
- limes_runtime
- limes_docs

Install component groups
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- limes_vecops
- limes_music
- limes_core
- limes_audio
- limes

#]=======================================================================]

include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

project (
	Limes
	VERSION 2.7.0
	DESCRIPTION "C++ utilities and building blocks"
	HOMEPAGE_URL "www.github.com/benthevining/Limes"
	LANGUAGES CXX)

enable_testing ()

set (LIMES_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}")

include ("${CMAKE_CURRENT_LIST_DIR}/scripts/LimesMacros.cmake")

find_package (Oranges REQUIRED)

include (FeatureSummary)

set_package_properties (Oranges PROPERTIES TYPE REQUIRED PURPOSE "Required CMake modules")

set (Limes_DEPS_GRAPH_OUTPUT_TO_SOURCE util)

include (OrangesDefaultProjectSettings)

unset (Limes_DEPS_GRAPH_OUTPUT_TO_SOURCE)

set (LIMES_INSTALL_DEST "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
	 CACHE STRING "Directory below INSTALL_PREFIX where ${PROJECT_NAME} will be installed to")

add_subdirectory (libs)

oranges_create_doxygen_target (TARGET LimesDoxygen INPUT_PATHS libs INSTALL_COMPONENT limes_docs)

cpack_add_component (
	limes_docs
	DISPLAY_NAME "Limes documentation"
	DESCRIPTION "Install the Limes documentation"
	GROUP limes
	INSTALL_TYPES Developer)

#

mark_as_advanced (FORCE LIMES_INSTALL_DEST)

write_basic_package_version_file ("LimesConfigVersion.cmake" VERSION "${PROJECT_VERSION}"
								  COMPATIBILITY SameMajorVersion)

configure_package_config_file (
	"${CMAKE_CURRENT_LIST_DIR}/scripts/LimesConfig.cmake" "LimesConfig.cmake"
	INSTALL_DESTINATION "${LIMES_INSTALL_DEST}" PATH_VARS LIMES_ROOT_DIR)

install (
	FILES "${CMAKE_CURRENT_BINARY_DIR}/LimesConfigVersion.cmake"
		  "${CMAKE_CURRENT_BINARY_DIR}/LimesConfig.cmake"
		  "${CMAKE_CURRENT_LIST_DIR}/scripts/LimesMacros.cmake"
	DESTINATION "${LIMES_INSTALL_DEST}"
	COMPONENT limes_base)

cpack_add_component (limes_base HIDDEN)

cpack_add_component (
	limes_runtime GROUP limes DEPENDS limes_base limes_vecops_runtime limes_music_runtime
									  limes_core_runtime limes_audio_runtime INSTALL_TYPES User)

cpack_add_component (
	limes_dev GROUP limes DEPENDS limes_runtime limes_vecops_dev limes_music_dev limes_core_dev
								  limes_audio_dev INSTALL_TYPES Developer)

cpack_add_component_group (
	limes
	DISPLAY_NAME "Limes library"
	DESCRIPTION "All components of the Limes library"
	BOLD_TITLE
	PARENT_GROUP Citrus)

cpack_add_install_type (User DISPLAY_NAME "End-user installation")

cpack_add_install_type (Developer DISPLAY_NAME "Developer installation")

feature_summary (FILENAME "${CMAKE_BINARY_DIR}/package_features.txt" INCLUDE_QUIET_PACKAGES
																	 DEFAULT_DESCRIPTION WHAT ALL)

feature_summary (INCLUDE_QUIET_PACKAGES DEFAULT_DESCRIPTION QUIET_ON_EMPTY WHAT ALL)

export (PACKAGE Limes)

unset (LIMES_ROOT_DIR)
