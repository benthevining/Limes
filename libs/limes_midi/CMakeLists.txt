# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.22 FATAL_ERROR)

project (
	LimesMIDI
	VERSION "${Limes_VERSION}"
	DESCRIPTION "Limes MIDI library"
	HOMEPAGE_URL "${Limes_HOMEPAGE_URL}"
	LANGUAGES CXX)

add_library (limes_midi)

set_target_properties (
	limes_midi
	PROPERTIES VERSION "${PROJECT_VERSION}"
			   SOVERSION "${PROJECT_VERSION_MAJOR}"
			   LABELS Limes
			   FOLDER Limes/libs
			   EchoString "Building the limes_midi library...")

target_link_libraries (limes_midi PUBLIC Limes::limes_core)

set (LIMES_MIDI_INSTALL_INCLUDE_DIR "${LIMES_INSTALL_INCLUDE_DIR}/limes_midi")

target_sources (
	limes_midi PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/limes_midi.h>
					  $<INSTALL_INTERFACE:${LIMES_MIDI_INSTALL_INCLUDE_DIR}/limes_midi.h>)

install (FILES limes_midi.h limes_midi DESTINATION "${LIMES_MIDI_INSTALL_INCLUDE_DIR}"
		 COMPONENT limes_midi_dev)

target_include_directories (
	limes_midi PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
					  $<INSTALL_INTERFACE:${LIMES_MIDI_INSTALL_INCLUDE_DIR}>)

add_subdirectory (utilities)
# add_subdirectory (tuning)

set (limes_midi_sources limes_midi.h ${utilities_files} ${tuning_files})

source_group (TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${limes_midi_sources})

list (TRANSFORM limes_midi_sources
	  PREPEND "${CMAKE_INSTALL_PREFIX}/${LIMES_MIDI_INSTALL_INCLUDE_DIR}/")

source_group (TREE "${CMAKE_INSTALL_PREFIX}/${LIMES_MIDI_INSTALL_INCLUDE_DIR}"
			  FILES ${limes_midi_sources})

# cmake-format: off
install (
	TARGETS limes_midi
	EXPORT LimesMIDITargets
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT limes_midi_runtime
	NAMELINK_COMPONENT limes_midi_dev
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT limes_midi_dev
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT limes_midi_runtime
	INCLUDES DESTINATION "${LIMES_MIDI_INSTALL_INCLUDE_DIR}")
# cmake-format: on

install (
	EXPORT LimesMIDITargets
	NAMESPACE Limes::
	DESTINATION "${LIMES_INSTALL_DEST}"
	EXPORT_LINK_INTERFACE_LIBRARIES
	COMPONENT limes_midi_base)

export (EXPORT LimesMIDITargets FILE "${LIMES_EXPORT_DIR}/LimesMIDITargets.cmake" NAMESPACE Limes::)

# oranges_create_pkgconfig_file ( TARGET limes_midi INCLUDE_REL_PATH
# "${LIMES_MIDI_INSTALL_INCLUDE_DIR}" DESCRIPTION "${PROJECT_DESCRIPTION}" URL
# "${PROJECT_HOMEPAGE_URL}" VERSION "${PROJECT_VERSION}" INSTALL_COMPONENT limes_midi_dev)

cpack_add_component (limes_midi_base HIDDEN DEPENDS limes_libs_base)

cpack_add_component (limes_midi_runtime GROUP limes_midi DEPENDS limes_midi_base INSTALL_TYPES User)

cpack_add_component (limes_midi_dev GROUP limes_midi DEPENDS limes_midi_runtime
					 INSTALL_TYPES Developer)

cpack_add_component_group (
	limes_midi DISPLAY_NAME "${PROJECT_DESCRIPTION}"
	DESCRIPTION "All components of the ${PROJECT_DESCRIPTION}" PARENT_GROUP limes_libs)

add_library (Limes::limes_midi ALIAS limes_midi)
