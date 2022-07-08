# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

include (FeatureSummary)
include (CMakeDependentOption)
include (OrangesGeneratePlatformHeader)

cmake_dependent_option (
	LIMES_USE_POMMIER "Use the Pommier SIMD extensions for limes_vecops (SSE or NEON)" ON
	"PLAT_SSE OR PLAT_ARM_NEON" OFF)

mark_as_advanced (FORCE LIMES_USE_POMMIER)

add_feature_info ("Pommier SIMD functions" "${LIMES_USE_POMMIER}"
				  "Using Julien Pommier's SIMD functions for NEON/SSE")

if (NOT LIMES_USE_POMMIER)
	target_compile_definitions (limes_vecops PUBLIC LIMES_VECOPS_USE_POMMIER=0)

	return ()
endif ()

if (NOT (PLAT_SSE OR PLAT_ARM_NEON))
	message (FATAL_ERROR "Neither SSE or NEON is detected, LIMES_USE_POMMIER cannot be enabled!")
endif ()

target_compile_definitions (limes_vecops PUBLIC LIMES_VECOPS_USE_POMMIER=1)

set (pom_files pommier_wrapper.cpp pommier_wrapper.h)

if (PLAT_ARM_NEON)
	list (APPEND pom_files neon_mathfun.cpp neon_mathfun.h)
else ()
	list (APPEND pom_files sse_mathfun.cpp sse_mathfun.h)
endif ()

include (OrangesSourceFileUtils)

oranges_add_source_files (
	DIRECTORY_NAME impl/pommier
	TARGET limes_vecops
	INSTALL_COMPONENT limes_vecops_dev
	INSTALL_DIR "${LIMES_VECOPS_INSTALL_INCLUDE_DIR}"
	FILES ${pom_files})

set (pommier_files ${impl/pommier_files} PARENT_SCOPE)
