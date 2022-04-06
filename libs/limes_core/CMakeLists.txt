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

limes_core
-------------------------

The Limes core library of utilities.

Targets
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- Limes::limes_core

Install components
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- limes_core_dev
- limes_core_runtime

Install component groups
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- limes_core

#]=======================================================================]

include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

include (OrangesDefaultTarget)
include (OrangesUnityBuild)
include (OrangesGeneratePkgConfig)
include (OrangesGenerateExportHeader)
include (OrangesGeneratePlatformHeader)
include (CPackComponent)
include (GNUInstallDirs)

add_library (limes_core STATIC)

set_target_properties (limes_core PROPERTIES VERSION "${PROJECT_VERSION}"
											 SOVERSION "${PROJECT_VERSION_MAJOR}")

target_link_libraries (limes_core PUBLIC Limes::limes_vecops PRIVATE Oranges::OrangesDefaultTarget
																	 Oranges::OrangesUnityBuild)

oranges_generate_export_header (
	TARGET
	limes_core
	BASE_NAME
	limes
	HEADER
	limes_export.h
	REL_PATH
	Limes/limes_core)

oranges_generate_platform_header (
	TARGET
	limes_core
	BASE_NAME
	limes
	HEADER
	limes_platform.h
	REL_PATH
	Limes/limes_core)

set (LIMES_CORE_INSTALL_INCLUDE_DIR "${LIMES_INSTALL_INCLUDE_DIR}/limes_core" CACHE INTERNAL "")

target_sources (
	limes_core PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/limes_core.h>
					  $<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/limes_core.h>)

install (FILES "${CMAKE_CURRENT_LIST_DIR}/limes_core.h"
		 DESTINATION "${LIMES_CORE_INSTALL_INCLUDE_DIR}" COMPONENT limes_core_dev)

target_include_directories (
	limes_core PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
					  $<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}>)

add_subdirectory (data_structures)
add_subdirectory (math)
add_subdirectory (misc)
add_subdirectory (threading)
add_subdirectory (text)

install (
	TARGETS limes_core
	EXPORT LimesCoreTargets
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT limes_core_runtime
			NAMELINK_COMPONENT limes_core_dev
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT limes_core_dev
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT limes_core_runtime
	INCLUDES
	DESTINATION "${LIMES_CORE_INSTALL_INCLUDE_DIR}")

install (
	EXPORT LimesCoreTargets
	NAMESPACE Limes::
	DESTINATION "${LIMES_INSTALL_DEST}"
	EXPORT_LINK_INTERFACE_LIBRARIES
	COMPONENT limes_core_base)

oranges_create_pkgconfig_file (
	TARGET
	limes_core
	INCLUDE_REL_PATH
	"${LIMES_CORE_INSTALL_INCLUDE_DIR}"
	DESCRIPTION
	"Limes core utilities library"
	URL
	"${PROJECT_HOMEPAGE_URL}"
	VERSION
	"${PROJECT_VERSION}"
	INSTALL_COMPONENT
	limes_core_dev)

cpack_add_component (limes_core_base HIDDEN)

cpack_add_component (limes_core_runtime GROUP limes_core DEPENDS limes_core_base INSTALL_TYPES User)

cpack_add_component (limes_core_dev GROUP limes_core DEPENDS limes_core_runtime
					 INSTALL_TYPES Developer)

cpack_add_component_group (
	limes_core DISPLAY_NAME "Limes core library"
	DESCRIPTION "All components of the Limes core utilities library" PARENT_GROUP limes)

if(NOT TARGET Limes::limes_core)
	add_library (Limes::limes_core ALIAS limes_core)
endif()

target_link_libraries (Limes INTERFACE Limes::limes_core)
