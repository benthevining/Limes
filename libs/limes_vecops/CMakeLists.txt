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

add_library (limes_vecops STATIC)

set_target_properties (limes_vecops PROPERTIES VERSION "${PROJECT_VERSION}"
											   SOVERSION "${PROJECT_VERSION_MAJOR}")

target_link_libraries (limes_vecops PUBLIC Oranges::OrangesDefaultTarget Oranges::OrangesUnityBuild)

oranges_add_target_headers (
	TARGET
	limes_vecops
	SCOPE
	PUBLIC
	REL_PATH
	Limes/limes_vecops
	FILES
	limes_vecops.h)

target_include_directories (
	limes_vecops PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
						$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Limes/limes_vecops>)

add_subdirectory ("${CMAKE_CURRENT_LIST_DIR}/impl")
add_subdirectory ("${CMAKE_CURRENT_LIST_DIR}/util")
add_subdirectory ("${CMAKE_CURRENT_LIST_DIR}/fft")

find_package (FFTW MODULE QUIET)

if(TARGET FFTW::FFTW)
	message (DEBUG " -- limes_vecops - using FFTW for FFT")
	target_link_libraries (limes_vecops PUBLIC FFTW::FFTW)
	target_compile_definitions (limes_vecops PUBLIC LIMES_VECOPS_USE_FFTW=1)
endif()

lemons_sort_target_sources (limes_vecops "${CMAKE_CURRENT_LIST_DIR}")

set_target_properties (limes_vecops PROPERTIES LINKER_LANGUAGE CXX)

oranges_export_alias_target (limes_vecops Limes)

target_link_libraries (Limes INTERFACE Limes::limes_vecops)

oranges_install_targets (
	TARGETS
	limes_vecops
	EXPORT
	LimesVecopsTargets
	REL_PATH
	Limes/limes_vecops
	COMPONENT_PREFIX
	limes_vecops)

install (EXPORT LimesVecopsTargets NAMESPACE Limes:: DESTINATION "${LIMES_INSTALL_DEST}"
		 EXPORT_LINK_INTERFACE_LIBRARIES)

export (EXPORT LimesVecopsTargets NAMESPACE Limes:: FILE LimesVecopsTargets.cmake)
