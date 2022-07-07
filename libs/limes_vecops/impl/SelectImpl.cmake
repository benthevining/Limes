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
- MIPP (SSE, AVX, AVX-512 or NEON)
- Fallback

]]

include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.22 FATAL_ERROR)

define_property (
	GLOBAL
	PROPERTY LIMES_VECOPS_IMPLEMENTATION
	BRIEF_DOCS "Name of the vecops implementation"
	FULL_DOCS
		"Name of the underlying vecops implementation library used by limes_vecops. May be vDSP, IPP, MIPP, or Fallback."
	)

include (FeatureSummary)
include (CMakeDependentOption)
include (OrangesGeneratePlatformHeader)

if (PLAT_APPLE)
	set (vdsp_init ON)
else ()
	set (vdsp_init OFF)
endif ()

if (PLAT_INTEL)
	set (ipp_init ON)
else ()
	set (ipp_init OFF)
endif ()

if (PLAT_SSE OR PLAT_AVX OR PLAT_AVX512 OR PLAT_ARM_NEON)
	set (mipp_init ON)
else ()
	set (mipp_init OFF)
endif ()

if (NOT (vdsp_init OR ipp_init OR mipp_init))
	set (fb_init ON)
else ()
	set (fb_init OFF)
endif ()

cmake_dependent_option (LIMES_USE_VDSP "Use vDSP for limes_vecops" "${vdsp_init}"
						"NOT (LIMES_USE_IPP OR LIMES_USE_MIPP OR LIMES_USE_VECOPS_FALLBACK)" OFF)

cmake_dependent_option (LIMES_USE_IPP "Use IPP for limes_vecops" "${ipp_init}"
						"NOT (LIMES_USE_VDSP OR LIMES_USE_MIPP OR LIMES_USE_VECOPS_FALLBACK)" OFF)

cmake_dependent_option (LIMES_USE_MIPP "Use MIPP for limes_vecops" "${mipp_init}"
						"NOT (LIMES_USE_VDSP OR LIMES_USE_IPP OR LIMES_USE_VECOPS_FALLBACK)" OFF)

cmake_dependent_option (LIMES_USE_VECOPS_FALLBACK "Use the limes_vecops fallback implementation"
						"${fb_init}" "NOT (LIMES_USE_VDSP OR LIMES_USE_IPP OR LIMES_USE_MIPP)" OFF)

unset (vdsp_init)
unset (ipp_init)
unset (mipp_init)
unset (fb_init)

mark_as_advanced (FORCE LIMES_USE_VDSP LIMES_USE_IPP LIMES_USE_MIPP LIMES_USE_VECOPS_FALLBACK)

#

if (LIMES_USE_VDSP)
	set_package_properties (Accelerate PROPERTIES TYPE RECOMMENDED
							PURPOSE "Apple's accelerated vector math library")

	find_package (Accelerate MODULE)
else ()
	set (Accelerate_FOUND OFF)
endif ()

if (Accelerate_FOUND AND TARGET Accelerate::Accelerate)
	target_link_libraries (limes_vecops PUBLIC Accelerate::Accelerate)

	target_compile_definitions (limes_vecops PUBLIC LIMES_VECOPS_USE_VDSP=1 LIMES_VECOPS_USE_IPP=0
													LIMES_VECOPS_USE_MIPP=0)

	set (LIMES_USE_VDSP ON CACHE BOOL "Use vDSP for limes_vecops" FORCE)
	set (LIMES_USE_IPP OFF CACHE BOOL "Use IPP for limes_vecops" FORCE)
	set (LIMES_USE_MIPP OFF CACHE BOOL "Use MIPP for limes_vecops" FORCE)

	set (impl_name vDSP)
else ()
	set (LIMES_USE_VDSP OFF CACHE BOOL "Use vDSP for limes_vecops" FORCE)

	target_compile_definitions (limes_vecops PUBLIC LIMES_VECOPS_USE_VDSP=0)

	if (LIMES_USE_IPP)
		set_package_properties (IPP PROPERTIES TYPE RECOMMENDED
								PURPOSE "Intel's accelerated vector math library")

		find_package (IPP MODULE COMPONENTS CORE S VM)
	else ()
		set (IPP_FOUND OFF)
	endif ()

	if (IPP_FOUND AND TARGET IPP::IPP)
		target_link_libraries (limes_vecops PUBLIC IPP::IPP)

		target_compile_definitions (limes_vecops PUBLIC LIMES_VECOPS_USE_IPP=1
														LIMES_VECOPS_USE_MIPP=0)

		set (LIMES_USE_IPP ON CACHE BOOL "Use IPP for limes_vecops" FORCE)
		set (LIMES_USE_MIPP OFF CACHE BOOL "Use MIPP for limes_vecops" FORCE)

		set (impl_name IPP)
	else ()
		set (LIMES_USE_IPP OFF CACHE BOOL "Use IPP for limes_vecops" FORCE)

		target_compile_definitions (limes_vecops PUBLIC LIMES_VECOPS_USE_IPP=0)

		if (LIMES_USE_MIPP)
			set_package_properties (MIPP PROPERTIES TYPE RECOMMENDED
									PURPOSE "SIMD vector math acceleration")

			find_package (MIPP MODULE)
		else ()
			set (MIPP_FOUND OFF)
		endif ()

		if (MIPP_FOUND AND TARGET MIPP::MIPP)
			target_link_libraries (limes_vecops PUBLIC MIPP::MIPP)

			target_compile_definitions (limes_vecops PUBLIC LIMES_VECOPS_USE_MIPP=1)

			set (LIMES_USE_MIPP ON CACHE BOOL "Use MIPP for limes_vecops" FORCE)

			set (impl_name MIPP)
		else ()
			set (LIMES_USE_MIPP OFF CACHE BOOL "Use MIPP for limes_vecops" FORCE)

			target_compile_definitions (limes_vecops PUBLIC LIMES_VECOPS_USE_MIPP=0)

			set (impl_name Fallback)
		endif ()
	endif ()
endif ()

message (DEBUG " -- limes_vecops - using the ${impl_name} backend")

add_feature_info ("Accelerated vecops" "NOT ${impl_name} STREQUAL Fallback"
				  "Using the ${impl_name} backend for the limes_vecops functions")

set_property (GLOBAL PROPERTY LIMES_VECOPS_IMPLEMENTATION "${impl_name}")

unset (impl_name)
