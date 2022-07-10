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

# check if the user changed any cache variables for this config run - allow changing one to override
# the others
if (DEFINED CACHE{LIMES_PREV_VECOPS_PREFS})

	message (TRACE "Checking previous config run's values for changes...")

	list (LENGTH pref_var_list pref_var_list_len)

	math (EXPR pref_var_list_len "${pref_var_list_len} - 1" OUTPUT_FORMAT DECIMAL)

	foreach (idx RANGE "${pref_var_list_len}")

		list (GET LIMES_PREV_VECOPS_PREFS "${idx}" pref_prev_val)
		list (GET pref_var_list "${idx}" pref_var_name)

		if (NOT DEFINED "${pref_var_name}")
			continue ()
		endif ()

		# this option has changed, and it's now on - assume the user wants this option to override
		# the others
		if ("${${pref_var_name}}" AND NOT "${pref_prev_val}")

			message (DEBUG "Variable ${pref_var_name} is overriding the others!")

			foreach (var_name IN LISTS pref_var_list)
				if (NOT "${var_name}" STREQUAL "${pref_var_name}")
					set ("${var_name}" OFF)
				endif ()
			endforeach ()

			break ()
		endif ()

	endforeach ()

	unset (pref_var_list_len)
	unset (pref_prev_val)
	unset (pref_var_name)

endif ()

#

set (LIMES_USE_VDSP_desc "Use vDSP for limes_vecops (best on Apple platforms)")
set (LIMES_USE_IPP_desc "Use IPP for limes_vecops (best on Intel platforms)")
set (LIMES_USE_MIPP_desc "Use MIPP for limes_vecops (SSE, AVX, AVX-512, or ARM NEON)")
set (LIMES_USE_NE10_desc "Use NE10 for limes_vecops (Heavily optimized for ARM, but float only)")
set (LIMES_USE_VECOPS_FALLBACK_desc "Use the limes_vecops fallback implementation")

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

if (NOT (LIMES_USE_VDSP_init OR LIMES_USE_IPP_init OR LIMES_USE_MIPP_init OR LIMES_USE_NE10_init))
	set (LIMES_USE_VECOPS_FALLBACK_init ON)
endif ()

#

foreach (var_name IN LISTS pref_var_list)

	message (TRACE "${var_name}_init - initialized to ${${var_name}_init}")

	unset (var_cond)

	foreach (other_varname IN LISTS pref_var_list)
		if (NOT "${other_varname}" STREQUAL "${var_name}")
			list (APPEND var_cond "NOT ${other_varname}")
		endif ()
	endforeach ()

	cmake_dependent_option ("${var_name}" "${${var_name}_desc}" "${${var_name}_init}" "${var_cond}"
							OFF)

	unset (${var_name}_init)

	message (DEBUG "${var_name} is: ${${var_name}}")

endforeach ()

unset (var_cond)

mark_as_advanced (FORCE ${pref_var_list})

#

set_package_properties (Accelerate PROPERTIES TYPE RECOMMENDED
						PURPOSE "Apple's accelerated vector math library")

set_package_properties (IPP PROPERTIES TYPE RECOMMENDED
						PURPOSE "Intel's accelerated vector math library")

set_package_properties (MIPP PROPERTIES TYPE RECOMMENDED PURPOSE "SIMD vector math acceleration")

set_package_properties (NE10 PROPERTIES TYPE RECOMMENDED PURPOSE "Optimized ARM math library")

#

set (LIMES_USE_VDSP_def LIMES_VECOPS_USE_VDSP)
set (LIMES_USE_IPP_def LIMES_VECOPS_USE_IPP)
set (LIMES_USE_MIPP_def LIMES_VECOPS_USE_MIPP)
set (LIMES_USE_NE10_def LIMES_VECOPS_USE_NE10)

macro (__limes_vecops_set_only varname)
	set ("${varname}" ON CACHE BOOL "${${varname}_desc}" FORCE)

	if (NOT ("${varname}" STREQUAL "LIMES_USE_VECOPS_FALLBACK"))
		target_compile_definitions (limes_vecops PUBLIC "${${varname}_def}=1")
	endif ()

	foreach (off_varname IN LISTS pref_var_list)
		if (NOT ("${off_varname}" STREQUAL "${varname}"))

			set ("${off_varname}" OFF CACHE BOOL "${${off_varname}_desc}" FORCE)

			if (NOT ("${off_varname}" STREQUAL "LIMES_USE_VECOPS_FALLBACK"))
				target_compile_definitions (limes_vecops PUBLIC "${${off_varname}_def}=0")
			endif ()
		endif ()
	endforeach ()
endmacro ()

#

if (LIMES_USE_VDSP)
	find_package (Accelerate MODULE)
endif ()

if (LIMES_USE_VDSP AND Accelerate_FOUND AND TARGET Accelerate::Accelerate)

	target_link_libraries (limes_vecops PUBLIC Accelerate::Accelerate)

	__limes_vecops_set_only (LIMES_USE_VDSP)

	set (impl_name vDSP)

else ()

	if (LIMES_USE_IPP)
		find_package (IPP 7 MODULE COMPONENTS CORE S VM)
	endif ()

	if (LIMES_USE_IPP AND IPP_FOUND AND TARGET IPP::IPP)

		target_link_libraries (limes_vecops PUBLIC IPP::IPP)

		__limes_vecops_set_only (LIMES_USE_IPP)

		set (impl_name IPP)

	else ()

		if (LIMES_USE_MIPP)
			find_package (MIPP MODULE)
		endif ()

		if (LIMES_USE_MIPP AND MIPP_FOUND AND TARGET MIPP::MIPP)

			target_link_libraries (limes_vecops PUBLIC MIPP::MIPP)

			__limes_vecops_set_only (LIMES_USE_MIPP)

			set (impl_name MIPP)

		else ()

			if (LIMES_USE_NE10)
				find_package (NE10 MODULE)
			endif ()

			if (LIMES_USE_NE10 AND NE10_FOUND AND TARGET NE10::NE10)

				target_link_libraries (limes_vecops PUBLIC NE10::NE10)

				__limes_vecops_set_only (LIMES_USE_NE10)

				set (impl_name NE10)

			else ()

				__limes_vecops_set_only (LIMES_USE_VECOPS_FALLBACK)

				set (impl_name Fallback)

			endif ()
		endif ()
	endif ()
endif ()

#

set (num_on 0)

foreach (varname IN LISTS pref_var_list)
	unset (${varname}_desc)
	unset (${varname}_def)

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

unset (impl_name)
unset (tmp_prefs_list)

foreach (pref_var IN LISTS pref_var_list)
	list (APPEND tmp_prefs_list "${${pref_var}}")
endforeach ()

set (LIMES_PREV_VECOPS_PREFS "${tmp_prefs_list}" CACHE INTERNAL "")

message (TRACE "Saving preferences for next time: ${LIMES_PREV_VECOPS_PREFS}")

unset (tmp_prefs_list)
unset (pref_var_list)

list (POP_BACK CMAKE_MESSAGE_INDENT)
