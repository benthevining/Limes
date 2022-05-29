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

#

include (OrangesGeneratePlatformHeader)

# Pommier

# cmake-format: off
if (LIMES_USE_POMMIER
	OR ((PLAT_SSE OR PLAT_ARM_NEON)
		AND NOT (MSVC AND NOT ORANGES_ARM_NEON)))
# cmake-format: on
	set (__limes_use_pommier ON)
else ()
	set (__limes_use_pommier OFF)
endif ()

#

# FFTW

find_package (FFTW MODULE QUIET)

if (TARGET FFTW::FFTW)
	set (__limes_use_fftw ON)
else ()
	set (__limes_use_fftw OFF)
endif ()

#

if (LIMES_VECOPS_BACKEND)
	if ("${LIMES_VECOPS_BACKEND}" MATCHES VDSP)
		set (__limes_use_vdsp ON)
		set (__limes_use_ipp OFF)
		set (__limes_use_mipp OFF)
		set (__limes_use_vecops_fallback OFF)

		return ()
	elseif (("${LIMES_VECOPS_BACKEND}" MATCHES IPP) AND (NOT "${LIMES_VECOPS_BACKEND}" MATCHES MIPP
														))
		set (__limes_use_vdsp OFF)
		set (__limes_use_ipp ON)
		set (__limes_use_mipp OFF)
		set (__limes_use_vecops_fallback OFF)

		return ()
	elseif ("${LIMES_VECOPS_BACKEND}" MATCHES MIPP)
		set (__limes_use_vdsp OFF)
		set (__limes_use_ipp OFF)
		set (__limes_use_mipp ON)
		set (__limes_use_vecops_fallback OFF)

		return ()
	elseif ("${LIMES_VECOPS_BACKEND}" MATCHES Fallback)
		set (__limes_use_vdsp OFF)
		set (__limes_use_ipp OFF)
		set (__limes_use_mipp OFF)
		set (__limes_use_vecops_fallback ON)

		return ()
	else ()
		message (WARNING "Unknown vecops backend requested: ${LIMES_VECOPS_BACKEND}")
	endif ()
endif ()

#

# vDSP

# cmake-format: off
if ((PLAT_APPLE OR LIMES_USE_VDSP)
	AND NOT LIMES_USE_IPP
	AND NOT LIMES_USE_MIPP
	AND NOT LIMES_USE_VECOPS_FALLBACK)
# cmake-format: on

	# find_package (Accelerate MODULE COMPONENTS vDSP vForce BLAS)

	set (__limes_use_vdsp ON)
	set (__limes_use_ipp OFF)
	set (__limes_use_mipp OFF)
	set (__limes_use_vecops_fallback OFF)

	return ()
endif ()

# IPP

# cmake-format: off
if ((PLAT_INTEL OR LIMES_USE_IPP)
	AND NOT LIMES_USE_MIPP
	AND NOT LIMES_USE_VECOPS_FALLBACK)
# cmake-format: on

	find_package (IPP MODULE QUIET COMPONENTS CORE S VM)

	set_package_properties (IPP PROPERTIES TYPE RECOMMENDED
							PURPOSE "Intel's accelerated vector math library")

	if (TARGET Intel::IntelIPP)

		set (__limes_use_vdsp OFF)
		set (__limes_use_ipp ON)
		set (__limes_use_mipp OFF)
		set (__limes_use_vecops_fallback OFF)

		return ()

	elseif (LIMES_USE_IPP)
		message (WARNING "Intel IPP could not be found!")
	endif ()
endif ()

#

# MIPP

# cmake-format: off
if (LIMES_USE_MIPP OR
	((PLAT_SSE OR PLAT_AVX OR PLAT_AVX512 OR PLAT_ARM_NEON)
		AND NOT LIMES_USE_VECOPS_FALLBACK))
# cmake-format: on

	find_package (MIPP MODULE QUIET)

	set_package_properties (MIPP PROPERTIES TYPE RECOMMENDED
							PURPOSE "SIMD vector math acceleration")

	if (TARGET aff3ct::MIPP)

		set (__limes_use_vdsp OFF)
		set (__limes_use_ipp OFF)
		set (__limes_use_mipp ON)
		set (__limes_use_vecops_fallback OFF)

		return ()

	elseif (LIMES_USE_MIPP)
		message (WARNING "MIPP could not be found!")
	endif ()
endif ()

#

# Fallback

set (__limes_use_vdsp OFF)
set (__limes_use_ipp OFF)
set (__limes_use_mipp OFF)
set (__limes_use_vecops_fallback ON)
