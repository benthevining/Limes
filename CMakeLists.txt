# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.22 FATAL_ERROR)

project (
	Limes
	VERSION 5.0.2
	LANGUAGES CXX
	DESCRIPTION "C++ utilities and building blocks"
	HOMEPAGE_URL "www.github.com/benthevining/Limes")

option (CMAKE_FIND_PACKAGE_PREFER_CONFIG
		"Prefer config files to find modules in find_package search" ON)

option (BUILD_SHARED_LIBS "Build shared libraries, instead of static" OFF)

option (LIMES_MINIMAL "Build only the core libraries, no extra stuff" OFF)

include (CMakeDependentOption)

cmake_dependent_option (LIMES_BUILD_PROGRAMS "Build the utility programs Limes ships" ON
						"NOT LIMES_MINIMAL" OFF)

cmake_dependent_option (LIMES_BUILD_TESTS "Build the Limes unit tests" "${PROJECT_IS_TOP_LEVEL}"
						"NOT LIMES_MINIMAL" OFF)

cmake_dependent_option (LIMES_BUILD_DOCS "Build the Limes documentation" "${PROJECT_IS_TOP_LEVEL}"
						"NOT LIMES_MINIMAL" OFF)

cmake_dependent_option (LIMES_BUILD_JUCE "Build the JUCE bindings for the Limes libraries" ON
						"NOT LIMES_MINIMAL" OFF)

option (ORANGES_MAINTAINER_BUILD "Build with static analysis tools and default compiler warnings"
		"${PROJECT_IS_TOP_LEVEL}")

mark_as_advanced (CMAKE_FIND_PACKAGE_PREFER_CONFIG ORANGES_MAINTAINER_BUILD)

enable_testing ()

#

list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package (Oranges 3.1.1 REQUIRED)

include (FeatureSummary)

set_package_properties (Oranges PROPERTIES TYPE REQUIRED PURPOSE "Required CMake modules")

#

if (PROJECT_IS_TOP_LEVEL)
	# cmake-format: off
	set_property (
		GLOBAL PROPERTY
			DEBUG_CONFIGURATIONS Debug
			USE_FOLDERS YES
			PREDEFINED_TARGETS_FOLDER Utility
			REPORT_UNDEFINED_PROPERTIES "${CMAKE_BINARY_DIR}/undefined_properties.log")
	# cmake-format: on

	set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
	set (CMAKE_FIND_PACKAGE_SORT_ORDER NATURAL)
	set (CMAKE_INSTALL_MESSAGE LAZY)
	set (CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION ON)

	include (OrangesWipeCacheTarget)
	include (OrangesUninstallTarget)
	include (OrangesDefaultCPackSettings)
	include (OrangesInstallSystemLibs)
endif ()

#

include (GNUInstallDirs)

set (
	LIMES_INSTALL_DEST "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
	CACHE STRING
		  "Directory below INSTALL_PREFIX where the Limes CMake package files will be installed to")

mark_as_advanced (FORCE LIMES_INSTALL_DEST)

set (LIMES_EXPORT_DIR "${CMAKE_CURRENT_BINARY_DIR}/export"
	 CACHE INTERNAL "Directory where build tree export files are being written to")

#

include (CPackComponent)

add_subdirectory (libs)

if (LIMES_BUILD_PROGRAMS)
	add_subdirectory (programs)
endif ()

if (LIMES_BUILD_JUCE)
	# add_subdirectory (juce)
elseif (LIMES_BUILD_EXAMPLE_PLUGINS)
	message (
		WARNING
			"Limes example JUCE plugins cannot be built because the LIMES_BUILD_JUCE option is OFF")
endif ()

add_feature_info ("JUCE bindings" "${LIMES_BUILD_JUCE}"
				  "Build the JUCE API bindings for the Limes libraries")

if (LIMES_BUILD_TESTS)
	add_subdirectory (tests)
endif ()

add_feature_info ("Unit tests" "${LIMES_BUILD_TESTS}" "Build the Limes unit test runner")

#

if (LIMES_BUILD_DOCS)
	add_subdirectory (docs)
endif ()

add_feature_info ("Docs" "${LIMES_BUILD_DOCS}" "Build the Limes documentation")

#

if (NOT LIMES_BUILD_PROGRAMS)
	set (exclude_binbuild |LimesBinaryBuilder.cmake)
endif ()

install (DIRECTORY cmake DESTINATION "${LIMES_INSTALL_DEST}" COMPONENT limes_dev
		 PATTERN ".DS_Store${exclude_binbuild}" EXCLUDE)

include (CMakePackageConfigHelpers)

write_basic_package_version_file (Limes-config-version.cmake VERSION "${PROJECT_VERSION}"
								  COMPATIBILITY SameMajorVersion)

configure_package_config_file (cmake/Limes-config.cmake Limes-config.cmake
							   INSTALL_DESTINATION "${LIMES_INSTALL_DEST}" NO_SET_AND_CHECK_MACRO)

install (FILES "${CMAKE_CURRENT_BINARY_DIR}/Limes-config-version.cmake"
			   "${CMAKE_CURRENT_BINARY_DIR}/Limes-config.cmake" DESTINATION "${LIMES_INSTALL_DEST}"
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
	FILENAME "${PROJECT_BINARY_DIR}/package_features.txt"
	INCLUDE_QUIET_PACKAGES
	WHAT ALL
	DESCRIPTION
		"This version of Limes was compiled with the following features and third party libraries:")

install (FILES "${PROJECT_BINARY_DIR}/package_features.txt" DESTINATION "${LIMES_INSTALL_DEST}"
		 COMPONENT limes_base)

if (PROJECT_IS_TOP_LEVEL)
	feature_summary (INCLUDE_QUIET_PACKAGES DEFAULT_DESCRIPTION QUIET_ON_EMPTY WHAT ALL)
endif ()

export (PACKAGE Limes)
