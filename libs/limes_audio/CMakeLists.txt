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

limes_audio
-------------------------

The Limes library of utilities for working with realtime audio.

Dependencies
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- limes_data_structures

Targets
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- Limes::limes_audio

Install components
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- limes_audio_dev
- limes_audio_runtime

Install component groups
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- limes_audio

#]=======================================================================]

include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

include (OrangesDefaultTarget)
include (OrangesUnityBuild)
include (OrangesGeneratePkgConfig)
include (CPackComponent)
include (GNUInstallDirs)

add_library (limes_audio STATIC)

set_target_properties (limes_audio PROPERTIES VERSION "${PROJECT_VERSION}"
											  SOVERSION "${PROJECT_VERSION_MAJOR}")

target_link_libraries (limes_audio PUBLIC Limes::limes_data_structures
					   PRIVATE Oranges::OrangesDefaultTarget Oranges::OrangesUnityBuild)

set (LIMES_AUDIO_INSTALL_INCLUDE_DIR "${LIMES_INSTALL_INCLUDE_DIR}/limes_audio" CACHE INTERNAL "")

target_sources (
	limes_audio PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/limes_audio.h>
					   $<INSTALL_INTERFACE:${LIMES_AUDIO_INSTALL_INCLUDE_DIR}/limes_audio.h>)

install (FILES "${CMAKE_CURRENT_LIST_DIR}/limes_audio.h"
		 DESTINATION "${LIMES_AUDIO_INSTALL_INCLUDE_DIR}" COMPONENT limes_audio_dev)

target_include_directories (
	limes_audio PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
					   $<INSTALL_INTERFACE:${LIMES_AUDIO_INSTALL_INCLUDE_DIR}>)

add_subdirectory (streams)
add_subdirectory (util)
add_subdirectory (psola)

install (
	TARGETS limes_audio
	EXPORT LimesAudioTargets
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT limes_audio_runtime
			NAMELINK_COMPONENT limes_audio_dev
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT limes_audio_dev
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT limes_audio_runtime
	INCLUDES
	DESTINATION "${LIMES_AUDIO_INSTALL_INCLUDE_DIR}")

install (
	EXPORT LimesAudioTargets
	NAMESPACE Limes::
	DESTINATION "${LIMES_INSTALL_DEST}"
	EXPORT_LINK_INTERFACE_LIBRARIES
	COMPONENT limes_audio_base)

oranges_create_pkgconfig_file (
	TARGET limes_audio
	INCLUDE_REL_PATH "${LIMES_AUDIO_INSTALL_INCLUDE_DIR}"
	DESCRIPTION "Limes realtime audio utilities library"
	URL "${PROJECT_HOMEPAGE_URL}"
	VERSION "${PROJECT_VERSION}"
	INSTALL_COMPONENT limes_audio_dev)

cpack_add_component (limes_audio_base HIDDEN)

cpack_add_component (limes_audio_runtime GROUP limes_audio DEPENDS limes_audio_base
					 INSTALL_TYPES User)

cpack_add_component (limes_audio_dev GROUP limes_audio DEPENDS limes_audio_runtime
					 INSTALL_TYPES Developer)

cpack_add_component_group (
	limes_audio DISPLAY_NAME "Limes audio library"
	DESCRIPTION "All components of the Limes audio utilities library" PARENT_GROUP limes)

if(NOT TARGET Limes::limes_audio)
	add_library (Limes::limes_audio ALIAS limes_audio)
endif()

target_link_libraries (Limes INTERFACE Limes::limes_audio)
