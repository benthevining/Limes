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
include (CPackComponent)
include (GNUInstallDirs)

add_library (limes_music STATIC)

set_target_properties (limes_music PROPERTIES VERSION "${PROJECT_VERSION}"
											  SOVERSION "${PROJECT_VERSION_MAJOR}")

target_link_libraries (limes_music PRIVATE Oranges::OrangesDefaultTarget Oranges::OrangesUnityBuild
										   Limes::limes_core)

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

lemons_sort_target_sources (limes_music "${CMAKE_CURRENT_LIST_DIR}")

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
	COMPONENT limes_music_runtime)

export (EXPORT LimesMusicTargets NAMESPACE Limes:: FILE LimesMusicTargets.cmake)

cpack_add_component (limes_music_runtime GROUP limes_music)

cpack_add_component (limes_music_dev GROUP limes_music DEPENDS limes_music_runtime)

cpack_add_component_group (
	limes_music DISPLAY_NAME "Limes music library"
	DESCRIPTION "All components of the Limes music theory utilities library" PARENT_GROUP limes)

if(NOT TARGET Limes::limes_music)
	add_library (Limes::limes_music ALIAS limes_music)
endif()

target_link_libraries (Limes INTERFACE Limes::limes_music)
