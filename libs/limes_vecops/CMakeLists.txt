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
	LimesVecops
	VERSION "${Limes_VERSION}"
	DESCRIPTION "Limes vector operations library"
	HOMEPAGE_URL "${Limes_HOMEPAGE_URL}"
	LANGUAGES CXX ASM)

list (APPEND CMAKE_MESSAGE_INDENT "limes_vecops - ")

add_library (limes_vecops)

include (SelectImpl.cmake)

set_target_properties (
	limes_vecops
	PROPERTIES VERSION "${PROJECT_VERSION}"
			   SOVERSION "${PROJECT_VERSION_MAJOR}"
			   LABELS Limes
			   FOLDER Limes/libs
			   EchoString "Building the limes_vecops library...")

target_link_libraries (limes_vecops PUBLIC Limes::limes_core)

# cmake-format: off
target_compile_options (
	limes_vecops
	PRIVATE
		"$<$<AND:$<PLATFORM_ID:watchOS>,$<CXX_COMPILER_ID:Clang,AppleClang>>:-fgnu-inline-asm>"
	PUBLIC
		"$<$<CXX_COMPILER_ID:GNU>:-flax-vector-conversions>")
# cmake-format: on

set (LIMES_VECOPS_INSTALL_INCLUDE_DIR "${LIMES_INSTALL_INCLUDE_DIR}/limes_vecops")

target_sources (
	limes_vecops PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/limes_vecops.h>
						$<INSTALL_INTERFACE:${LIMES_VECOPS_INSTALL_INCLUDE_DIR}/limes_vecops.h>)

install (FILES limes_vecops.h limes_vecops DESTINATION "${LIMES_VECOPS_INSTALL_INCLUDE_DIR}"
		 COMPONENT limes_vecops_dev)

target_include_directories (
	limes_vecops PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
						$<INSTALL_INTERFACE:${LIMES_VECOPS_INSTALL_INCLUDE_DIR}>)

add_subdirectory (vecops)
add_subdirectory (fft)
add_subdirectory (resampling)
# add_subdirectory (filters)

set (limes_vecops_sources
	 # cmake-format: sortable
	 ${fft_files} ${filters_files} ${resampling_files} ${vecops_files} limes_vecops.h)

source_group (TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${limes_vecops_sources})

list (TRANSFORM limes_vecops_sources
	  PREPEND "${CMAKE_INSTALL_PREFIX}/${LIMES_VECOPS_INSTALL_INCLUDE_DIR}/")

source_group (TREE "${CMAKE_INSTALL_PREFIX}/${LIMES_VECOPS_INSTALL_INCLUDE_DIR}"
			  FILES ${limes_vecops_sources})

# cmake-format: off
install (
	TARGETS limes_vecops
	EXPORT LimesVecopsTargets
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT limes_vecops_runtime
	NAMELINK_COMPONENT limes_vecops_dev
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT limes_vecops_dev
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT limes_vecops_runtime
	INCLUDES DESTINATION "${LIMES_VECOPS_INSTALL_INCLUDE_DIR}")
# cmake-format: on

install (
	EXPORT LimesVecopsTargets
	NAMESPACE Limes::
	DESTINATION "${LIMES_INSTALL_DEST}"
	EXPORT_LINK_INTERFACE_LIBRARIES
	COMPONENT limes_vecops_base)

export (EXPORT LimesVecopsTargets FILE "${LIMES_EXPORT_DIR}/LimesVecopsTargets.cmake"
		NAMESPACE Limes::)

# oranges_create_pkgconfig_file ( TARGET limes_vecops INCLUDE_REL_PATH
# "${LIMES_VECOPS_INSTALL_INCLUDE_DIR}" DESCRIPTION "${PROJECT_DESCRIPTION}" URL
# "${PROJECT_HOMEPAGE_URL}" VERSION "${PROJECT_VERSION}" INSTALL_COMPONENT limes_vecops_dev)

cpack_add_component (limes_vecops_base HIDDEN DEPENDS limes_libs_base)

cpack_add_component (limes_vecops_runtime GROUP limes_vecops DEPENDS limes_vecops_base
					 INSTALL_TYPES User)

cpack_add_component (limes_vecops_dev GROUP limes_vecops DEPENDS limes_vecops_runtime
					 INSTALL_TYPES Developer)

cpack_add_component_group (
	limes_vecops DISPLAY_NAME "${PROJECT_DESCRIPTION}"
	DESCRIPTION "All components of the ${PROJECT_DESCRIPTION}" PARENT_GROUP limes_libs)

add_library (Limes::limes_vecops ALIAS limes_vecops)

list (POP_BACK CMAKE_MESSAGE_INDENT)
