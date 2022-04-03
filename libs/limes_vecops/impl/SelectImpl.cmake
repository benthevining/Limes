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

#

option (LIMES_IGNORE_VDSP "Ignore vDSP for vecops" OFF)
option (LIMES_IGNORE_IPP "Ignore Intel IPP for vecops" OFF)
option (LIMES_IGNORE_MIPP "Ignore MIPP for vecops" OFF)

mark_as_advanced (FORCE LIMES_IGNORE_VDSP LIMES_IGNORE_IPP)

#

# Apple - default to using vDSP first
if(APPLE)
	if(LIMES_IGNORE_VDSP)
		target_compile_definitions (limes_vecops PUBLIC LIMES_VECOPS_USE_VDSP=0)
	else()
		target_compile_definitions (limes_vecops PUBLIC LIMES_VECOPS_USE_VDSP=1)

		target_sources (
			limes_vecops
			PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/vdsp.h>
				   $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Limes/limes_vecops/vdsp.h>)

		target_link_libraries (limes_vecops PUBLIC "-framework vDSP")

		message (VERBOSE "limes_vecops -- using vDSP")

		return ()
	endif()
endif()

#

# IPP: check if we're on an Intel platform

try_compile (compile_result "${CMAKE_CURRENT_BINARY_DIR}/try_compile"
			 "${CMAKE_CURRENT_LIST_DIR}/check_intel.cpp" OUTPUT_VARIABLE compile_output)

string (REGEX REPLACE ".*LIMES_INTEL_PLATFORM ([a-zA-Z0-9_-]*).*" "\\1" intel_platform
					  "${compile_output}")

if(intel_platform)
	if(LIMES_IGNORE_IPP)
		target_compile_definitions (limes_vecops PUBLIC LIMES_VECOPS_USE_IPP=0)
	else()
		find_package (IPP MODULE QUIET COMPONENTS CORE S VM)

		if(TARGET Intel::IntelIPP)
			target_link_libraries (limes_vecops PUBLIC Intel::IntelIPP)

			target_compile_definitions (limes_vecops PUBLIC LIMES_VECOPS_USE_IPP=1)

			target_sources (
				limes_vecops
				PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/ipp.h>
					   $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Limes/limes_vecops/ipp.h>)

			message (VERBOSE "limes_vecops -- using IPP")

			set (LIMES_VECOPS_USING_IPP TRUE)

			return ()
		endif()
	endif()
endif()

#

if(LIMES_IGNORE_MIPP)
	target_compile_definitions (limes_vecops PUBLIC LIMES_VECOPS_USE_MIPP=0)
else()
	find_package (MIPP MODULE QUIET)

	if(TARGET aff3ct::MIPP)
		target_link_libraries (limes_vecops PUBLIC aff3ct::MIPP)

		target_compile_definitions (limes_vecops PUBLIC LIMES_VECOPS_USE_MIPP=1)

		target_sources (
			limes_vecops
			PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/mipp.h>
				   $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Limes/limes_vecops/mipp.h>)

		message (VERBOSE "limes_vecops -- using MIPP")

		set (LIMES_VECOPS_USING_MIPP TRUE)

		return ()
	endif()
endif()

#

# Fallback :(

target_sources (
	limes_vecops
	PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/fallback.h>
		   $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Limes/limes_vecops/fallback.h>)

message (VERBOSE "limes_vecops -- using fallback implementation")
