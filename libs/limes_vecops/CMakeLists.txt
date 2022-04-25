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

limes_vecops
-------------------------

A vector operations library wrapping various platform and vendor specific high-performance APIs.

Dependencies
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- limes_core

Options
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- LIMES_IGNORE_VDSP
- LIMES_IGNORE_IPP
- LIMES_IGNORE_MIPP
- LIMES_USE_IPP
- LIMES_USE_MIPP
- LIMES_USE_VECOPS_FALLBACK
- LIMES_IGNORE_POMMIER
- LIMES_IGNORE_FFTW

Targets
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- Limes::limes_vecops

Target properties
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- VECOPS_IMPLEMENTATION : string name of the vecops implementation being used

Install components
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- limes_vecops_dev
- limes_vecops_runtime

Install component groups
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- limes_vecops

#]=======================================================================]

include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.22 FATAL_ERROR)

include (OrangesDefaultTarget)
include (OrangesUnityBuild)
include (OrangesGeneratePkgConfig)
include (CPackComponent)
include (GNUInstallDirs)

define_property (
	TARGET PROPERTY VECOPS_IMPLEMENTATION BRIEF_DOCS "Name of the vecops implementation"
	FULL_DOCS "Name of the underlying vecops implementation library used by limes_vecops")

project (
	LimesVecops
	VERSION "${Limes_VERSION}"
	DESCRIPTION "Limes vector operations library"
	HOMEPAGE_URL "${Limes_HOMEPAGE_URL}"
	LANGUAGES CXX ASM)

add_library (limes_vecops STATIC)

set_target_properties (limes_vecops PROPERTIES VERSION "${PROJECT_VERSION}"
											   SOVERSION "${PROJECT_VERSION_MAJOR}")

target_link_libraries (limes_vecops PUBLIC Limes::limes_core PRIVATE Oranges::OrangesDefaultTarget
																	 Oranges::OrangesUnityBuild)

if(CMAKE_SYSTEM_NAME STREQUAL watchOS)
	target_compile_options (limes_vecops PRIVATE -fgnu-inline-asm)
endif()

# fixes a bug present when compiling vDSP with GCC
target_compile_options (limes_vecops PUBLIC $<$<CXX_COMPILER_ID:GNU>:-flax-vector-conversions>)

set (LIMES_VECOPS_INSTALL_INCLUDE_DIR "${LIMES_INSTALL_INCLUDE_DIR}/limes_vecops" CACHE INTERNAL "")

target_sources (
	limes_vecops PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/limes_vecops.h>
						$<INSTALL_INTERFACE:${LIMES_VECOPS_INSTALL_INCLUDE_DIR}/limes_vecops.h>)

install (FILES limes_vecops.h DESTINATION "${LIMES_VECOPS_INSTALL_INCLUDE_DIR}"
		 COMPONENT limes_vecops_dev)

target_include_directories (
	limes_vecops PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
						$<INSTALL_INTERFACE:${LIMES_VECOPS_INSTALL_INCLUDE_DIR}>)

add_subdirectory (impl)
add_subdirectory (fft)

install (
	TARGETS limes_vecops
	EXPORT LimesVecopsTargets
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT limes_vecops_runtime
			NAMELINK_COMPONENT limes_vecops_dev
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT limes_vecops_dev
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT limes_vecops_runtime
	INCLUDES
	DESTINATION "${LIMES_VECOPS_INSTALL_INCLUDE_DIR}")

install (
	EXPORT LimesVecopsTargets
	NAMESPACE Limes::
	DESTINATION "${LIMES_INSTALL_DEST}"
	EXPORT_LINK_INTERFACE_LIBRARIES
	COMPONENT limes_vecops_base)

oranges_create_pkgconfig_file (
	TARGET limes_vecops
	INCLUDE_REL_PATH "${LIMES_VECOPS_INSTALL_INCLUDE_DIR}"
	DESCRIPTION "${PROJECT_DESCRIPTION}"
	URL "${PROJECT_HOMEPAGE_URL}"
	VERSION "${PROJECT_VERSION}"
	INSTALL_COMPONENT limes_vecops_dev)

cpack_add_component (limes_vecops_base HIDDEN)

cpack_add_component (limes_vecops_runtime GROUP limes_vecops DEPENDS limes_vecops_base
					 INSTALL_TYPES User)

cpack_add_component (limes_vecops_dev GROUP limes_vecops DEPENDS limes_vecops_runtime
					 INSTALL_TYPES Developer)

cpack_add_component_group (
	limes_vecops DISPLAY_NAME "${PROJECT_DESCRIPTION}"
	DESCRIPTION "All components of the ${PROJECT_DESCRIPTION}" PARENT_GROUP limes)

add_library (Limes::limes_vecops ALIAS limes_vecops)

target_link_libraries (Limes INTERFACE Limes::limes_vecops)

if(Limes_IS_TOP_LEVEL)
	include (OrangesDefaultWarnings)

	target_link_libraries (limes_vecops PRIVATE $<BUILD_INTERFACE:Oranges::OrangesDefaultWarnings>)
endif()
