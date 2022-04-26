# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

#[[

Implementations, in order of preference:
- vDSP (Apple only)
- IPP (Intel platforms only)
- MIPP
- Fallback

]]

include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

#

include (OrangesCmakeDevTools)
include (FeatureSummary)

if(LIMES_VECOPS_BACKEND)
	if("${LIMES_VECOPS_BACKEND}" MATCHES vDSP)
		set (LIMES_IGNORE_VDSP OFF)
		set (LIMES_USE_IPP OFF)
		set (LIMES_USE_MIPP OFF)
	elseif("${LIMES_VECOPS_BACKEND}" STREQUAL IPP OR "${LIMES_VECOPS_BACKEND}" STREQUAL ipp)
		set (LIMES_IGNORE_IPP OFF)
		set (LIMES_USE_IPP ON)
	elseif("${LIMES_VECOPS_BACKEND}" MATCHES MIPP)
		set (LIMES_IGNORE_MIPP OFF)
		set (LIMES_USE_MIPP ON)
	elseif("${LIMES_VECOPS_BACKEND}" MATCHES Fallback)
		set (LIMES_USE_VECOPS_FALLBACK ON)
	else()
		message (WARNING "Unknown vecops backend requested: ${LIMES_VECOPS_BACKEND}")
	endif()
endif()

at_most_one (more_than_one LIMES_USE_IPP LIMES_USE_MIPP LIMES_USE_VECOPS_FALLBACK)

if(more_than_one)
	message (
		WARNING
			"Defining more than one of LIMES_USE_IPP, LIMES_USE_MIPP, or LIMES_USE_VECOPS_FALLBACK to truthy values is non-deterministic behavior!"
		)
endif()

unset (more_than_one)

#

function(_limes_vecops_impl_set_properties)

	target_sources (
		limes_vecops
		PRIVATE
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/${LIMES_VECOPS_IMPL_HEADER_NAME}>
			$<INSTALL_INTERFACE:${LIMES_VECOPS_INSTALL_INCLUDE_DIR}/impl/${LIMES_VECOPS_IMPL_HEADER_NAME}>
		)

	install (FILES "${LIMES_VECOPS_IMPL_HEADER_NAME}"
			 DESTINATION "${LIMES_VECOPS_INSTALL_INCLUDE_DIR}/impl" COMPONENT limes_vecops_dev)

	set_target_properties (limes_vecops PROPERTIES VECOPS_IMPLEMENTATION
												   "${LIMES_VECOPS_IMPL_NAME}")

	add_feature_info (vecops_accelerated "NOT ${LIMES_VECOPS_IMPL_NAME} MATCHES Fallback"
					  "Using the ${LIMES_VECOPS_IMPL_NAME} backend for the vecops library")

	message (VERBOSE "limes_vecops -- using ${LIMES_VECOPS_IMPL_NAME}")

	set (LIMES_VECOPS_BACKEND "${LIMES_VECOPS_IMPL_NAME}"
		 CACHE STRING "Backend being used for the vector operations library" FORCE)

	mark_as_advanced (FORCE LIMES_VECOPS_BACKEND)

	set_property (CACHE LIMES_VECOPS_BACKEND PROPERTY STRINGS "vDSP;IPP;MIPP;Fallback")

	unset (LIMES_VECOPS_IMPL_NAME)
	unset (LIMES_VECOPS_IMPL_HEADER_NAME)

endfunction()

cmake_language (DEFER CALL _limes_vecops_impl_set_properties)

#

# editorconfig-checker-disable
if(APPLE
   AND NOT LIMES_IGNORE_VDSP
   AND NOT LIMES_USE_IPP
   AND NOT LIMES_USE_MIPP
   AND NOT LIMES_USE_VECOPS_FALLBACK)
	# editorconfig-checker-enable

	# find_package (Accelerate MODULE COMPONENTS vDSP vForce BLAS)

	target_compile_definitions (limes_vecops PUBLIC LIMES_VECOPS_USE_VDSP=1)

	# target_link_libraries (limes_vecops PUBLIC Apple::Accelerate)

	set (LIMES_VECOPS_IMPL_HEADER_NAME vdsp.h)
	set (LIMES_VECOPS_IMPL_NAME vDSP)

	return ()
endif()

#

if(NOT LIMES_IGNORE_IPP AND NOT LIMES_USE_MIPP AND NOT LIMES_USE_VECOPS_FALLBACK)

	# check if we're on an Intel platform
	include (OrangesGeneratePlatformHeader)

	if(ORANGES_INTEL OR LIMES_USE_IPP)

		find_package (IPP MODULE QUIET COMPONENTS CORE S VM)

		set_package_properties (IPP PROPERTIES TYPE RECOMMENDED
								PURPOSE "Intel's accelerated vector math library")

		if(TARGET Intel::IntelIPP)
			if(NOT ORANGES_INTEL)
				message (
					WARNING
						"Enabling Intel IPP, but non-Intel platform detected. A different vecops backend is recommended"
					)
			endif()

			target_link_libraries (limes_vecops PUBLIC Intel::IntelIPP)

			target_compile_definitions (limes_vecops PUBLIC LIMES_VECOPS_USE_IPP=1)

			set (LIMES_VECOPS_IMPL_HEADER_NAME ipp.h)
			set (LIMES_VECOPS_IMPL_NAME IPP)

			set (LIMES_VECOPS_USING_IPP TRUE CACHE INTERNAL "")

			return ()

		elseif(LIMES_USE_IPP)
			message (WARNING "Intel IPP could not be found!")
		endif()
	endif()
endif()

#

if(NOT LIMES_IGNORE_MIPP AND NOT LIMES_USE_VECOPS_FALLBACK)

	find_package (MIPP MODULE QUIET)

	set_package_properties (MIPP PROPERTIES TYPE RECOMMENDED
							PURPOSE "SIMD vector math acceleration")

	if(TARGET aff3ct::MIPP)
		target_link_libraries (limes_vecops PUBLIC aff3ct::MIPP)

		target_compile_definitions (limes_vecops PUBLIC LIMES_VECOPS_USE_MIPP=1)

		set (LIMES_VECOPS_IMPL_HEADER_NAME mipp.h)
		set (LIMES_VECOPS_IMPL_NAME MIPP)

		set (LIMES_VECOPS_USING_MIPP TRUE CACHE INTERNAL "")

		return ()

	elseif(LIMES_USE_MIPP)
		message (WARNING "MIPP could not be found!")
	endif()
endif()

#

set (LIMES_VECOPS_IMPL_HEADER_NAME fallback.h)
set (LIMES_VECOPS_IMPL_NAME Fallback)
