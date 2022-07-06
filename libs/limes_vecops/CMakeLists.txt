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

include (CMakeDependentOption)

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

cmake_dependent_option (LIMES_USE_VDSP "Use vDSP for limes_vecops" OFF
						"NOT (LIMES_USE_IPP OR LIMES_USE_MIPP OR LIMES_USE_VECOPS_FALLBACK)" OFF)

cmake_dependent_option (LIMES_USE_IPP "Use IPP for limes_vecops" OFF
						"NOT (LIMES_USE_VDSP OR LIMES_USE_MIPP OR LIMES_USE_VECOPS_FALLBACK)" OFF)

cmake_dependent_option (LIMES_USE_MIPP "Use MIPP for limes_vecops" OFF
						"NOT (LIMES_USE_VDSP OR LIMES_USE_IPP OR LIMES_USE_VECOPS_FALLBACK)" OFF)

cmake_dependent_option (LIMES_USE_VECOPS_FALLBACK "Use the limes_vecops fallback implementation"
						OFF "NOT (LIMES_USE_VDSP OR LIMES_USE_IPP OR LIMES_USE_MIPP)" OFF)

option (LIMES_USE_POMMIER "Use the Pommier SIMD extensions for limes_vecops (SSE or NEON)" ON)
option (LIMES_USE_FFTW "Use FFTW for limes_vecops" ON)

mark_as_advanced (LIMES_USE_VDSP LIMES_USE_IPP LIMES_USE_MIPP LIMES_USE_VECOPS_FALLBACK
				  LIMES_USE_POMMIER LIMES_USE_FFTW)

#

project (
	LimesVecops
	VERSION "${Limes_VERSION}"
	DESCRIPTION "Limes vector operations library"
	HOMEPAGE_URL "${Limes_HOMEPAGE_URL}"
	LANGUAGES CXX ASM)

add_library (limes_vecops)

set_target_properties (
	limes_vecops
	PROPERTIES VERSION "${PROJECT_VERSION}"
			   SOVERSION "${PROJECT_VERSION_MAJOR}"
			   LABELS Limes
			   FOLDER Limes/libs
			   EchoString "Building the limes_vecops library...")

set_property (TARGET limes_vecops APPEND PROPERTY EXPORT_PROPERTIES LIMES_VECOPS_IMPLEMENTATION
												  LIMES_FFT_IMPLEMENTATION)

target_link_libraries (limes_vecops PUBLIC Limes::limes_core)

# cmake-format: off
target_compile_options (
	limes_vecops
	PUBLIC
		# fixes a bug present when compiling vDSP with GCC
		"$<$<CXX_COMPILER_ID:GNU>:-flax-vector-conversions>"
	PRIVATE
		# allows inline assembly to be used on watchOS
		"$<$<AND:$<PLATFORM_ID:watchOS>,$<CXX_COMPILER_ID:Clang,AppleClang>>:-fgnu-inline-asm>"
	)
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

add_subdirectory (impl) # NB - this must be first!
add_subdirectory (fft)
# add_subdirectory (resampling)

set (limes_vecops_sources limes_vecops.h ${impl_files} ${fft_files} ${resampling_files})

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
