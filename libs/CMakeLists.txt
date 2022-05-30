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
- limes_libs_dev
- limes_libs_runtime

Install component groups
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- limes_libs

#]=======================================================================]

include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.22 FATAL_ERROR)

project (
	LimesLibraries
	VERSION "${Limes_VERSION}"
	DESCRIPTION "C++ utility libraries"
	HOMEPAGE_URL "${Limes_HOMEPAGE_URL}"
	LANGUAGES CXX ASM)

add_library (LimesDefaultLibrary INTERFACE)

set_target_properties (
	LimesDefaultLibrary PROPERTIES VERSION "${Limes_VERSION}" SOVERSION "${Limes_VERSION_MAJOR}"
								   LABELS Limes FOLDER Limes/libs)

target_link_libraries (
	LimesDefaultLibrary
	INTERFACE Oranges::OrangesDefaultCXXTarget $<BUILD_INTERFACE:Oranges::OrangesIPO>
			  $<BUILD_INTERFACE:Oranges::OrangesUnityBuild>)

# cmake-format: off
install (
	TARGETS LimesDefaultLibrary
	EXPORT LimesDefaultLibraryTarget
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT limes_libs_base
	NAMELINK_COMPONENT limes_libs_base
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT limes_libs_base
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT limes_libs_base
	INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
# cmake-format: on

install (
	EXPORT LimesDefaultLibraryTarget
	NAMESPACE Limes::
	DESTINATION "${LIMES_INSTALL_DEST}"
	EXPORT_LINK_INTERFACE_LIBRARIES
	COMPONENT limes_libs_base)

cpack_add_component (limes_libs_base HIDDEN)

add_library (Limes INTERFACE)

set_target_properties (
	Limes PROPERTIES VERSION "${Limes_VERSION}" SOVERSION "${Limes_VERSION_MAJOR}" LABELS Limes)

set (LIMES_INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_INCLUDEDIR}/Limes")

include (OrangesGeneratePkgConfig)
include (OrangesSWIG)

add_subdirectory (limes_core)
add_subdirectory (limes_vecops)
add_subdirectory (limes_data_structures)
add_subdirectory (limes_locale)
add_subdirectory (limes_midi)
add_subdirectory (limes_audio)
add_subdirectory (limes_music)

target_link_libraries (
	Limes
	INTERFACE Limes::limes_core
			  Limes::limes_audio
			  Limes::limes_data_structures
			  Limes::limes_locale
			  Limes::limes_midi
			  Limes::limes_music
			  Limes::limes_vecops)

target_sources (Limes INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/limes.h>
								$<INSTALL_INTERFACE:${LIMES_INSTALL_INCLUDE_DIR}/limes.h>)

install (FILES limes.h DESTINATION "${LIMES_INSTALL_INCLUDE_DIR}" COMPONENT limes_libs_dev)

target_include_directories (Limes INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/limes.h>
											$<INSTALL_INTERFACE:${LIMES_INSTALL_INCLUDE_DIR}>)

# cmake-format: off
install (
	TARGETS Limes
	EXPORT LimesLibraryTargets
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT limes_libs_runtime
	NAMELINK_COMPONENT limes_libs_dev
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT limes_libs_dev
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT limes_libs_runtime
	INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
# cmake-format: on

install (
	EXPORT LimesLibraryTargets
	NAMESPACE Limes::
	DESTINATION "${LIMES_INSTALL_DEST}"
	EXPORT_LINK_INTERFACE_LIBRARIES
	COMPONENT limes_libs_base)

# oranges_create_pkgconfig_file ( TARGET Limes INCLUDE_REL_PATH "${LIMES_INSTALL_INCLUDE_DIR}"
# DESCRIPTION "${Limes_DESCRIPTION}" URL "${Limes_HOMEPAGE_URL}" VERSION "${Limes_VERSION}"
# INSTALL_COMPONENT limes_libs_dev)

cpack_add_component (limes_libs_base HIDDEN)

cpack_add_component (
	limes_libs_runtime
	GROUP limes_libs
	DEPENDS # cmake-format: sortable
			limes_libs_base limes_vecops_runtime limes_music_runtime limes_core_runtime
			limes_audio_runtime
	INSTALL_TYPES User)

cpack_add_component (
	limes_libs_dev
	GROUP limes_libs
	DEPENDS # cmake-format: sortable
			limes_libs_runtime limes_vecops_dev limes_music_dev limes_core_dev limes_audio_dev
	INSTALL_TYPES Developer)

cpack_add_component_group (
	limes_libs
	DISPLAY_NAME "Limes libraries"
	DESCRIPTION "All components of the Limes libraries"
	BOLD_TITLE
	PARENT_GROUP limes)

add_library (Limes::Limes ALIAS Limes)
