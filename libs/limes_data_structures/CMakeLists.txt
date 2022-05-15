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

limes_data_structures
-------------------------

The Limes library of data structures.

Dependencies
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- limes_vecops
	- limes_core

Targets
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- Limes::limes_data_structures

Install components
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- limes_data_structures_dev
- limes_data_structures_runtime

Install component groups
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- limes_data_structures

#]=======================================================================]

include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

if (TARGET Limes::limes_data_structures)
	message (WARNING "Target Limes::limes_data_structures already exists!")
	return ()
endif ()

project (
	LimesDataStructures
	VERSION "${Limes_VERSION}"
	DESCRIPTION "Limes data structures library"
	HOMEPAGE_URL "${Limes_HOMEPAGE_URL}"
	LANGUAGES CXX)

add_library (limes_data_structures STATIC)

set_target_properties (
	limes_data_structures
	PROPERTIES VERSION "${PROJECT_VERSION}"
			   SOVERSION "${PROJECT_VERSION_MAJOR}"
			   LABELS Limes
			   FOLDER Limes/libs
			   EchoString "Building the limes_data_structures library...")

target_link_libraries (limes_data_structures PUBLIC Limes::limes_vecops PRIVATE LimesDefaultLibrary)

set (LIMES_DATA_STRUCTURES_INSTALL_INCLUDE_DIR "${LIMES_INSTALL_INCLUDE_DIR}/limes_data_structures")

target_sources (
	limes_data_structures
	PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/limes_data_structures.h>
		   $<INSTALL_INTERFACE:${LIMES_DATA_STRUCTURES_INSTALL_INCLUDE_DIR}/limes_data_structures.h>
	)

install (FILES limes_data_structures.h DESTINATION "${LIMES_DATA_STRUCTURES_INSTALL_INCLUDE_DIR}"
		 COMPONENT limes_data_structures_dev)

target_include_directories (
	limes_data_structures PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
								 $<INSTALL_INTERFACE:${LIMES_DATA_STRUCTURES_INSTALL_INCLUDE_DIR}>)

add_subdirectory (data_structures)
add_subdirectory (fifo)
add_subdirectory (linked_list)
add_subdirectory (vectors)

set (limes_data_structures_sources limes_data_structures.h ${data_structures_files} ${fifo_files}
								   ${linked_list_files} ${vectors_files})

source_group (TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${limes_data_structures_sources})

list (TRANSFORM limes_data_structures_sources
	  PREPEND "${CMAKE_INSTALL_PREFIX}/${LIMES_DATA_STRUCTURES_INSTALL_INCLUDE_DIR}/")

source_group (TREE "${CMAKE_INSTALL_PREFIX}/${LIMES_DATA_STRUCTURES_INSTALL_INCLUDE_DIR}"
			  FILES ${limes_data_structures_sources})

# cmake-format: off
install (
	TARGETS limes_data_structures
	EXPORT LimesDataStructuresTargets
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT limes_data_structures_runtime
	NAMELINK_COMPONENT limes_data_structures_dev
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT limes_data_structures_dev
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT limes_data_structures_runtime
	INCLUDES DESTINATION "${LIMES_DATA_STRUCTURES_INSTALL_INCLUDE_DIR}")
# cmake-format: on

install (
	EXPORT LimesDataStructuresTargets
	NAMESPACE Limes::
	DESTINATION "${LIMES_INSTALL_DEST}"
	EXPORT_LINK_INTERFACE_LIBRARIES
	COMPONENT limes_data_structures_base)

oranges_create_pkgconfig_file (
	TARGET limes_data_structures
	INCLUDE_REL_PATH "${LIMES_DATA_STRUCTURES_INSTALL_INCLUDE_DIR}"
	DESCRIPTION "${PROJECT_DESCRIPTION}"
	URL "${PROJECT_HOMEPAGE_URL}"
	VERSION "${PROJECT_VERSION}"
	INSTALL_COMPONENT limes_data_structures_dev)

cpack_add_component (limes_data_structures_base HIDDEN DEPENDS limes_libs_base)

cpack_add_component (limes_data_structures_runtime GROUP limes_data_structures
					 DEPENDS limes_data_structures_base INSTALL_TYPES User)

cpack_add_component (limes_data_structures_dev GROUP limes_data_structures
					 DEPENDS limes_data_structures_runtime INSTALL_TYPES Developer)

cpack_add_component_group (
	limes_data_structures DISPLAY_NAME "${PROJECT_DESCRIPTION}"
	DESCRIPTION "All components of the ${PROJECT_DESCRIPTION}" PARENT_GROUP limes_libs)

add_library (Limes::limes_data_structures ALIAS limes_data_structures)
