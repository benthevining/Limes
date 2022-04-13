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

limes_music
-------------------------

The Limes library of utilities for working with musical and music-theory related concepts.

Dependencies
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- limes_data_structures

Targets
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- Limes::limes_music

Install components
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- limes_music_dev
- limes_music_runtime

Install component groups
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- limes_music

#]=======================================================================]

include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

include (OrangesDefaultTarget)
include (OrangesUnityBuild)
include (OrangesGeneratePkgConfig)
include (CPackComponent)
include (GNUInstallDirs)

project (
	LimesMusic
	VERSION "${Limes_VERSION}"
	DESCRIPTION "Limes music theory library"
	HOMEPAGE_URL "${Limes_HOMEPAGE_URL}"
	LANGUAGES CXX)

add_library (limes_music STATIC)

set_target_properties (limes_music PROPERTIES VERSION "${PROJECT_VERSION}"
											  SOVERSION "${PROJECT_VERSION_MAJOR}")

target_link_libraries (limes_music PUBLIC Limes::limes_data_structures
					   PRIVATE Oranges::OrangesDefaultTarget Oranges::OrangesUnityBuild)

set (LIMES_MUSIC_INSTALL_INCLUDE_DIR "${LIMES_INSTALL_INCLUDE_DIR}/limes_music" CACHE INTERNAL "")

target_sources (
	limes_music PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/limes_music.h>
					   $<INSTALL_INTERFACE:${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/limes_music.h>)

install (FILES "${CMAKE_CURRENT_LIST_DIR}/limes_music.h"
		 DESTINATION "${LIMES_MUSIC_INSTALL_INCLUDE_DIR}" COMPONENT limes_music_dev)

target_include_directories (
	limes_music PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
					   $<INSTALL_INTERFACE:${LIMES_MUSIC_INSTALL_INCLUDE_DIR}>)

add_subdirectory (harmony)
add_subdirectory (rhythm)

install (
	TARGETS limes_music
	EXPORT LimesMusicTargets
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT limes_music_runtime
			NAMELINK_COMPONENT limes_music_dev
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT limes_music_dev
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT limes_music_runtime
	INCLUDES
	DESTINATION "${LIMES_MUSIC_INSTALL_INCLUDE_DIR}")

install (
	EXPORT LimesMusicTargets
	NAMESPACE Limes::
	DESTINATION "${LIMES_INSTALL_DEST}"
	EXPORT_LINK_INTERFACE_LIBRARIES
	COMPONENT limes_music_base)

oranges_create_pkgconfig_file (
	TARGET limes_music
	INCLUDE_REL_PATH "${LIMES_MUSIC_INSTALL_INCLUDE_DIR}"
	DESCRIPTION "${PROJECT_DESCRIPTION}"
	URL "${PROJECT_HOMEPAGE_URL}"
	VERSION "${PROJECT_VERSION}"
	INSTALL_COMPONENT limes_music_dev)

cpack_add_component (limes_music_base HIDDEN)

cpack_add_component (limes_music_runtime GROUP limes_music DEPENDS limes_music_base
					 INSTALL_TYPES User)

cpack_add_component (limes_music_dev GROUP limes_music DEPENDS limes_music_runtime
					 INSTALL_TYPES Developer)

cpack_add_component_group (
	limes_music DISPLAY_NAME "${PROJECT_DESCRIPTION}"
	DESCRIPTION "All components of the ${PROJECT_DESCRIPTION}" PARENT_GROUP limes)

if(NOT TARGET Limes::limes_music)
	add_library (Limes::limes_music ALIAS limes_music)
endif()

target_link_libraries (Limes INTERFACE Limes::limes_music)
