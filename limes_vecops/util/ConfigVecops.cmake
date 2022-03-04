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

# Apple - default to using vDSP first
if(APPLE)
	option (LIMES_IGNORE_VDSP "Ignore vDSP for vecops" OFF)

	if(LIMES_IGNORE_VDSP)
		target_compile_definitions (limes_vecops INTERFACE LIMES_VECOPS_USE_VDSP=0)
	else()
		target_compile_definitions (limes_vecops INTERFACE LIMES_VECOPS_USE_VDSP=1)
		target_sources (limes_vecops INTERFACE "${CMAKE_CURRENT_LIST_DIR}/../impl/vdsp.h")

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

	find_package (IPP)

	if(TARGET Intel::IPP)
		target_link_libraries (limes_vecops INTERFACE Intel::IPP)

		target_compile_definitions (limes_vecops INTERFACE LIMES_VECOPS_USE_IPP=1)
		target_sources (limes_vecops INTERFACE "${CMAKE_CURRENT_LIST_DIR}/../impl/ipp.h")

		message (VERBOSE "limes_vecops -- using IPP")

		return ()
	endif()
endif()

#

# See if any of MIPP's supported architectures are supported on this platform

try_compile (compile_result "${CMAKE_CURRENT_BINARY_DIR}/try_compile"
			 "${CMAKE_CURRENT_LIST_DIR}/check_mipp.cpp" OUTPUT_VARIABLE compile_output)

string (REGEX REPLACE ".*LIMES_USE_MIPP ([a-zA-Z0-9_-]*).*" "\\1" use_mipp "${compile_output}")

if(use_mipp)

	message (STATUS "MIPP")

	include (LemonsGetCPM)

	CPMAddPackage (
		NAME
		MIPP
		GITHUB_REPOSITORY
		aff3ct/MIPP
		GIT_TAG
		origin/master
		DOWNLOAD_ONLY
		ON)

	target_compile_definitions (limes_vecops INTERFACE LIMES_VECOPS_USE_MIPP=1)
	target_include_directories (limes_vecops INTERFACE "${MIPP_SOURCE_DIR}/src")
	target_sources (limes_vecops INTERFACE "${CMAKE_CURRENT_LIST_DIR}/../impl/mipp.h")

	message (VERBOSE "limes_vecops -- using MIPP")

	return ()
endif()

#

# Fallback :(

target_sources (limes_vecops INTERFACE "${CMAKE_CURRENT_LIST_DIR}/../impl/fallback.h")
message (VERBOSE "limes_vecops -- using fallback implementation")
