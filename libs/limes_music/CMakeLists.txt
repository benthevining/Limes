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
- limes_core

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

cmake_minimum_required (VERSION 3.22 FATAL_ERROR)

project (
	LimesMusic
	VERSION "${Limes_VERSION}"
	DESCRIPTION "Limes music theory library"
	HOMEPAGE_URL "${Limes_HOMEPAGE_URL}"
	LANGUAGES CXX)

add_library (limes_music)

set_target_properties (
	limes_music
	PROPERTIES VERSION "${PROJECT_VERSION}"
			   SOVERSION "${PROJECT_VERSION_MAJOR}"
			   LABELS Limes
			   FOLDER Limes/libs
			   EchoString "Building the limes_music library...")

target_link_libraries (limes_music PUBLIC Limes::limes_core)

set (LIMES_MUSIC_INSTALL_INCLUDE_DIR "${LIMES_INSTALL_INCLUDE_DIR}/limes_music")

target_sources (
	limes_music PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/limes_music.h>
					   $<INSTALL_INTERFACE:${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/limes_music.h>)

install (FILES limes_music.h DESTINATION "${LIMES_MUSIC_INSTALL_INCLUDE_DIR}"
		 COMPONENT limes_music_dev)

target_include_directories (
	limes_music PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
					   $<INSTALL_INTERFACE:${LIMES_MUSIC_INSTALL_INCLUDE_DIR}>)

add_subdirectory (harmony)
add_subdirectory (rhythm)

set (limes_music_sources limes_music.h ${harmony_files} ${rhythm_files})

source_group (TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${limes_music_sources})

list (TRANSFORM limes_music_sources
	  PREPEND "${CMAKE_INSTALL_PREFIX}/${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/")

source_group (TREE "${CMAKE_INSTALL_PREFIX}/${LIMES_MUSIC_INSTALL_INCLUDE_DIR}"
			  FILES ${limes_music_sources})

# cmake-format: off
install (
	TARGETS limes_music
	EXPORT LimesMusicTargets
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT limes_music_runtime
	NAMELINK_COMPONENT limes_music_dev
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT limes_music_dev
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT limes_music_runtime
	INCLUDES DESTINATION "${LIMES_MUSIC_INSTALL_INCLUDE_DIR}")
# cmake-format: on

install (
	EXPORT LimesMusicTargets
	NAMESPACE Limes::
	DESTINATION "${LIMES_INSTALL_DEST}"
	EXPORT_LINK_INTERFACE_LIBRARIES
	COMPONENT limes_music_base)

export (EXPORT LimesMusicTargets FILE "${LIMES_EXPORT_DIR}/LimesMusicTargets.cmake"
		NAMESPACE Limes::)

# oranges_create_pkgconfig_file ( TARGET limes_music INCLUDE_REL_PATH
# "${LIMES_MUSIC_INSTALL_INCLUDE_DIR}" DESCRIPTION "${PROJECT_DESCRIPTION}" URL
# "${PROJECT_HOMEPAGE_URL}" VERSION "${PROJECT_VERSION}" INSTALL_COMPONENT limes_music_dev)

cpack_add_component (limes_music_base HIDDEN DEPENDS limes_libs_base)

cpack_add_component (limes_music_runtime GROUP limes_music DEPENDS limes_music_base
					 INSTALL_TYPES User)

cpack_add_component (limes_music_dev GROUP limes_music DEPENDS limes_music_runtime
					 INSTALL_TYPES Developer)

cpack_add_component_group (
	limes_music DISPLAY_NAME "${PROJECT_DESCRIPTION}"
	DESCRIPTION "All components of the ${PROJECT_DESCRIPTION}" PARENT_GROUP limes_libs)

add_library (Limes::limes_music ALIAS limes_music)
