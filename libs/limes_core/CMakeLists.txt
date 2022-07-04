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
	LimesCore
	VERSION "${Limes_VERSION}"
	DESCRIPTION "Limes core utilities library"
	HOMEPAGE_URL "${Limes_HOMEPAGE_URL}"
	LANGUAGES CXX)

add_library (limes_core)

set_target_properties (
	limes_core
	PROPERTIES VERSION "${PROJECT_VERSION}"
			   SOVERSION "${PROJECT_VERSION_MAJOR}"
			   LABELS Limes
			   FOLDER Limes/libs
			   EchoString "Building the limes_core library...")

target_link_libraries (limes_core PUBLIC LimesDefaultLibrary)

include (OrangesGenerateStandardHeaders)

oranges_generate_standard_headers (
	limes_core
	BASE_NAME limes
	NO_AGGREGATE_HEADER
	EXPORT_HEADER limes_export.h
	PLATFORM_HEADER limes_platform.h
	INSTALL_COMPONENT limes_core_dev
	SCOPE PUBLIC
	REL_PATH Limes/limes_core SOURCE_GROUP_NAME Generated)

include (LimesConfigVersionHeader)

limes_config_version_header (
	FILENAME limes_version.h
	NAMESPACE limes
	FUNCTION_NAME getLimesVersion
	TARGET limes_core
	SCOPE PUBLIC
	REL_PATH Limes/limes_core)

# source_group (TREE "${CMAKE_CURRENT_BINARY_DIR}" FILES
# "${CMAKE_CURRENT_BINARY_DIR}/limes_version.h" PREFIX Generated)

#

set (LIMES_CORE_INSTALL_INCLUDE_DIR "${LIMES_INSTALL_INCLUDE_DIR}/limes_core")

target_sources (
	limes_core
	PUBLIC # cmake-format: sortable
		   $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/limes_core.h>
		   $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/limes_namespace.h>
		   $<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/limes_core.h>
		   $<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/limes_namespace.h>)

install (FILES limes_core.h limes_core limes_namespace.h
		 DESTINATION "${LIMES_CORE_INSTALL_INCLUDE_DIR}" COMPONENT limes_core_dev)

target_include_directories (
	limes_core PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
					  $<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}>)

add_subdirectory (data_structures)
add_subdirectory (files)
add_subdirectory (hashes)
add_subdirectory (locale)
add_subdirectory (math)
add_subdirectory (memory)
add_subdirectory (meta)
add_subdirectory (misc)
add_subdirectory (serializing)
add_subdirectory (system)
add_subdirectory (text)
add_subdirectory (threading)
add_subdirectory (time)

set (
	limes_core_sources
	# cmake-format: sortable
	${data_structures_files}
	${files_files}
	${hashes_files}
	${locale_files}
	${math_files}
	${memory_files}
	${meta_files}
	${misc_files}
	${serializing_files}
	${system_files}
	${text_files}
	${threading_files}
	${time_files}
	limes_core.h
	limes_namespace.h)

source_group (TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${limes_core_sources})

list (TRANSFORM limes_core_sources
	  PREPEND "${CMAKE_INSTALL_PREFIX}/${LIMES_CORE_INSTALL_INCLUDE_DIR}/")

source_group (TREE "${CMAKE_INSTALL_PREFIX}/${LIMES_CORE_INSTALL_INCLUDE_DIR}"
			  FILES ${limes_core_sources})

#

# cmake-format: off
install (
	TARGETS limes_core
	EXPORT LimesCoreTargets
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT limes_core_runtime
	NAMELINK_COMPONENT limes_core_dev
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT limes_core_dev
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT limes_core_runtime
	INCLUDES DESTINATION "${LIMES_CORE_INSTALL_INCLUDE_DIR}")
# cmake-format: on

install (
	EXPORT LimesCoreTargets
	NAMESPACE Limes::
	DESTINATION "${LIMES_INSTALL_DEST}"
	EXPORT_LINK_INTERFACE_LIBRARIES
	COMPONENT limes_core_base)

export (EXPORT LimesCoreTargets FILE "${LIMES_EXPORT_DIR}/LimesCoreTargets.cmake" NAMESPACE Limes::)

# oranges_create_pkgconfig_file ( TARGET limes_core INCLUDE_REL_PATH
# "${LIMES_CORE_INSTALL_INCLUDE_DIR}" DESCRIPTION "${PROJECT_DESCRIPTION}" URL
# "${PROJECT_HOMEPAGE_URL}" VERSION "${PROJECT_VERSION}" INSTALL_COMPONENT limes_core_dev)

cpack_add_component (limes_core_base HIDDEN DEPENDS limes_libs_base)

cpack_add_component (limes_core_runtime GROUP limes_core DEPENDS limes_core_base INSTALL_TYPES User)

cpack_add_component (limes_core_dev GROUP limes_core DEPENDS limes_core_runtime
					 INSTALL_TYPES Developer)

cpack_add_component_group (
	limes_core DISPLAY_NAME "${PROJECT_DESCRIPTION}"
	DESCRIPTION "All components of the ${PROJECT_DESCRIPTION}" PARENT_GROUP limes_libs)

add_library (Limes::limes_core ALIAS limes_core)
