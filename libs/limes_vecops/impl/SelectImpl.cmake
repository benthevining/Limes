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

cmake_minimum_required (VERSION 3.22 FATAL_ERROR)

#

include (FeatureSummary)

if (LIMES_VECOPS_BACKEND)
	macro (__lvo_impl_set_not impl)
		set (LIMES_USE_${impl} OFF CACHE BOOL "Use the ${impl} backend for limes_vecops" FORCE)
	endmacro ()

	macro (__lvo_impl_set_yes impl)
		set (LIMES_USE_${impl} ON CACHE BOOL "Use the ${impl} backend for limes_vecops" FORCE)
	endmacro ()

	if ("${LIMES_VECOPS_BACKEND}" MATCHES VDSP)
		__lvo_impl_set_yes (VDSP)
		__lvo_impl_set_not (IPP)
		__lvo_impl_set_not (MIPP)
		__lvo_impl_set_not (VECOPS_FALLBACK)
	elseif (("${LIMES_VECOPS_BACKEND}" MATCHES IPP) AND (NOT "${LIMES_VECOPS_BACKEND}" MATCHES MIPP
														))
		__lvo_impl_set_yes (IPP)
		__lvo_impl_set_not (VDSP)
		__lvo_impl_set_not (MIPP)
		__lvo_impl_set_not (VECOPS_FALLBACK)
	elseif ("${LIMES_VECOPS_BACKEND}" MATCHES MIPP)
		__lvo_impl_set_yes (MIPP)
		__lvo_impl_set_not (VDSP)
		__lvo_impl_set_not (IPP)
		__lvo_impl_set_not (VECOPS_FALLBACK)
	elseif ("${LIMES_VECOPS_BACKEND}" MATCHES Fallback)
		__lvo_impl_set_yes (VECOPS_FALLBACK)
		__lvo_impl_set_not (VDSP)
		__lvo_impl_set_not (IPP)
		__lvo_impl_set_not (MIPP)
	else ()
		message (WARNING "Unknown vecops backend requested: ${LIMES_VECOPS_BACKEND}")
		unset (CACHE{LIMES_VECOPS_BACKEND})
		unset (LIMES_VECOPS_BACKEND)
	endif ()
endif ()

#

function (__limes_vecops_impl_set_properties impl)

	string (TOLOWER "${impl}" header_name)

	set (header_name "${header_name}.h")

	target_sources (
		limes_vecops
		PRIVATE "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/${header_name}>"
				"$<INSTALL_INTERFACE:${LIMES_VECOPS_INSTALL_INCLUDE_DIR}/impl/${header_name}>")

	install (FILES "${header_name}" DESTINATION "${LIMES_VECOPS_INSTALL_INCLUDE_DIR}/impl"
			 COMPONENT limes_vecops_dev)

	set_property (GLOBAL PROPERTY LIMES_VECOPS_IMPLEMENTATION "${impl}")

	add_feature_info (vecops_accelerated "NOT ${impl} MATCHES Fallback"
					  "Using the ${impl} backend for the vecops library")

	if (NOT "${impl}" STREQUAL Fallback)
		target_compile_definitions (limes_vecops PUBLIC LIMES_VECOPS_USE_${impl}=1)

		set (LIMES_VECOPS_USING_${impl} ON CACHE INTERNAL "")
	endif ()

	message (VERBOSE "limes_vecops -- using ${impl}")

	set (LIMES_VECOPS_BACKEND "${impl}"
		 CACHE STRING "Backend being used for the vector operations library" FORCE)

	mark_as_advanced (LIMES_VECOPS_BACKEND)

	set_property (CACHE LIMES_VECOPS_BACKEND PROPERTY STRINGS "vDSP;IPP;MIPP;Fallback")

endfunction ()

#

include (OrangesGeneratePlatformHeader)

# vDSP
# cmake-format: off
if (LIMES_USE_VDSP
	OR (PLAT_APPLE AND
		NOT (LIMES_USE_IPP OR LIMES_USE_MIPP OR LIMES_USE_VECOPS_FALLBACK)))
# cmake-format: on

	find_package (Accelerate MODULE)

	set_package_properties (Accelerate PROPERTIES TYPE RECOMMENDED
							PURPOSE "Apple's accelerated vector math library")

	if (Accelerate_FOUND AND TARGET Accelerate::Accelerate)
		target_link_libraries (limes_vecops PUBLIC Accelerate::Accelerate)

		__limes_vecops_impl_set_properties (VDSP)

		return ()
	endif ()
endif ()

#

# IPP
# cmake-format: off
if (LIMES_USE_IPP
	OR (PLAT_INTEL AND
		NOT (LIMES_USE_MIPP OR LIMES_USE_VECOPS_FALLBACK)))
# cmake-format: on

	find_package (IPP MODULE COMPONENTS CORE S VM)

	set_package_properties (IPP PROPERTIES TYPE RECOMMENDED
							PURPOSE "Intel's accelerated vector math library")

	if (IPP_FOUND AND TARGET IPP::IPP)
		target_link_libraries (limes_vecops PUBLIC IPP::IPP)

		__limes_vecops_impl_set_properties (IPP)

		return ()
	endif ()
endif ()

#

# MIPP
# cmake-format: off
if (LIMES_USE_MIPP
	OR (NOT LIMES_USE_VECOPS_FALLBACK
		AND (PLAT_SSE OR PLAT_AVX OR PLAT_AVX512 OR PLAT_ARM_NEON)))
# cmake-format: on

	find_package (MIPP MODULE)

	set_package_properties (MIPP PROPERTIES TYPE RECOMMENDED
							PURPOSE "SIMD vector math acceleration")

	if (TARGET MIPP::MIPP)
		target_link_libraries (limes_vecops PUBLIC MIPP::MIPP)

		__limes_vecops_impl_set_properties (MIPP)

		return ()
	endif ()
endif ()

#

__limes_vecops_impl_set_properties (Fallback)
