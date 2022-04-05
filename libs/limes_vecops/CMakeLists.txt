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

limes_vecops
-------------------------

A vector operations library wrapping various platform and vendor specific high-performance APIs.

Options
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- LIMES_IGNORE_VDSP
- LIMES_IGNORE_IPP
- LIMES_IGNORE_MIPP

Targets
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- Limes::limes_vecops

Install components
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- limes_vecops_dev
- limes_vecops_runtime

Install component groups
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- limes_vecops

#]=======================================================================]

include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

include (OrangesDefaultTarget)
include (OrangesUnityBuild)
include (OrangesGeneratePkgConfig)
include (CPackComponent)
include (GNUInstallDirs)

add_library (limes_vecops STATIC)

set_target_properties (limes_vecops PROPERTIES VERSION "${PROJECT_VERSION}"
											   SOVERSION "${PROJECT_VERSION_MAJOR}")

target_link_libraries (limes_vecops PUBLIC Oranges::OrangesDefaultTarget Oranges::OrangesUnityBuild)

set (LIMES_VECOPS_INSTALL_INCLUDE_DIR "${LIMES_INSTALL_INCLUDE_DIR}/limes_vecops" CACHE INTERNAL "")

target_sources (
	limes_vecops PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/limes_vecops.h>
						$<INSTALL_INTERFACE:${LIMES_VECOPS_INSTALL_INCLUDE_DIR}/limes_vecops.h>)

install (FILES "${CMAKE_CURRENT_LIST_DIR}/limes_vecops.h"
		 DESTINATION "${LIMES_VECOPS_INSTALL_INCLUDE_DIR}" COMPONENT limes_vecops_dev)

target_include_directories (
	limes_vecops PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
						$<INSTALL_INTERFACE:${LIMES_VECOPS_INSTALL_INCLUDE_DIR}>)

add_subdirectory (impl)
add_subdirectory (util)
add_subdirectory (fft)

lemons_sort_target_sources (limes_vecops "${CMAKE_CURRENT_LIST_DIR}")

install (
	TARGETS limes_vecops
	EXPORT LimesVecopsTargets
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT limes_vecops_runtime
			NAMELINK_COMPONENT limes_vecops_dev
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT limes_vecops_dev
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT limes_vecops_runtime
	INCLUDES
	DESTINATION "${LIMES_VECOPS_INSTALL_INCLUDE_DIR}")

install (
	EXPORT LimesVecopsTargets
	NAMESPACE Limes::
	DESTINATION "${LIMES_INSTALL_DEST}"
	EXPORT_LINK_INTERFACE_LIBRARIES
	COMPONENT limes_vecops_base)

oranges_create_pkgconfig_file (
	TARGET
	limes_vecops
	INCLUDE_REL_PATH
	"${LIMES_VECOPS_INSTALL_INCLUDE_DIR}"
	DESCRIPTION
	"Limes vector operations library"
	URL
	"${PROJECT_HOMEPAGE_URL}"
	VERSION
	"${PROJECT_VERSION}"
	INSTALL_COMPONENT
	limes_vecops_dev)

cpack_add_component (limes_vecops_base HIDDEN)

cpack_add_component (limes_vecops_runtime GROUP limes_vecops DEPENDS limes_vecops_base)

cpack_add_component (limes_vecops_dev GROUP limes_vecops DEPENDS limes_vecops_runtime)

cpack_add_component_group (
	limes_vecops DISPLAY_NAME "Limes vecops library"
	DESCRIPTION "All components of the Limes vector operations library" PARENT_GROUP limes)

if(NOT TARGET Limes::limes_vecops)
	add_library (Limes::limes_vecops ALIAS limes_vecops)
endif()

target_link_libraries (Limes INTERFACE Limes::limes_vecops)
