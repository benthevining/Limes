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

project (
	LimesLocale
	VERSION "${Limes_VERSION}"
	DESCRIPTION "Limes localization library"
	HOMEPAGE_URL "${Limes_HOMEPAGE_URL}"
	LANGUAGES CXX)

add_library (limes_locale STATIC)

set_target_properties (limes_locale PROPERTIES VERSION "${PROJECT_VERSION}"
											   SOVERSION "${PROJECT_VERSION_MAJOR}")

target_link_libraries (limes_locale PUBLIC Limes::limes_data_structures
					   PRIVATE Oranges::OrangesDefaultTarget Oranges::OrangesUnityBuild)

set (LIMES_LOCALE_INSTALL_INCLUDE_DIR "${LIMES_INSTALL_INCLUDE_DIR}/limes_locale" CACHE INTERNAL "")

target_sources (
	limes_locale PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/limes_locale.h>
						$<INSTALL_INTERFACE:${LIMES_LOCALE_INSTALL_INCLUDE_DIR}/limes_locale.h>)

install (FILES limes_locale.h DESTINATION "${LIMES_LOCALE_INSTALL_INCLUDE_DIR}"
		 COMPONENT limes_locale_dev)

target_include_directories (
	limes_locale PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
						$<INSTALL_INTERFACE:${LIMES_LOCALE_INSTALL_INCLUDE_DIR}>)

add_subdirectory (countries)
add_subdirectory (languages)

source_group (TREE "${CMAKE_CURRENT_LIST_DIR}" FILES limes_locale.h ${countries_files}
													 ${languages_files})

unset (countries_files)
unset (languages_files)

install (
	TARGETS limes_locale
	EXPORT LimesLocaleTargets
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT limes_locale_runtime
			NAMELINK_COMPONENT limes_locale_dev
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT limes_locale_dev
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT limes_locale_runtime
	INCLUDES
	DESTINATION "${LIMES_LOCALE_INSTALL_INCLUDE_DIR}")

install (
	EXPORT LimesLocaleTargets
	NAMESPACE Limes::
	DESTINATION "${LIMES_INSTALL_DEST}"
	EXPORT_LINK_INTERFACE_LIBRARIES
	COMPONENT limes_locale_base)

oranges_create_pkgconfig_file (
	TARGET limes_locale
	INCLUDE_REL_PATH "${LIMES_LOCALE_INSTALL_INCLUDE_DIR}"
	DESCRIPTION "${PROJECT_DESCRIPTION}"
	URL "${PROJECT_HOMEPAGE_URL}"
	VERSION "${PROJECT_VERSION}"
	INSTALL_COMPONENT limes_locale_dev)

cpack_add_component (limes_locale_base HIDDEN)

cpack_add_component (limes_locale_runtime GROUP limes_locale DEPENDS limes_locale_base
					 INSTALL_TYPES User)

cpack_add_component (limes_locale_dev GROUP limes_locale DEPENDS limes_locale_runtime
					 INSTALL_TYPES Developer)

cpack_add_component_group (
	limes_locale DISPLAY_NAME "${PROJECT_DESCRIPTION}"
	DESCRIPTION "All components of the ${PROJECT_DESCRIPTION}" PARENT_GROUP limes)

add_library (Limes::limes_locale ALIAS limes_locale)

target_link_libraries (Limes INTERFACE Limes::limes_locale)

if(Limes_IS_TOP_LEVEL)
	include (OrangesDefaultWarnings)

	target_link_libraries (limes_locale PRIVATE $<BUILD_INTERFACE:Oranges::OrangesDefaultWarnings>)
endif()
