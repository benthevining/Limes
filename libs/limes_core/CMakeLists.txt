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

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

include (OrangesDefaultTarget)
include (OrangesUnityBuild)
include (OrangesExportHeader)

add_library (limes_core STATIC)

set_target_properties (limes_core PROPERTIES VERSION "${PROJECT_VERSION}"
											 SOVERSION "${PROJECT_VERSION_MAJOR}")

target_link_libraries (limes_core PUBLIC Limes::limes_vecops Oranges::OrangesDefaultTarget
										 Oranges::OrangesUnityBuild)

oranges_generate_export_header (
	TARGET
	limes_core
	BASE_NAME
	limes
	HEADER
	limes_export.h
	REL_PATH
	Limes/limes_core)

oranges_add_target_headers (
	TARGET
	limes_core
	SCOPE
	PUBLIC
	REL_PATH
	Limes/limes_core
	FILES
	limes_core.h)

target_include_directories (
	limes_core INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
						 $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Limes/limes_core>)

add_subdirectory ("${CMAKE_CURRENT_LIST_DIR}/data_structures"
				  "${CMAKE_CURRENT_BINARY_DIR}/limes_core/data_structures")
add_subdirectory ("${CMAKE_CURRENT_LIST_DIR}/math" "${CMAKE_CURRENT_BINARY_DIR}/limes_core/math")
add_subdirectory ("${CMAKE_CURRENT_LIST_DIR}/misc" "${CMAKE_CURRENT_BINARY_DIR}/limes_core/misc")
add_subdirectory ("${CMAKE_CURRENT_LIST_DIR}/threading"
				  "${CMAKE_CURRENT_BINARY_DIR}/limes_core/threading")
add_subdirectory ("${CMAKE_CURRENT_LIST_DIR}/text" "${CMAKE_CURRENT_BINARY_DIR}/limes_core/text")

lemons_sort_target_sources (limes_core "${CMAKE_CURRENT_LIST_DIR}")

oranges_export_alias_target (limes_core Limes)

target_link_libraries (Limes INTERFACE Limes::limes_core)

oranges_install_targets (
	TARGETS
	limes_core
	EXPORT
	LimesTargets
	REL_PATH
	Limes/limes_core
	COMPONENT_PREFIX
	limes_core)
