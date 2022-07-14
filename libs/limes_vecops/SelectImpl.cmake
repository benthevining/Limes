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

This file initializes the following cache variables:

+---------------------------+-------------------------+-----------------------------------+
| CMake variable            | Implementation name     | Notes                             |
+---------------------------+-------------------------+-----------------------------------+
| LIMES_USE_VDSP            | Apple Accelerate (vDSP) | The best choice on Apple hardware |
| LIMES_USE_IPP             | Intel IPP               | The best choice on Intel hardware |
| LIMES_USE_MIPP            | MIPP library            | SSE, AVX, AVX-512 or NEON         |
| LIMES_USE_NE10            | NE10 library            | ARM optimized, float only         |
| LIMES_USE_VECOPS_FALLBACK | Fallback (built-in)     | "Plain ol' C++ code"              |
+---------------------------+-------------------------+-----------------------------------+

Exactly one of these variables will be set to ON, the others will all be OFF.

This file contains some ugly code to make sure that changing one of these variables from the
command line in a subsequent configuration run will override the previous cached values of
the other variables.

]]

include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.22 FATAL_ERROR)

include (FeatureSummary)
include (CMakeDependentOption)
include (OrangesGeneratePlatformHeader)

list (APPEND CMAKE_MESSAGE_INDENT "SelectImpl.cmake -- ")

define_property (
	GLOBAL
	PROPERTY LIMES_VECOPS_IMPLEMENTATION
	BRIEF_DOCS "Name of the vecops implementation"
	FULL_DOCS
		"Name of the underlying vecops implementation library used by limes_vecops. May be vDSP, IPP, MIPP, NE10, or Fallback."
	)

#

set (pref_var_list LIMES_USE_VDSP LIMES_USE_IPP LIMES_USE_MIPP LIMES_USE_NE10
				   LIMES_USE_VECOPS_FALLBACK)

set (LIMES_USE_VDSP_desc "Use vDSP for limes_vecops (best on Apple platforms)")
set (LIMES_USE_IPP_desc "Use IPP for limes_vecops (best on Intel platforms)")
set (LIMES_USE_MIPP_desc "Use MIPP for limes_vecops (SSE, AVX, AVX-512, or ARM NEON)")
set (LIMES_USE_NE10_desc "Use NE10 for limes_vecops (Heavily optimized for ARM, but float only)")
set (LIMES_USE_VECOPS_FALLBACK_desc "Use the limes_vecops fallback implementation")

set (LIMES_USE_VDSP_def LIMES_VECOPS_USE_VDSP)
set (LIMES_USE_IPP_def LIMES_VECOPS_USE_IPP)
set (LIMES_USE_MIPP_def LIMES_VECOPS_USE_MIPP)
set (LIMES_USE_NE10_def LIMES_VECOPS_USE_NE10)

set (LIMES_USE_VDSP_name vDSP)
set (LIMES_USE_IPP_name IPP)
set (LIMES_USE_MIPP_name MIPP)
set (LIMES_USE_NE10_name NE10)
set (LIMES_USE_VECOPS_FALLBACK_name Fallback)

set (LIMES_USE_VDSP_req_package Accelerate)
set (LIMES_USE_VDSP_req_target Accelerate::Accelerate)

set_package_properties (Accelerate PROPERTIES TYPE RECOMMENDED
						PURPOSE "Apple's accelerated vector math library")

set (LIMES_USE_IPP_req_package IPP)
set (LIMES_USE_IPP_req_target IPP::IPP)

set_package_properties (IPP PROPERTIES TYPE RECOMMENDED
						PURPOSE "Intel's accelerated vector math library")

set (LIMES_USE_MIPP_req_package MIPP)
set (LIMES_USE_MIPP_req_target MIPP::MIPP)

set_package_properties (MIPP PROPERTIES TYPE RECOMMENDED PURPOSE "SIMD vector math acceleration")

set (LIMES_USE_NE10_req_package NE10)
set (LIMES_USE_NE10_req_target NE10::NE10)

set_package_properties (NE10 PROPERTIES TYPE RECOMMENDED PURPOSE "Optimized ARM math library")

#

foreach (var_name IN LISTS pref_var_list)
	set (${var_name}_init OFF)
endforeach ()

if (PLAT_APPLE)
	set (LIMES_USE_VDSP_init ON)
endif ()

if (PLAT_INTEL)
	set (LIMES_USE_IPP_init ON)
endif ()

if (PLAT_SSE OR PLAT_AVX OR PLAT_AVX512 OR PLAT_ARM_NEON)
	set (LIMES_USE_MIPP_init ON)
endif ()

if (PLAT_ARM)
	set (LIMES_USE_NE10_init ON)
endif ()

set (LIMES_USE_VECOPS_FALLBACK_init ON)

