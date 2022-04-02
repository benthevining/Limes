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

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

project (
	Limes
	VERSION 2.3.0
	DESCRIPTION "C++ utilities and building blocks"
	HOMEPAGE_URL "www.github.com/benthevining/Limes"
	LANGUAGES CXX)

enable_testing ()

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
	if("${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 10.0.0)
		message (
			FATAL_ERROR
				"This library requires C++ concepts, which are not supported on versions of GCC before 10. Your current version: ${CMAKE_CXX_COMPILER_VERSION}"
			)
	endif()
endif()

set (LIMES_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}")

include ("${CMAKE_CURRENT_LIST_DIR}/scripts/LimesMacros.cmake")

find_package (Oranges REQUIRED)

set (Limes_DEPS_GRAPH_OUTPUT_TO_SOURCE util)

include (OrangesDefaultProjectSettings)

#

add_subdirectory (libs)

include (OrangesDefaultInstallSettings)

oranges_create_doxygen_target (TARGET LimesDoxygen INPUT_PATHS libs)

#

write_basic_package_version_file ("LimesConfigVersion.cmake" VERSION "${PROJECT_VERSION}"
								  COMPATIBILITY SameMajorVersion)

install (EXPORT LimesTargets NAMESPACE Limes:: DESTINATION "${LIMES_INSTALL_DEST}"
		 EXPORT_LINK_INTERFACE_LIBRARIES)

export (EXPORT LimesTargets NAMESPACE Limes:: FILE LimesTargets.cmake)

configure_package_config_file (
	"${CMAKE_CURRENT_LIST_DIR}/scripts/LimesConfig.cmake" "LimesConfig.cmake"
	INSTALL_DESTINATION "${LIMES_INSTALL_DEST}" PATH_VARS LIMES_ROOT_DIR)

install (FILES "${CMAKE_CURRENT_BINARY_DIR}/LimesConfigVersion.cmake"
			   "${CMAKE_CURRENT_BINARY_DIR}/LimesConfig.cmake" DESTINATION "${LIMES_INSTALL_DEST}")

export (PACKAGE Limes)
