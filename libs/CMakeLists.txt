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

include (OrangesGeneratePkgConfig)
include (CPackComponent)
include (GNUInstallDirs)

add_library (Limes INTERFACE)

set_target_properties (Limes PROPERTIES VERSION "${Limes_VERSION}" SOVERSION
																   "${Limes_VERSION_MAJOR}")

set (LIMES_INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_INCLUDEDIR}/Limes" CACHE INTERNAL "")

add_subdirectory (limes_core)
add_subdirectory (limes_vecops)
add_subdirectory (limes_data_structures)
add_subdirectory (limes_locale)
add_subdirectory (limes_midi)
add_subdirectory (limes_audio)
add_subdirectory (limes_music)

target_sources (Limes INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/limes.h>
								$<INSTALL_INTERFACE:${LIMES_INSTALL_INCLUDE_DIR}/limes.h>)

install (FILES limes.h DESTINATION "${LIMES_INSTALL_INCLUDE_DIR}" COMPONENT limes_dev)

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
	COMPONENT limes_base)

oranges_create_pkgconfig_file (
	TARGET Limes
	INCLUDE_REL_PATH "${LIMES_INSTALL_INCLUDE_DIR}"
	DESCRIPTION "${Limes_DESCRIPTION}"
	URL "${Limes_HOMEPAGE_URL}"
	VERSION "${Limes_VERSION}"
	INSTALL_COMPONENT limes_dev)

add_library (Limes::Limes ALIAS Limes)