#

if (DEFINED CACHE{LIMES_VECOPS_SAVED_IMPL_PREFS})

	message (TRACE "Values from last time: ${LIMES_VECOPS_SAVED_IMPL_PREFS}")

	list (LENGTH pref_var_list list_length)

	math (EXPR list_length "${list_length} - 1" OUTPUT_FORMAT DECIMAL)

	# cmake-lint: disable=E1120
	foreach (idx RANGE "${list_length}")

		list (GET pref_var_list "${idx}" var_name)
		list (GET LIMES_VECOPS_SAVED_IMPL_PREFS "${idx}" prev_value)

		if ("${${var_name}}" AND NOT "${prev_value}")

			message (DEBUG
					 "${var_name} was changed from OFF to ON, and is overriding the other options")

			set ("${var_name}" ON CACHE BOOL "${${var_name}_desc}" FORCE)

			foreach (other_varname IN LISTS pref_var_list)
				if (NOT "${other_varname}" STREQUAL "${var_name}")
					set ("${other_varname}" OFF CACHE BOOL "${${other_varname}_desc}" FORCE)
				endif ()
			endforeach ()

		endif ()
	endforeach ()
endif ()

#

foreach (var_name IN LISTS pref_var_list)

	message (TRACE "${var_name}_init is: ${${var_name}_init}")

	unset (var_cond)

	foreach (other_varname IN LISTS pref_var_list)
		if (NOT "${other_varname}" STREQUAL "${var_name}")
			list (APPEND var_cond "NOT ${other_varname}")
		endif ()
	endforeach ()

	if (NOT "${var_name}" STREQUAL "LIMES_USE_VECOPS_FALLBACK")
		find_package ("${${var_name}_req_package}" MODULE)

		list (APPEND var_cond "${${var_name}_req_package}_FOUND")
		list (APPEND var_cond "TARGET ${${var_name}_req_target}")
	endif ()

	message (TRACE "${var_name} condition list: ${var_cond}")

	cmake_dependent_option ("${var_name}" "${${var_name}_desc}" "${${var_name}_init}" "${var_cond}"
							OFF)

	message (DEBUG "${var_name} is: ${${var_name}}")

	# as soon as one is ON, we know that's the one, and the others will be OFF
	if ("${${var_name}}")

		foreach (other_varname IN LISTS pref_var_list)
			if (NOT "${other_varname}" STREQUAL "${var_name}")
				set ("${other_varname}" OFF CACHE BOOL "${${other_varname}_desc}" FORCE)

				if (NOT ("${other_varname}" STREQUAL "LIMES_USE_VECOPS_FALLBACK"))
					target_compile_definitions (limes_vecops PUBLIC "${${other_varname}_def}=0")
					message (STATUS "${${other_varname}_def}=0")
				endif ()
			endif ()
		endforeach ()

		target_link_libraries (limes_vecops PUBLIC "${${var_name}_req_target}")

		set ("${var_name}" ON CACHE BOOL "${${var_name}_desc}" FORCE)

		if (NOT ("${var_name}" STREQUAL "LIMES_USE_VECOPS_FALLBACK"))
			target_compile_definitions (limes_vecops PUBLIC "${${var_name}_def}=1")
			message (STATUS "${${var_name}_def}=1")
		endif ()

		set (impl_name "${${var_name}_name}")

		break ()

	endif ()
endforeach ()

mark_as_advanced (FORCE ${pref_var_list})

#

set (num_on 0)

foreach (varname IN LISTS pref_var_list)
	if (${${varname}})
		math (EXPR num_on "${num_on} + 1" OUTPUT_FORMAT DECIMAL)
	endif ()
endforeach ()

if ("${num_on}" EQUAL 0)
	message (FATAL_ERROR "Internal error - none of the impl cache variables are ON")
elseif ("${num_on}" GREATER 1)
	message (FATAL_ERROR "Internal error - more than 1 of the impl cache variables are ON")
endif ()

unset (num_on)

#

message (VERBOSE "Using the ${impl_name} backend")

add_feature_info ("Accelerated vecops" "NOT LIMES_USE_VECOPS_FALLBACK"
				  "Using the ${impl_name} backend for the limes_vecops functions")

set_property (GLOBAL PROPERTY LIMES_VECOPS_IMPLEMENTATION "${impl_name}")

#

unset (valslist)

foreach (var_name IN LISTS pref_var_list)
	list (APPEND valslist "${${var_name}}")
endforeach ()

set (LIMES_VECOPS_SAVED_IMPL_PREFS "${valslist}" CACHE INTERNAL "")

message (TRACE "Saving values for next time: ${LIMES_VECOPS_SAVED_IMPL_PREFS}")

#

list (POP_BACK CMAKE_MESSAGE_INDENT)
