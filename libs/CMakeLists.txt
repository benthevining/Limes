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

Limes
-------------------------

The Limes library.

Targets
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- Limes::Limes

Install components
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- limes_dev
- limes_runtime

Install component groups
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- limes

#]=======================================================================]

include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

include (CPackComponent)
include (GNUInstallDirs)

add_library (Limes INTERFACE)

set_target_properties (Limes PROPERTIES VERSION "${PROJECT_VERSION}" SOVERSION
																	 "${PROJECT_VERSION_MAJOR}")

set (LIMES_INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_INCLUDEDIR}/Limes" CACHE INTERNAL "")

add_subdirectory (limes_core)
add_subdirectory (limes_vecops)
add_subdirectory (limes_audio)
add_subdirectory (limes_music)

include (LimesConfigVersionHeader)

limes_config_version_header (
	FILENAME
	LimesVersion.h
	NAMESPACE
	limes
	FUNCTION_NAME
	getLimesVersion
	TARGET
	Limes
	SCOPE
	INTERFACE
	REL_PATH
	Limes/generated)

target_sources (Limes INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/limes.h>
								$<INSTALL_INTERFACE:${LIMES_INSTALL_INCLUDE_DIR}/limes.h>)

install (FILES "${CMAKE_CURRENT_LIST_DIR}/limes.h" DESTINATION "${LIMES_INSTALL_INCLUDE_DIR}"
		 COMPONENT limes_dev)

target_include_directories (Limes INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
											$<INSTALL_INTERFACE:${LIMES_INSTALL_INCLUDE_DIR}>)

install (
	TARGETS Limes
	EXPORT LimesTargets
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT limes_runtime
			NAMELINK_COMPONENT limes_dev
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT limes_dev
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT limes_runtime
	INCLUDES
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

install (
	EXPORT LimesTargets
	NAMESPACE Limes::
	DESTINATION "${LIMES_INSTALL_DEST}"
	EXPORT_LINK_INTERFACE_LIBRARIES
	COMPONENT limes_runtime)

export (EXPORT LimesTargets NAMESPACE Limes:: FILE LimesAudioTargets.cmake)

if(NOT TARGET Limes::Limes)
	add_library (Limes::Limes ALIAS Limes)
endif()
