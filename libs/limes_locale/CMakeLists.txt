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

limes_locale
-------------------------

A library for internationalization and locale-related operations.

Dependencies
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- limes_data_structures

Targets
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- Limes::limes_locale

Install components
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- limes_locale_dev
- limes_locale_runtime

Install component groups
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- limes_locale

#]=======================================================================]

include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.22 FATAL_ERROR)

include (OrangesDefaultTarget)
include (OrangesUnityBuild)
include (OrangesGeneratePkgConfig)
include (CPackComponent)
include (GNUInstallDirs)

add_library (limes_locale STATIC)

set_target_properties (limes_locale PROPERTIES VERSION "${PROJECT_VERSION}"
											   SOVERSION "${PROJECT_VERSION_MAJOR}")

target_link_libraries (limes_locale PUBLIC Limes::limes_data_structures
					   PRIVATE Oranges::OrangesDefaultTarget Oranges::OrangesUnityBuild)

set (LIMES_VECOPS_INSTALL_INCLUDE_DIR "${LIMES_INSTALL_INCLUDE_DIR}/limes_locale" CACHE INTERNAL "")

target_sources (
	limes_locale PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/limes_locale.h>
						$<INSTALL_INTERFACE:${LIMES_VECOPS_INSTALL_INCLUDE_DIR}/limes_locale.h>)

install (FILES "${CMAKE_CURRENT_LIST_DIR}/limes_locale.h"
		 DESTINATION "${LIMES_VECOPS_INSTALL_INCLUDE_DIR}" COMPONENT limes_locale_dev)

target_include_directories (
	limes_locale PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
						$<INSTALL_INTERFACE:${LIMES_VECOPS_INSTALL_INCLUDE_DIR}>)

add_subdirectory (languages)

install (
	TARGETS limes_locale
	EXPORT LimesLocaleTargets
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT limes_locale_runtime
			NAMELINK_COMPONENT limes_locale_dev
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT limes_locale_dev
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT limes_locale_runtime
	INCLUDES
	DESTINATION "${LIMES_VECOPS_INSTALL_INCLUDE_DIR}")

install (
	EXPORT LimesLocaleTargets
	NAMESPACE Limes::
	DESTINATION "${LIMES_INSTALL_DEST}"
	EXPORT_LINK_INTERFACE_LIBRARIES
	COMPONENT limes_locale_base)

oranges_create_pkgconfig_file (
	TARGET limes_locale
	INCLUDE_REL_PATH "${LIMES_VECOPS_INSTALL_INCLUDE_DIR}"
	DESCRIPTION "Limes locale library"
	URL "${PROJECT_HOMEPAGE_URL}"
	VERSION "${PROJECT_VERSION}"
	INSTALL_COMPONENT limes_locale_dev)

cpack_add_component (limes_locale_base HIDDEN)

cpack_add_component (limes_locale_runtime GROUP limes_locale DEPENDS limes_locale_base
					 INSTALL_TYPES User)

cpack_add_component (limes_locale_dev GROUP limes_locale DEPENDS limes_locale_runtime
					 INSTALL_TYPES Developer)

cpack_add_component_group (
	limes_locale DISPLAY_NAME "Limes vecops library"
	DESCRIPTION "All components of the Limes vector operations library" PARENT_GROUP limes)

if(NOT TARGET Limes::limes_locale)
	add_library (Limes::limes_locale ALIAS limes_locale)
endif()

target_link_libraries (Limes INTERFACE Limes::limes_locale)
