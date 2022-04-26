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
- LIMES_USE_IPP
- LIMES_USE_MIPP
- LIMES_USE_VECOPS_FALLBACK
- LIMES_IGNORE_POMMIER
- LIMES_IGNORE_FFTW
- LIMES_BUILD_TESTS
- LIMES_BUILD_DOCS

Cache variables
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- LIMES_VECOPS_BACKEND=<vDSP|IPP|MIPP|Fallback>
- LIMES_INSTALL_DEST - Directory below INSTALL_PREFIX where the Limes CMake package files will be installed to

Required packages
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- Oranges

Targets
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- Limes::limes_core
- Limes::limes_vecops
- Limes::limes_music
- Limes::limes_audio
- Limes::limes_data_structures
- Limes::limes_locale
- Limes::BinaryBuilder
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
- limes_data_structures_dev
- limes_data_structures_runtime
- limes_locale_dev
- limes_locale_runtime
- limes_dev
- limes_runtime
- limes_docs
- limes_binary_builder_dev
- limes_binary_builder_runtime

Install component groups
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- limes_vecops
- limes_music
- limes_core
- limes_audio
- limes_data_structures
- limes_locale
- limes_binary_builder
- limes

find_package components
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- All
- Audio
- Core
- DataStructures
- Locale
- Music
- Vecops
- BinaryBuilder

#]=======================================================================]

include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

option (CMAKE_FIND_PACKAGE_PREFER_CONFIG
		"Prefer config files to find modules in find_package search" ON)

option (LIMES_BUILD_TESTS "Build the Limes unit tests" OFF)
option (LIMES_BUILD_DOCS "Build the Limes documentation" OFF)

mark_as_advanced (CMAKE_FIND_PACKAGE_PREFER_CONFIG)

project (
	Limes
	VERSION 4.5.0
	DESCRIPTION "C++ utilities and building blocks"
	HOMEPAGE_URL "www.github.com/benthevining/Limes"
	LANGUAGES CXX ASM)

enable_testing ()

#

list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package (Oranges 2.17 REQUIRED)

include (FeatureSummary)

set_package_properties (Oranges PROPERTIES TYPE REQUIRED PURPOSE "Required CMake modules")

#

if(PROJECT_IS_TOP_LEVEL)
	set_property (GLOBAL PROPERTY DEBUG_CONFIGURATIONS Debug)
	set_property (GLOBAL PROPERTY USE_FOLDERS YES)
	set_property (GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "Targets")
	set_property (GLOBAL PROPERTY REPORT_UNDEFINED_PROPERTIES
								  "${CMAKE_BINARY_DIR}/undefined_properties.log")

	set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
	set (CMAKE_FIND_PACKAGE_SORT_ORDER NATURAL)
	set (CMAKE_INSTALL_MESSAGE LAZY)
	set (CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION ON)

	include (OrangesDeprecateDirectoryScopedCommands)
	include (OrangesWipeCacheTarget)
	include (OrangesUninstallTarget)
	include (OrangesDefaultCPackSettings)
	include (OrangesInstallSystemLibs)

	set (Limes_DEPS_GRAPH_OUTPUT_TO_SOURCE util)

	include (OrangesGraphVizConfig)

	unset (Limes_DEPS_GRAPH_OUTPUT_TO_SOURCE)
endif()

#

include (GNUInstallDirs)

set (
	LIMES_INSTALL_DEST "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
	CACHE STRING
		  "Directory below INSTALL_PREFIX where the Limes CMake package files will be installed to")

mark_as_advanced (FORCE LIMES_INSTALL_DEST)

#

add_subdirectory (libs)
# add_subdirectory (binary_builder)

if(LIMES_BUILD_TESTS)
	add_subdirectory (tests)
endif()

#

include (CPackComponent)

if(LIMES_BUILD_DOCS)
	include (OrangesDoxygenConfig)

	oranges_create_doxygen_target (TARGET LimesDoxygen INPUT_PATHS libs
								   INSTALL_COMPONENT limes_docs)

	cpack_add_component (
		limes_docs
		DISPLAY_NAME "Limes documentation"
		DESCRIPTION "Install the Limes documentation"
		GROUP limes
		INSTALL_TYPES Developer)
endif()

#

install (FILES "${CMAKE_CURRENT_LIST_DIR}/cmake/LimesConfigVersionHeader.cmake"
			   "${CMAKE_CURRENT_LIST_DIR}/cmake/scripts/VersionHeader.h" COMPONENT limes_dev
		 DESTINATION "${LIMES_INSTALL_DEST}/cmake")

include (CMakePackageConfigHelpers)

write_basic_package_version_file ("LimesConfigVersion.cmake" VERSION "${PROJECT_VERSION}"
								  COMPATIBILITY SameMajorVersion)

configure_package_config_file (
	"${CMAKE_CURRENT_LIST_DIR}/cmake/LimesConfig.cmake" "LimesConfig.cmake"
	INSTALL_DESTINATION "${LIMES_INSTALL_DEST}" NO_SET_AND_CHECK_MACRO)

install (FILES "${CMAKE_CURRENT_BINARY_DIR}/LimesConfigVersion.cmake"
			   "${CMAKE_CURRENT_BINARY_DIR}/LimesConfig.cmake" DESTINATION "${LIMES_INSTALL_DEST}"
		 COMPONENT limes_base)

cpack_add_component (limes_base HIDDEN)

cpack_add_component (
	limes_runtime
	GROUP limes
	DEPENDS # cmake-format: sortable
			limes_base limes_vecops_runtime limes_music_runtime limes_core_runtime
			limes_audio_runtime
	INSTALL_TYPES User)

cpack_add_component (
	limes_dev
	GROUP limes
	DEPENDS # cmake-format: sortable
			limes_runtime limes_vecops_dev limes_music_dev limes_core_dev limes_audio_dev
	INSTALL_TYPES Developer)

cpack_add_component_group (
	limes
	DISPLAY_NAME "Limes library"
	DESCRIPTION "All components of the Limes library"
	BOLD_TITLE
	PARENT_GROUP Citrus)

cpack_add_install_type (User DISPLAY_NAME "End-user installation")

cpack_add_install_type (Developer DISPLAY_NAME "Developer installation")

#

feature_summary (
	FILENAME "${CMAKE_BINARY_DIR}/package_features.txt"
	INCLUDE_QUIET_PACKAGES
	WHAT ALL
	DESCRIPTION
		"This version of Limes was compiled with the following features and third party libraries:")

install (FILES "${CMAKE_BINARY_DIR}/package_features.txt" DESTINATION "${LIMES_INSTALL_DEST}"
		 CONFIGURATIONS Debug COMPONENT limes_base)

if(PROJECT_IS_TOP_LEVEL)
	feature_summary (INCLUDE_QUIET_PACKAGES DEFAULT_DESCRIPTION QUIET_ON_EMPTY WHAT ALL)
endif()

export (PACKAGE Limes)

set (Limes_FOUND TRUE CACHE INTERNAL "")
