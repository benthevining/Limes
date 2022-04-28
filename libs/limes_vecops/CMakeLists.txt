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

Cache variables
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- LIMES_VECOPS_BACKEND=<vDSP|IPP|MIPP|Fallback>

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

if (TARGET Limes::limes_vecops)
	message (WARNING "Target Limes::limes_vecops already exists!")
	return ()
endif ()

define_property (
	GLOBAL
	PROPERTY LIMES_VECOPS_IMPLEMENTATION
	BRIEF_DOCS "Name of the vecops implementation"
	FULL_DOCS
		"Name of the underlying vecops implementation library used by limes_vecops. May be vDSP, IPP, MIPP, or Fallback."
	)

define_property (
	GLOBAL
	PROPERTY LIMES_FFT_IMPLEMENTATION
	BRIEF_DOCS "Name of the FFT implementation"
	FULL_DOCS
		"Name of the underlying FFT implementation library used by limes_vecops. May be vDSP, IPP, FFTW, or Fallback."
	)

if (LIMES_PROPERTIES_LIST_FILE)
	file (
		APPEND "${LIMES_PROPERTIES_LIST_FILE}"
		"LIMES_VECOPS_IMPLEMENTATION GLOBAL\nLIMES_FFT_IMPLEMENTATION GLOBAL\n")
endif ()

option (LIMES_IGNORE_VDSP "Ignore vDSP for vecops" OFF)
option (LIMES_IGNORE_IPP "Ignore Intel IPP for vecops" OFF)
option (LIMES_IGNORE_MIPP "Ignore MIPP for vecops" OFF)
option (LIMES_IGNORE_POMMIER "Ignore the Pommier SIMD extensions" OFF)
option (LIMES_IGNORE_FFTW "Ignore FFTW for limes_vecops" OFF)
option (LIMES_USE_VECOPS_FALLBACK "Use the vecops fallback implementation" OFF)

include (CMakeDependentOption)

cmake_dependent_option (LIMES_USE_IPP "Use IPP for vecops, if available" OFF
						"NOT LIMES_IGNORE_IPP" OFF)
cmake_dependent_option (LIMES_USE_MIPP "Use MIPP for vecops, if available" OFF
						"NOT LIMES_IGNORE_MIPP" OFF)

mark_as_advanced (
	FORCE
	LIMES_IGNORE_VDSP
	LIMES_IGNORE_IPP
	LIMES_IGNORE_MIPP
	LIMES_IGNORE_POMMIER
	LIMES_IGNORE_FFTW
	LIMES_USE_VECOPS_FALLBACK
	LIMES_USE_IPP
	LIMES_USE_MIPP)

#

project (
	LimesVecops
	VERSION "${Limes_VERSION}"
	DESCRIPTION "Limes vector operations library"
	HOMEPAGE_URL "${Limes_HOMEPAGE_URL}"
	LANGUAGES CXX ASM)

add_library (limes_vecops STATIC)

set_target_properties (
	limes_vecops
	PROPERTIES VERSION "${PROJECT_VERSION}"
			   SOVERSION "${PROJECT_VERSION_MAJOR}"
			   LABELS Limes
			   FOLDER Limes/libs
			   EchoString "Building the limes_vecops library...")

set_property (
	TARGET limes_vecops APPEND
	PROPERTY EXPORT_PROPERTIES LIMES_VECOPS_IMPLEMENTATION
			 LIMES_FFT_IMPLEMENTATION)

include (OrangesDefaultTarget)
include (OrangesUnityBuild)

target_link_libraries (
	limes_vecops PUBLIC Limes::limes_core
	PRIVATE Oranges::OrangesDefaultTarget
			$<BUILD_INTERFACE:Oranges::OrangesUnityBuild>)

if (CMAKE_SYSTEM_NAME STREQUAL watchOS)
	target_compile_options (limes_vecops PRIVATE -fgnu-inline-asm)
endif ()

# fixes a bug present when compiling vDSP with GCC
target_compile_options (
	limes_vecops PUBLIC $<$<CXX_COMPILER_ID:GNU>:-flax-vector-conversions>)

set (LIMES_VECOPS_INSTALL_INCLUDE_DIR
	 "${LIMES_INSTALL_INCLUDE_DIR}/limes_vecops")

target_sources (
	limes_vecops
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/limes_vecops.h>
		$<INSTALL_INTERFACE:${LIMES_VECOPS_INSTALL_INCLUDE_DIR}/limes_vecops.h>)

install (FILES limes_vecops.h DESTINATION "${LIMES_VECOPS_INSTALL_INCLUDE_DIR}"
		 COMPONENT limes_vecops_dev)

target_include_directories (
	limes_vecops
	PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
		   $<INSTALL_INTERFACE:${LIMES_VECOPS_INSTALL_INCLUDE_DIR}>)

add_subdirectory (impl) # NB - this must be first!
add_subdirectory (fft)

set (limes_vecops_sources limes_vecops.h ${impl_files} ${fft_files})

source_group (TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${limes_vecops_sources})

list (TRANSFORM limes_vecops_sources
	  PREPEND "${CMAKE_INSTALL_PREFIX}/${LIMES_VECOPS_INSTALL_INCLUDE_DIR}/")

source_group (TREE "${CMAKE_INSTALL_PREFIX}/${LIMES_VECOPS_INSTALL_INCLUDE_DIR}"
			  FILES ${limes_vecops_sources})

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

include (OrangesGeneratePkgConfig)

oranges_create_pkgconfig_file (
	TARGET limes_vecops
	INCLUDE_REL_PATH "${LIMES_VECOPS_INSTALL_INCLUDE_DIR}"
	DESCRIPTION "${PROJECT_DESCRIPTION}"
	URL "${PROJECT_HOMEPAGE_URL}"
	VERSION "${PROJECT_VERSION}"
	INSTALL_COMPONENT limes_vecops_dev)

include (CPackComponent)

cpack_add_component (limes_vecops_base HIDDEN)

cpack_add_component (limes_vecops_runtime GROUP limes_vecops
					 DEPENDS limes_vecops_base INSTALL_TYPES User)

cpack_add_component (limes_vecops_dev GROUP limes_vecops
					 DEPENDS limes_vecops_runtime INSTALL_TYPES Developer)

cpack_add_component_group (
	limes_vecops DISPLAY_NAME "${PROJECT_DESCRIPTION}"
	DESCRIPTION "All components of the ${PROJECT_DESCRIPTION}"
	PARENT_GROUP limes_libs)

add_library (Limes::limes_vecops ALIAS limes_vecops)

target_link_libraries (Limes INTERFACE Limes::limes_vecops)

if (Limes_IS_TOP_LEVEL)
	include (OrangesDefaultWarnings)
	include (OrangesAllIntegrations)

	target_link_libraries (
		limes_vecops PRIVATE $<BUILD_INTERFACE:Oranges::OrangesDefaultWarnings>
							 $<BUILD_INTERFACE:Oranges::OrangesAllIntegrations>)
endif ()
